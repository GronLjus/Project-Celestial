#pragma once
#include "IHandleMessages.h"
#include "GUIObject.h"
#include "ResourceObject.h"
#include "CelestialSlicedList.h"

namespace Entities
{

	class GUIEntityHandler : public CrossHandlers::IHandleMessages
	{

	public:
		GUIEntityHandler();
		void Init(CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* gameObjects);
		void Update(unsigned int time);
		~GUIEntityHandler();

	private:

		Resources::ScreenTarget* getScreenTarget(unsigned int time, CelestialMath::vectorUI2 mouse);

		CrossHandlers::CelestialSlicedList<Resources::ScreenTarget*>* screenTargets;
		unsigned int maxScreenTargets;

		CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* gameObjects;
		Resources::GUIObject* focusedObject;
		Message keyMessage;
		Resources::ScreenTarget* dragTarget;

		void handleMouseMovement(Resources::ScreenTarget* target, unsigned int time, CelestialMath::vectorUI2 mouse);
		void handleMouseClick(Resources::ScreenTarget* target, unsigned int time, CelestialMath::vectorUI2 mouse, unsigned char key);
		void handleMouseWheel(Resources::ScreenTarget* target, unsigned int time, CelestialMath::vectorUI2 mouse, short delta);
		void handleMouseDrag(Resources::ScreenTarget* target, unsigned int time, CelestialMath::vectorUI2 mouse, unsigned char key,unsigned int dragStatus);

		void setCommonScriptParameters(unsigned int script, unsigned int time, unsigned int targetId, CelestialMath::vectorUI2 mouse);
		void runScript(unsigned int script, unsigned int time);

		void triggerScript(unsigned int script, unsigned int time, unsigned int targetId, CelestialMath::vectorUI2 mouse, short wheelDelta);
		void triggerScript(unsigned int script, unsigned int time, unsigned int targetId, CelestialMath::vectorUI2 mouse, unsigned int dragStatus);
		void triggerScript(unsigned int script, unsigned int time, unsigned int targetId, CelestialMath::vectorUI2 mouse);
		
	};
}
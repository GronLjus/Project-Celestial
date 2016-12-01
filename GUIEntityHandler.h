#pragma once
#include "IHandleMessages.h"
#include "GUILayout.h"
#include "ResourceObject.h"
#include "CelestialSlicedList.h"
#include "KeyCodes.h"

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

		struct key {

			key() {}
			key(Input::CelestialKeyCategories cat, unsigned char key) : cat(cat), keyVal(key){}
			Input::CelestialKeyCategories cat;
			unsigned char keyVal;

		};

		CrossHandlers::CelestialStack<key>* pressedKeys;

		Resources::GUILayout* screenLayout;
		Resources::ScreenTarget* lastTarget;

		CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* gameObjects;
		Resources::GUIObject* focusedObject;
		Resources::ScreenTarget* dragTarget;

		void setCommonScriptParameters(unsigned int script, unsigned int time, unsigned int targetId, CelestialMath::vectorUI2 mouse);
		void runScript(unsigned int script, unsigned int time);

		void triggerScript(unsigned int script, unsigned int time, unsigned int targetId, CelestialMath::vectorUI2 mouse, short wheelDelta);
		void triggerScript(unsigned int script, unsigned int time, unsigned int targetId, CelestialMath::vectorUI2 mouse, unsigned int dragStatus);
		void triggerScript(unsigned int script, unsigned int time, unsigned int targetId, CelestialMath::vectorUI2 mouse); 
		void triggerScript(unsigned int script, unsigned int time, unsigned int targetId, unsigned char key);

	};
}
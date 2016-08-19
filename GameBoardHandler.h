#pragma once
#include "GameBoard.h"
#include "IHandleMessages.h"
#include "CelestialSlicedList.h"
#include "RoutingManager.h"

namespace Entities
{

	class GameBoardHandler : public CrossHandlers::IHandleMessages
	{

		public:
			GameBoardHandler();
			void Init(CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* gameObjects);
			void UpdateMessages(unsigned int time);
			void Update(unsigned int time);
			~GameBoardHandler();

		private:
			RoutingManager* routing;
			CelestialMath::Vector3 hookPos;
			CelestialMath::Vector3 hookScale;
			CelestialMath::Vector3 hookRot;
			bool hookObject;

			unsigned int hookTargets;
			unsigned int* hookColl;

			CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* gameObjects;
			Resources::GameBoard* localGameBoard;
			Resources::GameObject* trackedObject;

			short trackedMX;
			short trackedMY;

			CelestialMath::Vector2 mouseCell;
			bool resetMouse;

			void splitObject(Resources::GameObject* object, CelestialMath::Vector3 position, float width, unsigned int time);
			void transformHookedObject(CelestialMath::Vector3 mousePos);

			void addScriptParamNum(unsigned int script, unsigned int num, unsigned int time);
			void triggerMouseScript(unsigned int script, unsigned int objectId, unsigned int time, int mouseX, int mouseY);
			void triggerMouseScript(unsigned int script, unsigned int objectId, unsigned int time, int mouseX, int mouseY, short delta);
			void triggerMouseScript(unsigned int script, unsigned int objectId, unsigned int time, int mouseX, int mouseY, unsigned int dragStatus);
			void triggerNodeScript(unsigned int script, unsigned int obj, unsigned int goalNode, unsigned int currentNode, unsigned int time);

			CelestialMath::Vector3 getMouseWorldLine(unsigned int mouseX, unsigned int mouseY) const;
			Resources::ScriptableObject* getMouseObject(CelestialMath::Vector3 direction) const;
			void handleMouseMovement(unsigned int mouseX, unsigned int mouseY, unsigned int time);
			unsigned int dragScript;
			unsigned int dragId;
			CelestialMath::Vector3 boardPos;
			void sendCommonScriptParams(unsigned int script, unsigned int objectId, unsigned int time, int mouseX, int mouseY);
			void runScript(unsigned int script, unsigned int time);

	};
}
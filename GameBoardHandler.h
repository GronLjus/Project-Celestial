#pragma once
#include "GameBoard.h"
#include "IHandleMessages.h"
#include "CelestialSlicedList.h"
#include "MouseHandler.h"

namespace Entities
{

	class GameBoardHandler : public CrossHandlers::IHandleMessages
	{

		public:
			GameBoardHandler();
			void Init(CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* gameObjects);
			void UpdateMessages(unsigned int time, unsigned int clock);
			void Update(unsigned int time, unsigned int clock, float timeDiff);
			~GameBoardHandler();

		private:
			RoutingManager* routing;

			CelestialMath::Vector3 hookPos;
			CelestialMath::Vector3 hookScale;
			CelestialMath::Vector3 hookRot;
			CelestialMath::Vector3 hookSide;

			bool snap;
			unsigned int hookStatus;
			bool hookOccupied;

			unsigned int lastTime;
			unsigned int hookTargets;

			CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* gameObjects;
			Resources::GameBoard* localGameBoard;
			Resources::GameObject* trackedObject;

			short trackedMX;
			short trackedMY;

			CelestialMath::Vector2 mouseCell;
			CelestialMath::Vector3 mouseCache;

			bool resetMouse;

			void runScript(unsigned int script, unsigned int time);
			void addScriptParamNum(unsigned int script, unsigned int num, unsigned int time);
			void addScriptParamFloat(unsigned int script, float num, unsigned int time);

			void triggerNodeScript(unsigned int script, unsigned int obj, unsigned int goalNode, unsigned int currentNode, unsigned int time);

			void triggerSplitScript(unsigned int script, unsigned int obj, CelestialMath::Vector3 pos, unsigned int time);

			void splitObject(Resources::GameObject* object, CelestialMath::Vector3 position, float width, unsigned int time);
			void transformHookedObject(CelestialMath::Vector3 mousePos);

			CelestialMath::Vector3 snapMouse(unsigned int amounts, unsigned int* collidedObjects, Resources::PositionableObject* lastObj, CelestialMath::Vector3 worldMouse);
			CelestialMath::Vector3 handleTracked(unsigned int time);

			CelestialMath::Vector3 getClosestPositionOnObj(Resources::GameGridObject* grid, CelestialMath::Vector3 mouse,CelestialMath::Vector3 origin) const;

			void handleInput(CrossHandlers::Message* currentMessage, unsigned int time);

			bool filterObjects(unsigned int* objects, unsigned int amounts, std::string filters);

			MouseHandler* mH;


	};
}
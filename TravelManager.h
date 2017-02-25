#pragma once
#include "CelestialList.h"
#include "GameTravelObject.h"
#include "RouteNodeObject.h"
#include "GameRouteObject.h"
#include "SerializableObject.h"
#include "Route.h"
#include "NodeGroup.h"
#include "PathFinder.h"
#include "RoutingBase.h"

namespace Entities
{

	class TravelManager : public Resources::SerializableObject, public RoutingBase
	{

		public:
			TravelManager(unsigned int maxClock);

			unsigned int* Update(unsigned int time, unsigned int &scripts, float timeDiff);

			void Travel(Resources::GameTravelObject* object, unsigned int goal, unsigned int time);
			void Spawn(Resources::GameTravelObject* object, unsigned int cell);
			void UpdateObject(Resources::GameTravelObject* obj, unsigned int time);

			virtual ~TravelManager();

		protected:
			virtual void init(
				CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* gameObjects,
				CrossHandlers::CelestialSlicedList<RouteNodeObject*>* routeNodes,
				CrossHandlers::CelestialSlicedList<Route*>* roads,
				CrossHandlers::CelestialSlicedList<NodeGroup*>* nodeGroups);

			void clearObjects();

			float getOffsetDist() const;
			void handlePrimedObject(Resources::GameTravelObject* obj);
			void handleQueing(Resources::GameTravelObject* obj, unsigned int localId, Route* route, float distSqr, unsigned int time);


		private:

			unsigned int maxSOuts;
			unsigned int* scriptOuts;

			unsigned int maxClock;
			unsigned int lastClock;
			unsigned int modClock;

			float offsetDist;

			PathFinder* pathFinder;

			float getObjDistSqr(Resources::GameTravelObject* obj, RouteNodeObject* node);
			void checkObjPastNode(Resources::GameTravelObject* obj, RouteNodeObject* currentNode);
			float travelObject(Resources::GameTravelObject* obj, RouteNodeObject* currentNode, RouteNodeObject* goalNode, CelestialMath::Vector3 dir, float distSqr, float timeDiff);
			bool updateObject(Resources::GameTravelObject* obj, unsigned int time, unsigned int &scriptPlace, float timeDiff);

			void handleNearNode(Resources::GameTravelObject* obj, RouteNodeObject* currentNode, RouteNodeObject* goalNode, RouteNodeObject* nextGoal, unsigned int time);
			bool handleNodeArrival(Resources::GameTravelObject* obj, RouteNodeObject* currentNode, RouteNodeObject* goalNode, RouteNodeObject* nextGoal);
			void handleTravel(Resources::GameTravelObject* obj, RouteNodeObject* currentNode, RouteNodeObject* goalNode, CelestialMath::Vector3 dir, float distSqr, float timeDiff, unsigned int time);

			unsigned int addToOutScripts(unsigned int script, unsigned int place);

	};
}
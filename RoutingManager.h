#pragma once

#include "SplitManager.h"

#include "CelestialList.h"
#include "GameTravelObject.h"
#include "RouteNodeObject.h"
#include "GameRouteObject.h"
#include "GameGridObject.h"
#include "SerializableObject.h"
#include "Route.h"
#include "NodeGroup.h"


namespace Entities
{

	class RoutingManager : public Resources::SerializableObject, public SplitManager
	{

		public:
			RoutingManager(unsigned int maxClock);
			void Init(CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* gameObjects);
			void AddNode(CelestialMath::Vector3 position, float width, unsigned int id);
			unsigned int AddNode(CelestialMath::Vector3 position, unsigned int* objects, unsigned int amounts);
			unsigned int FindNode(CelestialMath::Vector3 position, unsigned int* objects, unsigned int amounts);

			void AddRoad(unsigned int node1, unsigned int node2);
			unsigned int AddGroup(Resources::GameObject* object);

			void PopulateGrid(Resources::GameGridObject* grid, float nodeWidth);
			void ClearNodes();

			void HandleNode(unsigned int nodeId, unsigned int* objects, unsigned int amounts);
			RouteNodeObject* GetNode(unsigned int id) const;

			void ClearRoads(RouteNodeObject* node);

			//Serializable Interface
			virtual char* Serialize(unsigned int &size);
			virtual char* Unserialize(char* data);

			virtual ~RoutingManager();

		private:

			void addNewRoad(RouteNodeObject* node1, RouteNodeObject* node2);

			RouteNodeObject* getPreExistantNode(CelestialMath::Vector3 position, unsigned int* objects, unsigned int amounts) const;

			void handleObjectNode(RouteNodeObject* preExist, unsigned int* objects, unsigned int amounts);

	};
}
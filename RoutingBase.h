#pragma once
#include "CelestialList.h"
#include "RouteNodeObject.h"
#include "NodeGroup.h"
#include "GameTravelObject.h"

namespace Entities
{

	class RoutingBase
	{

		public:
			RoutingBase()
			{

				this->gameObjects = nullptr;
				this->travelObjects = nullptr;
				this->roads = nullptr;
				this->routeNodes = nullptr;
				this->nodeGroups = nullptr;

			}

		protected:

			virtual void init(
				CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* gameObjects,
				CrossHandlers::CelestialList<Resources::GameTravelObject*>* travelObjects,
				CrossHandlers::CelestialSlicedList<RouteNodeObject*>* routeNodes,
				CrossHandlers::CelestialSlicedList<Route*>* roads,
				CrossHandlers::CelestialSlicedList<NodeGroup*>* nodeGroups)
			{

				this->gameObjects = gameObjects;
				this->travelObjects = travelObjects;
				this->roads = roads;
				this->routeNodes = routeNodes;
				this->nodeGroups = nodeGroups;

			}

			CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* gameObjects;
			CrossHandlers::CelestialList<Resources::GameTravelObject*>* travelObjects;
			CrossHandlers::CelestialSlicedList<RouteNodeObject*>* routeNodes;
			CrossHandlers::CelestialSlicedList<Route*>* roads;
			CrossHandlers::CelestialSlicedList<NodeGroup*>* nodeGroups;

		public:
			virtual ~RoutingBase()
			{

				if (travelObjects != nullptr)
				{

					delete travelObjects;

				}

				if (routeNodes != nullptr)
				{

					routeNodes->KillList();
					delete routeNodes;

				}

				if (roads != nullptr)
				{

					roads->KillList();
					delete roads;

				}

				if (nodeGroups)
				{

					nodeGroups->KillList();
					delete nodeGroups;

				}
			}
	};
}
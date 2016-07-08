#pragma once

#include "RouteNodeObject.h"
#include "GameRouteObject.h"

namespace Entities
{

	class RoutingManager
	{

		public:
			RoutingManager();
			void Init(CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* gameObjects);
			unsigned int AddNode(CelestialMath::Vector3 position, Resources::GameRouteObject* obj);
			unsigned int AddNode(CelestialMath::Vector3 position, unsigned int* objects, unsigned int amounts);
			virtual ~RoutingManager();

		private:
			CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* gameObjects;
			CrossHandlers::CelestialSlicedList<RouteNodeObject*>* routeNodes;

	};
}
#pragma once

#include "RouteNodeObject.h"

namespace Entities
{

	class RoutingManager
	{

		public:
			RoutingManager();
			void Init(CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* gameObjects);
			virtual ~RoutingManager();

		private:
			CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* gameObjects;
	};
}
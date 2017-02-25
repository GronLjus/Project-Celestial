#pragma once
#include "RouteNodeObject.h"
#include "PrioQueue.h"

namespace Entities
{

	class PathFinder
	{
		public:
			PathFinder(CrossHandlers::CelestialSlicedList<RouteNodeObject*>* routeNodes);
			unsigned int PathFindOpenRoad(RouteNodeObject* at, RouteNodeObject* start, RouteNodeObject* end);
			unsigned int PathFind(RouteNodeObject* start, RouteNodeObject* through, RouteNodeObject* end);
			unsigned int PathFind(RouteNodeObject* start, RouteNodeObject* through, RouteNodeObject* end, float &dist);
			
			unsigned int* GetPath() const;

			~PathFinder();

		private:

			CrossHandlers::CelestialSlicedList<RouteNodeObject*>* routeNodes;

			void resetNodes();

			unsigned char pathFindVal;
			unsigned int* path;
			unsigned char totalPaths;

			
	};
}
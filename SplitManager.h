#pragma once
#include "CelestialList.h"
#include "RouteNodeObject.h"
#include "GameTravelObject.h"
#include "TravelManager.h"

namespace Entities
{

	class SplitManager : public TravelManager
	{

		public:
			virtual ~SplitManager() {}

		protected:
			SplitManager(unsigned int maxClock);

			enum objNode { objNode_ON, objNode_ABOVE, objNode_BELOW, objNode_NA };
			void handleSplit(RouteNodeObject* intersect, RouteNodeObject* lower, RouteNodeObject* upper);
			objNode shouldIntersect(RouteNodeObject* intersect, RouteNodeObject* from, RouteNodeObject* to, Resources::GameTravelObject* obj);

		private:

			float offsetDist;

	};
}
#pragma once
#include "GameObject.h"
#include "RouteNodeObject.h"

namespace Resources
{

	class GameRouteObject : public GameObject
	{

		public:
			GameRouteObject(CrossHandlers::BoundingBox* baseBox, CrossHandlers::BoundingSphere* baseSphere, unsigned int meshId);
			virtual GameObjectType GetType() const;

			unsigned int GetRoutenodes() const;
			RouteNodeObject* GetRouteNode(unsigned int localId) const;
			void AddRouteNode(RouteNodeObject* node);

			virtual ~GameRouteObject();

		private:
			CrossHandlers::CelestialSlicedList<RouteNodeObject*>* nodes;
			unsigned int routeNodes;

	};
}

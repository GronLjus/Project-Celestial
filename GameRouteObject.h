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
			Entities::RouteNodeObject* GetRouteNode(unsigned int localId) const;
			Entities::RouteNodeObject* GetRouteNode(CelestialMath::Vector3 position) const;
			unsigned int GetWidth() const;

			void SetWidth(unsigned int width);
			void AddRouteNode(Entities::RouteNodeObject* node);
			virtual ~GameRouteObject();

		private:
			CrossHandlers::CelestialSlicedList<Entities::RouteNodeObject*>* nodes;
			unsigned int routeNodes;
			unsigned int width;

	};
}

#pragma once
#include "GameObject.h"

namespace Resources
{

	class GameRouteObject : public GameObject
	{

		public:
			GameRouteObject(CrossHandlers::BoundingBox* baseBox, CrossHandlers::BoundingSphere* baseSphere, unsigned int meshId, std::string name);
			GameRouteObject();
			
			virtual GameObjectType GetType() const;

			void SetWidth(unsigned int width);

			unsigned int GetUpperNode() const;
			unsigned int GetMiddleNode() const;
			unsigned int GetLowerNode() const;
			unsigned int GetRoute() const;

			void SetUpperNode(unsigned int node);
			void SetMiddleNode(unsigned int node);
			void SetLowerNode(unsigned int node);
			void SetRoute(unsigned int route);
			
			unsigned int GetWidth() const;

			CrossHandlers::Intersection ContainsPoint(CelestialMath::Vector3 point);

			//Serializable Interface
			virtual char* Serialize(unsigned int &size);
			virtual char* Unserialize(char* data);

			virtual ~GameRouteObject();

		private:
			unsigned int upperNode;
			unsigned int middleNode;
			unsigned int lowerNode;
			unsigned int route;

			unsigned int width;

	};
}

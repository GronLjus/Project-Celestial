#pragma once

#include "BaseObject.h"
#include "CelestialMath.h"
#include "CelestialSlicedList.h"

namespace Resources
{

	class RouteNodeObject : public BaseObject
	{

	public:
		RouteNodeObject();
		RouteNodeObject(CelestialMath::Vector3 position, unsigned int width);
		virtual void Update(CrossHandlers::Message* mess);
		void SetPosition(CelestialMath::Vector3 pos);

		CelestialMath::Vector3 GetPosition() const;
		unsigned int GetObjId() const;
		RouteNodeObject* GetRoute(unsigned int localId, float &dist);
		unsigned int GetRoutes() const;

		void LinkObj(unsigned int objId);
		void AddRoute(RouteNodeObject* node);
		void RemoveRoute(unsigned int goalId);

		virtual ~RouteNodeObject();


	private:
		struct route
		{

			bool deleted;
			RouteNodeObject* goal;
			float distSQR;
			CelestialMath::Vector3 direction;

			route operator= (const route* rt) {return route(); }
			route() :deleted(true) {}
			~route() { deleted = true; }

		};

		unsigned int objId;
		
		CelestialMath::Vector3 position;
		unsigned int width;

		CrossHandlers::CelestialSlicedList<route>* routes;
		unsigned int maxRoutes;

	};
}
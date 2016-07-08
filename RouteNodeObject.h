#pragma once

#include "CelestialMath.h"
#include "CelestialSlicedList.h"

namespace Entities
{

	class RouteNodeObject
	{

	public:
		RouteNodeObject();
		RouteNodeObject(CelestialMath::Vector3 position, unsigned int width);
		void SetPosition(CelestialMath::Vector3 pos);

		CelestialMath::Vector3 GetPosition() const;
		unsigned int GetObjId() const;
		RouteNodeObject* GetRoute(unsigned int localId, float &dist);
		unsigned int GetRoutes() const;
		unsigned int GetWidth() const;

		void SetObjId(unsigned int id);
		void LinkObj(unsigned int objId);
		void AddRoute(RouteNodeObject* node);
		void RemoveRoute(unsigned int goalId);
		unsigned int GetId() const;
		void SetId(unsigned int id);

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

		unsigned int id;
		unsigned int objId;
		
		CelestialMath::Vector3 position;
		unsigned int width;

		CrossHandlers::CelestialSlicedList<route>* routes;
		unsigned int maxRoutes;

	};
}
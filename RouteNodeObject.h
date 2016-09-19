#pragma once

#include "CelestialMath.h"
#include "CelestialSlicedList.h"
#include "SerializableObject.h"
#include "Route.h"

namespace Entities
{

	class RouteNodeObject : public Resources::SerializableObject
	{

	public:

		RouteNodeObject();
		RouteNodeObject(CelestialMath::Vector3 position, float width);
		void SetPosition(CelestialMath::Vector3 pos);

		CelestialMath::Vector3 GetPosition() const;
		unsigned int GetObjId() const;
		Route* GetRoute(unsigned int localId);
		Route* GetRoute(unsigned int localId, Route::Direction &dir);
		unsigned int GetMaxRoutes() const;
		unsigned int GetRoutes() const;
		unsigned int GetWidth() const;

		unsigned int GetParent() const;
		unsigned char GetOpenSet() const;
		unsigned char GetClosedSet() const;

		unsigned int GetStep() const;

		unsigned int GetLocalId(unsigned int globalId) const;

		float GetHeuristic() const;

		void SetHeuristic(float heuristic);

		void SetObjId(unsigned int id);
		void LinkObj(unsigned int objId);
		void AddRoute(Route* node);
		void RemoveRoute(unsigned int goalId);

		unsigned int GetId() const;
		void SetId(unsigned int id);
		
		void SetParent(unsigned int parent);
		void SetOpenset(unsigned char open);
		void SetClosedset(unsigned char closed);
		
		void SetStep(unsigned int step);

		//Serializable Interface
		virtual char* Serialize(unsigned int &size);
		virtual char* Unserialize(char* data);

		virtual ~RouteNodeObject();


	private:
		struct route
		{

			bool deleted;
			Route* rte;
			Route::Direction dir;
			

			route operator= (const route* rt) {return route(); }
			route() :deleted(true) {}
			~route() { deleted = true; }

		};

		float heurustic;

		unsigned int id;
		unsigned int objId;
		
		CelestialMath::Vector3 position;

		float width;

		CrossHandlers::CelestialSlicedList<route>* routes;
		unsigned int routesAmount;

		unsigned char closedSet;
		unsigned char openSet;
		unsigned int parent;
		unsigned int step;

	};
}
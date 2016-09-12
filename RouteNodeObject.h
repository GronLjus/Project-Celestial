#pragma once

#include "CelestialMath.h"
#include "CelestialSlicedList.h"
#include "SerializableObject.h"
#include "Road.h"

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
		RouteNodeObject* GetRoute(unsigned int localId, float &dist);
		RouteNodeObject* GetRoute(unsigned int localId);
		unsigned int GetMaxRoutes() const;
		unsigned int GetRoutes() const;
		unsigned int GetWidth() const;
		unsigned int GetParent() const;
		unsigned char GetOpenSet() const;
		unsigned char GetClosedSet() const;
		unsigned int GetStep() const;
		unsigned int GetUpId() const;
		unsigned int GetDownId() const;
		float GetQuelength(unsigned int localId) const;
		unsigned int GetQTime(unsigned int localId) const;
		float GetQDiff(unsigned int localId) const;

		unsigned int GetLocalId(unsigned int globalId) const;
		bool CanTravel(unsigned int localId);

		unsigned int GetRoad() const;
		float GetHeuristic() const;

		void SetHeuristic(float heuristic);

		void SetRoad(unsigned int road);

		void SetObjId(unsigned int id);
		void LinkObj(unsigned int objId);
		void AddRoute(RouteNodeObject* node, Road::Direction direction);
		void AddRoute(RouteNodeObject* node);
		void RemoveRoute(unsigned int goalId);

		void SetDownId(unsigned int id);
		void SetUpId(unsigned int id);

		unsigned int GetId() const;
		void SetId(unsigned int id);
		void SetParent(unsigned int parent);
		void SetOpenset(unsigned char open);
		void SetClosedset(unsigned char closed);
		void SetStep(unsigned int step);

		void QueueRoute(unsigned int localId, float length, unsigned int time);
		void TravelRoute(unsigned int localId, unsigned int objId);
		void TravelDone(unsigned int localId, unsigned int objId);

		void SetDirection(unsigned int localId, Road::Direction dir);

		//Serializable Interface
		virtual char* Serialize(unsigned int &size);
		virtual char* Unserialize(char* data);

		virtual ~RouteNodeObject();


	private:
		struct route
		{

			bool deleted;
			RouteNodeObject* goal;
			float distSQR;
			CelestialMath::Vector3 direction;
			unsigned int lastObject;
			Road::Direction travelDirection;
			float qLength;

			float qDiff;
			unsigned int qTime;

			route operator= (const route* rt) {return route(); }
			route() :deleted(true) {}
			~route() { deleted = true; }

		};

		float heurustic;

		unsigned int road;

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

		unsigned int upId;
		unsigned int downId;

	};
}
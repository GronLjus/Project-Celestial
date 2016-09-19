#pragma once
#include "CelestialMath.h"

namespace Entities
{

	class Route
	{
		public:
			enum Direction{ Direction_UP, Direction_DOWN, Direction_NA};
			Route(float dist ,CelestialMath::Vector3 direction);

			Direction GetDirection() const; 
			unsigned int GetNode(Direction dir) const;

			float GetQuelength() const;
			unsigned int GetQTime() const;
			float GetQDiff() const;
			float GetDistance() const;

			bool CanTravel(Direction dir) const;
			unsigned int GetLastObj() const;

			void SetNode(unsigned int node, Direction dir);
			void Queue(float length, unsigned int time);
			void Travel(unsigned int obj, Direction dir);
			void TravelDone(unsigned int obj);

			~Route();

		private:
			unsigned int upnode;
			unsigned int downNode;
			Direction travelDir;

			float dist;
			CelestialMath::Vector3 direction;

			unsigned int lastObject;
			float qLength;
			float qDiff;
			unsigned int qTime;

	};
}
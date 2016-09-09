#pragma once
#include "GameObject.h"

namespace Resources
{

	enum TravelStatus{ TravelStatus_TRAVELING, TravelStatus_WAITING, TravelStatus_READY, TravelStatus_TRAVELNEAR, TravelStatus_PRIMED, TravelStatus_NA};

	class GameTravelObject : public GameObject
	{

		public:
			GameTravelObject(CrossHandlers::BoundingBox* baseBox, CrossHandlers::BoundingSphere* baseSphere, unsigned int meshId);
			GameTravelObject();
			
			virtual GameObjectType GetType() const;

			unsigned int GetGoalNode() const;
			unsigned int GetFinalGoalNode() const;
			unsigned int GetFilter() const;
			float GetSpeed() const;
			unsigned int GetLastTime() const;
			unsigned int GetNode() const;
			TravelStatus GetStatus() const;
			unsigned int PeekNextGoal(bool &reCalc) const;

			void SetStatus(TravelStatus status);
			void SetTravelSpeed(float cells);
			void SetGoalNode(unsigned int* goals);
			void SetFinalGoalNode(unsigned int goal);
			void SetFilter(unsigned int filter);
			void Time(unsigned int time);
			bool StepGoal();
			void SetNode(unsigned int node);

			//Serializable Interface
			virtual char* Serialize(unsigned int &size);
			virtual char* Unserialize(char* data);

			virtual ~GameTravelObject();

		private:
			unsigned int currentNode;
			unsigned char goalAmounts;
			unsigned char goal;
			unsigned int* goals;
			unsigned int finalGoal;
			unsigned int filter;
			unsigned int lastTime;
			float speed;
			TravelStatus status;

	};
}
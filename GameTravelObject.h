#pragma once
#include "GameObject.h"

namespace Resources
{

	class GameTravelObject : public GameObject
	{

		public:
			GameTravelObject(CrossHandlers::BoundingBox* baseBox, CrossHandlers::BoundingSphere* baseSphere, unsigned int meshId);
			virtual GameObjectType GetType() const;

			unsigned int GetGoalNode() const;
			unsigned int GetFinalGoalNode() const;
			unsigned int GetFilter() const;
			float GetSpeed() const;
			unsigned int GetLastTime() const;
			unsigned int GetNode() const;

			void SetTravelSpeed(float cells);
			void SetGoalNode(unsigned int* goals);
			void SetFinalGoalNode(unsigned int goal);
			void SetFilter(unsigned int filter);
			void Time(unsigned int time);
			bool StepGoal();
			void SetNode(unsigned int node);

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

	};
}
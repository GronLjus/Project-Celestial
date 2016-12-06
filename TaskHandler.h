#pragma once
#include "IHandleMessages.h"
#include "ITaskManager.h"
#include "CelestialSlicedList.h"

namespace Tasking
{

	class TaskHandler : public IHandleMessages
	{

		private:
			ITaskManager* subManager[Resources::TaskClass_NA];
			CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* gameObjects;

			unsigned int diff;

			unsigned int maxClock;
			unsigned int clock;
			unsigned int timeWarp;

			unsigned int lastTime;
			unsigned int sumTime;

		public:
			TaskHandler(unsigned int maxClock);
			void Init(CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* gameObjects);

			void Update(unsigned int time);
			void UpdateMessages(unsigned int time);

			unsigned int GetClock() const;

			float GetClockDiff() const;

			~TaskHandler();

	};
}
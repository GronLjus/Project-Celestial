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

		public:
			TaskHandler();
			void Init(CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* gameObjects);

			void Update(unsigned int time);
			void UpdateMessages(unsigned int time);

			~TaskHandler();

	};
}
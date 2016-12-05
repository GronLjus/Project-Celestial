#pragma once
#include "ITaskManager.h"

namespace Tasking
{

	class SystemTaskHandler : public ITaskManager
	{

		public:
			SystemTaskHandler();
			virtual void AddTask(Resources::TaskObject* object, unsigned int fireTime);
			virtual void RemoveTask(Resources::TaskObject* object);
			virtual void Update(unsigned int time);
			~SystemTaskHandler();

		private:
			CrossHandlers::CelestialStack<Resources::TaskObject*>* taskList;

			bool timeObject(Resources::TaskObject* object, unsigned int time);
			unsigned int lastTime;


	};
}
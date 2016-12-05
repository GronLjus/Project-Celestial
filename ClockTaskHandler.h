#pragma once
#include "ITaskManager.h"

namespace Tasking
{

	class ClockTaskHandler : public ITaskManager
	{

	public:
		ClockTaskHandler();
		virtual void AddTask(Resources::TaskObject* object, unsigned int fireTime);
		virtual void RemoveTask(Resources::TaskObject* object);
		virtual void Update(unsigned int time);
		~ClockTaskHandler();

	private:
		CrossHandlers::CelestialStack<Resources::TaskObject*>* taskList;

		bool timeObject(Resources::TaskObject* object, unsigned int time);
		unsigned int lastTime;

	};
}

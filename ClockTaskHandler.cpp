#include "stdafx.h"
#include "ClockTaskHandler.h"

using namespace Tasking;
using namespace Resources;

ClockTaskHandler::ClockTaskHandler()
{

	taskList = new CelestialStack<TaskObject*>(false);
	lastTime = 0;

}

void ClockTaskHandler::AddTask(TaskObject* object, unsigned int fireTime)
{

	object->Queue(TaskClass_CLOCK, fireTime);
	taskList->PushElement(object);

	if (lastTime > fireTime)//The time has passed
	{

		object->SetTime(1);

	}
}

void ClockTaskHandler::RemoveTask(TaskObject* object)
{

	object->Kill();

}

bool ClockTaskHandler::timeObject(TaskObject* object, unsigned int time)
{

	bool retVal = false;
	unsigned int lastFired = object->GetTime();

	if (lastFired == 0 && time > object->GetFireTime())
	{

		lastFired = 1;
		retVal = true;

	}
	else if (lastFired != 0 && time < lastTime)
	{

		lastFired = 0;

	}

	object->SetTime(lastFired);
	return retVal;

}

void ClockTaskHandler::Update(unsigned int time)
{

	while (taskList->GetCount() > 0)
	{

		TaskObject* task = taskList->PeekElement();

		if (task->ShouldKill())
		{

			taskList->Remove();

		}
		else
		{

			taskList->PopElement();

		}

		if (timeObject(task, time))
		{

			trigger(task, time);

		}
	}

	lastTime = time;
	taskList->Rewind();

}

ClockTaskHandler::~ClockTaskHandler()
{

	delete taskList;

}
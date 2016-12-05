#include "stdafx.h"
#include "SystemTaskHandler.h"

using namespace Tasking;
using namespace Resources;

SystemTaskHandler::SystemTaskHandler()
{

	taskList = new CelestialStack<TaskObject*>(false);
	lastTime = 0;

}

void SystemTaskHandler::AddTask(TaskObject* object, unsigned int fireTime)
{

	object->Queue(TaskClass_SYSTEM, fireTime);
	taskList->PushElement(object);

}

void SystemTaskHandler::RemoveTask(TaskObject* object)
{

	object->Kill();

}

bool SystemTaskHandler::timeObject(TaskObject* object, unsigned int time)
{

	bool retVal = false;
	unsigned int lastFired = object->GetTime() + time;

	if (lastFired >= object->GetFireTime())
	{

		lastFired = 0;
		retVal = true;

	}

	object->SetTime(lastFired);
	return retVal;

}

void SystemTaskHandler::Update(unsigned int time)
{

	unsigned int diff = time - lastTime;
	lastTime = time;

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

		if (timeObject(task, diff))
		{

			trigger(task, time);

		}
	}

	taskList->Rewind();

}

SystemTaskHandler::~SystemTaskHandler()
{

	delete taskList;

}
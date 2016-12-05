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

void SystemTaskHandler::Update(unsigned int time)
{

	unsigned int diff = time - lastTime;

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

		if (task->Time(diff))
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
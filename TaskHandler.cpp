#include "stdafx.h"
#include "TaskHandler.h"
#include "SystemTaskHandler.h"
#include "ClockTaskHandler.h"

using namespace Resources;
using namespace Tasking;
using namespace CrossHandlers;

TaskHandler::TaskHandler() : IHandleMessages(200, MessageSource_TASKS)
{

	subManager[TaskClass_CLOCK] = new ClockTaskHandler();
	subManager[TaskClass_SYSTEM] = new SystemTaskHandler();

	filter = MessageType_TASKING;

	clock = 0;
	lastTime = 0;
	sumTime = 0;
	timeWarp = 1000;

}

void TaskHandler::Init(CelestialSlicedList<BaseObject*>* gameObjects)
{

	this->gameObjects = gameObjects;

	if (subManager[TaskClass_CLOCK] != nullptr)
	{

		subManager[TaskClass_CLOCK]->Init(this->outQueue, this->messageBuffer, this->outMessages, this->currentMessage);

	}

	if (subManager[TaskClass_SYSTEM] != nullptr)
	{

		subManager[TaskClass_SYSTEM]->Init(this->outQueue, this->messageBuffer, this->outMessages, this->currentMessage);

	}
}

void TaskHandler::Update(unsigned int time)
{

	unsigned int diff = time - lastTime;
	lastTime = time;
	sumTime += diff;

	if (sumTime > timeWarp)
	{

		clock+= sumTime/timeWarp;
		clock %= maxClock;
		sumTime %= timeWarp;

	}

	subManager[TaskClass_SYSTEM]->Update(time);
	subManager[TaskClass_CLOCK]->Update(clock);

}

void TaskHandler::UpdateMessages(unsigned int time)
{

	Message* currentMessage = inQueue->PopMessage();

	while (currentMessage->type != MessageType_NA)
	{

		unsigned int task = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);
		unsigned int type = currentMessage->params[4] | ((int)currentMessage->params[5] << 8) | ((int)currentMessage->params[6] << 16) | ((int)currentMessage->params[7] << 24);

		if (currentMessage->mess == TaskMess_ADDTASK && 
			subManager[type] != nullptr)
		{

			unsigned int fTime = currentMessage->params[8] | ((int)currentMessage->params[9] << 8) | ((int)currentMessage->params[10] << 16) | ((int)currentMessage->params[11] << 24);
			TaskObject* taskObj = (TaskObject*)gameObjects->GetValue(task);
			subManager[type]->AddTask(taskObj, fTime);

		}
		else if (currentMessage->mess == TaskMess_REMOVETASK)
		{

			TaskObject* taskObj = (TaskObject*)gameObjects->GetValue(task);
			taskObj->Kill();
			//subManager[type]->RemoveTask(taskObj);

		}

		currentMessage = inQueue->PopMessage();

	}
}

unsigned int TaskHandler::GetClock() const
{

	return clock;

}

TaskHandler::~TaskHandler()
{


	for (unsigned int i = 0; i < TaskClass_NA; i++)
	{

		if (subManager[i] != nullptr)
		{

			delete subManager[i];

		}
	}
}
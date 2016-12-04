#include "stdafx.h"
#include "ITaskManager.h"

using namespace Tasking;
using namespace CrossHandlers;
using namespace Resources;

void ITaskManager::Init(MessageQueue* queue, Message* mBuffer, unsigned int maxMess, unsigned int &currentMess) 
{

	this->queue = queue;
	this->mBuffer = mBuffer;
	this->maxMess = maxMess;
	this->currentMess = currentMess;

}

void ITaskManager::addParam(unsigned int script, int value, unsigned int time)
{

	unsigned char tempBuff[]{ script >> 0, script >> 8, script >> 16, script >> 24,
		0.0f, 0.0f,0.0f,0.0f
	};
	memcpy(&(tempBuff[4]), &(value), 4);

	mBuffer[this->currentMess].timeSent = time;
	mBuffer[this->currentMess].destination = MessageSource_CELSCRIPT;
	mBuffer[this->currentMess].type = MessageType_SCRIPT;
	mBuffer[this->currentMess].mess = ScriptMess_ADDPARNUM;
	mBuffer[this->currentMess].read = false;
	mBuffer[this->currentMess].SetParams(tempBuff, 0, 8);
	queue->PushMessage(&mBuffer[this->currentMess]);
	this->currentMess = (this->currentMess + 1) % maxMess;


}

void ITaskManager::addParam(unsigned int script, float value, unsigned int time)
{

	unsigned char tempBuff[]{ script >> 0, script >> 8, script >> 16, script >> 24,
		0.0f, 0.0f,0.0f,0.0f
	};
	memcpy(&(tempBuff[4]), &(value), 4);

	mBuffer[this->currentMess].timeSent = time;
	mBuffer[this->currentMess].destination = MessageSource_CELSCRIPT;
	mBuffer[this->currentMess].type = MessageType_SCRIPT;
	mBuffer[this->currentMess].mess = ScriptMess_ADDPARFLOAT;
	mBuffer[this->currentMess].read = false;
	mBuffer[this->currentMess].SetParams(tempBuff, 0, 8);
	queue->PushMessage(&mBuffer[this->currentMess]);
	this->currentMess = (this->currentMess + 1) % maxMess;



}

void ITaskManager::addParam(unsigned int script, std::string value, unsigned int time)
{

	unsigned char tempBuff[]{ script >> 0, script >> 8, script >> 16, script >> 24
	};

	mBuffer[this->currentMess].timeSent = time;
	mBuffer[this->currentMess].destination = MessageSource_CELSCRIPT;
	mBuffer[this->currentMess].type = MessageType_SCRIPT;
	mBuffer[this->currentMess].mess = ScriptMess_ADDPARASTR;
	mBuffer[this->currentMess].read = false;
	mBuffer[this->currentMess].SetParams(tempBuff, 0, 4);
	mBuffer[this->currentMess].SetParams((unsigned char*)value.c_str(), 4, 4);
	queue->PushMessage(&mBuffer[this->currentMess]);
	this->currentMess = (this->currentMess + 1) % maxMess;


}

void ITaskManager::runScript(unsigned int script, unsigned int time)
{

	unsigned char tempBuff[]{ script >> 0, script >> 8, script >> 16, script >> 24 };
	mBuffer[this->currentMess].timeSent = time;
	mBuffer[this->currentMess].destination = MessageSource_CELSCRIPT;
	mBuffer[this->currentMess].type = MessageType_SCRIPT;
	mBuffer[this->currentMess].mess = ScriptMess_RUN;
	mBuffer[this->currentMess].read = false;
	mBuffer[this->currentMess].SetParams(tempBuff, 0, 4);
	queue->PushMessage(&mBuffer[this->currentMess]);
	this->currentMess = (this->currentMess + 1) % maxMess;

}

void ITaskManager::trigger(TaskObject* task, unsigned int time)
{

	addParam(task->GetScript(), (int)task->GetTargetId(), time);

	while (task->GetIParams()->GetCount() > 0)
	{

		addParam(task->GetScript(), task->GetIParams()->PopElement(), time);

	}

	while (task->GetFParams()->GetCount() > 0)
	{

		addParam(task->GetScript(), task->GetFParams()->PopElement(), time);

	}

	while (task->GetSParams()->GetCount() > 0)
	{

		addParam(task->GetScript(), task->GetSParams()->PopElement(), time);

	}

	task->GetIParams()->Rewind();
	task->GetFParams()->Rewind();
	task->GetSParams()->Rewind();

	runScript(task->GetScript(), time);
}

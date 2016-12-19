#include "stdafx.h"
#include "ITaskManager.h"

using namespace Tasking;
using namespace CrossHandlers;
using namespace Resources;

void ITaskManager::Init(MessageBuffer* mBuffer) 
{

	this->mBuffer = mBuffer;

}

void ITaskManager::addParam(unsigned int script, int value, unsigned int time)
{

	unsigned char tempBuff[]{ script >> 0, script >> 8, script >> 16, script >> 24,
		0.0f, 0.0f,0.0f,0.0f
	};
	memcpy(&(tempBuff[4]), &(value), 4);

	Message* msg = mBuffer->GetCurrentMess();

	msg->timeSent = time;
	msg->destination = MessageSource_CELSCRIPT;
	msg->type = MessageType_SCRIPT;
	msg->mess = ScriptMess_ADDPARNUM;
	msg->read = false;
	msg->SetParams(tempBuff, 0, 8);

	mBuffer->PushMessageOut();

}

void ITaskManager::addParam(unsigned int script, float value, unsigned int time)
{

	unsigned char tempBuff[]{ script >> 0, script >> 8, script >> 16, script >> 24,
		0.0f, 0.0f,0.0f,0.0f
	};
	memcpy(&(tempBuff[4]), &(value), 4);

	Message* msg = mBuffer->GetCurrentMess();

	msg->timeSent = time;
	msg->destination = MessageSource_CELSCRIPT;
	msg->type = MessageType_SCRIPT;
	msg->mess = ScriptMess_ADDPARFLOAT;
	msg->read = false;
	msg->SetParams(tempBuff, 0, 8);

	mBuffer->PushMessageOut();

}

void ITaskManager::addParam(unsigned int script, std::string value, unsigned int time)
{

	unsigned char tempBuff[]{ script >> 0, script >> 8, script >> 16, script >> 24
	};

	Message* msg = mBuffer->GetCurrentMess();

	msg->timeSent = time;
	msg->destination = MessageSource_CELSCRIPT;
	msg->type = MessageType_SCRIPT;
	msg->mess = ScriptMess_ADDPARASTR;
	msg->read = false;
	msg->SetParams(tempBuff, 0, 4);
	msg->SetParams((unsigned char*)value.c_str(), 4, 4);

	mBuffer->PushMessageOut();

}

void ITaskManager::runScript(unsigned int script, unsigned int time)
{

	unsigned char tempBuff[]{ script >> 0, script >> 8, script >> 16, script >> 24 };

	Message* msg = mBuffer->GetCurrentMess();

	msg->timeSent = time;
	msg->destination = MessageSource_CELSCRIPT;
	msg->type = MessageType_SCRIPT;
	msg->mess = ScriptMess_RUN;
	msg->read = false;
	msg->SetParams(tempBuff, 0, 4);

	mBuffer->PushMessageOut();

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

#include "stdafx.h"
#include "TaskObject.h"

using namespace CrossHandlers;
using namespace Resources;

TaskObject::TaskObject(unsigned int script,
	CelestialStack<int>* iParams,
	CelestialStack<float>* fParams,
	CelestialStack<std::string>* sParams)
{

	this->script = script;
	this->iParams = iParams;
	this->fParams = fParams;
	this->sParams = sParams;

	type = TaskClass_NA;
	kill = false;

}

char* TaskObject::Serialize(unsigned int &size)
{

	unsigned int iVal = iParams->GetCount();
	unsigned int fVal = fParams->GetCount();
	unsigned int sVal = 0;

	unsigned int sCount = sParams->GetCount();

	for (unsigned int i = 0; i < sCount; i++)
	{

		std::string val = sParams->PopElement();
		sVal += val.size();

	}

	sParams->Rewind();

	size = 1 + 4*4 + (iParams->GetCount()*4) + sVal + (fParams->GetCount()*4);

	char* byteVal = new char[size];

	byteVal[0] = SerializableType_TASK;

	memcpy(&byteVal[1], &script, sizeof(unsigned int));
	memcpy(&byteVal[5], &iVal, sizeof(unsigned int));
	unsigned int offset = 9;

	for (unsigned int i = 0; i < iVal; i++)
	{

		unsigned int value = iParams->PopElement();
		memcpy(&byteVal[offset], &value, sizeof(int));
		offset += sizeof(int);

	}

	iParams->Rewind();
	memcpy(&byteVal[offset], &fVal, sizeof(unsigned int));
	offset += sizeof(unsigned int);

	for (unsigned int i = 0; i < fVal; i++)
	{

		float value = fParams->PopElement();
		memcpy(&byteVal[offset], &value, sizeof(float));
		offset += sizeof(float);

	}

	fParams->Rewind();
	memcpy(&byteVal[offset], &sVal, sizeof(unsigned int));
	offset += sizeof(unsigned int);

	for (unsigned int i = 0; i < sCount; i++)
	{

		std::string val = sParams->PopElement();

		memcpy(&byteVal[offset], val.c_str(), val.size());
		offset += val.size();

	}

	return byteVal;

}

char* TaskObject::Unserialize(char* data)
{

	iParams = new CelestialStack<int>(false);
	fParams = new CelestialStack<float>(false);
	sParams = new CelestialStack<std::string>(false);

	unsigned int iVal = 0;
	unsigned int fVal = 0;
	unsigned int sVal = 0;

	memcpy(&script, data, sizeof(unsigned int));
	memcpy(&iVal, &data[4], sizeof(unsigned int));

	unsigned int offset = 8;

	for (unsigned int i = 0; i < iVal; i++)
	{
		int val = 0;
		memcpy(&val, &data[offset], sizeof(int));
		iParams->PushElement(val);
		offset += sizeof(int);
		
	}

	memcpy(&fVal, &data[offset], sizeof(unsigned int));
	offset += sizeof(unsigned int);

	for (unsigned int i = 0; i < fVal; i++)
	{
		float val = 0;
		memcpy(&val, &data[offset], sizeof(float));
		fParams->PushElement(val);
		offset += sizeof(float);

	}

	memcpy(&sVal, &data[offset], sizeof(unsigned int));
	offset += sizeof(unsigned int);
	unsigned int end = offset + sVal;

	while (offset < end)
	{

		std::string value = std::string(&data[offset]);
		sParams->PushElement(value);
		offset += value.size();

	}

	return nullptr;

}

CelestialStack<int>* TaskObject::GetIParams() const
{

	return iParams;

}

CelestialStack<float>* TaskObject::GetFParams() const
{

	return fParams;

}

CelestialStack<std::string>* TaskObject::GetSParams() const
{

	return sParams;

}

unsigned int TaskObject::GetScript() const
{

	return script;

}

TaskClass TaskObject::GetType() const
{

	return type;

}

void TaskObject::Queue(TaskClass type, unsigned int fireTime)
{

	this->type = type;
	lastFired = 0;
	this->fireTime = fireTime;

}

unsigned int TaskObject::GetTime() const
{

	return lastFired;

}

unsigned int TaskObject::GetFireTime() const
{

	return fireTime;

}

void TaskObject::SetTime(unsigned int time)
{

	lastFired = time;

}

void TaskObject::Kill()
{

	kill = true;

}

bool TaskObject::ShouldKill() const
{

	return kill;

}

void TaskObject::Update(CrossHandlers::Message* mess) {}

TaskObject::~TaskObject()
{

	delete iParams;
	delete fParams;
	delete sParams;

}
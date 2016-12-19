#include "stdafx.h"
#include "MessageBuffer.h"

using namespace CrossHandlers;


MessageBuffer::MessageBuffer(unsigned int startMax,
	MessageQueue* outQueue,
	MessageSource source)
{

	this->source = source;
	currentMess = 0;
	maxMess = startMax;
	buffer = new Message*[maxMess];

	this->outQueue = outQueue;

	for (int i = 0; i < maxMess; i++)
	{

		buffer[i] = new Message();
		buffer[i]->source = source;
		buffer[i]->read = true;

	}
}

Message* MessageBuffer::GetCurrentMess() const
{

	return buffer[currentMess];

}

void MessageBuffer::PushMessageOut()
{

	buffer[currentMess]->read = false;
	outQueue->PushMessage(buffer[currentMess]);

	currentMess++;
	currentMess %= maxMess;

	if (buffer[currentMess]->read == false)
	{

		currentMess = maxMess + 1;
		expandBuffer();

	}
}

void MessageBuffer::expandBuffer()
{

	maxMess += 100;
	Message** newBuff = new Message*[maxMess];

	//TODO: Performance check, likely no bottleneck but gains could be possible
	for (unsigned int i = 0; i < maxMess - 100; i++)
	{

		newBuff[i] = buffer[i];

	}

	for (unsigned int i = maxMess - 100; i < maxMess; i++)
	{

		newBuff[i] = new Message();
		newBuff[i]->source = source;
		newBuff[i]->read = true;

	}

	delete[] buffer;
	buffer = newBuff;

}

MessageBuffer::~MessageBuffer()
{

	for (unsigned int i = 0; i < maxMess; i++)
	{

		delete buffer[i];

	}

	delete[] buffer;

}
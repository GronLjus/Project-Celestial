#include "stdafx.h"
#include "IHandleMessages.h"
using namespace CrossHandlers;

IHandleMessages::IHandleMessages(unsigned int outMessages, MessageSource ms)
{

	outQueue = new MessageQueue();
	inQueue = new MessageQueue();
	mBuffer = new MessageBuffer(outMessages, outQueue, ms);
	filter = MessageType_NA;

}

MessageQueue* IHandleMessages::GetMessages()
{

	return outQueue;

}

void IHandleMessages::HandleMessage(Message* mess)
{

	if (mess->type == filter)
	{

		inQueue->PushMessage(mess);

	}

}

IHandleMessages::~IHandleMessages()
{

	delete inQueue;
	delete outQueue;
	delete mBuffer;

}
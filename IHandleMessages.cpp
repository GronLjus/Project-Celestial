#include "stdafx.h"
#include "IHandleMessages.h"
using namespace CrossHandlers;

IHandleMessages::IHandleMessages(unsigned int outMessages, MessageSource ms)
{

	outQueue = new MessageQueue();
	inQueue = new MessageQueue();
	filter = MessageType_NA;

	currentMessage = 0;
	outMessages = 200;
	messageBuffer = new Message[outMessages];

	for (int i = 0; i < outMessages; i++)
	{

		messageBuffer[i].source = ms;
		messageBuffer[i].read = true;

	}
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
	delete[] messageBuffer;

}
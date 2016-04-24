#include "stdafx.h"
#include "MessageQueue.h"

using namespace CrossHandlers;

MessageQueue::MessageQueue()
{

	queueCount = 0; 
	baseMess.destination = MessageSource_NA;
	baseMess.source = MessageSource_NA;
	baseMess.type = MessageType_NA;
	readQueue = new CelestialListNode<Message*>(&baseMess);
	addQueue = readQueue;

}

void MessageQueue::PushMessage(Message* mess)
{

	lock.lock();
	
	if (queueCount > 0)
	{

		if (addQueue->GetNext() != nullptr)
		{

			addQueue->GetNext()->SetNodeObject(mess);

		}
		else
		{

			addQueue->SetNext(new CelestialListNode<Message*>(mess));

		}

		addQueue = addQueue->GetNext();

	}
	else
	{

		addQueue->SetNodeObject(mess);
	
	}

	queueCount++;
	lock.unlock();

	
}

Message* MessageQueue::PopMessage()
{

	lock.lock();
	Message* mess = &baseMess;

	if (queueCount > 0)
	{

		mess = readQueue->GetNodeObject();
		queueCount--;

		if (queueCount == 0)
		{

			addQueue = readQueue;

		}
		else
		{

			CelestialListNode<Message*>* nextRead = readQueue->GetNext();
			readQueue->SetNext(addQueue->GetNext());
			addQueue->SetNext(readQueue);
			readQueue = nextRead;

		}
	}

	lock.unlock();
	return mess;

}

MessageQueue::~MessageQueue()
{

	while (readQueue != nullptr)
	{

		CelestialListNode<Message*>* newqueue = readQueue->GetNext();
		delete readQueue;
		readQueue = newqueue;
	
	}
}
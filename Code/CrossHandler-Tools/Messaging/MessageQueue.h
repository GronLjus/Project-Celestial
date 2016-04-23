#pragma once
#include "Message.h"
#include "CelestialListNode.h"
#include <mutex>

namespace CrossHandlers
{

	class MessageQueue
	{
	private:
		CelestialListNode<Message*>* readQueue;
		CelestialListNode<Message*>* addQueue;
		unsigned int queueCount;
		std::mutex lock;
		Message baseMess;

	public:
		MessageQueue();
		void PushMessage(Message* mess);
		Message* PopMessage();
		~MessageQueue();
	};
}
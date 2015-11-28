#pragma once
#include "MessageQueue.h"

namespace CrossHandlers
{

	class IHandleMessages
	{

		public:
			IHandleMessages(unsigned int outMessages,MessageSource ms);
			virtual MessageQueue* GetMessages();
			virtual void HandleMessage(Message* mess);
			~IHandleMessages();

		protected:
			MessageQueue* inQueue;
			MessageQueue* outQueue;
			MessageType filter;
			Message* messageBuffer;
			unsigned int outMessages;
			unsigned int currentMessage;

	};
}
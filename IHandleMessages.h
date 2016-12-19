#pragma once
#include "MessageBuffer.h"

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
			MessageBuffer* mBuffer;
			MessageType filter;

	};
}
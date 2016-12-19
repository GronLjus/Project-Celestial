#pragma once
#include "MessageQueue.h"
#include "Message.h"

namespace CrossHandlers
{

	class MessageBuffer
	{
		public:
			MessageBuffer(unsigned int startMax,
				MessageQueue* outQueue,
				MessageSource source);

			Message* GetCurrentMess() const;
			void PushMessageOut();

			~MessageBuffer();

		private:
			void expandBuffer();
			MessageSource source;
			MessageQueue* outQueue;
			Message** buffer;
			unsigned int currentMess;
			unsigned int maxMess;

	};
}
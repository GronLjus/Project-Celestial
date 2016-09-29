#pragma once
#include "RunTimeOperations.h"
#include "MessageQueue.h"
#include "HeapMemory.h"

namespace Logic
{

	class KubLingMachine
	{

		private:

			float fReg[4];
			int iReg[4];
			char cReg[4];

			CrossHandlers::MessageQueue* queue;
			CrossHandlers::Message* mBuffer;
			unsigned int maxMess;
			unsigned int currentMess;
			CrossHandlers::Message* lastMess;

			unsigned int time;
			void sendMessage(unsigned int mess, CrossHandlers::MessageSource dest);

			const unsigned short wait = 512;
			unsigned char stackMem[524287];
			HeapMemory* heapMem;

		public:
			KubLingMachine(CrossHandlers::MessageQueue* queue,
				CrossHandlers::Message* mBuffer,
				unsigned int maxMess,
				unsigned int currentMess);

			RunTimeError RunScript(unsigned long long* code, unsigned int &counter, unsigned int time);

			~KubLingMachine();

	};
}
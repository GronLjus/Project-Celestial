#pragma once
#include "RunTimeOperations.h"
#include "MessageQueue.h"
#include "HeapMemory.h"
#include "CelestialSlicedList.h"
#include "BaseObject.h"

namespace Logic
{

	class KubLingMachine
	{

		private:

			float fReg[4];
			int iReg[5];
			char cReg[4];

			CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* objectContainer;

			CrossHandlers::MessageQueue* queue;
			CrossHandlers::Message* mBuffer;
			unsigned int maxMess;
			unsigned int currentMess;
			CrossHandlers::Message* lastMess;

			unsigned int time;
			void sendMessage(unsigned int mess, unsigned int retVar, CrossHandlers::MessageSource dest, unsigned int returnAdr, unsigned int sender);

			const unsigned short wait = 512;
			char* stackMem;
			HeapMemory* heapMem;

			bool kill;

			char* getMem(unsigned int adr) const;
			unsigned int maxStack;

		public:
			KubLingMachine(CrossHandlers::MessageQueue* queue,
				CrossHandlers::Message* mBuffer,
				unsigned int maxMess,
				unsigned int &currentMess,
				char* stackMem,
				unsigned int maxStack,
				CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* objectContainer);

			void SetHeap(HeapMemory* heap);
			void SetRegister(unsigned char reg, unsigned int value);
			void Kill();

			RunTimeError RunScript(unsigned long long* code, unsigned int &counter, unsigned int time, unsigned int sender);

			~KubLingMachine();

	};
}
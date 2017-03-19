#pragma once
#include "RunTimeOperations.h"
#include "MessageBuffer.h"
#include "HeapMemory.h"
#include "CelestialSlicedList.h"
#include "BaseObject.h"

namespace Logic
{

	class KubLingMachine
	{

		enum registers{ registers_INT = 6, registers_FLOAT = 4, registers_ADR = 4, registers_CHAR = 4};

		private:

			float fReg[registers_FLOAT];
			int iReg[registers_INT];
			unsigned int aReg[registers_ADR];
			char cReg[registers_CHAR];

			CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* objectContainer;

			CrossHandlers::MessageBuffer* mBuffer;
			CrossHandlers::Message* lastMess;

			unsigned int time;
			void sendMessage(unsigned int mess, unsigned int retVar, CrossHandlers::MessageSource dest, unsigned int returnAdr, unsigned int sender);

			const unsigned short wait = 512;

			unsigned int memOffset;
			unsigned int memStart;

			char* stackMem;
			HeapMemory* heapMem;

			bool kill;

			char* getMem(unsigned int adr) const;
			char* getAbsMem(unsigned int adr) const;

			unsigned int adr(unsigned int adr, unsigned int offset);
			unsigned int resolveAdr(unsigned int adr) const;

			unsigned int maxStack;

		public:
			KubLingMachine(CrossHandlers::MessageBuffer* mBuffer,
				char* stackMem,
				unsigned int maxStack,
				CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* objectContainer);

			void SetMemStart(unsigned int offset);
			void SetMemOffset(unsigned int offset);

			void SetHeap(HeapMemory* heap);
			void SetRegister(unsigned char reg, unsigned int value);
			void Kill();

			unsigned int GetMemOffset() const;

			RunTimeError RunScript(unsigned long long* code, unsigned int &counter, unsigned int time, unsigned int sender);

			~KubLingMachine();

	};
}
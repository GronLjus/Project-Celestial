#include "stdafx.h"
#include "KubLingMachine.h"

using namespace Logic;
using namespace CrossHandlers;

KubLingMachine::KubLingMachine(MessageBuffer* mBuffer,
	char* stackMem,
	unsigned int maxStack,
	CelestialSlicedList<Resources::BaseObject*>* objectContainer)
{

	this->objectContainer = objectContainer;
	this->mBuffer = mBuffer;
	lastMess = mBuffer->GetCurrentMess();

	iReg[4] = 0;

	this->stackMem = stackMem;
	this->maxStack = maxStack;
	kill = false;

}

void KubLingMachine::SetHeap(HeapMemory* heap)
{

	this->heapMem = heap;

}

void KubLingMachine::sendMessage(unsigned int mess, unsigned int retVar, MessageSource dest, unsigned int returnAdr, unsigned int sender)
{

	lastMess->timeSent = time;
	lastMess->destination = dest;
	lastMess->mess = mess;
	lastMess->returnParam = returnAdr;
	lastMess->returnMess = retVar;
	lastMess->senderId = sender;

	lastMess->type = dest == MessageSource_CELSCRIPT ? MessageType_SCRIPT :
		dest == MessageSource_ENTITIES ? MessageType_ENTITIES :
		dest == MessageSource_GRAPHICS ? MessageType_GRAPHICS :
		dest == MessageSource_GUIENTITIES ? MessageType_GUIENTITIES :
		dest == MessageSource_INPUT ? MessageType_INPUT :
		dest == MessageSource_OBJECT ? MessageType_OBJECT :
		dest == MessageSource_RESOURCES ? MessageType_RESOURCES :
		dest == MessageSource_TASKS ? MessageType_TASKING :
		MessageType_SYSTEM;

	if (dest != MessageSource_OBJECT)
	{

		if (returnAdr- iReg[4] > maxStack)
		{

			lastMess->returnParam -= iReg[4];

		}

		mBuffer->PushMessageOut();
		lastMess = mBuffer->GetCurrentMess();

	}
	else
	{

		unsigned int objId;
		memcpy(&objId, getMem(returnAdr), 4);

		Resources::BaseObject* obj = objectContainer->GetValue(objId);
		unsigned char* retValue = obj->Update(lastMess);

		if (retValue != nullptr)
		{

			memcpy(heapMem->GetMemory(heapMem->GetAddress(SystemMem_TRANSLATED)), retValue, 16);

		}
	}
}

void KubLingMachine::Kill()
{

	kill = true;

}

void KubLingMachine::SetRegister(unsigned char reg, unsigned int value)
{

	iReg[reg] = value;

}

char* KubLingMachine::getMem(unsigned int adr) const
{

	if (adr - iReg[4] >= maxStack)
	{

		adr -= maxStack + iReg[4];
		return heapMem->GetMemory(adr);

	}
	else
	{

		return &stackMem[adr];

	}
}

unsigned int KubLingMachine::adr(unsigned int adr, unsigned int offset)
{

	if (adr >= maxStack)
	{

		unsigned int var = adr - maxStack;
		adr = maxStack + heapMem->GetAddress(var);

	}

	return adr+offset;

}

RunTimeError KubLingMachine::RunScript(unsigned long long* code, unsigned int &counter, unsigned int time, unsigned int sender)
{

	std::string s = "";
	const char* st;
	unsigned int size;
	this->time = time;
	bool stop = false;

	while (!stop && !kill)
	{

		unsigned long long line = code[counter];
		opcode op = opcode(line >> 59);

		unsigned char reg1 = (line & 0x600000000000000) >> 57;
		unsigned char reg2 = (line & 0x180000000000000) >> 55;
		unsigned char reg3 = (line & 0x60000000000000) >> 53;
		unsigned char type = (line & 0x1FFFFF00000000) >> 32;

		unsigned int scal = (line & 0xFFFFFFFF);

		char* src;
		char* dst;

		switch (op)
		{
		case Logic::opcode_STOP:
			return RunTimeError_OK;
		case Logic::opcode_LOAD:

			if (type == 0)
			{

				memcpy(&iReg[reg1], getMem(aReg[reg2] + iReg[4]), sizeof(unsigned int));

			}
			else if (type == 1)
			{

				memcpy(&fReg[reg1], getMem(aReg[reg2] + iReg[4]), sizeof(float));

			}
			else
			{

				memcpy(&cReg[reg1], getMem(aReg[reg2] + iReg[4]), sizeof(char));

			}
			break;
		case Logic::opcode_CAST:

			if (type == 0)
			{

				iReg[reg1] = fReg[reg2];

			}
			else if (type == 1)
			{

				fReg[reg1] = iReg[reg2];

			}
			else
			{

				cReg[reg1] = iReg[reg2];

			}
			break;
		case Logic::opcode_TO:

			if (type == 0)
			{

				iReg[reg1] = cReg[reg2];

			}
			else if(type == 2)
			{

				cReg[reg1] = iReg[reg2];

			}
			else if (type == 3)
			{

				aReg[reg1] = iReg[reg2];

			}
			else if (type == 4)
			{

				iReg[reg1] = aReg[reg2];

			}
			break;
		case Logic::opcode_STORE:

			if (type == 0)
			{

				memcpy(getMem(aReg[reg2] + iReg[4]), &iReg[reg1], sizeof(unsigned int));

			}
			else if (type == 1)
			{

				memcpy(getMem(aReg[reg2] + iReg[4]), &fReg[reg1], sizeof(float));

			}
			else
			{

				memcpy(getMem(aReg[reg2] + iReg[4]), &cReg[reg1], sizeof(char));

			}
			break;
		case Logic::opcode_PLACE:

			if (type == 0)
			{

				memcpy(&iReg[reg1], &scal, sizeof(unsigned int));

			}
			else if (type == 1)
			{

				memcpy(&fReg[reg1], &scal, sizeof(float));

			}
			else if(type == 2)
			{

				memcpy(&cReg[reg1], &scal, 1);

			}
			else if (type == 3)
			{

				memcpy(&aReg[reg1], &scal, sizeof(unsigned int));

			}
			else
			{

				iReg[4] = scal;

			}
			break;
		case Logic::opcode_PLACEADR:

			if (type == 0)
			{

				aReg[reg3] = adr(iReg[reg1], iReg[reg2]);

			}
			else
			{

				iReg[reg3] = adr(iReg[reg1] + iReg[4], iReg[reg2]);

			}
			break;
		case Logic::opcode_SAVE:
			memcpy(getMem(aReg[reg1] + iReg[4]), &scal, iReg[reg2]);
			break;
		case Logic::opcode_COPY:
			dst = getMem(aReg[reg1] + iReg[4]);
			src = getMem(aReg[reg2] + iReg[4]);
			memcpy(dst, src, iReg[reg3]);
			break;
		case Logic::opcode_SEND:
			sendMessage(iReg[reg1], cReg[0] + iReg[4], MessageSource(iReg[reg2]), aReg[reg3] + iReg[4],sender);
			break;
		case Logic::opcode_STPRM:
			lastMess->SetParams((unsigned char*)getMem(aReg[reg1] + iReg[4]), iReg[reg2], iReg[reg3]);
			break;
		case Logic::opcode_PRM:
			lastMess->SetParam(scal, iReg[reg1]);
			break;
		case Logic::opcode_ADD:

			if (type == 0)
			{

				iReg[reg3] = iReg[reg1] + iReg[reg2];

			}
			else if (type == 1)
			{

				fReg[reg3] = fReg[reg1] + fReg[reg2];

			}
			else if(type == 2)
			{

				cReg[reg3] = cReg[reg1] + cReg[reg2];

			}
			else
			{

				aReg[reg3] = aReg[reg1] + iReg[reg2];

			}
			break;
		case Logic::opcode_SUB:

			if (type == 0)
			{

				iReg[reg3] = iReg[reg1] - iReg[reg2];

			}
			else if (type == 1)
			{

				fReg[reg3] = fReg[reg1] - fReg[reg2];

			}
			else if(type == 2)
			{

				cReg[reg3] = cReg[reg1] - cReg[reg2];

			}
			else if (type == 3)
			{

				aReg[reg3] = aReg[reg1] - iReg[reg2];

			}
			break;
		case Logic::opcode_MUL:

			if (type == 0)
			{

				iReg[reg3] = iReg[reg1] * iReg[reg2];

			}
			else if (type == 1)
			{

				fReg[reg3] = fReg[reg1] * fReg[reg2];

			}
			else
			{

				cReg[reg3] = cReg[reg1] * cReg[reg2];

			}
			break;
		case Logic::opcode_DIV:

			if (type == 0)
			{

				iReg[reg3] = iReg[reg1] / iReg[reg2];

			}
			else if (type == 1)
			{

				fReg[reg3] = fReg[reg1] / fReg[reg2];

			}
			else
			{

				cReg[reg3] = cReg[reg1] / cReg[reg2];

			}
			break;
		case Logic::opcode_MOD:

			if (type == 0)
			{

				iReg[reg3] = iReg[reg1] % iReg[reg2];

			}
			else
			{

				cReg[reg3] = cReg[reg1] % cReg[reg2];

			}
			break;
		case Logic::opcode_CMPREBYTES:
			cReg[reg3] = memcmp(getMem(aReg[reg1]), getMem(aReg[reg2]), iReg[reg3]);
			cReg[reg3] = cReg[reg3] == 0 ? 255 : 0;
			break;
		case Logic::opcode_CMPRE:

			if (type == 0)
			{

				cReg[reg3] = iReg[reg1] == iReg[reg2] ? 255 : 0;

			}
			else if (type == 1)
			{

				cReg[reg3] = fReg[reg1] == fReg[reg2] ? 255 : 0;

			}
			else
			{

				cReg[reg3] = cReg[reg1] == cReg[reg2] ? 255 : 0;

			}
			break;
		case Logic::opcode_GRTR:

			if (type == 0)
			{

				cReg[reg3] = iReg[reg1] > iReg[reg2] ? 255 : 0;

			}
			else if (type == 1)
			{

				cReg[reg3] = fReg[reg1] > fReg[reg2] ? 255 : 0;

			}
			else if(type == 2)
			{

				cReg[reg3] = cReg[reg1] > cReg[reg2] ? 255 : 0;

			}
			else
			{

				cReg[reg3] = aReg[reg1] > aReg[reg2] ? 255 : 0;

			}
			break;
		case Logic::opcode_BITCMP:

			if (type == 0)
			{

				cReg[reg3] = (iReg[reg1] >> scal) & 0x1;

			}
			else if (type == 2)
			{

				cReg[reg3] = (cReg[reg1] >> scal) & 0x1;

			}
			break;
		case Logic::opcode_INV:
			
			if (type == 0)
			{

				iReg[reg1] = ~iReg[reg1];

			}
			else if (type == 2)
			{

				cReg[reg1] = ~cReg[reg1];

			}
			break;
		case Logic::opcode_JMPIF:

			if (cReg[reg1] != 0)
			{

				counter = iReg[reg2] - 1;

			}

			break;
		case Logic::opcode_JMPINV:

			if (cReg[reg1] != 0)
			{

				break;

			}
		case Logic::opcode_JMP:
			counter = iReg[reg2] - 1;
			break;
		case Logic::opcode_RETURN:
			counter++;
			return RunTimeError(iReg[reg1]);
			break;
		case Logic::opcode_FTS:
			s = std::to_string(fReg[reg1]);
			st = s.c_str();
			size = s.size();
			memcpy(getMem(aReg[reg2] + iReg[4]), &size,4);
			memcpy(getMem(aReg[reg2] + iReg[4] + 4), st, s.size());
			break;
		case Logic::opcode_ADR:
			iReg[reg2] = heapMem->GetAddress(aReg[reg1]);
			break;
		case Logic::opcode_ALLOC:
			aReg[reg3] = heapMem->SetAddress(iReg[reg1], heapMem->Allocate(iReg[reg2])) + maxStack;
			break;
		case Logic::opcode_DALLOC:
			heapMem->DeAllocate(heapMem->GetAddress(iReg[reg1]), iReg[reg2]);
			break;
		default:
			return RunTimeError_OHNO;
			break;


		}

		counter++;

	}

	return RunTimeError_ABORT;

}

KubLingMachine::~KubLingMachine()
{


}

#include "stdafx.h"
#include "KubLingMachine.h"

using namespace Logic;
using namespace CrossHandlers;

KubLingMachine::KubLingMachine(MessageQueue* queue,
	Message* mBuffer,
	unsigned int maxMess,
	unsigned int &currentMess,
	char* stackMem,
	CelestialSlicedList<Resources::BaseObject*>* objectContainer)
{

	this->objectContainer = objectContainer;
	this->queue = queue;
	this->mBuffer = mBuffer;
	this->maxMess = maxMess;
	this->currentMess = currentMess;
	lastMess = &(mBuffer[currentMess]);

	iReg[4] = 0;

	this->stackMem = stackMem;
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
		MessageType_SYSTEM;

	if (dest != MessageSource_OBJECT)
	{

		queue->PushMessage(lastMess);

		currentMess++;
		currentMess %= maxMess;
		lastMess = &(mBuffer[currentMess]);

	}
	else
	{

		unsigned int objId;
		memcpy(&objId, &stackMem[returnAdr], 4);
		Resources::BaseObject* obj = objectContainer->GetValue(objId);
		obj->Update(lastMess);

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

	if (adr- iReg[4] > heapMem->GetOffset())
	{

		return heapMem->GetMemory(adr - heapMem->GetOffset()- iReg[4]);

	}
	else
	{

		return &stackMem[adr];

	}
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
		unsigned char type = (line & 0x18000000000000) >> 51;

		unsigned int scal = (line & 0xFFFFFFFF);


		switch (op)
		{
		case Logic::opcode_STOP:
			return RunTimeError_OK;
		case Logic::opcode_LOAD:

			if (type == 0)
			{

				memcpy(&iReg[reg1], getMem(iReg[reg2] + iReg[4]), sizeof(unsigned int));

			}
			else if (type == 1)
			{

				memcpy(&fReg[reg1], getMem(iReg[reg2] + iReg[4]), sizeof(float));

			}
			else
			{

				memcpy(&cReg[reg1], getMem(iReg[reg2] + iReg[4]), sizeof(char));

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
			break;
		case Logic::opcode_STORE:

			if (type == 0)
			{

				memcpy(getMem(iReg[reg2] + iReg[4]), &iReg[reg1], sizeof(unsigned int));

			}
			else if (type == 1)
			{

				memcpy(getMem(iReg[reg2] + iReg[4]), &fReg[reg1], sizeof(float));

			}
			else
			{

				memcpy(getMem(iReg[reg2] + iReg[4]), &cReg[reg1], sizeof(char));

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
			else
			{

				iReg[4] = scal;

			}
			break;
		case Logic::opcode_SAVE:
			memcpy(getMem(iReg[reg1] + iReg[4]), &scal, iReg[reg2]);
			break;
		case Logic::opcode_COPY:
			memcpy(getMem(iReg[reg1] + iReg[4]), getMem(iReg[reg2] + iReg[4]), iReg[reg3]);
			break;
		case Logic::opcode_SEND:
			sendMessage(iReg[reg1], cReg[0] + iReg[4], MessageSource(iReg[reg2]), iReg[reg3] + iReg[4],sender);
			break;
		case Logic::opcode_STPRM:
			lastMess->SetParams((unsigned char*)getMem(iReg[reg1] + iReg[4]), iReg[reg2], iReg[reg3]);
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
			else
			{

				cReg[reg3] = cReg[reg1] + cReg[reg2];

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
			else
			{

				cReg[reg3] = cReg[reg1] - cReg[reg2];

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
			else
			{

				cReg[reg3] = cReg[reg1] > cReg[reg2] ? 255 : 0;

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
			s.size();
			memcpy(getMem(iReg[reg2] + iReg[4]), &size,4);
			memcpy(getMem(iReg[reg2] + iReg[4] + 4), st, s.size());
			break;
		case Logic::opcode_ADR:
			iReg[reg2] = heapMem->GetAddress(iReg[reg1]);
			break;
		case Logic::opcode_ALLOC:
			iReg[reg3] = heapMem->SetAddress(iReg[reg1], heapMem->Allocate(iReg[reg2]));
			break;
		case Logic::opcode_DALLOC:
			heapMem->DeAllocate(iReg[reg1], iReg[reg2]);
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

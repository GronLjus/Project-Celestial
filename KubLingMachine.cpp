#include "stdafx.h"
#include "KubLingMachine.h"

using namespace Logic;
using namespace CrossHandlers;

KubLingMachine::KubLingMachine(MessageQueue* queue,
	Message* mBuffer,
	unsigned int maxMess,
	unsigned int currentMess)
{

	this->queue = queue;
	this->mBuffer = mBuffer;
	this->maxMess = maxMess;
	this->currentMess = currentMess;

	//lastMess = &(mBuffer[0]);

	heapMem = new HeapMemory(1024000);

}

void KubLingMachine::sendMessage(unsigned int mess, MessageSource dest)
{

	lastMess->timeSent = time;
	lastMess->destination = dest;
	lastMess->mess = mess;

	lastMess->type = dest == MessageSource_CELSCRIPT ? MessageType_SCRIPT :
		dest == MessageSource_ENTITIES ? MessageType_ENTITIES :
		dest == MessageSource_GRAPHICS ? MessageType_GRAPHICS :
		dest == MessageSource_GUIENTITIES ? MessageType_GUIENTITIES :
		dest == MessageSource_INPUT ? MessageType_INPUT :
		dest == MessageSource_OBJECT ? MessageType_OBJECT :
		dest == MessageSource_RESOURCES ? MessageType_RESOURCES :
		MessageType_SYSTEM;

	queue->PushMessage(lastMess);

	currentMess++;
	currentMess %= maxMess;
	lastMess = &(mBuffer[currentMess]);

}

RunTimeError KubLingMachine::RunScript(unsigned long long* code, unsigned int &counter, unsigned int time)
{

	this->time = time;
	bool stop = false;

	while (!stop)
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

				memcpy(&iReg[reg1], &stackMem[iReg[reg2]], sizeof(unsigned int));

			}
			else if (type == 1)
			{

				memcpy(&fReg[reg1], &stackMem[iReg[reg2]], sizeof(float));

			}
			else
			{

				memcpy(&cReg[reg1], &stackMem[iReg[reg2]], sizeof(float));

			}
			break;
		case Logic::opcode_STORE:

			if (type == 0)
			{

				memcpy(&stackMem[iReg[reg2]], &iReg[reg1], sizeof(unsigned int));

			}
			else if (type == 1)
			{

				memcpy(&stackMem[iReg[reg2]], &fReg[reg1], sizeof(float));

			}
			else
			{

				memcpy(&stackMem[iReg[reg2]], &cReg[reg1], sizeof(float));

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
			else
			{

				memcpy(&cReg[reg1], &scal, sizeof(float));

			}
			break;
		case Logic::opcode_HEAP:
			memcpy(heapMem->GetMemory(iReg[reg1]), &stackMem[iReg[reg2]], iReg[reg3]);
			break;
		case Logic::opcode_MOV:
			memcpy(&stackMem[iReg[reg1]], heapMem->GetMemory(iReg[reg2]), iReg[reg3]);
			break;
		case Logic::opcode_SAVE:
			memcpy(&stackMem[iReg[reg1]], &iReg[reg3], iReg[reg2]);
			break;
		case Logic::opcode_SEND:
			sendMessage(iReg[reg1], MessageSource(iReg[reg2]));
			break;
		case Logic::opcode_STPRM:
			lastMess->SetParams(&stackMem[iReg[reg1]], iReg[reg2], iReg[reg3]);
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
		case Logic::opcode_CMPRE:

			if (type == 0)
			{

				cReg[reg3] = iReg[reg1] == iReg[reg2];

			}
			else if (type == 1)
			{

				cReg[reg3] = fReg[reg1] == fReg[reg2];

			}
			else
			{

				cReg[reg3] = cReg[reg1] == cReg[reg2];

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
		case Logic::opcode_JMPINV:

			if (cReg[reg1] != 0)
			{

				break;

			}
		case Logic::opcode_JMP:
			counter = iReg[reg1] - 1;
			break;
		case Logic::opcode_RETURN:
			return RunTimeError(iReg[reg1]);
			break;
		case Logic::opcode_ALLOC:
			iReg[reg1] = heapMem->Allocate(iReg[reg2]);
			break;
		case Logic::opcode_MALLOC:
			heapMem->DeAllocate(iReg[reg1], iReg[reg2]);
			break;
		default:
			return RunTimeError_OHNO;
			break;


		}

		counter++;

	}
}

KubLingMachine::~KubLingMachine()
{

	delete heapMem;

}

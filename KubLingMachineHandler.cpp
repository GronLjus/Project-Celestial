#include "stdafx.h"
#include "KubLingMachineHandler.h"
#include "GUIObject.h"
#include "KubLingCompiled.h"
#include "GUILayout.h"
#include <thread>

using namespace Logic;
using namespace Resources;
using namespace CrossHandlers;


KubLingMachineHandler::KubLingMachineHandler(MessageBuffer* mBuffer,
	CelestialSlicedList<BaseObject*>* objectContainer)
{

	totalMachines = 1;
	machines = new KubLingMachine*[totalMachines];

	waitingLabels = new CelestialSlicedList<KubLingMachineHandler::waitingLabel>(16);
	readyMachines = new CelestialStack<KubLingMachine*>(false);
	runningMachines = new CelestialStack<machineContainer>(false);
	runningMachinesSecondary = new CelestialStack<machineContainer>(false);
	primedCode = new CelestialStack<unsigned int>(false);

	waitingLabels->Add(waitingLabel());

	baseStack = 0;

	for (unsigned int i = 0; i < totalMachines; i++)
	{

		machines[i] = new KubLingMachine(mBuffer, stackMem, 524287, objectContainer);
		readyMachines->PushElement(machines[i]);

	}
}

bool KubLingMachineHandler::Stopped() const
{

	return readyMachines->GetCount() == totalMachines;

}

void KubLingMachineHandler::SetCode(unsigned long long* code, unsigned int size, unsigned int memOffset, HeapMemory* heap)
{

	baseStack = memOffset;
	unsigned int remainder = KubLingMachineHandler::MAXSTACK - baseStack;
	block blck;
	blck.size = remainder;
	blck.adr = 0;
	stackHoles.Push(blck);

	this->heap = heap;

	for (unsigned int i = 0; i < totalMachines; i++)
	{

		machines[i]->SetHeap(heap);

	}

	this->code = code;
	this->totalCode = size;

}

KubLingMachineHandler::block KubLingMachineHandler::handleNeighbour(block block1, block block2, block::neighbour neigh)
{

	if (neigh == block::neighbour_RIGHT)//Block 1 is to the right of block 2, move the starting adr
	{

		block1.adr = block2.adr;

	}
	
	if (neigh != block::neighbour_NA)
	{

		block1.size += block2.size;

	}

	return block1;

}

KubLingMachineHandler::block::neighbour KubLingMachineHandler::isNeighbour(block block1, block block2)
{

	if (block1.adr == block2.adr + block2.size)//block1 is to the right of block2
	{

		return block::neighbour_RIGHT;
		block2.size += block1.size;

	}
	else if (block1.adr + block1.size == block2.adr)//block1 is to the left of block2
	{

		return block::neighbour_LEFT;

	}

	return block::neighbour_NA;

}

void KubLingMachineHandler::deFragHoles()
{

	std::vector<block> blocks = stackHoles.oVect;
	stackHoles.Clear();

	while(!blocks.empty())
	{

		block lastBlock = blocks.back();
		blocks.pop_back();
		bool keepTesting = false;

		do
		{

			keepTesting = false;

			for (unsigned int i = 0; i < blocks.size(); i++)
			{

				block thisBlock = blocks[i];
				block::neighbour neigh = isNeighbour(lastBlock, thisBlock);

				if (neigh != block::neighbour_NA)
				{

					lastBlock = handleNeighbour(lastBlock, thisBlock, neigh);
					blocks.erase(blocks.begin() + i);
					keepTesting = true;

				}
			}

		} while (keepTesting);

		stackHoles.Push(lastBlock);

	}
}

unsigned int KubLingMachineHandler::PrimeLabel(unsigned int start, unsigned int memOffset, unsigned int initSize, unsigned int memSize)
{


	block blck = stackHoles.Top();

	if (blck.size < memSize)//The hole is too small to fit the script
	{

		deFragHoles();//Try welding the small holes together and make large holes
		blck = stackHoles.Top();

		if (blck.size < memSize)//Still too small, stack overflow!!!
		{

			return 0;

		}
	}

	stackHoles.Pop();

	KubLingMachineHandler::waitingLabel waiting;
	waiting.start = start;
	waiting.memOffst = memOffset;
	waiting.dynMemOffset = blck.adr + baseStack;
	waiting.initSize = initSize;
	waiting.memSize = memSize;
	waiting.adr = blck.adr;

	blck.adr += memSize;
	blck.size -= memSize;
	stackHoles.Push(blck);

	return waitingLabels->Add(waiting);

}

void KubLingMachineHandler::SetStackVar(unsigned int translatedId, unsigned int var, unsigned char* data, unsigned char size)
{

	waitingLabel wL = waitingLabels->GetValue(translatedId);
	SetStackVar(var + wL.dynMemOffset, data, size);

}

void KubLingMachineHandler::SetStackVar(unsigned int var, unsigned char* data, unsigned char size)
{

	if (var < MAXSTACK)
	{

		memcpy(&stackMem[var], data, size);

	}
	else
	{

		var -= MAXSTACK;
		memcpy(heap->GetMemory(var), data, size);

	}
}

void KubLingMachineHandler::SetHeapVar(unsigned int var, unsigned char* data, unsigned char size)
{

	memcpy(heap->GetMemory(heap->GetAddress(var)), data, size);

}

RunTimeError KubLingMachineHandler::RunCode(unsigned int translatedId)
{

	primedCode->PushElement(translatedId);
	return RunTimeError_OK;

}

void KubLingMachineHandler::Update(unsigned int time)
{

	bool abort = false;

	while ((primedCode->GetCount() > 0 || runningMachines->GetCount() > 0)
		&& !abort)
	{

		while (primedCode->GetCount() > 0 && readyMachines->GetCount() > 0)
		{

			unsigned int labelToRun = primedCode->PopElement();
			KubLingMachine* machine = readyMachines->PopElement();
			waitingLabel wL = waitingLabels->GetValue(labelToRun);


			machineContainer container;
			container.status = machineStatus_PRIMED;
			container.localLabel = labelToRun;
			container.counter = wL.start;
			container.machine = machine;
			container.sleep = 0;
			container.lastTime = time;
			runningMachines->PushElement(container);

		}

		unsigned int runnings = runningMachines->GetCount();

		for (unsigned int i = 0; i < runnings; i++)
		{

			machineContainer container = runningMachines->PopElement();
			waitingLabel wL = waitingLabels->GetValue(container.localLabel);
			KubLingMachine* machine = container.machine;

			machine->SetMemOffset(wL.dynMemOffset);

			if (container.status == machineStatus_PRIMED)
			{

				machine->SetRegister(3, 0);
				machine->SetRegister(4, wL.memOffst);
				machine->SetRegister(5, wL.initSize);

			}

			RunTimeError rte = RunTimeError_OK;

			if (container.sleep == 0)
			{

				rte = container.machine->RunScript(code, container.counter, time, container.localLabel);

			}
			else
			{

				rte = RunTimeError_HALT;
				container.status = machineStatus_SLEEPING;
				container.sleep = max(0, container.sleep - (time - container.lastTime));

			}

			container.lastTime = time;
			wL.dynMemOffset = container.machine->GetMemOffset();

			if (rte == RunTimeError_WAITINGFORWAR || rte == RunTimeError_MSGFULL || rte == RunTimeError_HALT)
			{

				waitingLabels->Add(wL, container.localLabel);
				container.status = machineStatus_RUNNING;
				runningMachinesSecondary->PushElement(container);

			}
			else
			{

				block blck;
				blck.size = wL.memSize;
				blck.adr = wL.adr;

				block lastBlck = stackHoles.Top();
				block::neighbour neigh = isNeighbour(blck, lastBlck);

				if (neigh != block::neighbour_NA)//The two blocks are neighbours and should be merged
				{

					blck = handleNeighbour(blck, lastBlck, neigh);
					stackHoles.Pop();

				}

				stackHoles.Push(blck);

				readyMachines->PushElement(container.machine);

				waitingLabels->Add(wL, container.localLabel);
				waitingLabels->Remove(container.localLabel);

			}
		}

		abort = runningMachines->GetCount() == 0 
			&& readyMachines->GetCount() == 0 
			&& primedCode->GetCount() != 0;

	}

	CelestialStack<machineContainer>*temp = runningMachines;
	runningMachines = runningMachinesSecondary;
	runningMachinesSecondary = temp;

}

void KubLingMachineHandler::KillMachines()
{

	for (unsigned int i = 0; i < totalMachines; i++)
	{

		machines[i]->Kill();

	}
}

KubLingMachineHandler::~KubLingMachineHandler()
{

	for (unsigned int i = 0; i < totalMachines; i++)
	{

		delete machines[i];

	}

	delete[] machines;

	delete waitingLabels;
	delete readyMachines;
	delete runningMachines;
	delete runningMachinesSecondary;
	delete primedCode;

}
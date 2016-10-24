#include "stdafx.h"
#include "KubLingMachineHandler.h"
#include "GUIObject.h"
#include "KubLingCompiled.h"
#include "GUILayout.h"
#include <thread>

using namespace Logic;
using namespace Resources;
using namespace CrossHandlers;


KubLingMachineHandler::KubLingMachineHandler(MessageQueue* queue,
	Message* mBuffer,
	unsigned int maxMess,
	unsigned int &currentMess,
	CelestialSlicedList<BaseObject*>* objectContainer)
{

	totalMachines = 1;
	machines = new KubLingMachine*[totalMachines];

	waitingLabels = new CelestialSlicedList<KubLingMachineHandler::waitingLabel>(16);
	readyMachines = new CelestialStack<KubLingMachine*>(false);
	runningMachines = new CelestialStack<machineContainer>(false);
	runningMachinesSecondary = new CelestialStack<machineContainer>(false);
	primedCode = new CelestialStack<unsigned int>(false);

	for (unsigned int i = 0; i < totalMachines; i++)
	{

		machines[i] = new KubLingMachine(queue, mBuffer, maxMess, currentMess, stackMem, objectContainer);
		readyMachines->PushElement(machines[i]);

	}
}

bool KubLingMachineHandler::Stopped() const
{

	return readyMachines->GetCount() == totalMachines;

}

void KubLingMachineHandler::SetCode(unsigned long long* code, unsigned int size, HeapMemory* heap)
{

	for (unsigned int i = 0; i < totalMachines; i++)
	{

		machines[i]->SetHeap(heap);

	}

	this->code = code;
	this->totalCode = size;

}

unsigned int KubLingMachineHandler::PrimeLabel(unsigned int start, unsigned int memOffset)
{

	KubLingMachineHandler::waitingLabel waiting;
	waiting.start = start;
	waiting.memOffst = memOffset;

	return waitingLabels->Add(waiting);

}

void KubLingMachineHandler::SetStackVar(unsigned int translatedId, unsigned int var, unsigned char* data, unsigned char size)
{

	memcpy(&stackMem[var], data, size);

}

RunTimeError KubLingMachineHandler::RunCode(unsigned int translatedId)
{

	primedCode->PushElement(translatedId);
	return RunTimeError_OK;

}

void KubLingMachineHandler::Update(unsigned int time)
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

		if (container.status == machineStatus_PRIMED)
		{

			machine->SetRegister(3, 0);
			machine->SetRegister(4, wL.memOffst);

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

		if (rte == RunTimeError_WAITINGFORWAR || rte == RunTimeError_MSGFULL || rte == RunTimeError_HALT)
		{

			container.status = machineStatus_RUNNING;
			runningMachinesSecondary->PushElement(container);

		}
		else
		{

			readyMachines->PushElement(container.machine);

			waitingLabels->Add(wL, container.localLabel);
			waitingLabels->Remove(container.localLabel);

		}
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
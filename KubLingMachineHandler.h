#pragma once
#include "KubLingMachine.h"

namespace Logic
{

	class KubLingMachineHandler
	{ 

	private:
		
		enum machineStatus { machineStatus_PRIMED, machineStatus_RUNNING, machineStatus_SLEEPING, machineStatus_NA};

		struct machineContainer
		{

			unsigned int counter = 0;
			KubLingMachine* machine = nullptr;
			machineStatus status = machineStatus_NA;
			unsigned int localLabel;

			unsigned int sleep;
			unsigned int lastTime;

		};

		struct waitingLabel
		{

			unsigned int start;
			unsigned int memOffst;

			void operator=(const waitingLabel* wL)
			{

			}

		};

		unsigned int totalMachines;
		KubLingMachine** machines;

		CrossHandlers::CelestialSlicedList<waitingLabel>* waitingLabels;

		CrossHandlers::CelestialStack<KubLingMachine*>* readyMachines;
		CrossHandlers::CelestialStack<unsigned int>* primedCode;
		CrossHandlers::CelestialStack<machineContainer>* runningMachines;
		CrossHandlers::CelestialStack<machineContainer>* runningMachinesSecondary;

		HeapMemory* heap;
		unsigned long long* code;
		unsigned int totalCode;

		char stackMem[524287];

	public:

		static const unsigned int MAXSTACK = 524287;

		KubLingMachineHandler(CrossHandlers::MessageQueue* queue,
			CrossHandlers::Message* mBuffer,
			unsigned int maxMess,
			unsigned int &currentMess,
			CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* objectContainer);

		void SetCode(unsigned long long* code, unsigned int size, HeapMemory* heap);

		unsigned int PrimeLabel(unsigned int start, unsigned int memOffset);

		void SetStackVar(unsigned int translatedId, unsigned int var, unsigned char* data, unsigned char size);
		void SetHeapVar(unsigned int var, unsigned char* data, unsigned char size);

		RunTimeError RunCode(unsigned int translatedId);

		bool Stopped() const;
		void Update(unsigned int time);

		void KillMachines();
		~KubLingMachineHandler();

	};
}
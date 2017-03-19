#pragma once
#include "KubLingMachine.h"
#include "CelestialPrioQueue.h"

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
			unsigned int initSize;
			unsigned int dynMemOffset;
			unsigned int memSize;
			unsigned int adr;

			void operator=(const waitingLabel* wL)
			{

			}

		};

		struct block
		{

			enum neighbour{ neighbour_LEFT, neighbour_RIGHT, neighbour_NA};

			unsigned int adr;
			unsigned int size;

			friend bool operator<=(const block& block1, const block& block2)
			{
				return block1.size >= block2.size;

			}

			friend bool operator<(const block& block1, const block& block2)
			{
				return block1.size > block2.size;

			}

			friend bool operator==(const block& block1, const block& block2)
			{
				return block1.adr == block2.adr;

			}
		};

		unsigned int baseStack;
		CrossHandlers::CelestialPrioQueue<block> stackHoles;

		void deFragHoles();
		block handleNeighbour(block block1, block block2, block::neighbour neigh);
		block::neighbour isNeighbour(block block1, block block2);

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

		KubLingMachineHandler(CrossHandlers::MessageBuffer* mBuffer,
			CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* objectContainer);

		void SetCode(unsigned long long* code, unsigned int size, unsigned int memOffset, HeapMemory* heap);

		unsigned int PrimeLabel(unsigned int start, unsigned int memOffset, unsigned int initSize, unsigned int memSize);

		void SetStackVar(unsigned int translatedId, unsigned int var, unsigned char* data, unsigned char size);
		void SetStackVar(unsigned int var, unsigned char* data, unsigned char size);

		void SetHeapVar(unsigned int var, unsigned char* data, unsigned char size);

		RunTimeError RunCode(unsigned int translatedId);

		bool Stopped() const;
		void Update(unsigned int time);

		void KillMachines();
		~KubLingMachineHandler();

	};
}
#include "stdafx.h"
#include "HeapMemory.h"

using namespace Logic;

HeapMemory::HeapMemory(unsigned int size, unsigned int addresses)
{

	incrSize = size;
	blockSize = size;
	memoryBlock = new char[blockSize];
	addressSpace = new unsigned int[addresses];

	for (unsigned int i = 0; i < addresses; i++)
	{

		addressSpace[i] = 0;

	}

	unsigned int offst = 0;
	memcpy(memoryBlock, &offst, 4);

	block startBlock;
	startBlock.adr = 4;
	startBlock.size = blockSize;
	holes.push(startBlock);

}

unsigned int HeapMemory::SetAddress(unsigned int var, unsigned int address)
{

	addressSpace[var] = address;
	return addressSpace[var];

}

unsigned int HeapMemory::GetAddress(unsigned int var) const
{

	return addressSpace[var];

}

unsigned int HeapMemory::GetOffset() const
{

	unsigned int offset = 0;
	memcpy(&offset, &memoryBlock[0], 4);

	memoryBlock[0] = 0;
	memoryBlock[1] = 0;
	memoryBlock[2] = 0;
	memoryBlock[3] = 0;

	return offset;

}

void HeapMemory::expandMemory()
{

	char* temp = new char[blockSize + incrSize];

	memcpy(temp, memoryBlock, blockSize);

	block startBlock;
	startBlock.adr = blockSize;
	startBlock.size = incrSize;
	holes.push(startBlock);

	blockSize += incrSize;

	delete[] memoryBlock;
	memoryBlock = temp;

}

unsigned int HeapMemory::Allocate(unsigned int blockSize)
{

	if (blockSize > incrSize)
	{

		return 0;

	}

	if (holes.empty())
	{

		expandMemory();

	}

	block newMem = holes.top();
	holes.pop();

	if (blockSize > newMem.size)
	{

		expandMemory();
		newMem = holes.top();
		holes.pop();

	}

	unsigned int adr = newMem.adr;

	newMem.adr += blockSize;
	newMem.size -= blockSize;

	if (newMem.size > 0)
	{

		holes.push(newMem);

	}

	return adr;

}

void HeapMemory::DeAllocate(unsigned int address, unsigned int size)
{

	if (address != 0)
	{

		block mem;
		mem.adr = address;
		mem.size = size;
		holes.push(mem);

	}
}

char* HeapMemory::GetMemory(unsigned int adress)
{

	return &memoryBlock[adress];

}

HeapMemory::~HeapMemory()
{

	delete[] memoryBlock;
	delete[] addressSpace;

}
#include "stdafx.h"
#include "HeapMemory.h"

using namespace Logic;

HeapMemory::HeapMemory(unsigned int size)
{

	incrSize = size;
	blockSize = size;
	memoryBlock = new char[blockSize];

	block startBlock;
	startBlock.adr = 1;
	startBlock.size = blockSize;
	holes.push(startBlock);

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

	block mem;
	mem.adr = address;
	mem.size = size;
	holes.push(mem);

}

char* HeapMemory::GetMemory(unsigned int adress)
{

	return &memoryBlock[adress];

}

HeapMemory::~HeapMemory()
{

	delete[] memoryBlock;

}
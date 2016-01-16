#include "stdafx.h"
#include "MemoryPool.h"

using namespace Logic;
using namespace std;
using namespace CrossHandlers;

MemoryPool::MemoryPool(unsigned int pageSize)
{

	memoryA = new CelestialSlicedList<unsigned char>(pageSize);
	variables = new CelestialSlicedList<memBlock>(100);
	holeVal = 0;
	adrLast = 0;

}

void MemoryPool::resortHoles(unsigned char pivot)
{

	unsigned char startPlace = 0;
	memBlock pivBlock = holes[pivot];

	while (pivot > 0 && holes[pivot].length > holes[pivot - 1].length)
	{

		memBlock temp = holes[pivot];
		holes[pivot] = holes[pivot - 1];
		holes[pivot - 1] = temp;
		memoryA->Add(pivot + 1, temp.place);
		memoryA->Add(pivot + 1, temp.place + temp.length);
		pivot--;

	}

	while (pivot < holeVal-1 && holes[pivot].length < holes[pivot + 1].length)
	{

		memBlock temp = holes[pivot];
		holes[pivot] = holes[pivot + 1];
		holes[pivot + 1] = temp;
		memoryA->Add(pivot + 1, temp.place);
		memoryA->Add(pivot + 1, temp.place + temp.length);
		pivot++;

	}

	memoryA->Add(pivot + 1, pivBlock.place);
	memoryA->Add(pivot + 1, pivBlock.place + pivBlock.length);

	if (holes[pivot].length == 0)
	{

		holeVal--;

	}
}

unsigned int MemoryPool::findAddress(unsigned int var, unsigned int valSize)
{

	memBlock varMem = variables->GetValue(var);
	unsigned int adress = varMem.place;
	bool varExists = varMem.length > 0;

	if (varMem.length == valSize)
	{

		adress = varMem.place;

	}
	else if (varMem.length > valSize)
	{

		adress = varMem.place;
		unsigned char rightVal = memoryA->GetValue(varMem.place + varMem.length + 3);

		if (rightVal != 0)//Expand Hole
		{

			holes[rightVal - 1].length += varMem.length - valSize;
			resortHoles(rightVal - 1);

		}
		else//Create new Hole
		{

			memBlock newHole;
			newHole.length = varMem.length - valSize;
			newHole.place = varMem.place + valSize + 3;

			holeVal += holeVal == 255 ? 0 : 1;
			holes[holeVal] = newHole;

			memoryA->Add(holeVal, newHole.place);
			memoryA->Add(holeVal, newHole.place+newHole.length-1);

			resortHoles(holeVal);

		}
	}
	else if (varMem.place + valSize + 2 != adrLast)
	{

		unsigned int size = valSize + 2;
		adress = adrLast;
		
		if (holes[0].length >= size)
		{

			unsigned char hole = 0;

			while (hole < holeVal && holes[hole].length > size) { hole++; }

			adress = holes[hole].place;
			holes[hole].length -= valSize;
			holes[hole].place += valSize;

			if (holes[hole].length > 0)
			{

				memoryA->Add(hole - 1, holes[hole].place);

			}

			resortHoles(hole);

		}

		if(varExists)
		{

			unsigned char rightNeighbour = memoryA->GetValue(varMem.place + valSize + 3);
			unsigned char leftNeighbour = memoryA->GetValue(varMem.place - 1);

			if (rightNeighbour == 0 && leftNeighbour == 0)
			{

				memBlock newHole;
				newHole.length = varMem.length;
				newHole.place = varMem.place;

				holeVal += holeVal == 255 ? 0 : 1;
				holes[holeVal] = newHole;

				memoryA->Add(holeVal, newHole.place);
				memoryA->Add(holeVal, newHole.place + newHole.length - 1);
				resortHoles(holeVal);

			}
			else if (rightNeighbour != 0 && leftNeighbour == 0)
			{

				memoryA->Add(rightNeighbour, varMem.place);
				holes[rightNeighbour - 1].length += varMem.length + 2;
				holes[rightNeighbour - 1].place = varMem.place;
				resortHoles(rightNeighbour - 1);

			}
			else if (rightNeighbour == 0 && leftNeighbour != 0)
			{

				memoryA->Add(leftNeighbour, varMem.place+varMem.length+2);
				holes[leftNeighbour - 1].length += varMem.length+2;
				resortHoles(leftNeighbour - 1);

			}
			else
			{

				holes[leftNeighbour - 1].length += varMem.length + 
					holes[rightNeighbour - 1].length +
					2;

				memoryA->Add(leftNeighbour - 1, holes[leftNeighbour - 1].place);
				memoryA->Add(leftNeighbour - 1, holes[leftNeighbour - 1].place + holes[leftNeighbour - 1].length - 1);
				resortHoles(leftNeighbour - 1);
				holes[rightNeighbour - 1].length = 0;
				resortHoles(rightNeighbour - 1);

			}
		}
	}

	return adress;

}

MemErrorCode MemoryPool::AddVariable(unsigned int var, unsigned char* val, unsigned int valSize)
{

	MemErrorCode err = ErrorCode_OK;

	unsigned int adress = findAddress(var, valSize+2);

	if (adress == adrLast)
	{

		adrLast += valSize + 2;

	}

	memoryA->Add(0, adress);
	memoryA->Add(0, adress+valSize+2);

	memBlock varMem = variables->GetValue(var);
	varMem.place = adress;
	varMem.length = valSize;
	variables->Add(varMem, var);
	writeData(var, valSize, val);

	return err;

}

void MemoryPool::writeData(unsigned int var, unsigned int bytes, unsigned char* val)
{

	memBlock mem = variables->GetValue(var);
	unsigned int totalBytes = bytes;
	unsigned int globalPlace = mem.place + 1;

	while (totalBytes > 0)
	{

		unsigned int localPlace = globalPlace % memoryA->GetSliceSize();
		unsigned int toend = memoryA->GetSliceSize() - localPlace;
		unsigned int readBytes = toend > totalBytes ? totalBytes : toend;

		memcpy(&(memoryA->GetSlice(globalPlace)[localPlace]), val, readBytes);

		totalBytes -= readBytes;
		globalPlace += readBytes;

	}
}

MemErrorCode MemoryPool::readData(unsigned int var, unsigned int offset, unsigned int bytes, unsigned char* &val)
{

	MemErrorCode err = ErrorCode_OK;
	memBlock mem = variables->GetValue(var);

	unsigned int totalBytes = bytes;
	unsigned int globalPlace = mem.place+1+offset;

	while (totalBytes > 0)
	{

		unsigned int localPlace = globalPlace % memoryA->GetSliceSize();
		unsigned int toend = memoryA->GetSliceSize() - localPlace;
		unsigned int readBytes = toend > totalBytes ? totalBytes : toend;

		memcpy(val, &(memoryA->GetSlice(globalPlace)[localPlace]), readBytes);

		totalBytes -= readBytes;
		globalPlace += readBytes;

	}

	return err;

}

MemErrorCode MemoryPool::ReadVariable(unsigned int var, unsigned int offset, unsigned int &bytes, unsigned char* &val)
{

	bytes = offset + bytes > variables->GetValue(var).length ? variables->GetValue(var).length-(offset-bytes) : bytes;
	return readData(var, offset, bytes, val);

}

MemErrorCode MemoryPool::ReadVariable(unsigned int var, unsigned char* &val, unsigned int &valSize)
{

	memBlock varH = variables->GetValue(var);
	valSize = varH.length < valSize ? varH.length: valSize;
	return ReadVariable(var, 0, valSize, val);

}

MemErrorCode MemoryPool::CopyVariable(unsigned int dst, unsigned int src)
{

	MemErrorCode err = ErrorCode_OK;
	memBlock mem = variables->GetValue(src);

	unsigned int totalBytes = mem.length;
	unsigned int globalPlace = mem.place + mem.length + 2;

	while (totalBytes > 0)
	{

		unsigned int localPlace = globalPlace % memoryA->GetSliceSize();
		unsigned int toend = memoryA->GetSliceSize() - localPlace;
		unsigned int readBytes = toend > totalBytes ? totalBytes : toend;

		writeData(dst, readBytes, &(memoryA->GetSlice(globalPlace)[localPlace]));

		totalBytes -= readBytes;
		globalPlace += readBytes;

	}

	return err;

}

MemoryPool::~MemoryPool()
{

	delete memoryA;
	delete variables;

}
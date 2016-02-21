#include "stdafx.h"
#include "MemoryPool.h"

using namespace Logic;
using namespace std;
using namespace CrossHandlers;

MemoryPool::MemoryPool(unsigned int pageSize)
{

	memoryA = new CelestialSlicedList<unsigned char>(pageSize,0);
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
		pivot--;

	}

	while (pivot < holeVal-1 && holes[pivot].length < holes[pivot + 1].length)
	{

		memBlock temp = holes[pivot];
		holes[pivot] = holes[pivot + 1];
		holes[pivot + 1] = temp;
		pivot++;

	}

	if (holes[pivot].length == 0)
	{

		holeVal--;

	}
}

unsigned int MemoryPool::findAddress(unsigned int var, unsigned int valSize)
{

	memBlock varMem = variables->GetValue(var);
	unsigned int adress = varMem.length > 0 ? varMem.place : adrLast;
	unsigned char rightHole = 0;
	unsigned char leftHole = 0;
	unsigned char suitableHole = 0;

	for (unsigned char i = 0; i < holeVal && (suitableHole == 0 || rightHole == 0 || (leftHole == 0 && varMem.place > 0)); i++)//Find possible holes
	{

		if (holes[i].place == varMem.place + varMem.length)
		{rightHole = i + 1;}
		else if (varMem.place > 0 && holes[i].place + holes[i].length == varMem.place - 1)
		{leftHole = i + 1;}
		else if (holes[i].length >= valSize && (suitableHole == 0 || holes[i].length < holes[suitableHole-1].length))
		{suitableHole = i + 1;}
	}

	if (varMem.length > valSize)//We need to shrink the memory block
	{

		unsigned char holeValue = 0;

		if (rightHole != 0)//Expand Hole
		{

			holeValue = rightHole - 1;
			holes[rightHole - 1].length += varMem.length - valSize;
			holes[rightHole - 1].place -= varMem.length - valSize;


		}
		else//Create new Hole
		{

			memBlock newHole;
			newHole.length = varMem.length - valSize;
			newHole.place = varMem.place + valSize;
			holeValue = holeVal;
			holes[holeVal] = newHole;
			holeVal += holeVal == 254 ? 0 : 1;

		}

		resortHoles(holeValue);

	}
	else if (valSize > varMem.length)//We need to expand the memory block or move it
	{

		unsigned char leftSize = leftHole != 0 ? holes[leftHole - 1].length : 0;
		unsigned char rightSize = rightHole != 0 ? holes[rightHole - 1].length : 0;

		if (rightSize >= valSize-varMem.length)//We only need to eat the right hole
		{

			holes[rightHole - 1].length -= valSize - varMem.length;
			holes[rightHole - 1].place += valSize - varMem.length;
			resortHoles(rightHole - 1);

		}
		else if (leftSize >= valSize - varMem.length)//We only need to eat the left hole
		{

			holes[leftHole - 1].length -= valSize - varMem.length;
			adress = holes[leftHole - 1].place + holes[leftHole - 1].length + 1;
			resortHoles(leftHole - 1);

		}
		else if (leftSize + rightSize >= valSize - varMem.length)//We need to eat both
		{

			adress = holes[leftHole - 1].place;
			unsigned char toEat = valSize - (holes[leftHole - 1].length + varMem.length);
			holes[leftHole - 1].length = 0;
			resortHoles(leftHole - 1);

			holes[rightHole - 1].length -= toEat;
			holes[rightHole - 1].place += toEat;
			resortHoles(rightHole - 1);

		}
		else if(suitableHole != 0)//There are no nearby holes to eat but one further away
		{

			unsigned char hole = 0;
			adress = holes[suitableHole - 1].place;
			holes[suitableHole - 1].length -= valSize;
			holes[suitableHole - 1].place += valSize;
			resortHoles(suitableHole - 1);

		}
		else if (adress != adrLast)//Move the block to the end of the memory
		{

			adress = adrLast;
			holeVal += holeVal == 254 ? 0 : 1;
			holes[holeVal] = varMem;

		}
	}

	return adress;

}

MemErrorCode MemoryPool::AddVariable(unsigned int var, unsigned char* val, unsigned int valSize)
{

	MemErrorCode err = ErrorCode_OK;

	unsigned int adress = findAddress(var, valSize);

	if (adress == adrLast)
	{

		adrLast += valSize;

	}

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
	unsigned int globalPlace = mem.place;

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
	unsigned int globalPlace = mem.place+offset;

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
	unsigned int globalPlace = mem.place + mem.length;

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
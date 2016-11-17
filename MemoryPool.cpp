#include "stdafx.h"
#include "MemoryPool.h"

using namespace Logic;
using namespace std;
using namespace CrossHandlers;

MemoryPool::MemoryPool(unsigned int maxStack)
{

	variables = new CelestialSlicedList<memBlock>(100);
	offsets = new CelestialSlicedList<offsetStack>(100);
	holeVal = 0;
	adrLast = 0;
	sysAdr = 0;
	this->maxStack = maxStack;

	maxOffsets = 10;

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

unsigned int MemoryPool::AddVariable(unsigned int var, unsigned int valSize)
{

	unsigned int adress = findAddress(var, valSize);

	if (adress == adrLast)
	{

		adrLast += valSize;

	}

	memBlock varMem = variables->GetValue(var);
	varMem.place = adress;
	varMem.length = valSize;
	variables->Add(varMem, var);
	return adress;

}

unsigned int MemoryPool::AddVariable(unsigned int var, unsigned int adr, unsigned int valSize)
{

	memBlock varMem = variables->GetValue(var);
	varMem.place = adr;
	varMem.length = valSize;
	variables->Add(varMem, var);
	return adr;

}

void MemoryPool::AddOffset(unsigned int var, unsigned int offAdr, unsigned int offsize)
{

	offsetStack stack = offsets->GetValue(var);

	if (offsets->GetValue(var).offsets == nullptr)
	{

		stack.offsets = new offset[maxOffsets];
		stack.lastOffset = 0;
		stack.offsets[0].offsetVar = 0;
		stack.offsets[0].size = 0;

	}

	if (stack.lastOffset < maxStack - 1)
	{

		stack.lastOffset++;

		stack.offsets[stack.lastOffset].offsetVar = offAdr;
		stack.offsets[stack.lastOffset].size = offsize;

		offsets->Add(stack, var);

	}
}

unsigned int MemoryPool::GetVarLength(unsigned int var) const
{

	return variables->GetValue(var).length;

}

unsigned int MemoryPool::GetMemorySize() const
{

	return adrLast;

}

void MemoryPool::AddSystemMem(Resources::KubLingCompiled* compiled)
{

	maxVar = compiled->GetMaxVar();
	AddVariable(maxVar, maxVar);
	maxVar++;

	for (unsigned int i = 0; i < compiled->GetMaxParams('n'); i++)
	{

		unsigned int adr = compiled->GetAdr(i, 'n');

		if (adr != 0)
		{

			adr--;
			AddVariable(adr, sizeof(unsigned int));
			memBlock varMem = variables->GetValue(adr);
			compiled->AddParamAdr(i, varMem.place, 'n');

		}
	}

	for (unsigned int i = 0; i < compiled->GetMaxParams('f'); i++)
	{

		unsigned int adr = compiled->GetAdr(i, 'f');

		if (adr != 0)
		{

			adr--;
			AddVariable(adr, sizeof(float));
			memBlock varMem = variables->GetValue(adr);
			compiled->AddParamAdr(i, varMem.place, 'f');

		}
	}

	for (unsigned int i = 0; i < RunTimeParams_NA; i++)
	{

		unsigned int adr = compiled->GetAdr(RunTimeParams(i));

		if (adr != 0)
		{

			adr--;
			AddVariable(adr, sizeof(unsigned int));
			memBlock varMem = variables->GetValue(adr);
			compiled->AddSystemParamAdr(RunTimeParams(i), varMem.place);

		}
	}

	for (unsigned int i = 0; i < compiled->GetMaxParams('s'); i++)
	{

		unsigned int adr = compiled->GetAdr(i, 's');

		if (adr != 0)
		{

			adr--;
			AddVariable(adr, 64);
			memBlock varMem = variables->GetValue(adr);
			compiled->AddParamAdr(i, varMem.place, 's');

		}
	}


	maxVar = compiled->GetMaxVar();
	AddVariable(maxVar, 64);

	sysAdr = adrLast;

}

MemoryPool::offset MemoryPool::GetOffset(unsigned int var)
{


	offsetStack offStack = offsets->GetValue(var);

	offset retVal;
	retVal.size = 0;
	
	if (offStack.offsets != nullptr)
	{

		retVal = offStack.offsets[offStack.lastOffset];

	}

	if (offStack.lastOffset > 0)
	{

		offStack.lastOffset--;

	}

	offsets->Add(offStack, var);
	return retVal;

}

unsigned int MemoryPool::GetMaxStack() const
{

	return maxStack;

}

unsigned int MemoryPool::GetMaxVar() const
{

	return maxVar;

}

unsigned int MemoryPool::GetVarSize(unsigned int var) const
{

	memBlock varMem = variables->GetValue(var);
	return varMem.length;

}

unsigned int MemoryPool::GetVarAdr(unsigned int var) const
{

	memBlock varMem = variables->GetValue(var);
	return varMem.place;

}

unsigned int MemoryPool::GetStartingAdr() const
{

	return sysAdr;

}

MemoryPool::~MemoryPool()
{

	for (unsigned int i = 0; i < offsets->GetHighest(); i++)
	{

		offsetStack off = offsets->GetValue(i);

		if (off.offsets != nullptr)
		{

			delete[] off.offsets;

		}
	}

	delete variables;
	delete offsets;

}
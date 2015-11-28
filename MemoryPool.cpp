#include "stdafx.h"
#include "MemoryPool.h"

using namespace Logic;
using namespace std;
using namespace CrossHandlers;

MemoryPool::MemoryPool(unsigned int pageSize)
{

	memoryA = new CelestialSlicedList<unsigned char>(pageSize);
	memoryB = new CelestialSlicedList<unsigned char>(pageSize);
	variables = new CelestialSlicedList<memBlock>(100);
	holeVal = 0;
	adrLast = 0;

}

void MemoryPool::resortHoles(unsigned char pivot)
{

	unsigned char startPlace = 0;
	memBlock memB = holes[pivot];

	for (unsigned char i = 0; memB.length > 0 && i < holeVal && holes[i].length > memB.length; i++)
	{

		startPlace = i;

	}

	for (unsigned char i = pivot; i > startPlace; i--)
	{

		holes[i] = holes[i - 1];
		memoryB->Add(i, holes[i].place);
		memoryB->Add(i, holes[i].place + holes[i].length);

	}

	if (memB.length > 0)
	{

		holes[startPlace] = memB;
		memoryB->Add(startPlace, memB.place);
		memoryB->Add(startPlace, memB.place + memB.length);

	}
	else
	{
		
		holeVal--;

	}
}

unsigned int MemoryPool::findAddress(unsigned int var, unsigned int valSize)
{

	memBlock varMem = variables->GetValue(var);
	unsigned int adress = varMem.place;
	bool varExists = varMem.length > 0;
	unsigned int endMem = varMem.length + 2;
	endMem = endMem % 2 != 0 ? endMem + 1 : endMem;
	endMem = varMem.place + (endMem / 2);

	if (varMem.length == valSize)
	{

		adress = varMem.place;

	}
	else if (varMem.length > valSize)
	{

		adress = varMem.place;
		unsigned char neighbourVal = memoryB->GetValue(endMem + 1);

		if (neighbourVal != 0)//Expand Hole
		{

			holes[neighbourVal - 1].length += varMem.length - valSize;
			resortHoles(neighbourVal - 1);

		}
		else//Create new Hole
		{

			memBlock newMemB;
			newMemB.length = varMem.length - valSize;
			newMemB.place = varMem.place + valSize;

			holeVal += holeVal == 255 ? 0 : 1;
			holes[holeVal] = newMemB;
			resortHoles(neighbourVal - 1);

		}
	}
	else if (endMem + 1 != adrLast)
	{

		adress = adrLast;
		unsigned char rightNeighbour = varExists ? memoryB->GetValue(endMem + 1) : 0;
		unsigned char leftNeighbour = varExists && varMem.place > 0 ? memoryB->GetValue(varMem.place - 1) : 0;

		if (rightNeighbour > 0 && holes[rightNeighbour - 1].length + varMem.length > valSize)
		{

			holes[rightNeighbour - 1].length -= valSize - varMem.length;
			holes[rightNeighbour - 1].place += valSize - varMem.length;
			resortHoles(rightNeighbour - 1);

		}
		else if (leftNeighbour > 0 && holes[leftNeighbour - 1].length + varMem.length > valSize)
		{

			holes[leftNeighbour - 1].length -= valSize - varMem.length;
			adress = holes[leftNeighbour - 1].place + holes[leftNeighbour - 1].length;
			resortHoles(leftNeighbour - 1);

		}
		else if (rightNeighbour*leftNeighbour > 0 && holes[leftNeighbour - 1].length + holes[rightNeighbour - 1].length + varMem.length > valSize)
		{

			holes[rightNeighbour - 1].length -= valSize - (varMem.length + holes[leftNeighbour - 1].length);
			holes[rightNeighbour - 1].place += valSize - (varMem.length + holes[leftNeighbour - 1].length);
			resortHoles(rightNeighbour - 1);

			holes[leftNeighbour - 1].length = 0;
			adress = holes[leftNeighbour - 1].place;
			resortHoles(leftNeighbour - 1);

		}
		else
		{

			unsigned char holePlace = 0;

			for (int i = 1; i <= holeVal && holes[i - 1].length > valSize; i++)
			{

				holePlace = i;

			}

			if (holePlace > 0)
			{

				holePlace--;
				adress = holes[holePlace].place;
				holes[holePlace].length -= valSize - varMem.length;
				holes[holePlace].place += valSize - varMem.length;
				resortHoles(holePlace);

				if (rightNeighbour*leftNeighbour > 0)//Merge the two holes
				{

					holes[leftNeighbour - 1].length += holes[rightNeighbour - 1].length + varMem.length;
					resortHoles(leftNeighbour - 1);
					holes[rightNeighbour - 1].length = 0;
					resortHoles(rightNeighbour - 1);

				}
				else if (rightNeighbour > 0)
				{

					holes[rightNeighbour - 1].length += varMem.length;
					holes[rightNeighbour - 1].place = varMem.place;
					resortHoles(rightNeighbour - 1);

				}
				else
				{

					holes[leftNeighbour - 1].length += varMem.length;
					resortHoles(leftNeighbour - 1);

				}
			}
		}
	}

	return adress;

}

MemErrorCode MemoryPool::AddVariable(unsigned int var, unsigned char* val, unsigned int valSize)
{

	MemErrorCode err = ErrorCode_OK;
	unsigned int endMem = valSize + 2;
	endMem = endMem % 2 != 0 ? endMem + 1 : endMem;
	endMem /=  2;

	unsigned int adress = findAddress(var, valSize);

	if (adress == adrLast)
	{

		adrLast += endMem;

	}

	memoryB->Add(0, adress);
	memoryB->Add(0, adress+endMem);

	memBlock varMem = variables->GetValue(var);
	varMem.place = adress;
	varMem.length = valSize;
	variables->Add(varMem, var);
	figureOutPageSizes(var);
	writeData(var, 0, valSize, val);

	return err;

}

void MemoryPool::figureOutPageSizes(unsigned int var)
{

	memBlock mem = variables->GetValue(var);
	unsigned int memLength = mem.length + 2;
	memLength -= memLength % 2 == 0 ? 0 : 1;
	memLength /= 2;

	bytesOnPages[0][0] = memLength < memoryA->GetSliceSize() - mem.place ? memLength : memoryA->GetSliceSize() - mem.place;
	bytesOnPages[0][1] = mem.length - bytesOnPages[0][0] > bytesOnPages[0][0] - 1 ? bytesOnPages[0][0] - 1 : mem.length - bytesOnPages[0][0];
	totalMidPages = (memLength - bytesOnPages[0][0]) / memoryA->GetSliceSize();
	bytesOnPages[1][0] = memoryA->GetSliceSize();
	bytesOnPages[1][1] = bytesOnPages[1][0];
	bytesOnPages[2][0] = mem.length - (bytesOnPages[0][0] + bytesOnPages[0][1] + totalMidPages*(bytesOnPages[1][0] + bytesOnPages[1][1]));
	bytesOnPages[2][1] = bytesOnPages[2][0] / 2;
	bytesOnPages[2][0] = bytesOnPages[2][0] - bytesOnPages[2][1];

}

void MemoryPool::writeData(unsigned int var, unsigned int page, unsigned int bytes, unsigned char* val)
{

	memBlock mem = variables->GetValue(var);
	unsigned char pages = page == 0 ? 0 :
		page <= totalMidPages ? 1 :
		2;

	unsigned int pageStart = page > 0 ? mem.place + bytesOnPages[0][0] + page*bytesOnPages[1][0] : mem.place;
	unsigned int bStart = page == 0 ? 1 : 0;

	if (bytesOnPages[pages][0] > 0)
	{

		memcpy(&(memoryA->GetSlice(pageStart)[memoryA->TransformId(pageStart)]), val, bytesOnPages[pages][0]);

	}

	if (bytesOnPages[pages][1] > 0)
	{

		memcpy(&(memoryB->GetSlice(pageStart + bStart)[memoryB->TransformId(pageStart + bStart)]), &(val[bytesOnPages[pages][0]]), bytesOnPages[pages][1]);

	}


	if (bytesOnPages[pages][0] + bytesOnPages[pages][1] < bytes)
	{

		unsigned int newBytes = bytes - bytesOnPages[pages][0] - bytesOnPages[pages][1];
		page++;
		writeData(var, page, newBytes, &(val[bytesOnPages[pages][0] + bytesOnPages[pages][1]]));

	}
}

void MemoryPool::writeDataOffset(unsigned int var, unsigned int offset, unsigned int bytes, unsigned char* val)
{

	memBlock mem = variables->GetValue(var);

	unsigned char pages = offset < bytesOnPages[0][0] + bytesOnPages[0][1] ? 0 :
		offset <  bytesOnPages[0][0] + bytesOnPages[0][1] + totalMidPages*(bytesOnPages[1][0] + bytesOnPages[1][1]) ? 1 :
		2;

	unsigned int midPage = pages != 0 ? (offset - bytesOnPages[0][0] - bytesOnPages[0][1]) / (memoryA->GetSliceSize() * 2) : 0;
	unsigned int blockOffset = pages == 2 ? offset - (bytesOnPages[0][0] + bytesOnPages[0][1] + totalMidPages*(bytesOnPages[1][1] + bytesOnPages[1][0])) :
		pages == 1 ? offset - (bytesOnPages[0][0] + bytesOnPages[0][1] + midPage*(bytesOnPages[1][1] + bytesOnPages[1][0])) :
		offset;

	unsigned int pageStart = pages > 0 ? mem.place + bytesOnPages[0][0] + midPage*bytesOnPages[1][0] : mem.place;
	unsigned int bStart = blockOffset < bytesOnPages[pages][0] ? 1 : blockOffset - bytesOnPages[pages][0] + 1;
	unsigned int aRead = blockOffset >= bytesOnPages[pages][0] ? 0 : bytesOnPages[pages][0] - blockOffset;
	aRead = aRead > bytes ? bytes : aRead;
	unsigned bRead = bytes - aRead <= bytesOnPages[pages][1] ? bytes - aRead : bytesOnPages[pages][1];
	bStart -= pages != 0 ? 1 : 0;


	if (bytesOnPages[pages][0] > 0)
	{

		memcpy(&(memoryA->GetSlice(pageStart)[memoryA->TransformId(pageStart)]), val, aRead);

	}

	if (bytesOnPages[pages][1] > 0)
	{

		memcpy(&(memoryB->GetSlice(pageStart + bStart)[memoryB->TransformId(pageStart + bStart)]), &(val[bytesOnPages[pages][0]]), bRead);

	}


	if (aRead + bRead < bytes)
	{

		unsigned int newBytes = bytes - aRead - bRead;
		writeData(var, offset + aRead + bRead, newBytes, &(val[bytesOnPages[pages][0] + bytesOnPages[pages][1]]));

	}
}

MemErrorCode MemoryPool::readVariable(unsigned int var, unsigned int offset, unsigned int bytes, unsigned char* &val)
{

	MemErrorCode err = ErrorCode_OK;
	memBlock mem = variables->GetValue(var);
	unsigned char pages = offset < bytesOnPages[0][0] + bytesOnPages[0][1] ? 0 :
		offset <  bytesOnPages[0][0] + bytesOnPages[0][1] + totalMidPages*(bytesOnPages[1][0] + bytesOnPages[1][1]) ? 1 :
		2;

	unsigned int midPage = pages != 0 ? (offset - bytesOnPages[0][0] - bytesOnPages[0][1]) / (memoryA->GetSliceSize() * 2) : 0;
	unsigned int blockOffset = pages == 2 ? offset - (bytesOnPages[0][0] + bytesOnPages[0][1] + totalMidPages*(bytesOnPages[1][1] + bytesOnPages[1][0])) :
		pages == 1 ? offset - (bytesOnPages[0][0] + bytesOnPages[0][1] + midPage*(bytesOnPages[1][1] + bytesOnPages[1][0])) :
		offset;

	unsigned int pageStart = pages > 0 ? mem.place + bytesOnPages[0][0] + midPage*bytesOnPages[1][0] : mem.place;
	unsigned int bStart = blockOffset < bytesOnPages[pages][0] ? 1 : blockOffset - bytesOnPages[pages][0]+1;
	unsigned int aRead = blockOffset >= bytesOnPages[pages][0] ? 0 : bytesOnPages[pages][0]-blockOffset;
	aRead = aRead > bytes ? bytes : aRead;
	unsigned bRead = bytes - aRead  <= bytesOnPages[pages][1] ? bytes - aRead : bytesOnPages[pages][1];
	bStart -= pages != 0 ? 1 : 0;


	if (aRead > 0)
	{

		memcpy(val, &(memoryA->GetSlice(pageStart + blockOffset)[memoryA->TransformId(pageStart + blockOffset)]), aRead);

	}

	if (bRead > 0)
	{

		memcpy(&(val[aRead]), &(memoryB->GetSlice(pageStart + bStart)[memoryB->TransformId(pageStart + bStart)]), bRead);

	}


	if (aRead + bRead < bytes)
	{

		unsigned int newBytes = bytes - aRead - bRead;
		err = ReadVariable(var, offset + aRead + bRead, newBytes, val);

	}

	return err;

}

MemErrorCode MemoryPool::ReadVariable(unsigned int var, unsigned int offset, unsigned int &bytes, unsigned char* &val)
{

	bytes = offset + bytes > variables->GetValue(var).length ? variables->GetValue(var).length-(offset-bytes) : bytes;
	figureOutPageSizes(var);
	return readVariable(var, offset, bytes, val);

}

MemErrorCode MemoryPool::ReadVariable(unsigned int var, unsigned char* &val, unsigned int &valSize)
{

	memBlock varH = variables->GetValue(var);
	valSize = varH.length < valSize ? varH.length: valSize;
	figureOutPageSizes(var);
	return ReadVariable(var, 0, valSize, val);

}

MemErrorCode MemoryPool::CopyVariable(unsigned int dst, unsigned int src)
{

	memBlock srcMem = variables->GetValue(src);
	unsigned int adr = findAddress(dst, srcMem.length);
	memBlock varMem = variables->GetValue(dst);
	varMem.place = adr;
	varMem.length = srcMem.length;
	variables->Add(varMem, dst);

	figureOutPageSizes(dst);
	unsigned int dstBytesOnPages[3][2]; 
	dstBytesOnPages[0][0] = bytesOnPages[0][0];
	dstBytesOnPages[0][1] = bytesOnPages[0][1];
	unsigned int dstTotalMidPages = totalMidPages;
	dstBytesOnPages[1][0] = bytesOnPages[1][0];
	dstBytesOnPages[1][1] = bytesOnPages[1][1];
	dstBytesOnPages[2][0] = bytesOnPages[2][0];
	dstBytesOnPages[2][1] = bytesOnPages[2][1];

	unsigned int offset = 0;
	unsigned int currentPage = 0;
	figureOutPageSizes(src);

	while (currentPage <= dstTotalMidPages)
	{

		unsigned int pageStart = currentPage > 0 ? srcMem.place + dstBytesOnPages[0][0] + currentPage*dstBytesOnPages[1][0] : srcMem.place;
		unsigned int page = currentPage == 0 ? 0 : currentPage < dstTotalMidPages ? 1 : 2;
		unsigned int bStart = currentPage == 0 ? 1 : 0;

		writeDataOffset(dst, offset, bytesOnPages[page][0], &(memoryA->GetSlice(pageStart)[memoryA->TransformId(pageStart)]));
		offset += bytesOnPages[page][0];
		writeDataOffset(dst, offset, bytesOnPages[page][1], &(memoryB->GetSlice(pageStart + bStart)[memoryA->TransformId(pageStart + bStart)]));
		offset += bytesOnPages[page][1];
		currentPage++;

	}

	return ErrorCode_OK;

}

MemoryPool::~MemoryPool()
{

	delete memoryA;
	delete memoryB;
	delete variables;

}
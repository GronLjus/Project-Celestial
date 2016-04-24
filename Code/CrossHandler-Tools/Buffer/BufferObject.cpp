#include "../../../stdafx.h"
#include "BufferObject.h"

using namespace CrossHandlers;

BufferObject::BufferObject()
{

	this->fDxBuffer = nullptr;
	this->lDxBuffer = nullptr;
	this->aDxBuffer = nullptr;

	tempBuffer = nullptr;
	step = 0;
	totTemp = 0;

}

void BufferObject::SetStep(unsigned int step)
{

	this->step = step;

}

void BufferObject::IncreaseTemp(unsigned int to)
{

	this->totTemp = to;

}

void BufferObject::SetTemp(void* temp)
{

	tempBuffer = temp;

}

unsigned int BufferObject::GetStep() const
{

	return step;

}

unsigned int BufferObject::GetTempAmount() const
{

	return totTemp;

}

void* BufferObject::GetTempBuffer() const
{

	return tempBuffer;

}

void BufferObject::ResetTemp()
{

	tempBuffer = nullptr;
	totTemp = 0;

}

void BufferObject::SetDXBuffer(DXBufferObject* object)
{

	fDxBuffer = object;
	lDxBuffer = fDxBuffer;

}

void BufferObject::AddDxBuffer(DXBufferObject* object)
{

	if(lDxBuffer != nullptr)
	{
	
		lDxBuffer->SetNext(object);

	}
	else
	{

		fDxBuffer = object;

	}

	lDxBuffer = object;
	aDxBuffer = lDxBuffer;

}

void BufferObject::SetActiveDxBuffer(DXBufferObject* object)
{

	this->aDxBuffer = object;

}

DXBufferObject* BufferObject::GetActiveDXBuffer()
{

	return aDxBuffer;

}

DXBufferObject* BufferObject::GetFirstDXBuffer()
{

	return fDxBuffer;

}

DXBufferObject* BufferObject::GetLastDXBuffer()
{

	return lDxBuffer;

}

BufferObject::~BufferObject()
{

	if (tempBuffer != nullptr)
	{
	
		delete[] tempBuffer;

	}

	if(fDxBuffer != nullptr)
	{

		delete fDxBuffer;

	}
}

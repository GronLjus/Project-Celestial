#include "stdafx.h"
#include "DXBufferObject.h"

using namespace CrossHandlers;

DXBufferObject::DXBufferObject()
{

	this->mapped = nullptr;
	this->internalBuffer = nullptr;
	this->next = nullptr;
	size = 0;
	companyBuffer = nullptr;

}

void DXBufferObject::SetBuffer(ID3D10Buffer* buffer)
{

	internalBuffer = buffer;

}

ID3D10Buffer* DXBufferObject::GetBuffer()
{

	return internalBuffer;

}

void DXBufferObject::Release()
{

	internalBuffer->Release();
	internalBuffer = nullptr;

}

void DXBufferObject::Map(void** mapped)
{

	this->mapped = mapped;

}

void DXBufferObject::UnMap()
{

	if(mapped != nullptr)
	{

		internalBuffer->Unmap();

	}

	this->mapped = nullptr;

}

void** DXBufferObject::GetMappedPointer()
{

	return this->mapped;

}

void DXBufferObject::SetNext(DXBufferObject* next)
{

	this->next = next;

}

DXBufferObject* DXBufferObject::GetNext()
{

	return next ;

}

int DXBufferObject::GetSize()
{

	return size;

}

void DXBufferObject::SetSize(int size)
{

	this->size = size;

}

void DXBufferObject::SetCompany(DXBufferObject*  buffer)
{

	companyBuffer = buffer;

}

DXBufferObject*  DXBufferObject::GetCompany()
{

	return companyBuffer;

}

DXBufferObject::~DXBufferObject()
{

	if(internalBuffer != nullptr)
	{

		internalBuffer->Release();

	}

	if(next != nullptr)
	{

		delete next;

	}
}

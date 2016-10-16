#include "stdafx.h"
#include "HeapMemContainer.h"

using namespace Resources;
using namespace Logic;


HeapMemContainer::HeapMemContainer(HeapMemory* heap)
{

	this->heap = heap;

}

HeapMemory* HeapMemContainer::GetHeap() const
{

	return heap;

}

HeapMemContainer::~HeapMemContainer()
{

	delete heap;

}
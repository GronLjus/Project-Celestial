#include "stdafx.h"
#include "NodeGroup.h"

using namespace Entities;

NodeGroup::NodeGroup()
{

	lockedID = 0;

}

unsigned int NodeGroup::GetLocked() const
{

	return lockedID;

}

void NodeGroup::Lock(unsigned int obj)
{

	lockedID = obj;

}

NodeGroup::~NodeGroup()
{

}
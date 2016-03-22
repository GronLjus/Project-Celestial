#include "stdafx.h"
#include "BaseObject.h"

using namespace Resources;

void BaseObject::SetId(unsigned int id)
{

	this->id = id;
	lastUpdated = 0;

}

void  BaseObject::setLastUpdated(unsigned int time)
{

	lastUpdated = time;

}

unsigned int BaseObject::LastUpdated() const
{

	return lastUpdated;

}

unsigned int BaseObject::GetId() const
{

	return id;

}
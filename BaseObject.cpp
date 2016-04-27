#include "stdafx.h"
#include "BaseObject.h"

using namespace Resources;

void BaseObject::SetId(unsigned int id)
{

	this->id = id;
	lastUpdated = 0;
	killMessage = nullptr;

}

CrossHandlers::Message* BaseObject::GetKillMessage() const
{

	return killMessage;

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

unsigned int BaseObject::GetTargetId() const
{

	return id;

}

unsigned int BaseObject::GetParentId() const
{
	
	return id;

}

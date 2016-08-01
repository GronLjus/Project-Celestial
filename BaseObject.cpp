#include "stdafx.h"
#include "BaseObject.h"

using namespace Resources;

BaseObject::BaseObject()
{

	killMessages = nullptr;

}

void BaseObject::SetId(unsigned int id)
{

	this->id = id;
	lastUpdated = 0;

}

CrossHandlers::Message** BaseObject::GetKillMessage(unsigned int &kills) const
{

	kills = this->kills;
	return killMessages;

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

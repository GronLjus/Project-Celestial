#include "stdafx.h"
#include "BaseObject.h"

using namespace Resources;

BaseObject::BaseObject()
{

	killMessages = nullptr;
	activeId = 0;
	kills = 0;
	maxKills = 0;

}

unsigned int BaseObject::GetActiveId() const
{

	return activeId;

}

void BaseObject::SetActiveId(unsigned int id)
{

	activeId = id;

}

void BaseObject::SetId(unsigned int id)
{

	this->id = id;
	lastUpdated = 0;

}

void BaseObject::addKillMessage(CrossHandlers::Message* mess)
{

	if (kills >= maxKills || killMessages == nullptr)
	{

		maxKills += 20;
		CrossHandlers::Message** temp = new CrossHandlers::Message*[maxKills];

		if (killMessages != nullptr)
		{

			for (unsigned int i = 0; i < kills; i++)
			{

				temp[i] = killMessages[i];

			}

			delete[] killMessages;

		}

		killMessages = temp;

	}

	killMessages[kills] = mess;
	kills++;

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

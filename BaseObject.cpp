#include "stdafx.h"
#include "BaseObject.h"

using namespace Resources;

void BaseObject::SetId(unsigned int id)
{

	this->id = id;

}

unsigned int BaseObject::GetId() const
{

	return id;

}
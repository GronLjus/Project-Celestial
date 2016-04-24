#include "stdafx.h"
#include "BoxedBool.h"

using namespace CrossHandlers;

BoxedBool::BoxedBool(bool val)
{

	this->value = val;

}


bool BoxedBool::operator==(const bool &value)
{

	return this->value==value;

}

bool BoxedBool::GetValue()
{

	return this->value;

}

void BoxedBool::SetValue(bool val)
{

	this->value = val;

}
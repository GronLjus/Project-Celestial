#include "stdafx.h"

#include "Road.h"

using namespace Entities;

Road::Road()
{

	dir = Direction_NA;
	nodes = 0;
	lastObject = 0;

}

Road::Direction Road::GetDirection() const
{

	return dir;

}

unsigned int Road::GetNodes() const
{

	return nodes;

}

unsigned int Road::GetLastNode() const
{

	return lastObject;

}

void Road::IncreaseNodes(unsigned int by)
{

	nodes += by;

}

void Road::DecreaseNodes(unsigned int by)
{

	nodes -= by;

}

void Road::SetLastObject(unsigned int object)
{

	lastObject = object;

}

void Road::SetDirection(Road::Direction dir)
{

	this->dir = dir;

}

Road::~Road()
{


}
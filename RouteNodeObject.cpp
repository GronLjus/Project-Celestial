#include "stdafx.h"
#include "RouteNodeObject.h"

using namespace CrossHandlers;
using namespace Entities;
using namespace CelestialMath;

RouteNodeObject::RouteNodeObject() : RouteNodeObject(Vector3(0,0,0),0)
{

	width = 0.0f;

}

RouteNodeObject::RouteNodeObject(Vector3 position, float width)
{

	this->position = position;
	this->width = width;
	routes = new CelestialSlicedList<route>(32);
	objId = 0;
	maxRoutes = 0;
	openSet = 0;
	closedSet = 0;
	parent = 0;

}

char* RouteNodeObject::Serialize(unsigned int &size)
{

	size = sizeof(float) * 4;
	char* byteVal = new char[size];
	memcpy(byteVal, &position.x, 4);
	memcpy(&byteVal[4], &position.y, 4);
	memcpy(&byteVal[8], &position.z, 4);
	memcpy(&byteVal[12], &width, 4);
	return byteVal;

}

char* RouteNodeObject::Unserialize(char* data)
{

	memcpy(&position.x, &data[0], 4);
	memcpy(&position.y, &data[4], 4);
	memcpy(&position.z, &data[8], 4);
	memcpy(&width, &data[12], 4);
	return nullptr;

}

void RouteNodeObject::SetPosition(Vector3 pos)
{

	position = pos;

}

void RouteNodeObject::SetObjId(unsigned int id)
{

	this->objId = id;

}

Vector3 RouteNodeObject::GetPosition() const
{

	return position;

}

unsigned int RouteNodeObject::GetObjId() const
{

	return objId;

}

RouteNodeObject* RouteNodeObject::GetRoute(unsigned int localId, float &dist)
{

	route rte = routes->GetValue(localId);

	if (!rte.deleted)
	{

		dist = rte.distSQR;
		return rte.goal;

	}

	return nullptr;

}

unsigned int RouteNodeObject::GetRoutes() const
{

	return maxRoutes;

}

unsigned int RouteNodeObject::GetWidth() const
{

	return width;

}

unsigned int RouteNodeObject::GetParent() const
{

	return parent;

}

unsigned char RouteNodeObject::GetOpenSet() const
{

	return openSet;

}

unsigned char RouteNodeObject::GetClosedSet() const
{

	return closedSet;

}

unsigned int RouteNodeObject::GetStep() const
{

	return step;

}

void RouteNodeObject::LinkObj(unsigned int objId)
{

	this->objId = objId;

}

void RouteNodeObject::SetParent(unsigned int parent)
{

	this->parent = parent;

}

void RouteNodeObject::SetOpenset(unsigned char open)
{

	this->openSet = open;

}

void RouteNodeObject::SetClosedset(unsigned char closed)
{

	this->closedSet = closed;

}

void RouteNodeObject::SetStep(unsigned int step)
{

	this->step = step;

}

void RouteNodeObject::AddRoute(RouteNodeObject* node)
{
	route rte;
	rte.deleted = false;
	rte.direction = node->GetPosition() - position;
	rte.distSQR = VectorDot(rte.direction);
	rte.goal = node;

	unsigned int x = routes->Add(rte);

	if (x+1 >= maxRoutes)
	{

		maxRoutes = x+1;

	}
}

unsigned int RouteNodeObject::GetId() const
{

	return id;

}

void RouteNodeObject::SetId(unsigned int id)
{

	this->id = id;

}

void RouteNodeObject::RemoveRoute(unsigned int id)
{

	bool found = false;

	for (unsigned int i = 0; i < maxRoutes && !found;i++)
	{
	
		route rte = routes->GetValue(i);

		if (rte.goal->GetId() == id && !rte.deleted)
		{

			found = true;
			routes->Remove(i);

		}
	}
}

RouteNodeObject::~RouteNodeObject()
{

	delete routes;

}
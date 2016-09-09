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

	road = 0;
	this->position = position;
	this->width = width;
	routes = new CelestialSlicedList<route>(32);
	objId = 0;
	routesAmount = 0;
	openSet = 0;
	closedSet = 0;
	parent = 0;
	upId = 0;
	downId = 0;
	heurustic = 0.0f;

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

float RouteNodeObject::GetHeuristic() const
{

	return heurustic;
}

void RouteNodeObject::SetHeuristic(float heuristic)
{

	this->heurustic = heuristic;

}

unsigned int RouteNodeObject::GetRoad() const
{

	return road;

}

unsigned int RouteNodeObject::GetDownId() const
{

	return downId;

}

unsigned int RouteNodeObject::GetUpId() const
{

	return upId;

}

void RouteNodeObject::SetRoad(unsigned int road)
{

	this->road = road;

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

RouteNodeObject* RouteNodeObject::GetRoute(unsigned int localId)
{

	route rte = routes->GetValue(localId);

	if (!rte.deleted)
	{

		return rte.goal;

	}

	return nullptr;

}

unsigned int RouteNodeObject::GetMaxRoutes() const
{

	return routes->GetHighest();

}

unsigned int RouteNodeObject::GetRoutes() const
{

	return routesAmount;

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

unsigned int RouteNodeObject::GetLocalId(unsigned int id) const
{

	unsigned int lId = 0;
	RouteNodeObject* route = nullptr;

	for (unsigned int i = 0; i < routesAmount && route == nullptr; i++)
	{

		route = routes->GetValue(i).goal;
		lId = i;

		if (route != nullptr && route->GetId() != id)
		{

			route = nullptr;

		}
	}

	return route == nullptr ? 0 : lId;

}
void  RouteNodeObject::SetDirection(unsigned int localId, Road::Direction dir)
{

	route rte = routes->GetValue(localId);

	if (!rte.deleted)
	{

		rte.travelDirection = dir;
		routes->Add(rte, localId);

	}
}

void RouteNodeObject::TravelRoute(unsigned int localId, unsigned int objId)
{

	route rte = routes->GetValue(localId);

	if (rte.travelDirection != Road::Direction_UP && !rte.deleted)
	{

		if (rte.lastObject == 0)
		{

			unsigned int oppId = rte.goal->GetLocalId(this->GetId());
			rte.goal->SetDirection(oppId, Road::Direction_UP);

		}

		rte.travelDirection = Road::Direction_DOWN;
		rte.lastObject = objId;
		routes->Add(rte, localId);

	}
}

void RouteNodeObject::TravelDone(unsigned int localId, unsigned int objId)
{

	route rte = routes->GetValue(localId);

	if (!rte.deleted && 
		(rte.lastObject == objId || rte.lastObject == 0))
	{

		unsigned int oppId = rte.goal->GetLocalId(this->GetId());
		rte.goal->SetDirection(oppId, Road::Direction_NA);

		rte.travelDirection = Road::Direction_NA;
		rte.lastObject = 0;
		routes->Add(rte, localId);

	}
}

bool RouteNodeObject::CanTravel(unsigned int localId)
{

	return routes->GetValue(localId).travelDirection != Road::Direction_UP;

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

void RouteNodeObject::AddRoute(RouteNodeObject* node, Road::Direction dir)
{
	route rte;
	rte.deleted = false;
	rte.direction = node->GetPosition() - position;
	rte.distSQR = VectorDot(rte.direction);
	rte.goal = node;
	rte.lastObject = 0;
	rte.travelDirection = Road::Direction_NA;

	unsigned int x = routes->Add(rte);

	if (dir == Road::Direction_DOWN)
	{

		downId = x + 1;

	}
	else if (dir == Road::Direction_UP)
	{

		upId = x + 1;

	}

	routesAmount++;

}

void RouteNodeObject::SetDownId(unsigned int id)
{

	downId = id;

}

void RouteNodeObject::SetUpId(unsigned int id)
{


	upId = id;

}

void RouteNodeObject::AddRoute(RouteNodeObject* node)
{

	AddRoute(node, Road::Direction_NA);

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
	unsigned int x = 0;

	for (unsigned int i = 0; i < routes->GetHighest() && !found;i++)
	{
	
		route rte = routes->GetValue(i);

		if (rte.goal->GetId() == id && !rte.deleted)
		{

			x = i;
			found = true;
			routes->Remove(i);

		}
	}

	x++;

	if (x == downId)
	{

		downId = 0;

	}
	else if (x == upId)
	{

		upId = 0;

	}

	routesAmount--;

}

RouteNodeObject::~RouteNodeObject()
{

	delete routes;

}
#include "stdafx.h"
#include "RouteNodeObject.h"

using namespace CrossHandlers;
using namespace Entities;
using namespace CelestialMath;

RouteNodeObject::RouteNodeObject() : RouteNodeObject(Vector3(0,0,0),0)
{

}

RouteNodeObject::RouteNodeObject(Vector3 position, float width)
{

	this->position = position;
	this->width = width;
	routes = new CelestialSlicedList<route>(32);
	objId = 0;
	routesAmount = 0;
	openSet = 0;
	closedSet = 0;
	parent = 0;
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

Route* RouteNodeObject::GetRoute(unsigned int localId, Route::Direction &dir)
{

	route rte = routes->GetValue(localId);

	if (!rte.deleted)
	{

		dir = rte.dir;
		return rte.rte;

	}

	return nullptr;

}

Route* RouteNodeObject::GetRoute(unsigned int localId)
{

	route rte = routes->GetValue(localId);

	if (!rte.deleted)
	{

		return rte.rte;

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
	Route* rte = nullptr;

	for (unsigned int i = 0; i < routes->GetHighest() && rte == nullptr; i++)
	{

		route rt = routes->GetValue(i);
		rte = rt.rte;
		lId = i;

		if (rte != nullptr && rte->GetNode(rt.dir) != id + 1)
		{

			rte = nullptr;

		}
	}

	return rte == nullptr ? 0 : lId;

}

bool  RouteNodeObject::ContainsRoute(unsigned int gId) const
{

	Route* rte = nullptr;

	for (unsigned int i = 0; i < routes->GetHighest() && rte == nullptr; i++)
	{

		route rt = routes->GetValue(i);
		rte = rt.rte;

		if (rte != nullptr && rte->GetNode(rt.dir) != gId + 1)
		{

			rte = nullptr;

		}
	}

	return rte != nullptr;

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

void RouteNodeObject::AddRoute(Route* node)
{
	route rte;
	rte.deleted = false;
	rte.rte = node;
	unsigned int downNode = node->GetNode(Route::Direction_DOWN);
	unsigned int upNode = node->GetNode(Route::Direction_UP);

	if (downNode == 0 || downNode == id + 1)
	{

		rte.dir = Route::Direction_UP;
		node->SetNode(id + 1, Route::Direction_DOWN);
	
	}
	else if(upNode == 0 || upNode == id + 1)
	{

		rte.dir = Route::Direction_DOWN;
		node->SetNode(id + 1, Route::Direction_UP);

	}

	unsigned int x = routes->Add(rte);
	routesAmount++;

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
	unsigned int x = GetLocalId(id);
	routes->Remove(x);

	routesAmount--;

}

RouteNodeObject::~RouteNodeObject()
{

	delete routes;

}
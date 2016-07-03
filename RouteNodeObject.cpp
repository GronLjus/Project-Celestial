#include "stdafx.h"
#include "RouteNodeObject.h"

using namespace CrossHandlers;
using namespace Resources;
using namespace CelestialMath;

RouteNodeObject::RouteNodeObject() : RouteNodeObject(Vector3(0,0,0),0)
{

}

RouteNodeObject::RouteNodeObject(Vector3 position, unsigned int width)
{

	this->position = position;
	width = width;
	routes = new CelestialSlicedList<route>(32);

}

void RouteNodeObject::Update(Message* mess)
{

	if (mess->type == MessageType_OBJECT)
	{

		Vector3 newVec;

		switch (mess->mess)
		{
		case ObjectMess_MOVE:
			memcpy(&newVec.x, mess->params, 4);
			memcpy(&newVec.y, &mess->params[4], 4);
			memcpy(&newVec.z, &mess->params[8], 4);
			position += newVec;
			break;
		case ObjectMess_POS:
			memcpy(&position.x, mess->params, 4);
			memcpy(&position.y, &mess->params[4], 4);
			memcpy(&position.z, &mess->params[8], 4);
			break;

		}
	}
}

void RouteNodeObject::SetPosition(Vector3 pos)
{

	position = pos;

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

void RouteNodeObject::LinkObj(unsigned int objId)
{

	this->objId = objId;

}

void RouteNodeObject::AddRoute(RouteNodeObject* node)
{
	route rte;
	rte.deleted = false;
	rte.direction = node->GetPosition() - position;
	rte.distSQR = VectorDot(rte.direction);
	rte.goal = node;

	unsigned int x = routes->Add(rte);

	if (x >= maxRoutes)
	{

		maxRoutes = x;

	}
}

void RouteNodeObject::RemoveRoute(unsigned int id)
{

	bool found = false;

	for (unsigned int i = 0; i < maxRoutes && !found;i++)
	{
	
		
		route rte = routes->GetValue(i);

		if (rte.goal->GetId() == id && !rte.deleted)
		{

			rte.deleted = true;
			rte.goal->RemoveRoute(GetId());
			routes->Add(rte, i);
			routes->Remove(i);

		}
	}
}

RouteNodeObject::~RouteNodeObject()
{

	delete routes;

}
#include "stdafx.h"
#include "RoutingManager.h"

using namespace Entities;
using namespace CrossHandlers;
using namespace Resources;
using namespace CelestialMath;

RoutingManager::RoutingManager()
{

	gameObjects = nullptr;
	routeNodes = new CelestialSlicedList<RouteNodeObject*>(32, nullptr);

}

void RoutingManager::Init(CelestialSlicedList<BaseObject*>* gameObjects)
{

	this->gameObjects = gameObjects;

}

unsigned int RoutingManager::AddNode(Vector3 position, GameRouteObject* obj)
{

	RouteNodeObject* preExist = obj->GetRouteNode(position);

	if (preExist != nullptr)
	{

		return preExist->GetId();

	}

	RouteNodeObject* node = new RouteNodeObject(position, obj->GetWidth());
	node->SetId(routeNodes->Add(node));
	obj->AddRouteNode(node);
	return node->GetId();

}

unsigned int RoutingManager::AddNode(Vector3 position, unsigned int* objects, unsigned int amounts)
{

	if (amounts != 0)
	{

		RouteNodeObject* preExist = nullptr;

		for (unsigned int i = 0; i < amounts && preExist == nullptr; i++)
		{

			preExist = ((GameRouteObject*)this->gameObjects->GetValue(objects[i]))->GetRouteNode(position);

		}

		if (preExist == nullptr)
		{

			preExist = new RouteNodeObject(position, 1.0f);
			preExist->SetId(routeNodes->Add(preExist));

		}

		for (unsigned int i = 0; i < amounts; i++)
		{

			GameRouteObject* obj = ((GameRouteObject*)this->gameObjects->GetValue(objects[i]));
			RouteNodeObject* nodeHere = obj->GetRouteNode(position);

			if (nodeHere == nullptr)
			{

				obj->AddRouteNode(preExist);

			}
		}

		return preExist->GetId()+1;

	}

	return 0;

}

RoutingManager::~RoutingManager()
{

	routeNodes->KillList();
	delete routeNodes;

}
#include "stdafx.h"
#include "RoutingManager.h"

using namespace Entities;
using namespace CrossHandlers;
using namespace Resources;

RoutingManager::RoutingManager()
{

	gameObjects = nullptr;

}

void RoutingManager::Init(CelestialSlicedList<BaseObject*>* gameObjects)
{

	this->gameObjects = gameObjects;

}

RoutingManager::~RoutingManager()
{


}
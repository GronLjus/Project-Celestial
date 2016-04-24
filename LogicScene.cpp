#include "stdafx.h"
#include "LogicScene.h"

using namespace Logic;
using namespace Resources;
using namespace CrossHandlers;

LogicScene::LogicScene()
{

	objects = new CelestialList<ResourceObject*>();

}

void LogicScene::ClearScene()
{

	objects->Reset();

}

void LogicScene::AddObject(ResourceObject* object)
{

	objects->AddElement(object);

}

CelestialList<ResourceObject*>* LogicScene::GetObjects()
{

	return objects;

}

LogicScene::~LogicScene()
{

	objects->Reset();
	delete objects;

}
#include "stdafx.h"
#include "GameRouteObject.h"

using namespace Resources;
using namespace CrossHandlers;
using namespace CelestialMath;

GameRouteObject::GameRouteObject(BoundingBox* baseBox, BoundingSphere* baseSphere, unsigned int meshId) : GameObject(baseBox,baseSphere,meshId)
{

	lowerNode = 0;
	middleNode = 0;
	upperNode = 0;
	route = 0;

}

GameRouteObject::GameRouteObject() : GameObject()
{

	lowerNode = 0;
	middleNode = 0;
	upperNode = 0;
	route = 0;

}

char* GameRouteObject::Unserialize(char* data)
{

	unsigned int offset = 0;

	memcpy(&route, data, sizeof(unsigned int));
	offset += sizeof(unsigned int);

	if (data[offset] == SerializableType_GAMEOBJECTSCENERY)
	{

		return GameObject::Unserialize(&data[offset + 1]);

	}

	return nullptr;

}

char* GameRouteObject::Serialize(unsigned int &size)
{

	unsigned int subSize = 0;
	char* subSerial = GameObject::Serialize(subSize);
	size = subSize + 1 + sizeof(unsigned int);
	char* byteVal = new char[size];

	byteVal[0] = SerializableType_GAMEOBJECTROUTE;
	memcpy(&byteVal[1], &route, sizeof(unsigned int));
	memcpy(&byteVal[1 + sizeof(unsigned int)], subSerial, subSize);
	delete[] subSerial;

	return byteVal;

}

GameObjectType GameRouteObject::GetType() const
{

	return GameObjectType_ROUTE;

}

unsigned int GameRouteObject::GetUpperNode() const
{

	return upperNode;

}

unsigned int GameRouteObject::GetMiddleNode() const
{

	return middleNode;

}

unsigned int GameRouteObject::GetLowerNode() const
{

	return lowerNode;

}

unsigned int GameRouteObject::GetRoute() const
{

	return route;

}

void GameRouteObject::SetUpperNode(unsigned int node)
{

	upperNode = node;
	
}

void GameRouteObject::SetMiddleNode(unsigned int node)
{

	middleNode = node;

}

void GameRouteObject::SetLowerNode(unsigned int node)
{

	lowerNode = node;
	
}

void GameRouteObject::SetRoute(unsigned int route)
{

	this->route = route;

}

Intersection GameRouteObject::ContainsPoint(Vector3 point)
{

	BoundingBox* box = GetBox();
	point.y = GetPosition().y;
	Intersection retVal = box->ContainsPoint(point);

	for (unsigned int i = 0; i < GetSubobjects() && retVal == Intersection_BACK; i++)
	{

		GameObject* sub = (GameObject*)GetSubobject(i);

		if (sub != nullptr)
		{

			box = sub->GetBox();
			retVal = box->ContainsPoint(point);

		}
	}

	return retVal;

}

unsigned int GameRouteObject::GetWidth() const
{

	return width;

}

void GameRouteObject::SetWidth(unsigned int width)
{

	this->width = width;

}

GameRouteObject::~GameRouteObject()
{


}
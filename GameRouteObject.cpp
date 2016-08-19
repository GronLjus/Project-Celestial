#include "stdafx.h"
#include "GameRouteObject.h"

using namespace Resources;
using namespace Entities;
using namespace CrossHandlers;
using namespace CelestialMath;

GameRouteObject::GameRouteObject(BoundingBox* baseBox, BoundingSphere* baseSphere, unsigned int meshId) : GameObject(baseBox,baseSphere,meshId)
{

	nodes = new CelestialSlicedList<RouteNodeObject*>(64, nullptr);
	routeNodes = 0;

}

GameObjectType GameRouteObject::GetType() const
{

	return GameObjectType_ROUTE;

}

unsigned int GameRouteObject::GetRoutenodes() const
{

	return routeNodes;

}

RouteNodeObject* GameRouteObject::GetRouteNode(unsigned int localId) const
{

	return nodes->GetValue(localId);

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

RouteNodeObject* GameRouteObject::GetRouteNode(Vector3 position) const
{

	RouteNodeObject* retval = nullptr;

	for (unsigned int i = 0; i < routeNodes && retval == nullptr;i++)
	{
	
		RouteNodeObject* val = nodes->GetValue(i);

		if (val != nullptr)
		{

			float dist = VectorDot(position - val->GetPosition());

			if (dist < 0.5f)
			{

				retval = val;

			}
		}
	}

	return retval;

}

unsigned int GameRouteObject::GetWidth() const
{

	return width;

}

void GameRouteObject::SetWidth(unsigned int width)
{

	this->width = width;

}

void GameRouteObject::AddRouteNode(RouteNodeObject* node)
{

	unsigned int left = 0;
	unsigned int right = 0;
	float distLeft = -1.0f;
	float distRight = -1.0f;

	for (unsigned int i = 0; i < routeNodes; i++)
	{

		RouteNodeObject* nodeHere = nodes->GetValue(i);

		if (nodeHere != nullptr)
		{

			Vector3 vect = node->GetPosition() - nodeHere->GetPosition();
			float distSQR = VectorDot(vect);
			float vD = VectorDot(vect, this->GetDirection());

			if (vD >= 0 && (distSQR < distRight || distRight < 0))
			{

				distRight = distSQR;
				right = i;

			}
			else if (vD < 0 && (distSQR < distLeft || distLeft < 0))
			{

				distLeft = distSQR;
				left = i;

			}

			float dist2 = -1.0f;
			bool isOnSame = false;

		}
	}

	RouteNodeObject* leftNode = nodes->GetValue(left);
	RouteNodeObject* rightNode = nodes->GetValue(right);

	if (distLeft < 0 && distRight >= 0)
	{

		node->AddRoute(rightNode);
		rightNode->AddRoute(node);

	}
	else if (distRight < 0 && distLeft >= 0)
	{

		node->AddRoute(leftNode);
		leftNode->AddRoute(node);

	}
	else if (distRight >= 0 && distLeft >= 0)
	{

		leftNode->RemoveRoute(rightNode->GetId());
		rightNode->RemoveRoute(leftNode->GetId());

		leftNode->AddRoute(node);
		rightNode->AddRoute(node);

		node->AddRoute(leftNode);
		node->AddRoute(rightNode);

	}

	InsertNode(node);

}

void GameRouteObject::RemoveNode(unsigned int localId)
{

	nodes->Remove(localId);

}

void GameRouteObject::InsertNode(Entities::RouteNodeObject* node)
{

	unsigned int newId = nodes->Add(node);

	if (newId + 1 > routeNodes)
	{

		routeNodes = newId + 1;

	}
}

GameRouteObject::~GameRouteObject()
{

	delete nodes;

}
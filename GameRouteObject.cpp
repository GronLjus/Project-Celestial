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

RouteNodeObject* GameRouteObject::GetRouteNode(Vector3 position) const
{

	RouteNodeObject* retval = nullptr;

	for (unsigned int i = 0; i < routeNodes && retval == nullptr;i++)
	{
	
		RouteNodeObject* val = nodes->GetValue(i);
		float dist = VectorDot(position, val->GetPosition());

		if (dist <= 1.0f)
		{

			retval = val;

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

	bool stop = false;

	for (unsigned int i = 0; i < routeNodes && !stop; i++)
	{

		RouteNodeObject* nodeHere = nodes->GetValue(i);

		if (nodeHere != nullptr)
		{

			Vector3 vect = node->GetPosition() - nodeHere->GetPosition();
			float distSQR = VectorDot(vect);
			float dist2 = -1.0f;
			bool isOnSame = false;

			for (unsigned int k = 0; k < nodeHere->GetRoutes() && !stop; k++)
			{

				RouteNodeObject* routeNode = nodeHere->GetRoute(k, dist2);

				if (routeNode->GetObjId() == GetId())
				{

					Vector3 dir = routeNode->GetPosition() - nodeHere->GetPosition();

					if (VectorDot(dir, vect) > 0)
					{

						isOnSame = true;

						if (dist2 > distSQR)
						{

							stop = true;
							nodeHere->RemoveRoute(routeNode->GetId());
							nodeHere->AddRoute(node);
							routeNode->AddRoute(node);

						}
					}
				}
			}

			if (!isOnSame)
			{

				stop = true;
				nodeHere->AddRoute(node);

			}
		}
	}

	unsigned int newId = nodes->Add(node);

	if (newId+1 > routeNodes)
	{

		routeNodes = newId+1;

	}
}

GameRouteObject::~GameRouteObject()
{

	delete nodes;

}
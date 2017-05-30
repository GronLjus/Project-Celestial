#include "stdafx.h"
#include "RoutingManager.h"

using namespace Entities;
using namespace CrossHandlers;
using namespace Resources;
using namespace CelestialMath;

RoutingManager::RoutingManager(unsigned int maxClock) : SplitManager(maxClock)
{

	gameObjects = nullptr;

}

void RoutingManager::Init(CelestialSlicedList<BaseObject*>* gameObjects)
{

	roads = new CelestialSlicedList<Route*>(32, nullptr);
	routeNodes = new CelestialSlicedList<RouteNodeObject*>(32, nullptr);
	nodeGroups = new CelestialSlicedList<NodeGroup*>(32, nullptr);

	init(gameObjects,
		routeNodes,
		roads,
		nodeGroups);

}

char* RoutingManager::Serialize(unsigned int &size)
{

	char** subData = nullptr;
	unsigned int* subSize = nullptr;
	unsigned int totalSize = 0;
	unsigned int subTot = 0;

	if (routeNodes->GetHighest() > 0)
	{

		subData = new char*[routeNodes->GetHighest()];
		subSize = new unsigned int[routeNodes->GetHighest()];

		for (unsigned int i = 0; i < routeNodes->GetHighest(); i++)
		{

			RouteNodeObject* subObject = routeNodes->GetValue(i);

			if (subObject != nullptr)
			{

				subData[subTot] = subObject->Serialize(subSize[subTot]);
				char* temp = new char[subSize[subTot] + sizeof(unsigned int)];
				memcpy(temp, &i, sizeof(unsigned int));

				memcpy(&temp[sizeof(unsigned int)], subData[subTot], subSize[subTot]);

				delete[] subData[subTot];
				subData[subTot] = temp;

				subSize[subTot] += sizeof(unsigned int);
				totalSize += subSize[subTot];
				subTot++;

			}
		}
	}

	char** routeData = nullptr;
	unsigned int* routeSize = nullptr;
	unsigned int totalRoutes = 0;
	unsigned int routeTot = 0;

	if (roads->GetHighest() > 0)
	{

		routeData = new char*[roads->GetHighest()];
		routeSize = new unsigned int[roads->GetHighest()];

		for (unsigned int i = 0; i < roads->GetHighest(); i++)
		{

			Route* road = roads->GetValue(i);

			if (road != nullptr)
			{

				char* tempVal = road->Serialize(routeSize[routeTot]);
				unsigned int objSize = routeSize[routeTot];
				routeSize[routeTot] += sizeof(unsigned int) * 2;
				routeData[routeTot] = new char[routeSize[routeTot]];

				memcpy(routeData[routeTot], &i, sizeof(unsigned int));
				memcpy(&routeData[routeTot][sizeof(unsigned int)], &objSize, sizeof(unsigned int));

				memcpy(&routeData[routeTot][sizeof(unsigned int) * 2], tempVal, objSize);

				totalRoutes += routeSize[routeTot];
				routeTot++;
				delete[] tempVal;

			}
		}
	}

	size = totalSize + totalRoutes + 1 + sizeof(unsigned int) * 2;
	char* byteVal = new char[size];
	byteVal[0] = SerializableType_ROUTEMANAGER;
	unsigned int offset = 1;

	memcpy(&byteVal[offset], &totalSize, sizeof(unsigned int));
	offset += sizeof(unsigned int);

	if (subData != nullptr)
	{

		for (unsigned int i = 0; i < subTot; i++)
		{

			memcpy(&byteVal[offset], subData[i], subSize[i]);
			delete[] subData[i];
			offset += subSize[i];

		}

		delete[] subData;
		delete[] subSize;

	}

	memcpy(&byteVal[offset], &totalRoutes, sizeof(unsigned int));
	offset += sizeof(unsigned int);

	if (routeData != nullptr)
	{

		for (unsigned int i = 0; i < routeTot; i++)
		{

			memcpy(&byteVal[offset], routeData[i], routeSize[i]);
			delete[] routeData[i];
			offset += routeSize[i];

		}

		delete[] routeData;
		delete[] routeSize;

	}

	return byteVal;

}

char* RoutingManager::Unserialize(char* data)
{

	unsigned int totalRoads = 0;
	memcpy(&totalRoads, data, sizeof(unsigned int));
	unsigned int offset = sizeof(unsigned int);

	while (offset - sizeof(unsigned int) < totalRoads)
	{

		unsigned int location = 0;
		unsigned int size = 0;
		memcpy(&location, &data[offset], sizeof(unsigned int));
		offset += sizeof(unsigned int);
		memcpy(&size, &data[offset], sizeof(unsigned int));
		offset += sizeof(unsigned int);

		if (data[offset] != SerializableType_ROUTE)
		{

			return nullptr;

		}

		offset++;
		Route* nRoad = new Route();
		nRoad->SetId(location);
		roads->AddAt(nRoad, location);
		nRoad->Unserialize(&data[offset]);

		unsigned int uN = nRoad->GetNode(Route::Direction_UP);
		unsigned lN = nRoad->GetNode(Route::Direction_DOWN);

		if (uN != 0 && lN != 0)
		{

			RouteNodeObject* lowerNode = routeNodes->GetValue(lN - 1);
			RouteNodeObject* upperNode = routeNodes->GetValue(uN - 1);

			lowerNode->AddRoute(nRoad);
			upperNode->AddRoute(nRoad);

		}

		offset += size - 1;

	}

	return &data[offset];

}

void RoutingManager::AddNode(Vector3 position, float width, unsigned int id)
{

	RouteNodeObject* node = new RouteNodeObject(position, 0.98f);
	node->SetId(id);
	routeNodes->AddAt(node, id);

}

unsigned int RoutingManager::AddGroup(GameObject* object)
{

	unsigned int newGroup = nodeGroups->Add(nullptr) + 1;
	NodeGroup* group = new NodeGroup();
	nodeGroups->Add(group, newGroup-1);

	object->SetNodeGroup(newGroup);
	return newGroup;

}

bool RoutingManager::DeleteObject(Resources::GameObject* obj)
{

	if (obj->GetType() == GameObjectType_GRIDROUTE)
	{

		return DeleteObject((GameGridObject*)obj);

	}
	else if (obj->GetType() == GameObjectType_ROUTE)
	{

		return DeleteObject((GameRouteObject*)obj);

	}

	return false;

}

bool RoutingManager::deleteNodeGroup(GameRouteObject* obj)
{

	RouteNodeObject* upperNode = routeNodes->GetValue(obj->GetUpperNode() - 1);
	NodeGroup* groupToKill = nodeGroups->GetValue(upperNode->GetGroup());

	if (groupToKill == nullptr || groupToKill->GetLocked() > 0)
	{

		return false;

	}

	unsigned int groupId = upperNode->GetGroup();

	for (unsigned int i = 0; i < routeNodes->GetHighest(); i++)
	{

		RouteNodeObject* node = routeNodes->GetValue(i);

		if (node != nullptr
			&& node->GetGroup() == groupId)
		{

			unsigned int routes = node->GetRoutes();

			for (unsigned int k = 0; k < routes; k++)
			{

				Route::Direction dir;
				Route* rte = node->GetRoute(k, dir);

				if (rte != nullptr)
				{

					unsigned int oppositeNode = rte->GetNode(dir);
					node->RemoveRoute(oppositeNode);

					if (node->GetRoutes() == 0)//The node is empty
					{

						routeNodes->Kill(i);

					}
				}
			}
		}
	}

	return true;

}

bool RoutingManager::DeleteObject(GameRouteObject * obj)
{

	RouteNodeObject* upperNode = routeNodes->GetValue(obj->GetUpperNode() - 1);
	RouteNodeObject* lowerNode = routeNodes->GetValue(obj->GetLowerNode() - 1);

	if (upperNode->GetGroup() == lowerNode->GetGroup())
	{

		//return deleteNodeGroup(obj);

	}

	unsigned int lowerLocal = upperNode->GetLocalId(lowerNode->GetId());
	Route* rte = upperNode->GetRoute(lowerLocal);
	
	if (rte->GetDirection() != Route::Direction_NA)//The road is occupied
	{

		return false;

	}

	//Kill the road
	deleteRoad(rte);

	return true;

}

void RoutingManager::deleteRoad(Route* rte)
{

	RouteNodeObject* upperNode = routeNodes->GetValue( rte->GetNode(Route::Direction_UP) - 1);
	RouteNodeObject* lowerNode = routeNodes->GetValue(rte->GetNode(Route::Direction_DOWN) - 1);

	lowerNode->RemoveRoute(upperNode->GetId());
	upperNode->RemoveRoute(lowerNode->GetId());
	roads->Kill(rte->GetId());

	if (upperNode->GetRoutes() == 0)//The upper node is empty
	{

		routeNodes->Kill(upperNode->GetId());

	}

	if (lowerNode->GetRoutes() == 0)
	{

		routeNodes->Kill(lowerNode->GetId());

	}

}

bool RoutingManager::checkRoute(unsigned int x, unsigned int z, RouteNodeObject* first, GameGridObject* obj, unsigned int &routeId)
{

	unsigned int xNeigh = obj->GetNode(x, z);
	RouteNodeObject* neighObj = routeNodes->GetValue(xNeigh);

	unsigned int localId = first->GetLocalId(xNeigh);
	Route* rte = first->GetRoute(localId);
	routeId = rte->GetId();

	return rte->GetDirection() != Route::Direction_NA;

}

bool RoutingManager::DeleteObject(GameGridObject* obj)
{

	bool occupied = false;

	unsigned int* roadsToKill = new unsigned int[obj->GetNodeHeigth() * obj->GetNodeWidth() * 2];
	unsigned int runningTotal = 0;

	for (unsigned int x = 0; x < max(obj->GetNodeWidth()-1, 2) && !occupied; x++)
	{

		for (unsigned int z = 0; z < obj->GetNodeHeigth()-1 && !occupied; z++)
		{

			unsigned int nodeId = obj->GetNode(x, z);
			obj->PopulateGrid(0, x, z);
			RouteNodeObject* nodeObjct = routeNodes->GetValue(nodeId);

			unsigned int xNext = x + 1;

			if (xNext < obj->GetNodeWidth())
			{

				occupied = checkRoute(xNext, z, nodeObjct, obj, roadsToKill[runningTotal]);

			}

			unsigned int zNext = z + 1;

			if (xNext < obj->GetNodeWidth())
			{

				occupied = checkRoute(x, zNext, nodeObjct, obj, roadsToKill[runningTotal]);

			}
		}
	}

	if (!occupied)//The grid has no objects on it
	{

		for (unsigned int i = 0; i < runningTotal; i++)
		{

			deleteRoad(roads->GetValue(roadsToKill[i]));

		}

		for (unsigned int x = 0; x < obj->GetNodeWidth() && !occupied; x++)
		{

			for (unsigned int z = 0; z < obj->GetNodeHeigth() && !occupied; z++)
			{

				obj->PopulateGrid(0, x, z);

			}
		}


	}

	return occupied;

}

RouteNodeObject* RoutingManager::getPreExistantNode(Vector3 position, unsigned int* objects, unsigned int amounts) const
{

	RouteNodeObject* preExist = nullptr;
	GameRouteObject* lastObject = nullptr;

	for (unsigned int i = 0; i < amounts && preExist == nullptr; i++)
	{

		GameObject* gameObj = (GameObject*)this->gameObjects->GetValue(objects[i]);

		if (gameObj->GetType() == GameObjectType_ROUTE)
		{

			GameRouteObject* obj = ((GameRouteObject*)gameObj);

			if (lastObject == nullptr)
			{

				position = obj->GetObjectCenterLine(position);

			}
			else
			{

				position = obj->GetObjectCenterLine(position, lastObject->GetDirection());

			}

			position.y = obj->GetPosition().y + obj->GetScale().y / 2;

			if (obj != nullptr && obj->GetLowerNode() != 0)
			{

				RouteNodeObject* lower = routeNodes->GetValue(obj->GetLowerNode() - 1);
				Vector3 dir = lower->GetPosition() - position;

				if (abs(VectorDot(dir)) < CELESTIAL_EPSILON)
				{

					preExist = lower;

				}
				else if (obj->GetUpperNode() != 0)
				{

					RouteNodeObject* upper = routeNodes->GetValue(obj->GetUpperNode() - 1);
					dir = upper->GetPosition() - position;

					if (abs(VectorDot(dir)) < CELESTIAL_EPSILON)
					{

						preExist = upper;

					}
				}
			}


			lastObject = obj;
		}
		else if(gameObj->GetType() == GameObjectType_GRIDROUTE)
		{

			lastObject = nullptr;
			GameGridObject* obj = ((GameGridObject*)gameObj);

			unsigned int* nodes = obj->GetNodes();

			float closestDist = -1.0f;

			for (unsigned int x = 0; x < obj->GetNodeWidth(); x++)
			{

				for (unsigned int z = 0; z < obj->GetNodeHeigth(); z++)
				{

					unsigned int nodeHere = obj->GetNode(x, z);
					RouteNodeObject* node = this->routeNodes->GetValue(nodeHere);

					float dist = VectorDot(node->GetPosition() - position);

					if (dist < closestDist || closestDist < 0.0f)
					{

						preExist = node;
						closestDist = dist;
					
					}
				}
			}
		}
	}

	return preExist;

}

void RoutingManager::handleObjectNode(RouteNodeObject* preExist, unsigned int* objects, unsigned int amounts)
{

	for (unsigned int i = 0; i < amounts; i++)
	{

		GameObject* object = ((GameObject*)this->gameObjects->GetValue(objects[i]));

		if (object->GetType() == GameObjectType_ROUTE)
		{

			GameRouteObject* obj = (GameRouteObject*)object;

			if (obj->GetLowerNode() == 0)//First node on a route
			{

				obj->SetLowerNode(preExist->GetId() + 1);

			}
			else if (obj->GetUpperNode() == 0)//Second node on a route
			{

				obj->SetUpperNode(preExist->GetId() + 1);

				RouteNodeObject* lower = routeNodes->GetValue(obj->GetLowerNode() - 1);
				unsigned int routeId = 0;

				if (!lower->ContainsRoute(preExist->GetId()))//The node lacks route
				{

					Vector3 dir = lower->GetPosition() - preExist->GetPosition();
					float dist = sqrt(VectorDot(dir));
					Route* route = new Route(dist, dir / dist);
					routeId = roads->Add(route);
					route->SetId(routeId);

					preExist->AddRoute(route);
					lower->AddRoute(route);

				}
				else
				{

					unsigned int lId = lower->GetLocalId(preExist->GetId());
					Route* rt = lower->GetRoute(lId);
					routeId = rt->GetId();

				}

				obj->SetRoute(routeId + 1);

			}
			else if (preExist->GetId() + 1 != obj->GetLowerNode() &&
				preExist->GetId() + 1 != obj->GetUpperNode())
			{

				RouteNodeObject* lower = routeNodes->GetValue(obj->GetLowerNode() - 1);
				RouteNodeObject* upper = routeNodes->GetValue(obj->GetUpperNode() - 1);

				handleSplit(preExist, lower, upper);

				obj->SetMiddleNode(preExist->GetId() + 1);


			}

			if (preExist->GetGroup() == 0)
			{

				preExist->SetGroup(obj->GetNodeGroup());

			}
		}
	}
}

void RoutingManager::HandleNode(unsigned int nodeId, unsigned int* objects, unsigned int amounts)
{

	if (amounts != 0)
	{

		RouteNodeObject* preExist = routeNodes->GetValue(nodeId);
		handleObjectNode(preExist, objects, amounts);

	}
}

unsigned int RoutingManager::AddNode(Vector3 position, unsigned int* objects, unsigned int amounts)
{

	if (amounts != 0)
	{

		RouteNodeObject* preExist = getPreExistantNode(position, objects, amounts);

		if (preExist == nullptr)
		{

			GameRouteObject* obj = ((GameRouteObject*)this->gameObjects->GetValue(objects[0]));
			position.y = obj->GetPosition().y + obj->GetScale().y / 2;
			preExist = new RouteNodeObject(position, 0.98f);
			preExist->SetId(routeNodes->Add(preExist));

		}

		handleObjectNode(preExist, objects, amounts);
		return preExist->GetId()+1;

	}

	return 0;

}

unsigned int RoutingManager::FindNode(Vector3 position, unsigned int* objects, unsigned int amounts)
{

	if (amounts != 0)
	{

		RouteNodeObject* preExist = getPreExistantNode(position, objects, amounts);

		if (preExist != nullptr)
		{

			GameRouteObject* obj = ((GameRouteObject*)this->gameObjects->GetValue(objects[0]));
			return preExist->GetId() + 1;

		}
	}

	return 0;

}

void RoutingManager::addNewRoad(RouteNodeObject* node1, RouteNodeObject* node2)
{

	Vector3 dir = node1->GetPosition() - node2->GetPosition();
	float dist = sqrt(VectorDot(dir));
	Route* route = new Route(dist, dir / dist);
	route->SetId(roads->Add(route));

	node2->AddRoute(route);
	node1->AddRoute(route);

}

void RoutingManager::AddRoad(unsigned int node1, unsigned int node2)
{

	RouteNodeObject* node1Obj = routeNodes->GetValue(node1);
	RouteNodeObject* node2Obj = routeNodes->GetValue(node2);

	if (node1Obj != nullptr && node2Obj != nullptr)
	{

		addNewRoad(node1Obj, node2Obj);

	}
}

void RoutingManager::PopulateGrid(GameGridObject* grid, float width)
{

	grid->SizeGrid(width);

	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 dir = grid->GetDirection();

	dir /= sqrt(VectorDot(dir));

	Vector3 side = VectorCross(dir, up);

	Vector3 cornerPos = grid->GetPosition() - 
		(dir*(grid->GetScale().z / 2)) - 
		(side*(grid->GetScale().x / 2));

	float xSpacing = (grid->GetScale().x / grid->GetNodeWidth());
	float zSpacing = (grid->GetScale().z / grid->GetNodeHeigth());

	float xStart = xSpacing / 2;
	float zStart = zSpacing / 2;

	for (unsigned int x = 0; x < grid->GetNodeWidth(); x++)
	{

		for (unsigned int z = 0; z < grid->GetNodeHeigth(); z++)
		{

			Vector3 xOffset = side * (xStart + ((x)*xSpacing));
			Vector3 zOffset = dir * (zStart + ((z)*zSpacing));
			Vector3 offset = xOffset + zOffset;

			Vector3 nodePos = cornerPos + offset;

			nodePos.y = grid->GetPosition().y + grid->GetScale().y / 2;

			RouteNodeObject* nodeObj = new RouteNodeObject(nodePos, width);
			nodeObj->SetGroup(grid->GetNodeGroup());
			nodeObj->SetId(routeNodes->Add(nodeObj));
			grid->PopulateGrid(nodeObj->GetId(), x, z);

			//Link the node with its' neighbours
			if (z > 0)
			{

				RouteNodeObject* upperNode = routeNodes->GetValue(grid->GetNode(x, z - 1));
				addNewRoad(nodeObj, upperNode);

			}

			if (x > 0)
			{

				RouteNodeObject* leftNode = routeNodes->GetValue(grid->GetNode(x - 1, z));
				addNewRoad(nodeObj, leftNode);

			}
		}
	}
}

void RoutingManager::ClearNodes()
{

	roads->KillList();
	routeNodes->KillList();
	roads->Reset();
	routeNodes->Reset();

}

void RoutingManager::ClearRoads(RouteNodeObject* node)
{

	for (unsigned int i = 0; i < node->GetMaxRoutes(); i++)
	{

		Route::Direction dir;
		Route* rt = node->GetRoute(i, dir);

		if (rt != nullptr)
		{

			RouteNodeObject* neigh = routeNodes->GetValue(rt->GetNode(dir) - 1);
			node->RemoveRoute(neigh->GetId());
			neigh->RemoveRoute(node->GetId());

			roads->Kill(rt->GetId());

		}
	}
}

RouteNodeObject* RoutingManager::GetNode(unsigned int id) const
{

	return routeNodes->GetValue(id);

}

RoutingManager::~RoutingManager()
{


}
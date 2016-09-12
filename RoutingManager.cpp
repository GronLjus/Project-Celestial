#include "stdafx.h"
#include "RoutingManager.h"

using namespace Entities;
using namespace CrossHandlers;
using namespace Resources;
using namespace CelestialMath;

RoutingManager::RoutingManager()
{

	gameObjects = nullptr;
	roads = new CelestialSlicedList<Road*>(32, nullptr);
	routeNodes = new CelestialSlicedList<RouteNodeObject*>(32, nullptr);
	travelObjects = new CelestialList<GameTravelObject*>();
	pathFindVal = 1;

	totalPaths = 128;
	path = new unsigned[totalPaths];

	maxSOuts = 128;
	scriptOuts = new unsigned int[maxSOuts];

	offsetDist = 0.25f;

}

void RoutingManager::Init(CelestialSlicedList<BaseObject*>* gameObjects)
{

	this->gameObjects = gameObjects;

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
				totalSize += subSize[subTot];
				subTot++;

			}
		}
	}

	size = totalSize + 1 + sizeof(unsigned int);
	char* byteVal = new char[size];
	byteVal[0] = SerializableType_ROUTEMANAGER;
	memcpy(&byteVal[1], &totalSize, sizeof(unsigned int));
	unsigned int offset = 1 + sizeof(unsigned int);

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

	return byteVal;

}

char* RoutingManager::Unserialize(char* data)
{

	return data;

}

unsigned int RoutingManager::AddNode(Vector3 position, GameRouteObject* obj)
{

	return 0;

}

unsigned int RoutingManager::addToOutScripts(unsigned int script, unsigned int place)
{

	if (place >= maxSOuts)
	{

		maxSOuts += 128;
		unsigned int* newScripts = new unsigned int[maxSOuts];
		memcpy(newScripts, scriptOuts, maxSOuts - 128 * sizeof(unsigned int));

	}

	scriptOuts[place] = script;
	return place + 1;

}

void RoutingManager::handleNearNode(GameTravelObject* obj, RouteNodeObject* currentNode, RouteNodeObject* goalNode, unsigned int time)
{

	bool travelOn = false;

	if (goalNode->GetObjId() == 0)//The node is open
	{

		if (goalNode->GetRoad() == 0 &&
			obj->GetStatus() == TravelStatus_WAITING)//Object is at Intersection and waiting for go-ahead
		{

			if (obj->GetFinalGoalNode() == obj->GetGoalNode())//The intersection is the final goal
			{

				travelOn = true;

			}
			else //The final goal lies beyond
			{

				bool reCalc;
				unsigned int nextGoal = obj->PeekNextGoal(reCalc);

				if (reCalc)
				{

					unsigned int amounts = pathFind(currentNode,
						goalNode,
						routeNodes->GetValue(obj->GetFinalGoalNode()));

					if (amounts > 0)
					{

						obj->SetGoalNode(path);

					}

					nextGoal = obj->PeekNextGoal(reCalc);

				}

				RouteNodeObject* nextGoalNode = routeNodes->GetValue(nextGoal);
				unsigned int routeId = goalNode->GetLocalId(nextGoalNode->GetId());

				if (goalNode->CanTravel(routeId))//The route can be traveled
				{

					travelOn = true;

				}
				else//The road is occupied
				{

					unsigned int amounts = pathFindOpenRoad(currentNode,
						goalNode,
						routeNodes->GetValue(obj->GetFinalGoalNode()));

					if (amounts > 0)//There is a way around
					{

						obj->SetGoalNode(path);
						travelOn = true;

					}
				}
			}
		}
		else if(goalNode->GetRoad() != 0)//Object has arrived near a roadnode
		{

			travelOn = true;

		}
	}

	unsigned int localId = goalNode->GetLocalId(currentNode->GetId());

	if (travelOn)
	{

		obj->SetStatus(TravelStatus_TRAVELNEAR);
		goalNode->SetObjId(obj->GetId());
		goalNode->QueueRoute(localId, -(obj->GetQueueLength()), time);

	}
	else if(obj->GetStatus() != TravelStatus_WAITING)
	{

		obj->SetStatus(TravelStatus_WAITING);

	}
}

float RoutingManager::travelObject(GameTravelObject* obj, RouteNodeObject* currentNode, RouteNodeObject* goalNode, Vector3 dir, float distance, unsigned int time)
{

	bool slowDown = obj->GetStatus() == TravelStatus_TRAVELING &&
		goalNode->GetRoad() == 0;

	float offset = offsetDist + obj->GetQueueLength() / 2;
	distance -= !slowDown ? 0 : offset;
	float fact = distance;
	float timeFact = (float)(time - obj->GetLastTime()) / 1000.0f;
	float spd = obj->GetSpeed() * timeFact;

	//Object has a long way to go
	if (distance >= spd)
	{

		fact = spd;

	}

	obj->SetPosition(obj->GetPosition() + (dir * fact));
	obj->UpdateMatrix();
	return fact;

}

void RoutingManager::handleTravel(GameTravelObject* obj, RouteNodeObject* currentNode, RouteNodeObject* goalNode, Vector3 dir, float distance, unsigned int time)
{

	if (obj->GetStatus() == TravelStatus_QUEUEING)//The object is in a queue
	{

		unsigned int localId = goalNode->GetLocalId(currentNode->GetId());
		float lastQ = obj->LastQueue();
		unsigned int lastTime = goalNode->GetQTime(localId);

		if (lastTime != 0 &&
			lastTime >= obj->GetQTime())//The queue has moved
		{

			float diff = goalNode->GetQDiff(localId);
			lastQ += diff;
			obj->QueueTime(time);

		}
		
		if (lastQ > CELESTIAL_EPSILON)
		{

			lastQ -= travelObject(obj, currentNode, goalNode, dir, lastQ, time);
			obj->SetLastQueue(lastQ);

		}
	}
	else
	{

		travelObject(obj, currentNode, goalNode, dir, distance, time);
		Vector3 traveled = currentNode->GetPosition() + Vector3(0.0f, obj->GetScale().y / 2, 0.0f) - obj->GetPosition();
		float traveledDist = VectorDot(traveled);

		if (traveledDist > offsetDist &&
			currentNode->GetObjId() == obj->GetId())//The object has traveled beyond the objects threshold
		{

			currentNode->SetObjId(0);

		}
	}
}

bool RoutingManager::handleNodeArrival(GameTravelObject* obj, RouteNodeObject* currentNode, RouteNodeObject* goalNode)
{

	bool killNode = false;
	obj->SetNode(goalNode->GetId());
	
	unsigned int localId = currentNode->GetLocalId(goalNode->GetId());
	currentNode->TravelDone(localId, obj->GetId());

	if (obj->GetFinalGoalNode() == goalNode->GetId())//The object is at the final target
	{

		killNode = true;
		obj->SetStatus(TravelStatus_READY);

	}
	else//The object is at an intermediary node
	{

		bool recalculate = obj->StepGoal();

		if (recalculate)//We are out of goals
		{

			unsigned int amounts = pathFind(
				goalNode,
				goalNode,
				routeNodes->GetValue(obj->GetFinalGoalNode()));

			if (amounts > 0)
			{

				obj->SetGoalNode(path);

			}
		}

		RouteNodeObject* nextStop = routeNodes->GetValue(obj->GetGoalNode());
		obj->Point(nextStop->GetPosition() + Vector3(0.0f, obj->GetScale().y / 2, 0.0f));
		unsigned int localId = goalNode->GetLocalId(nextStop->GetId());
		goalNode->TravelRoute(localId, obj->GetId());

		obj->SetStatus(TravelStatus_TRAVELING);

	}

	return killNode;

}

void RoutingManager::handlePrimedObject(GameTravelObject* obj)
{

	RouteNodeObject* node = routeNodes->GetValue(obj->GetNode());
	RouteNodeObject* end = routeNodes->GetValue(obj->GetFinalGoalNode());
	unsigned int paths = pathFindOpenRoad(node, node, end);

	if (paths > 0)
	{

		obj->SetGoalNode(path);
		obj->SetStatus(TravelStatus_TRAVELING);

		RouteNodeObject* nextStop = routeNodes->GetValue(obj->GetGoalNode());
		unsigned int local = node->GetLocalId(nextStop->GetId());
		node->TravelRoute(local, obj->GetId());

		obj->Point(nextStop->GetPosition() + Vector3(0.0f, obj->GetScale().y / 2, 0.0f));

	}
}

void RoutingManager::handleQueing(GameTravelObject* obj, unsigned int localId, RouteNodeObject* goalNode, float distance, unsigned int time)
{

	float qDist = goalNode->GetQuelength(localId) + offsetDist;

	if (obj->GetStatus() != TravelStatus_QUEUEING)//The object isn't in the queue
	{

		qDist += obj->GetQueueLength()/2;

		if (distance - qDist <= CELESTIAL_EPSILON)//The object is in the queue
		{

			obj->SetLastQueue(0.0f);
			obj->QueueTime(time);
			obj->SetStatus(TravelStatus_QUEUEING);
			goalNode->QueueRoute(localId, obj->GetQueueLength(), 0);

		}
	}
}

unsigned int* RoutingManager::Update(unsigned int time, unsigned int &scripts)
{

	unsigned int scriptPlace = 0;
	CelestialListNode<GameTravelObject*>* lastNode = nullptr;
	CelestialListNode<GameTravelObject*>* node = travelObjects->GetFirstNode();

	while (node != nullptr)
	{

		bool killNode = false;
		GameTravelObject* obj = node->GetNodeObject();

		if (obj->GetStatus() == TravelStatus_PRIMED)
		{

			handlePrimedObject(obj);

		}

		if (obj->GetStatus() != TravelStatus_PRIMED)
		{

			RouteNodeObject* nodeObject = routeNodes->GetValue(obj->GetNode());
			RouteNodeObject* goalObject = routeNodes->GetValue(obj->GetGoalNode());

			Vector3 dir = goalObject->GetPosition() + Vector3(0.0f, obj->GetScale().y / 2, 0.0f) - obj->GetPosition();
			float distance = VectorDot(dir);
			distance = sqrt(distance);
			dir /= distance;
			unsigned int localId = goalObject->GetLocalId(nodeObject->GetId());
			float qDist = goalObject->GetQuelength(localId);

			float offset = offsetDist + obj->GetQueueLength() / 2;

			if (obj->GetStatus() != TravelStatus_TRAVELNEAR)
			{

				if (obj->GetStatus() != TravelStatus_WAITING &&
					obj->GetStatus() != TravelStatus_QUEUEING)
				{

					handleQueing(obj, localId, goalObject, distance, time);

				}
				
				if (distance - offset <= CELESTIAL_EPSILON)//Object is near a node
				{

					handleNearNode(obj, nodeObject, goalObject, time);

				}
			}

			if (obj->GetStatus() != TravelStatus_WAITING)
			{

				if (distance > CELESTIAL_EPSILON)//The object has not arrived 
				{

					handleTravel(obj, nodeObject, goalObject, dir, distance, time);

				}
				else//The object has arrived at a node
				{

					unsigned int script = obj->GetTravelNodeScript();

					if (script > 0)
					{

						scriptPlace = addToOutScripts(obj->GetId(), scriptPlace);

					}

					killNode = handleNodeArrival(obj, nodeObject, goalObject);

				}
			}

		}

		if (!killNode)
		{

			lastNode = node;
			node = node->GetNext();

		}
		else
		{

			if (lastNode != nullptr)
			{

				lastNode->SetNext(node->GetNext());
				delete node;
				node = lastNode->GetNext();

				if (node == nullptr)
				{

					travelObjects->SetLast(lastNode);

				}

				travelObjects->DecreaseCount();

			}
			else
			{

				travelObjects->PopElement();
				node = nullptr;

			}
		}

		obj->Time(time);

	}

	scripts = scriptPlace;
	return scriptOuts;

}

Road::Direction RoutingManager::getDirection(RouteNodeObject* start, RouteNodeObject* next) const
{

	if (start->GetRoad() != 0)
	{

		unsigned int subId = 0;
		RouteNodeObject* subObject = nullptr;

		for (unsigned int i = 0; i < start->GetRoutes() && subObject == nullptr; i++)
		{

			float dist;
			subObject = start->GetRoute(i, dist);
			subId = i;

			if (subObject != next)
			{

				subObject = nullptr;

			}
		}

		subId++;

		if (subId == start->GetDownId())
		{

			return Road::Direction_DOWN;

		}
		else if (subId == start->GetUpId())
		{

			return Road::Direction_UP;

		}
	}
	else if(next->GetRoad() != 0)
	{

		Road::Direction reverseDir = getDirection(next, start);

		if (reverseDir == Road::Direction_DOWN)
		{

			return Road::Direction_UP;

		}
		else if (reverseDir == Road::Direction_UP)
		{

			return Road::Direction_DOWN;

		}
	}

	return Road::Direction_NA;

}

RouteNodeObject* RoutingManager::getPreExistantNode(Vector3 position, unsigned int* objects, unsigned int amounts) const
{

	RouteNodeObject* preExist = nullptr;
	GameRouteObject* lastObject = nullptr;

	for (unsigned int i = 0; i < amounts && preExist == nullptr; i++)
	{

		GameRouteObject* obj = ((GameRouteObject*)this->gameObjects->GetValue(objects[i]));

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

	return preExist;

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

		for (unsigned int i = 0; i < amounts; i++)
		{

			GameRouteObject* obj = ((GameRouteObject*)this->gameObjects->GetValue(objects[i]));

			if (obj->GetLowerNode() == 0)
			{

				obj->SetLowerNode(preExist->GetId() + 1);

			}
			else if (obj->GetUpperNode() == 0)
			{

				obj->SetUpperNode(preExist->GetId() + 1);

				RouteNodeObject* lower = routeNodes->GetValue(obj->GetLowerNode() - 1);

				if (lower->GetUpId() != 0 && lower->GetDownId() == 0)
				{

					reverseNode(preExist, Road::Direction_UP);
					preExist->AddRoute(lower, Road::Direction_UP);
					lower->AddRoute(preExist, Road::Direction_DOWN);

				}
				else if(lower->GetUpId() == 0)
				{

					preExist->AddRoute(lower, Road::Direction_DOWN);
					lower->AddRoute(preExist, Road::Direction_UP);

				}
				else//Node is an intersection
				{

					preExist->AddRoute(lower);
					lower->AddRoute(preExist);

				}

				handleRoad(lower);

			}
			else if (preExist->GetId() + 1 != obj->GetLowerNode() &&
				preExist->GetId() + 1 != obj->GetUpperNode())
			{


				RouteNodeObject* lower = routeNodes->GetValue(obj->GetLowerNode() - 1);
				RouteNodeObject* upper = routeNodes->GetValue(obj->GetUpperNode() - 1);
				lower->RemoveRoute(upper->GetId());
				upper->RemoveRoute(lower->GetId());
				
				lower->AddRoute(preExist, Road::Direction_UP);
				upper->AddRoute(preExist, Road::Direction_DOWN);

				preExist->AddRoute(lower, Road::Direction_DOWN);
				preExist->AddRoute(upper, Road::Direction_UP);

				obj->SetMiddleNode(preExist->GetId() + 1);

			}
		}

		handleRoad(preExist);

		return preExist->GetId()+1;

	}

	return 0;

}

void RoutingManager::reverseNode(RouteNodeObject* object, Road::Direction dir)
{

	unsigned int dId = object->GetDownId();
	unsigned int uId = object->GetUpId();
	object->SetUpId(dId);
	object->SetDownId(uId);

	RouteNodeObject* nextNode = nullptr;

	if (dir == Road::Direction_UP && uId != 0)
	{

		nextNode = object->GetRoute(uId - 1);

	}
	else if (dir == Road::Direction_DOWN && dId != 0)
	{

		nextNode = object->GetRoute(dId - 1);

	}

	if(nextNode != nullptr && nextNode->GetRoad() != 0)
	{ 

		reverseNode(nextNode, dir);
	
	}
}

void RoutingManager::addToRoad(RouteNodeObject* object, unsigned int road)
{

	Road* roadObj = roads->GetValue(road);

	if (roadObj == nullptr)
	{

		return;

	}

	object->SetRoad(road + 1);
	roadObj->IncreaseNodes(1);

}

void RoutingManager::mergeRoads(RouteNodeObject* node, unsigned int road)
{
	
	if (node->GetRoad() == 0)
	{

		return;

	}

	if (node->GetRoad() - 1 != road)
	{

		handleOldRoad(node);//Remove the old road from the object
		addToRoad(node, road);
		
	}

	for (unsigned int i = 0; i < node->GetRoutes(); i++)
	{

		RouteNodeObject* objectToChange = node->GetRoute(i);

		if (objectToChange != nullptr && 
			objectToChange->GetRoad() != 0 &&
			objectToChange->GetRoad() - 1 != road)
		{

			mergeRoads(objectToChange, road);

		}
	}
}

void RoutingManager::handleOldRoad(RouteNodeObject* target)
{

	if (target->GetRoad() != 0)
	{

		Road* oldRoad = roads->GetValue(target->GetRoad() - 1);
		oldRoad->DecreaseNodes(1);

		if (oldRoad->GetNodes() == 0)
		{

			roads->Kill(target->GetRoad() - 1);

		}
	}
}

void RoutingManager::addToNewRoad(RouteNodeObject* object)
{

	Road* road = new Road();
	road->IncreaseNodes(1);
	unsigned int rd = roads->Add(road) + 1;
	object->SetRoad(rd);

}

void RoutingManager::handleRoad(RouteNodeObject* object)
{

	if (object->GetRoutes() > 2)//The target should be an intersection
	{

		if (object->GetRoad() != 0)//The target used to be a road
		{

			unsigned int oldId = object->GetRoad();
			handleOldRoad(object);
			object->SetRoad(0);

			bool first = false;

			for (unsigned int i = 0; i < object->GetRoutes(); i++)
			{

				RouteNodeObject* subNode = object->GetRoute(i);

				if (first && 
					subNode != nullptr && 
					subNode->GetRoutes() <= 2)
				{

					addToNewRoad(subNode);
					mergeRoads(subNode, subNode->GetRoad() - 1);

				}

				first = subNode != nullptr || first;

			}
		}

		object->SetDownId(0);
		object->SetUpId(0);

	}
	else if(object->GetRoad() == 0)
	{

		bool virgin = true;

		if (object->GetRoutes() > 0)
		{

			unsigned int firstId = 0;
			unsigned int secondId = 0;

			RouteNodeObject* subObj = nullptr;
			float dist;

			for (unsigned int i = 0; i < object->GetMaxRoutes(); i++)
			{

				subObj = object->GetRoute(i, dist);

				if (subObj != nullptr &&
					virgin)
				{

					firstId = i;

				}
				else if (subObj != nullptr)
				{

					secondId = i;

				}

				virgin = !(subObj != nullptr
					&& subObj->GetRoad() != 0);

			}

			if (!virgin)
			{

				addToRoad(object, subObj->GetRoad() - 1);

				if (object->GetRoutes() > 1)//The node has more then one connection
				{

					RouteNodeObject* first = object->GetRoute(firstId);
					RouteNodeObject* second = object->GetRoute(secondId);

					//The connections are of different roads
					if (first->GetRoad() != second->GetRoad() &&
						first->GetRoad() != 0 &&
						second->GetRoad() != 0)
					{

						mergeRoads(object, object->GetRoad() - 1);

					}
				}
			}
		}

		if(virgin)//The node is not connected to a road
		{

			addToNewRoad(object);

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

RouteNodeObject* RoutingManager::GetNode(unsigned int id) const
{

	return routeNodes->GetValue(id);

}

unsigned int RoutingManager::pathFindOpenRoad(RouteNodeObject* at, RouteNodeObject* start, RouteNodeObject* end)
{

	unsigned int returnVal = 0;
	float smallestDist = -1.0f;

	unsigned char offset = at->GetId() == start->GetId() ? 0 : 1;
	unsigned int* smallestPath = new unsigned int[totalPaths + offset];
	smallestPath[0] = start->GetId();
	unsigned char smallestPaths = 0;

	for (unsigned int i = 0; i < start->GetRoutes(); i++)
	{

		RouteNodeObject* subObject = start->GetRoute(i);

		if (subObject != nullptr &&
			subObject->GetId() != at->GetId())
		{

			bool isCandidate = start->CanTravel(i);

			if (isCandidate)
			{

				float dist;
				unsigned int paths = pathFind(start, subObject, end, dist);

				if (paths > 0)
				{

					if (dist < smallestDist ||
						smallestDist < 0)
					{

						smallestDist = dist; 
						smallestPaths = paths;
						memcpy(&smallestPath[offset], path, smallestPaths * sizeof(unsigned int));

					}
				}
			}
		}
	}

	if (smallestPaths > 0)
	{

		smallestPaths = min(totalPaths, smallestPaths + offset);
		memcpy(path, smallestPath, smallestPaths * sizeof(unsigned int));

	}

	delete[] smallestPath;
	return smallestPaths;

}

unsigned int RoutingManager::pathFind(RouteNodeObject* start, RouteNodeObject* through, RouteNodeObject* end)
{

	float dist;
	return pathFind(start, through, end, dist);

}

unsigned int RoutingManager::pathFind(RouteNodeObject* start, RouteNodeObject* through, RouteNodeObject* end, float &totalDist)
{

	if (end == nullptr)
	{

		return 0;

	}

	prio_queue openSet;
	prio_queue::nodeVal startNode;
	startNode.index = through->GetId();
	startNode.parentIndex = startNode.index;
	startNode.targetDist = VectorDot(end->GetPosition() - through->GetPosition());
	startNode.traveledDist = 0.0f;
	openSet.push(startNode);

	std::vector<unsigned int> closedSet;

	if (start->GetId() != through->GetId())
	{

		start->SetClosedset(pathFindVal);
		closedSet.push_back(start->GetId());

	}

	through->SetParent(start->GetId());
	through->SetOpenset(pathFindVal);
	through->SetStep(1);
	start->SetStep(0);

	bool reachedEnd = false;
	float dist = -1.0f;

	while (!openSet.empty() && !reachedEnd)
	{

		prio_queue::nodeVal currentNode = openSet.top();
		openSet.pop();
		closedSet.push_back(currentNode.index);
		RouteNodeObject* nodeObj = routeNodes->GetValue(currentNode.index);
		nodeObj->SetClosedset(pathFindVal);
		reachedEnd = currentNode.index == end->GetId();

		for (unsigned int i = 0; i < nodeObj->GetRoutes() && !reachedEnd; i++)
		{

			float dist;
			RouteNodeObject* neigh = nodeObj->GetRoute(i, dist);
			float tDist = VectorDot(end->GetPosition() - neigh->GetPosition());
			float trDist = currentNode.traveledDist + VectorDot(nodeObj->GetPosition() - neigh->GetPosition());
			float heuristic = tDist + trDist;

			if (neigh != nullptr)
			{
				//Check if the node exists in the closed set
				bool closed = neigh->GetClosedSet() == pathFindVal;

				if (!closed )
				{

					//Check if the node already exists in the open set
					bool exists = neigh->GetOpenSet() == pathFindVal;

					if (!exists ||
						(exists && neigh->GetHeuristic() > heuristic))
					{

						//We need to replace the node in the openset
						if (exists)
						{

							prio_queue newOpenSet;

							for (unsigned int j = 0; j < openSet.oVect.size(); j++)
							{

								if (openSet.oVect[j].index != neigh->GetId())
								{

									newOpenSet.push(openSet.oVect[j]);

								}

							}

							openSet = newOpenSet;

						}


						prio_queue::nodeVal neighNode;
						neighNode.index = neigh->GetId();
						neighNode.parentIndex = currentNode.index;
						neighNode.targetDist = tDist;
						neighNode.traveledDist = trDist;

						neigh->SetHeuristic(heuristic);
						neigh->SetOpenset(pathFindVal);
						neigh->SetParent(currentNode.index);
						neigh->SetStep(nodeObj->GetStep() + 1);
						reachedEnd = neighNode.index == end->GetId();

						if (!reachedEnd)
						{

							openSet.push(neighNode);

						}
						else
						{

							dist = neighNode.traveledDist;

						}
					}
				}
			}
		}
	}

	unsigned int retVal = 0;
	totalDist = 0;

	if (reachedEnd)
	{

		RouteNodeObject* node = end;

		while (node->GetId() != start->GetId())
		{

			if (node->GetStep() - 1 < totalPaths)
			{

				path[node->GetStep() - 1] = node->GetId();

			}

			RouteNodeObject* last = node;
			node = routeNodes->GetValue(node->GetParent());

			totalDist += VectorDot(last->GetPosition() - node->GetPosition());

			retVal++;

		}

	}

	pathFindVal++;
	pathFindVal = pathFindVal == 0 ? 1 : pathFindVal;
	return retVal;

}

RouteNodeObject* RoutingManager::getNeighbour(RouteNodeObject* object, Road::Direction dir) const
{

	RouteNodeObject* returnValue = nullptr;

	if (object->GetRoad() != 0)
	{

		if (dir == Road::Direction_DOWN &&
			object->GetDownId() != 0)
		{

			float dist;
			return object->GetRoute(object->GetDownId() - 1, dist);

		}
		else if (dir == Road::Direction_UP &&
			object->GetUpId() != 0)
		{

			float dist;
			return object->GetRoute(object->GetUpId() - 1, dist);

		}
		else
		{

			return nullptr;

		}
	}
}

void RoutingManager::Travel(GameTravelObject* object, unsigned int goal, unsigned int time)
{

	object->SetFinalGoalNode(goal);
	object->SetStatus(TravelStatus_PRIMED);
	travelObjects->PushElement(object);
	object->Time(time);

}

void RoutingManager::Spawn(GameTravelObject* object, unsigned int cell)
{

	object->SetNode(cell);
	object->SetPosition(routeNodes->GetValue(cell)->GetPosition() + Vector3(0.0f, object->GetScale().y/2,0.0f));
	RouteNodeObject* node = routeNodes->GetValue(cell);
	node->SetObjId(object->GetId());
	object->UpdateMatrix();

}

RoutingManager::~RoutingManager()
{

	routeNodes->KillList();
	roads->KillList();
	delete roads;
	delete routeNodes;
	delete travelObjects;
	delete[] path;
	delete[] scriptOuts;

}
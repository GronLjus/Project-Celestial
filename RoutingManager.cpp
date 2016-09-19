#include "stdafx.h"
#include "RoutingManager.h"

using namespace Entities;
using namespace CrossHandlers;
using namespace Resources;
using namespace CelestialMath;

RoutingManager::RoutingManager()
{

	gameObjects = nullptr;
	roads = new CelestialSlicedList<Route*>(32, nullptr);
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

		if (goalNode->GetRoutes() > 2 &&
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
				Route::Direction dir;
				unsigned int routeId = goalNode->GetLocalId(nextGoalNode->GetId());
				Route* route = goalNode->GetRoute(routeId, dir);

				if (route->CanTravel(dir))//The route can be traveled
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
		else if(goalNode->GetRoutes() <= 2)//Object has arrived near a roadnode
		{

			travelOn = true;

		}
	}


	if (travelOn)
	{

		obj->SetStatus(TravelStatus_TRAVELNEAR);
		goalNode->SetObjId(obj->GetId());
		unsigned int localId = currentNode->GetLocalId(goalNode->GetId());
		Route* route = currentNode->GetRoute(localId);
		route->Queue(-(obj->GetQueueLength()), time);

	}
	else if(obj->GetStatus() != TravelStatus_WAITING)
	{

		obj->SetStatus(TravelStatus_WAITING);

	}
}

float RoutingManager::travelObject(GameTravelObject* obj, RouteNodeObject* currentNode, RouteNodeObject* goalNode, Vector3 dir, float distance, unsigned int time)
{

	bool slowDown = obj->GetStatus() == TravelStatus_TRAVELING &&
		goalNode->GetRoutes() > 2;

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

		unsigned int localId = currentNode->GetLocalId(goalNode->GetId());
		float lastQ = obj->LastQueue();
		Route* rte = currentNode->GetRoute(localId);
		unsigned int lastTime = rte->GetQTime();

		if (lastTime != 0 &&
			lastTime >= obj->GetQTime())//The queue has moved
		{

			float diff = rte->GetQDiff();
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

	}

	Vector3 objEndPoint = obj->GetPosition() - (obj->GetDirection()*(obj->GetQueueLength() / 2));

	Vector3 traveled = currentNode->GetPosition() + Vector3(0.0f, obj->GetScale().y / 2, 0.0f) - objEndPoint;
	float traveledDist = VectorDot(traveled);

	if (traveledDist > offsetDist * offsetDist &&
		currentNode->GetObjId() == obj->GetId())//The object has traveled beyond the objects threshold
	{

		currentNode->SetObjId(0);

	}
}

bool RoutingManager::handleNodeArrival(GameTravelObject* obj, RouteNodeObject* currentNode, RouteNodeObject* goalNode)
{

	bool killNode = false;
	obj->SetNode(goalNode->GetId());
	
	unsigned int localId = currentNode->GetLocalId(goalNode->GetId());
	Route* rte = currentNode->GetRoute(localId);
	rte->TravelDone(obj->GetId());

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
		Route::Direction dir;
		Route* rte = goalNode->GetRoute(localId, dir);
		rte->Travel(obj->GetId(), dir);

		if (obj->GetStatus() != TravelStatus_QUEUEING)
		{

			obj->SetStatus(TravelStatus_TRAVELING);

		}
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
		Route::Direction dir;
		Route* rte = node->GetRoute(local, dir);
		rte->Travel(obj->GetId(), dir);

		obj->Point(nextStop->GetPosition() + Vector3(0.0f, obj->GetScale().y / 2, 0.0f));

	}
}

void RoutingManager::handleQueing(GameTravelObject* obj, unsigned int localId, Route* route, float distance, unsigned int time)
{

	float qDist = route->GetQuelength() + offsetDist;

	if (obj->GetStatus() != TravelStatus_QUEUEING)//The object isn't in the queue
	{

		qDist += obj->GetQueueLength()/2;

		if (distance - qDist <= CELESTIAL_EPSILON)//The object is in the queue
		{

			obj->SetLastQueue(0.0f);
			obj->QueueTime(time);
			obj->SetStatus(TravelStatus_QUEUEING);
			route->Queue(obj->GetQueueLength(), 0);

		}
	}
}

bool RoutingManager::updateObject(GameTravelObject* obj, unsigned int time, unsigned int &scriptPlace)
{

	bool killNode = false;

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
		unsigned int localId = nodeObject->GetLocalId(goalObject->GetId());
		Route* rte = nodeObject->GetRoute(localId);
		float qDist = rte->GetQuelength();

		float offset = offsetDist + obj->GetQueueLength() / 2;

		if (obj->GetStatus() != TravelStatus_TRAVELNEAR)
		{

			if (obj->GetStatus() != TravelStatus_WAITING &&
				obj->GetStatus() != TravelStatus_QUEUEING)
			{

				handleQueing(obj, localId, rte, distance, time);

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

	return killNode;

}

unsigned int* RoutingManager::Update(unsigned int time, unsigned int &scripts)
{

	unsigned int scriptPlace = 0;
	CelestialListNode<GameTravelObject*>* lastNode = nullptr;
	CelestialListNode<GameTravelObject*>* node = travelObjects->GetFirstNode();

	while (node != nullptr)
	{

		GameTravelObject* obj = node->GetNodeObject();
		bool killNode = updateObject(obj, time, scriptPlace);
		

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

			if (obj->GetLowerNode() == 0)//First node on a route
			{

				obj->SetLowerNode(preExist->GetId() + 1);

			}
			else if (obj->GetUpperNode() == 0)//Second node on a route
			{

				obj->SetUpperNode(preExist->GetId() + 1);

				RouteNodeObject* lower = routeNodes->GetValue(obj->GetLowerNode() - 1);
				Vector3 dir = lower->GetPosition() - preExist->GetPosition();
				float dist = sqrt(VectorDot(dir));

				Route* route = new Route(dist, dir / dist);
				roads->Add(route);
				preExist->AddRoute(route);
				lower->AddRoute(route);

			}
			else if (preExist->GetId() + 1 != obj->GetLowerNode() &&
				preExist->GetId() + 1 != obj->GetUpperNode())
			{

				RouteNodeObject* lower = routeNodes->GetValue(obj->GetLowerNode() - 1);
				RouteNodeObject* upper = routeNodes->GetValue(obj->GetUpperNode() - 1);

				handleSplit(preExist, lower, upper);

				obj->SetMiddleNode(preExist->GetId() + 1);


			}
		}

		return preExist->GetId()+1;

	}

	return 0;

}

void RoutingManager::handleSplit(RouteNodeObject* intersect, RouteNodeObject* lower, RouteNodeObject* upper)
{

	prio_queue objectList[objNode_NA];
	unsigned int localUpper = lower->GetLocalId(upper->GetId());
	Route::Direction upDownDir;
	Route* upDown = lower->GetRoute(localUpper, upDownDir);
	RouteNodeObject* goal = upDown->CanTravel(upDownDir) ? upper : lower;
	RouteNodeObject* from = lower == goal ? upper : lower;

	lower->RemoveRoute(upper->GetId());
	upper->RemoveRoute(lower->GetId());

	Vector3 lowerIntDir = intersect->GetPosition() - lower->GetPosition();
	float lID = sqrt(VectorDot(lowerIntDir));
	Vector3 IntersectUpDir = upper->GetPosition() - intersect->GetPosition();
	float iUD = sqrt(VectorDot(IntersectUpDir));

	Route* lowerInt = new Route(lID, lowerIntDir / lID);
	Route* IntUpper = new Route(iUD, IntersectUpDir / iUD);

	roads->Add(lowerInt);
	roads->Add(IntUpper);

	lower->AddRoute(lowerInt);
	upper->AddRoute(IntUpper);

	intersect->AddRoute(lowerInt);
	intersect->AddRoute(IntUpper);

	CelestialListNode<GameTravelObject*>* node = travelObjects->GetFirstNode();

	while (node != nullptr)
	{
		
		objNode inter = objNode_NA;
		GameTravelObject* obj = node->GetNodeObject();
		float dist = VectorDot(obj->GetPosition() - (goal->GetPosition() + Vector3(0.0f, obj->GetScale().y / 2, 0.0f)));

		float objUpDist = VectorDot(obj->GetPosition() - upper->GetPosition());
		float objLowDist = VectorDot(obj->GetPosition() - lower->GetPosition());

		if (obj->GetNode() == from->GetId() &&
			obj->GetGoalNode() == goal->GetId())//Object is going on the path
		{

			inter = shouldIntersect(intersect, lower, upper, obj);

		}

		if (inter != objNode_NA)
		{

			prio_queue::nodeVal value;
			value.traveledDist = 0;
			value.targetDist = dist;
			value.index = obj->GetId();

			objectList[inter].push(value);

		}

		node = node->GetNext();

	}

	unsigned int interGoal = intersect->GetLocalId(goal->GetId());
	Route::Direction iGDir;
	Route* iGR = intersect->GetRoute(interGoal, iGDir);
	float iGDist = iGR->GetDistance();

	while (!objectList[objNode_BELOW].empty())
	{

		prio_queue::nodeVal val = objectList[objNode_BELOW].top();
		objectList[objNode_BELOW].pop();
		GameTravelObject* obj = (GameTravelObject*)gameObjects->GetValue(val.index);
		iGR->Travel(obj->GetId(), iGDir);
		obj->SetNode(intersect->GetId());

		if (obj->GetStatus() == TravelStatus_QUEUEING ||
			obj->GetStatus() == TravelStatus_WAITING)
		{

			iGR->Queue(obj->GetQueueLength(), 0);

		}
	}

	unsigned int fromInterLocal = from->GetLocalId(intersect->GetId());
	Route::Direction fIDir;
	Route* fIR = from->GetRoute(fromInterLocal, fIDir);

	while (!objectList[objNode_ON].empty())
	{

		prio_queue::nodeVal val = objectList[objNode_ON].top();
		objectList[objNode_ON].pop();
		GameTravelObject* obj = (GameTravelObject*)gameObjects->GetValue(val.index);

		float distSqrt = sqrt(val.targetDist);
		distSqrt += obj->GetQueueLength() / 2;

		if (distSqrt > iGDist + offsetDist)//The object pokes out from the node
		{

			float overDist = distSqrt - (iGDist + offsetDist);

			fIR->Queue(overDist, 0);
			fIR->Travel(obj->GetId(), fIDir);

			handlePrimedObject(obj);//Renavigate object
			obj->SetStatus(TravelStatus_WAITING);

		}
		else
		{

			intersect->SetObjId(obj->GetId());
			obj->SetNode(intersect->GetId());
			iGR->Travel(obj->GetId(), iGDir);

			if (obj->GetStatus() == TravelStatus_QUEUEING ||
				obj->GetStatus() == TravelStatus_WAITING)
			{

				iGR->Queue(obj->GetQueueLength(), 0);

			}
		}
	}

	while (!objectList[objNode_ABOVE].empty())
	{

		prio_queue::nodeVal val = objectList[objNode_ABOVE].top();
		objectList[objNode_ABOVE].pop();
		GameTravelObject* obj = (GameTravelObject*)gameObjects->GetValue(val.index);

		fIR->Travel(obj->GetId(), fIDir);

		handlePrimedObject(obj);//Renavigate all objects
		float dist = VectorDot(obj->GetPosition() - (intersect->GetPosition() + Vector3(0.0f, obj->GetScale().y / 2, 0.0f)));
		handleQueing(obj, fromInterLocal, fIR, sqrt(dist), 1);

	}
}

RoutingManager::objNode RoutingManager::shouldIntersect(RouteNodeObject* intersect, RouteNodeObject* from, RouteNodeObject* to, GameTravelObject* obj)
{

	unsigned int interToLocal = intersect->GetLocalId(to->GetId());
	Route* rte = intersect->GetRoute(interToLocal);

	float toDist = rte->GetDistance();

	float goalDistSqr = pow(toDist + offsetDist + obj->GetQueueLength() / 2, 2);
	float goalDistSqr2 = pow(toDist - (offsetDist + obj->GetQueueLength() / 2), 2);

	float objDist = VectorDot(obj->GetPosition() - (to->GetPosition() + Vector3(0.0f, obj->GetScale().y / 2, 0.0f)));

	if (objDist > goalDistSqr)//Object is beyond the intersection
	{

		return objNode_ABOVE;

	}
	else if (objDist < goalDistSqr2)//Object is closer
	{

		return objNode_BELOW;

	}
	else
	{ 
	
		return objNode_ON;

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

	for (unsigned int i = 0; i < start->GetMaxRoutes(); i++)
	{

		Route::Direction dir;
		Route* rte = start->GetRoute(i, dir);

		if (rte != nullptr )
		{

			RouteNodeObject* subObject = routeNodes->GetValue(rte->GetNode(dir) -1 );

			bool isCandidate = rte->CanTravel(dir) &&
			subObject->GetId() != at->GetId();

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

		for (unsigned int i = 0; i < nodeObj->GetMaxRoutes() && !reachedEnd; i++)
		{

			float dist;
			Route::Direction dir;
			Route* rte = nodeObj->GetRoute(i, dir);

			if (rte != nullptr)
			{

				RouteNodeObject* neigh = routeNodes->GetValue(rte->GetNode(dir) - 1);
				float tDist = sqrt(VectorDot(end->GetPosition() - neigh->GetPosition()));
				dist = tDist;
				float trDist = currentNode.traveledDist + rte->GetDistance();
				float heuristic = tDist + trDist;
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
			unsigned int localId = node->GetLocalId(last->GetId());
			Route* rte = node->GetRoute(localId);

			totalDist += rte->GetDistance();
			retVal++;

		}

	}

	pathFindVal++;

	if (pathFindVal == 0)
	{

		resetNodes();
		pathFindVal = 1;

	}
	
	return retVal;

}

void RoutingManager::resetNodes()
{

	for (unsigned int i = 0; i < routeNodes->GetHighest(); i++)
	{

		RouteNodeObject* node = routeNodes->GetValue(i);

		if (node != nullptr)
		{

			node->SetClosedset(0);
			node->SetOpenset(0);

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
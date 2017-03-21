#include "stdafx.h"
#include "TravelManager.h"

using namespace Entities;
using namespace CrossHandlers;
using namespace Resources;
using namespace CelestialMath;

TravelManager::TravelManager(unsigned int maxClock)
{

	maxSOuts = 128;
	scriptOuts = new unsigned int[maxSOuts];

	this->maxClock = maxClock;
	modClock = 0;
	lastClock = 0;

	travelObjects = new CelestialList<GameTravelObject*>();
	offsetDist = 0.25f;

	pathFinder = nullptr;

}

void TravelManager::init(
	CelestialSlicedList<BaseObject*>* gameObjects,
	CelestialSlicedList<RouteNodeObject*>* routeNodes,
	CelestialSlicedList<Route*>* roads,
	CelestialSlicedList<NodeGroup*>* nodeGroups) 
{

	pathFinder = new PathFinder(routeNodes);
	RoutingBase::init(gameObjects, travelObjects, routeNodes, roads, nodeGroups);

}

float TravelManager::getOffsetDist() const
{

	return offsetDist;

}

void TravelManager::clearObjects()
{

	travelObjects->KillList();

}

void TravelManager::Travel(GameTravelObject* object, unsigned int goal, unsigned int time)
{

	time += modClock;

	object->SetFinalGoalNode(goal);
	object->SetStatus(TravelStatus_PRIMED);

	travelObjects->PushElement(object);
	object->Time(time);

}

void TravelManager::Spawn(GameTravelObject* object, unsigned int cell)
{

	object->SetNode(cell);
	object->SetPosition(routeNodes->GetValue(cell)->GetPosition() + Vector3(0.0f, object->GetScale().y / 2, 0.0f));
	RouteNodeObject* node = routeNodes->GetValue(cell);
	node->SetObjId(object->GetId());
	object->UpdateMatrix();
	object->SetStatus(TravelStatus_READY);

}

void TravelManager::UpdateObject(GameTravelObject* obj, unsigned int time)
{

	RouteNodeObject* node = routeNodes->GetValue(obj->GetNode());

	if (obj->GetStatus() == TravelStatus_READY)//The object is standing on a node
	{

		node->SetObjId(obj->GetId());

	}
	else
	{

		RouteNodeObject* goal = routeNodes->GetValue(obj->GetGoalNode());
		float dist = getObjDistSqr(obj, node);

		if (obj->GetStatus() == TravelStatus_TRAVELNEAR ||
			dist < offsetDist*offsetDist)
		{

			node->SetObjId(obj->GetId());

		}

		Route::Direction dir;
		unsigned int localId = node->GetLocalId(goal->GetId());
		Route* road = node->GetRoute(localId, dir);

		road->Travel(obj->GetId(), dist, dir);
		obj->Time(time);
		travelObjects->PushElement(obj);

	}
}

unsigned int TravelManager::addToOutScripts(unsigned int script, unsigned int place)
{

	if (place >= maxSOuts)
	{

		maxSOuts += 128;
		unsigned int* newScripts = new unsigned int[maxSOuts];
		memcpy(newScripts, scriptOuts, maxSOuts - 128 * sizeof(unsigned int));

	}

	bool dbl = false;

	for (unsigned int i = 0; i < place && !dbl; i++)
	{

		dbl = scriptOuts[i] == script;
		int dbg = 0;

	}

	scriptOuts[place] = script;
	return place + 1;

}

void TravelManager::handleNearNode(GameTravelObject* obj, RouteNodeObject* currentNode, RouteNodeObject* goalNode, RouteNodeObject* nextGoal, unsigned int time)
{

	bool isAtGoal = obj->GetFinalGoalNode() == obj->GetGoalNode();//The next node is the final goal
	bool travelOn = false;

	if (goalNode->GetObjId() == 0)//The node is open
	{

		if (goalNode->GetRoutes() > 2 &&
			obj->GetStatus() == TravelStatus_WAITING)//Object is at Intersection and waiting for go-ahead
		{

			if (isAtGoal)//The intersection is the final goal
			{

				travelOn = true;

			}
			else //The final goal lies beyond
			{

				bool groupLocked = false;

				if (nextGoal->GetGroup() > 0)
				{

					NodeGroup* group = nodeGroups->GetValue(nextGoal->GetGroup() - 1);
					groupLocked = group->GetLocked() > 0 && obj->GetId() != group->GetLocked();

				}

				Route::Direction dir;
				unsigned int routeId = goalNode->GetLocalId(nextGoal->GetId());
				Route* route = goalNode->GetRoute(routeId, dir);

				if (!groupLocked && route->CanTravel(dir))//The route can be traveled
				{

					travelOn = true;

				}
				else//The road is occupied
				{

					unsigned int amounts = pathFinder->PathFindOpenRoad(currentNode,
						goalNode,
						routeNodes->GetValue(obj->GetFinalGoalNode()));

					if (amounts > 0)//There is a way around
					{

						obj->SetGoalNode(pathFinder->GetPath());
						travelOn = true;
						bool reCalc = false;
						nextGoal = routeNodes->GetValue(obj->PeekNextGoal(reCalc));

					}
				}
			}
		}
		else if (goalNode->GetRoutes() <= 2)//Object has arrived near a roadnode
		{

			travelOn = true;

		}
	}


	if (travelOn)
	{

		if (!isAtGoal && nextGoal->GetGroup() > 0)
		{

			NodeGroup* group = nodeGroups->GetValue(nextGoal->GetGroup() - 1);
			group->Lock(obj->GetId());

		}

		if (!isAtGoal)
		{

			Route::Direction dir;
			unsigned int routeId = goalNode->GetLocalId(nextGoal->GetId());
			Route* route = goalNode->GetRoute(routeId, dir);
			route->Travel(obj->GetId(), dir);

		}

		obj->SetStatus(TravelStatus_TRAVELNEAR);
		goalNode->SetObjId(obj->GetId());
		unsigned int localId = currentNode->GetLocalId(goalNode->GetId());
		Route* route = currentNode->GetRoute(localId);
		route->Queue(-(obj->GetQueueLength()), time);

	}
	else if (obj->GetStatus() != TravelStatus_WAITING)
	{

		obj->SetStatus(TravelStatus_WAITING);

	}
}

float TravelManager::travelObject(GameTravelObject* obj, RouteNodeObject* currentNode, RouteNodeObject* goalNode, Vector3 dir, float distance, float timeDiff)
{

	bool slowDown = obj->GetStatus() == TravelStatus_TRAVELING &&
		goalNode->GetRoutes() > 2;

	float offset = offsetDist + obj->GetQueueLength() / 2;
	distance -= !slowDown ? 0 : offset;
	float fact = distance;
	float spd = obj->GetSpeed() * timeDiff;

	//Object has a long way to go
	if (distance >= spd)
	{

		fact = spd;

	}

	obj->SetPosition(obj->GetPosition() + (dir * fact));
	obj->UpdateMatrix();
	return fact;

}

float TravelManager::getObjDistSqr(GameTravelObject* obj, RouteNodeObject* node)
{

	Vector3 objEndPoint = obj->GetPosition() - (obj->GetDirection()*(obj->GetQueueLength() / 2));
	Vector3 traveled = node->GetPosition() + Vector3(0.0f, obj->GetScale().y / 2, 0.0f) - objEndPoint;
	return VectorDot(traveled);

}

void TravelManager::checkObjPastNode(Resources::GameTravelObject* obj, RouteNodeObject* currentNode)
{

	float traveledDist = getObjDistSqr(obj, currentNode);

	if (traveledDist > offsetDist * offsetDist &&
		currentNode->GetObjId() == obj->GetId())//The object has traveled beyond the objects threshold
	{

		currentNode->SetObjId(0);

	}
}

void TravelManager::handleTravel(GameTravelObject* obj, RouteNodeObject* currentNode, RouteNodeObject* goalNode, Vector3 dir, float distance, float timeDiff, unsigned int time)
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

			lastQ -= travelObject(obj, currentNode, goalNode, dir, lastQ, timeDiff);
			obj->SetLastQueue(lastQ);

		}
	}
	else
	{

		travelObject(obj, currentNode, goalNode, dir, distance, timeDiff);

	}

	checkObjPastNode(obj, currentNode);

}

bool TravelManager::handleNodeArrival(GameTravelObject* obj, RouteNodeObject* currentNode, RouteNodeObject* goalNode, RouteNodeObject* nextGoal)
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

			unsigned int amounts = pathFinder->PathFind(
				goalNode,
				goalNode,
				routeNodes->GetValue(obj->GetFinalGoalNode()));

			if (amounts > 0)
			{

				obj->SetGoalNode(pathFinder->GetPath());


			}

			nextGoal = routeNodes->GetValue(obj->GetGoalNode());

		}

		obj->Point(nextGoal->GetPosition() + Vector3(0.0f, obj->GetScale().y / 2, 0.0f));
		unsigned int localId = goalNode->GetLocalId(nextGoal->GetId());
		Route::Direction dir;
		Route* rte = goalNode->GetRoute(localId, dir);
		rte->Travel(obj->GetId(), dir);

		if (obj->GetStatus() != TravelStatus_QUEUEING)
		{

			obj->SetStatus(TravelStatus_TRAVELING);

		}
	}

	if (goalNode->GetGroup() != currentNode->GetGroup() && currentNode->GetGroup() > 0)
	{

		//Unlock the group of the old node
		NodeGroup* nodeGroup = nodeGroups->GetValue(currentNode->GetGroup() - 1);

		if (nodeGroup->GetLocked() == obj->GetId())
		{

			nodeGroup->Lock(0);

		}
	}

	return killNode;

}

void TravelManager::handlePrimedObject(GameTravelObject* obj)
{

	RouteNodeObject* node = routeNodes->GetValue(obj->GetNode());
	RouteNodeObject* end = routeNodes->GetValue(obj->GetFinalGoalNode());
	unsigned int paths = pathFinder->PathFindOpenRoad(node, node, end);

	if (paths > 0)
	{

		obj->SetGoalNode(pathFinder->GetPath());
		obj->SetStatus(TravelStatus_TRAVELING);

		RouteNodeObject* nextStop = routeNodes->GetValue(obj->GetGoalNode());
		unsigned int local = node->GetLocalId(nextStop->GetId());
		Route::Direction dir;
		Route* rte = node->GetRoute(local, dir);
		rte->Travel(obj->GetId(), dir);

		obj->Point(nextStop->GetPosition() + Vector3(0.0f, obj->GetScale().y / 2, 0.0f));

	}
}

void TravelManager::handleQueing(GameTravelObject* obj, unsigned int localId, Route* route, float distance, unsigned int time)
{

	float qDist = route->GetQuelength() + offsetDist;

	if (obj->GetStatus() != TravelStatus_QUEUEING)//The object isn't in the queue
	{

		qDist += obj->GetQueueLength() / 2;

		if (distance - qDist <= CELESTIAL_EPSILON)//The object is in the queue
		{

			obj->SetLastQueue(0.0f);
			obj->QueueTime(time);
			obj->SetStatus(TravelStatus_QUEUEING);
			route->Queue(obj->GetQueueLength(), time);

		}
	}
}

bool TravelManager::updateObject(GameTravelObject* obj, unsigned int time, unsigned int &scriptPlace, float timeDiff)
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

		bool reCalc = false;
		unsigned int nextGoalId = obj->PeekNextGoal(reCalc);

		if (reCalc)
		{

			unsigned int amounts = pathFinder->PathFind(nodeObject,
				goalObject,
				routeNodes->GetValue(obj->GetFinalGoalNode()));

			if (amounts > 0)
			{

				obj->SetGoalNode(pathFinder->GetPath());

			}

			nextGoalId = obj->PeekNextGoal(reCalc);

		}

		RouteNodeObject* nextGoal = routeNodes->GetValue(nextGoalId);

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

				handleNearNode(obj, nodeObject, goalObject, nextGoal, time);

			}
		}

		if (obj->GetStatus() != TravelStatus_WAITING)
		{

			if (distance > CELESTIAL_EPSILON)//The object has not arrived 
			{

				handleTravel(obj, nodeObject, goalObject, dir, distance, timeDiff, time);

			}
			else//The object has arrived at a node
			{

				unsigned int script = obj->GetTravelNodeScript();

				if (script > 0)
				{

					scriptPlace = addToOutScripts(obj->GetId(), scriptPlace);

				}

				killNode = handleNodeArrival(obj, nodeObject, goalObject, nextGoal);

			}
		}
	}

	return killNode;

}

unsigned int* TravelManager::Update(unsigned int time, unsigned int &scripts, float timeDiff)
{

	if (lastClock > time)
	{

		modClock += maxClock;

	}

	time += modClock;

	scripts = 0;
	CelestialListNode<GameTravelObject*>* lastNode = nullptr;
	CelestialListNode<GameTravelObject*>* node = travelObjects->GetFirstNode();

	while (node != nullptr)
	{

		GameTravelObject* obj = node->GetNodeObject();
		bool killNode = updateObject(obj, time, scripts, timeDiff);


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

	return scriptOuts;

}

TravelManager::~TravelManager()
{

	delete[] scriptOuts;

	if (this->pathFinder != nullptr)
	{

		delete pathFinder;

	}
}
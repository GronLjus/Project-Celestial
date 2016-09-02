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

	paths = 128;
	path = new unsigned[paths];

	maxSOuts = 128;
	scriptOuts = new unsigned int[maxSOuts];

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

unsigned int* RoutingManager::Update(unsigned int time, unsigned int &scripts)
{

	unsigned int scriptPlace = 0;
	CelestialListNode<GameTravelObject*>* lastNode = nullptr;
	CelestialListNode<GameTravelObject*>* node = travelObjects->GetFirstNode();

	while (node != nullptr)
	{

		GameTravelObject* obj = node->GetNodeObject();
		RouteNodeObject* rNode = routeNodes->GetValue(obj->GetGoalNode());
		Vector3 dir = rNode->GetPosition() + Vector3(0.0f, obj->GetScale().y / 2, 0.0f) - obj->GetPosition();
		float distSqr = VectorDot(dir);
		bool killNode = false;

		//Object has arrived at its goal
		if (distSqr <= CELESTIAL_EPSILON * CELESTIAL_EPSILON)
		{

			obj->SetNode(rNode->GetId());
			unsigned int script = obj->GetTravelNodeScript();

			if (obj->GetTravelNodeScript() > 0)
			{

				scriptPlace = addToOutScripts(obj->GetId(), scriptPlace);

			}

			//Object has arrived at its final destination
			if (obj->GetFinalGoalNode() == rNode->GetId())
			{

				killNode = true;

			}
			else //Object has arrived at a stop
			{

				bool recalculate = obj->StepGoal();

				if (recalculate)
				{

					unsigned int amounts = pathFind(routeNodes->GetValue(obj->GetGoalNode()),
						routeNodes->GetValue(obj->GetFinalGoalNode()));

					if (amounts > 0)
					{

						obj->SetGoalNode(path);

					}
				}

				RouteNodeObject* nextStop = routeNodes->GetValue(obj->GetGoalNode());
				obj->Point(nextStop->GetPosition() + Vector3(0.0f, obj->GetScale().y / 2, 0.0f));

			}
		}
		else //Object has not arrived
		{

			distSqr = sqrt(distSqr);
			dir /= distSqr;
			float fact = distSqr;
			float timeFact = (float)(time - obj->GetLastTime()) / 1000.0f;
			float spd = obj->GetSpeed() * timeFact;

			//Object has a long way to go
			if (distSqr >= spd)
			{

				fact = spd;

			}

			obj->SetPosition(obj->GetPosition() + (dir*fact));
			obj->UpdateMatrix();
			obj->Time(time);

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
	}

	scripts = scriptPlace;
	return scriptOuts;

}

unsigned int RoutingManager::AddNode(Vector3 position, unsigned int* objects, unsigned int amounts)
{

	if (amounts != 0)
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

				preExist->AddRoute(lower);
				lower->AddRoute(preExist);

			}
			else if (preExist->GetId() + 1 != obj->GetLowerNode() &&
				preExist->GetId() + 1 != obj->GetUpperNode())
			{


				RouteNodeObject* lower = routeNodes->GetValue(obj->GetLowerNode() - 1);
				RouteNodeObject* upper = routeNodes->GetValue(obj->GetUpperNode() - 1);
				lower->RemoveRoute(upper->GetId());
				upper->RemoveRoute(lower->GetId());
				
				lower->AddRoute(preExist);
				upper->AddRoute(preExist);

				preExist->AddRoute(lower);
				preExist->AddRoute(upper);

				obj->SetMiddleNode(preExist->GetId() + 1);

			}
		}

		handleRoad(nullptr, preExist);
		return preExist->GetId()+1;

	}

	return 0;

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

void RoutingManager::handleRoad(RouteNodeObject* source, RouteNodeObject* target)
{

	unsigned int oldId = 0;

	if (target->GetRoutes() > 2)
	{

		if (target->GetRoad() != 0)
		{

			oldId = target->GetRoad();
			handleOldRoad(target);

		}

		source = target;
		target->SetRoad(0);

	}
	else if (source == nullptr || source->GetRoad() == 0)
	{

		if (target->GetRoutes() == 0 || 
			(source != nullptr && source->GetRoad() == 0))
		{

			handleOldRoad(target);
			Road* road = new Road();
			road->IncreaseNodes(1);
			unsigned int rd = roads->Add(road) + 1;
			target->SetRoad(rd);

		}
		else
		{

			float dist; 

			for (unsigned int i = 0; i < target->GetMaxRoutes() && source == nullptr; i++)
			{

				source = target->GetRoute(i, dist);

			}

			handleRoad(source, target);

		}
	}
	else if(source->GetRoad() != target->GetRoad())
	{

		handleOldRoad(target);
		unsigned int newRoad = 0;
		target->SetRoad(source->GetRoad());

		Road* rd = roads->GetValue(target->GetRoad() - 1);
		rd->IncreaseNodes(1);

	}
	else
	{

		return;

	}

	if (source != nullptr && target->GetRoutes() > 0)
	{

		for (unsigned int i = 0; i < target->GetMaxRoutes(); i++)
		{

			float dist;
			RouteNodeObject* node = target->GetRoute(i, dist);

			if (node != nullptr &&
				node->GetRoad() != 0 &&
				(node->GetId() != source->GetId() || (node->GetRoutes() > 2 && node->GetRoad() != 0)) &&
				node->GetRoad() != oldId
				)
			{

				handleRoad(target, node);

			}

			if (node != nullptr && node->GetRoad() == oldId)
			{

				oldId = 0;

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

RouteNodeObject* RoutingManager::GetNode(unsigned int id) const
{

	return routeNodes->GetValue(id);

}

unsigned int RoutingManager::pathFind(RouteNodeObject* start, RouteNodeObject* end)
{

	if (end == nullptr)
	{

		return 0;

	}

	prio_queue openSet;
	prio_queue::nodeVal startNode;
	startNode.index = start->GetId();
	startNode.parentIndex = startNode.index;
	startNode.targetDist = VectorDot(end->GetPosition() - start->GetPosition());
	startNode.traveledDist = 0.0f;
	openSet.push(startNode);
	start->SetParent(start->GetId());
	start->SetOpenset(pathFindVal);
	start->SetStep(0);
	std::vector<unsigned int> closedSet;
	bool reachedEnd = false;
	float dist = -1.0f;

	while (!openSet.empty() && !reachedEnd)
	{

		prio_queue::nodeVal currentNode = openSet.top();
		openSet.pop();
		closedSet.push_back(currentNode.index);
		RouteNodeObject* nodeObj = routeNodes->GetValue(currentNode.index);
		nodeObj->SetClosedset(pathFindVal);

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

	if (reachedEnd)
	{

		RouteNodeObject* node = end;

		while (node->GetId() != start->GetId())
		{

			if (node->GetStep() - 1 < paths)
			{

				path[node->GetStep() - 1] = node->GetId();

			}

			node = routeNodes->GetValue(node->GetParent());
			retVal++;

		}

	}

	pathFindVal++;
	pathFindVal = pathFindVal == 0 ? 1 : pathFindVal;
	return retVal;

}

void RoutingManager::Travel(GameTravelObject* object, unsigned int goal, unsigned int time)
{

	unsigned int paths = pathFind(routeNodes->GetValue(object->GetNode()), routeNodes->GetValue(goal));
	object->SetFinalGoalNode(goal);
	unsigned int objId = object->GetId();
	unsigned int startNode = object->GetNode();

	if (paths > 0)
	{

		object->Time(time);
		object->SetGoalNode(path);
		RouteNodeObject* nextStop = routeNodes->GetValue(object->GetGoalNode());
		object->Point(nextStop->GetPosition() + Vector3(0.0f, object->GetScale().y / 2, 0.0f));
		travelObjects->PushElement(object);

	}
}

void RoutingManager::Spawn(GameTravelObject* object, unsigned int cell)
{

	object->SetNode(cell);
	object->SetPosition(routeNodes->GetValue(cell)->GetPosition() + Vector3(0.0f, object->GetScale().y/2,0.0f));
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
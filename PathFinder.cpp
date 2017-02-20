#include "stdafx.h"
#include "PathFinder.h"

using namespace Resources;
using namespace Entities;
using namespace CrossHandlers;

PathFinder::PathFinder(CelestialSlicedList<RouteNodeObject*>* routeNodes)
{

	this->routeNodes = routeNodes;

	pathFindVal = 1;

	totalPaths = 128;
	path = new unsigned[totalPaths];

}

unsigned int PathFinder::PathFindOpenRoad(RouteNodeObject* at, RouteNodeObject* start, RouteNodeObject* end)
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

		if (rte != nullptr)
		{

			RouteNodeObject* subObject = routeNodes->GetValue(rte->GetNode(dir) - 1);

			bool isCandidate = rte->CanTravel(dir) &&
				subObject->GetId() != at->GetId();

			if (isCandidate)
			{

				float dist;
				unsigned int paths = PathFind(start, subObject, end, dist);

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

unsigned int PathFinder::PathFind(RouteNodeObject* start, RouteNodeObject* through, RouteNodeObject* end)
{

	float dist;
	return PathFind(start, through, end, dist);

}

unsigned int PathFinder::PathFind(RouteNodeObject* start, RouteNodeObject* through, RouteNodeObject* end, float &totalDist)
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

				if (!closed)
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

unsigned int* PathFinder::GetPath() const
{

	return path;

}

void PathFinder::resetNodes()
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

PathFinder::~PathFinder()
{

	delete[] path;

}
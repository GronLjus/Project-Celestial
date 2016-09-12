#pragma once
#include "CelestialList.h"
#include "GameTravelObject.h"
#include "RouteNodeObject.h"
#include "GameRouteObject.h"
#include "SerializableObject.h"
#include "Road.h"
#include <vector>

namespace Entities
{

	class RoutingManager : public Resources::SerializableObject
	{

		public:
			RoutingManager();
			void Init(CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* gameObjects);
			unsigned int AddNode(CelestialMath::Vector3 position, Resources::GameRouteObject* obj);
			unsigned int AddNode(CelestialMath::Vector3 position, unsigned int* objects, unsigned int amounts);
			unsigned int* Update(unsigned int time, unsigned int &scripts);
			RouteNodeObject* GetNode(unsigned int id) const;
			void Travel(Resources::GameTravelObject* object, unsigned int goal, unsigned int time);
			void Spawn(Resources::GameTravelObject* object, unsigned int cell);
			void ClearNodes();

			//Serializable Interface
			virtual char* Serialize(unsigned int &size);
			virtual char* Unserialize(char* data);

			virtual ~RoutingManager();

		private:
			CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* gameObjects;
			CrossHandlers::CelestialList<Resources::GameTravelObject*>* travelObjects;
			CrossHandlers::CelestialSlicedList<RouteNodeObject*>* routeNodes;
			CrossHandlers::CelestialSlicedList<Road*>* roads;

			RouteNodeObject* getPreExistantNode(CelestialMath::Vector3 position, unsigned int* objects, unsigned int amounts) const;

			void addToRoad(RouteNodeObject* object, unsigned int road);
			void handleOldRoad(RouteNodeObject* target);
			void handleRoad(RouteNodeObject* object);
			void mergeRoads(RouteNodeObject* source, unsigned int road);
			void addToNewRoad(RouteNodeObject* source);

			void reverseNode(RouteNodeObject* object, Road::Direction dir);

			float travelObject(Resources::GameTravelObject* obj, RouteNodeObject* currentNode, RouteNodeObject* goalNode, CelestialMath::Vector3 dir, float distSqr, unsigned int time);
			
			void handleNearNode(Resources::GameTravelObject* obj, RouteNodeObject* currentNode, RouteNodeObject* goalNode, unsigned int time);
			bool handleNodeArrival(Resources::GameTravelObject* obj, RouteNodeObject* currentNode, RouteNodeObject* goalNode);
			void handlePrimedObject(Resources::GameTravelObject* obj);
			void handleQueing(Resources::GameTravelObject* obj, unsigned int localId, RouteNodeObject* goalNode, float distSqr, unsigned int time);
			void handleTravel(Resources::GameTravelObject* obj, RouteNodeObject* currentNode, RouteNodeObject* goalNode, CelestialMath::Vector3 dir, float distSqr, unsigned int time);

			unsigned int pathFindOpenRoad(RouteNodeObject* at, RouteNodeObject* start, RouteNodeObject* end);
			unsigned int pathFind(RouteNodeObject* start, RouteNodeObject* through, RouteNodeObject* end);
			unsigned int pathFind(RouteNodeObject* start, RouteNodeObject* through, RouteNodeObject* end, float &dist);
			unsigned char pathFindVal;
			unsigned int* path;
			unsigned char totalPaths;

			unsigned int addToOutScripts(unsigned int script, unsigned int place);
			unsigned int* scriptOuts;
			unsigned int maxSOuts;

			float offsetDist;

			//Road::Direction getDirection(RouteNodeObject* object) const;
			Road::Direction getDirection(RouteNodeObject* start, RouteNodeObject* next) const;

			RouteNodeObject* getNeighbour(RouteNodeObject* object, Road::Direction dir) const;

			class prio_queue
			{

			public:
				struct nodeVal
				{

					float targetDist;
					float traveledDist;
					unsigned int index;
					unsigned int parentIndex;

					friend bool operator<=(const nodeVal& oCell1, const nodeVal& oCell2)
					{
						return oCell1.targetDist + oCell1.traveledDist
							<= oCell2.targetDist + oCell2.traveledDist;

					}

					friend bool operator<(const nodeVal& oCell1, const nodeVal& oCell2)
					{
						return oCell1.targetDist + oCell1.traveledDist
							< oCell2.targetDist + oCell2.traveledDist;

					}

					friend bool operator==(const nodeVal& oCell1, const nodeVal& oCell2)
					{
						return oCell1.index == oCell2.index;

					}
				};

				prio_queue() :nSize(0)
				{

					oVect.resize(0);

				}

				void push(nodeVal value)
				{

					//Push the cell into the back of the heap
					oVect.push_back(value);
					unsigned int pivot = nSize++;

					//Bubble the last value up
					while (pivot > 0)
					{

						unsigned int leftRight = pivot % 2 == 0 ? 2 : 1;
						unsigned int newPivot = (pivot - leftRight) / 2;

						if (oVect[pivot] <= oVect[newPivot])
						{

							nodeVal tempVal = oVect[pivot];
							oVect[pivot] = oVect[newPivot];
							oVect[newPivot] = tempVal;
							pivot = newPivot;

						}
						else
						{

							return;

						}
					}
				}

				const nodeVal top() const
				{

					return oVect[0];

				}

				void pop()
				{

					//Replace the root node with the last in the heap
					oVect[0] = oVect.back();
					oVect.pop_back();
					nSize--;

					unsigned int pivot = 0;

					//Bubble the root node down
					while (pivot < nSize)
					{

						unsigned int left = 2 * pivot + 1;
						unsigned int right = 2 * pivot + 2;

						if (left >= nSize)//At the bottom of the heap
						{

							return;

						}
						else
						{

							unsigned int smallest = left;

							if (right < nSize && oVect[right] < oVect[left])
							{

								smallest = right;

							}

							if (oVect[smallest] <= oVect[pivot])//The child is smaller, flip them around
							{

								nodeVal tempVal = oVect[pivot];
								oVect[pivot] = oVect[smallest];
								oVect[smallest] = tempVal;
								pivot = smallest;

							}
							else
							{

								return;

							}
						}
					}
				}

				bool empty()
				{

					return nSize == 0;

				}

				unsigned int nSize;
				std::vector<nodeVal> oVect;
			};

	};
}
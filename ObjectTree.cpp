#include "stdafx.h"
#include "ObjectTree.h"

using namespace CrossHandlers;
using namespace Resources;
using namespace CelestialMath;
using namespace Entities;

ObjectTree::ObjectTree(unsigned int cells, unsigned int minCells, Vector2 position)
{

	box = new BoundingBox(cells, 1.0f, cells, position.x, 0, position.y);
	subTrees = nullptr;
	objects = nullptr;

	if (cells > minCells)
	{

		subTrees = new ObjectTree*[4];
		Vector2 topLeft = position + Vector2(-(float)((cells) / 4), -(float)((cells) / 4));
		Vector2 topRight = position + Vector2((cells) / 4, -(float)((cells) / 4));
		Vector2 bottomLeft = position + Vector2(-(float)((cells) / 4), (cells) / 4);
		Vector2 bottomRight = position + Vector2((cells) / 4, (cells) / 4);

		subTrees[0] = new ObjectTree(cells / 2, minCells, topLeft);
		subTrees[1] = new ObjectTree(cells / 2, minCells, topRight);
		subTrees[2] = new ObjectTree(cells / 2, minCells, bottomLeft);
		subTrees[3] = new ObjectTree(cells / 2, minCells, bottomRight);

	}
	else
	{

		objects = new CelestialSlicedList<GameObject*>(minCells*minCells);

	}

	this->size = cells;
	objectAmountMax = 0;

}

unsigned int ObjectTree::GetClosestObject(Vector3 origin, Vector3 unitDirection, float& smallestDistance) const
{

	unsigned int closedObject = 0;

	if (objects != nullptr)
	{

		float smallDistance = smallestDistance;

		for (unsigned int i = 0; i < objectAmountMax; i++)
		{

			GameObject* obj = objects->GetValue(i);

			if (obj != nullptr && obj->GetSphere()->IntersectsLine(origin, unitDirection, smallDistance) != Intersection_BACK)
			{

				if (obj->GetBox()->IntersectsLine(origin, unitDirection, smallDistance) != Intersection_BACK)
				{

					smallestDistance = smallDistance;
					closedObject = obj->GetId();

				}
			}
		}
	}
	else
	{

		for (unsigned int i = 0; i < 4; i++)
		{

			float dist = 0;

			if (subTrees[i]->GetBox()->IntersectsLine(origin,unitDirection,dist) != Intersection_BACK)
			{

				if (smallestDistance == 0 || smallestDistance >= dist)
				{

					dist = 0;
					unsigned int closestObject = subTrees[i]->GetClosestObject(origin, unitDirection, dist);

					if (dist < smallestDistance || smallestDistance == 0)
					{

						closedObject = closestObject;
						smallestDistance = dist;

					}
				}
			}
		}
	}

	return closedObject;

}

void ObjectTree::AddObject(GameObject* object)
{

	if (objects != nullptr)
	{

		objects->Add(object);

	}
	else
	{

		for (unsigned int i = 0; i < 4; i++)
		{

			if (subTrees[i]->GetBox()->IntersectsBounding(object->GetSphere(), Shape_SPHERE) != Intersection_FRONT)
			{

				if (subTrees[i]->GetBox()->IntersectsBounding(object->GetBox(), Shape_BOX) != Intersection_FRONT)
				{

					subTrees[i]->AddObject(object);

				}
			}
		}
	}

	objectAmountMax++;

}

unsigned int ObjectTree::GetObjects() const
{

	return objectAmountMax;

}

unsigned int ObjectTree::AddInstance(ViewObject* view, DrawingBoard* board)
{

	unsigned int checkedBoxes = 0;

	if (objects != nullptr)
	{

		for (unsigned int i = 0; i < objectAmountMax; i++)
		{

			GameObject* obj = objects->GetValue(i);

			if (obj != nullptr)
			{

				board->AddInstance(obj);

			}
		}
	}
	else
	{

		for (unsigned int i = 0; i < 4; i++)
		{

			if (subTrees[i]->GetObjects() > 0)
			{

				checkedBoxes++;
				Intersection inter = view->GetFrustum()->Check(subTrees[i]->GetBox(), Dimension_N);

				if (inter != Intersection_BACK)
				{

					checkedBoxes += subTrees[i]->AddInstance(view, board);

				}
			}
		}
	}

	return checkedBoxes;

}

BoundingBox* ObjectTree::GetBox() const
{

	return box;

}

ObjectTree::~ObjectTree()
{

	delete box;

	if (objects != nullptr)
	{

		delete objects;

	}
	else
	{

		delete subTrees[0];
		delete subTrees[1];
		delete subTrees[2];
		delete subTrees[3];
		delete[] subTrees;

	}
}
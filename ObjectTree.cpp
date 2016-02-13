#include "stdafx.h"
#include "ObjectTree.h"

using namespace CrossHandlers;
using namespace Resources;
using namespace CelestialMath;
using namespace Entities;

ObjectTree::ObjectTree(unsigned int cells, unsigned int minCells, unsigned int cellSize, Vector2 position)
{

	box = new BoundingBox(cells*cellSize, cells*cellSize, 1.0f, position.x, position.y, 0);
	subTrees = nullptr;
	objects = nullptr;

	if (cells > minCells)
	{

		subTrees = new ObjectTree*[4];
		Vector2 topLeft = position + Vector2(-(float)((cells*cellSize) / 4), -(float)((cells*cellSize) / 4));
		Vector2 topRight = position + Vector2((cells*cellSize) / 4, -(float)((cells*cellSize) / 4));
		Vector2 bottomLeft = position + Vector2(-(float)((cells*cellSize) / 4), (cells*cellSize) / 4);
		Vector2 bottomRight = position + Vector2((cells*cellSize) / 4, (cells*cellSize) / 4);

		subTrees[0] = new ObjectTree(cells / 2, minCells, cellSize, topLeft);
		subTrees[1] = new ObjectTree(cells / 2, minCells, cellSize, topRight);
		subTrees[2] = new ObjectTree(cells / 2, minCells, cellSize, bottomLeft);
		subTrees[3] = new ObjectTree(cells / 2, minCells, cellSize, bottomRight);

	}
	else
	{

		objects = new CelestialSlicedList<GameObject*>(cells*cells);

	}

	this->size = cells*cellSize;
	objectAmountMax = 0;

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

void ObjectTree::AddInstance(ViewObject* view, DrawingBoard* board)
{

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

			if (view->GetFrustum()->Check(subTrees[i]->GetBox(),Dimension_N) != Intersection_BACK)
			{

				subTrees[i]->AddInstance(view, board);

			}
		}
	}
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
#include "stdafx.h"
#include "GameGridObject.h"

using namespace Resources;
using namespace CrossHandlers;
using namespace CelestialMath;

GameGridObject::GameGridObject(CrossHandlers::BoundingBox* baseBox, CrossHandlers::BoundingSphere* baseSphere,
	unsigned int meshId, std::string name) : GameObject(baseBox, baseSphere, meshId, name)
{

	gridHeight = 0;
	gridWidth = 0;

	nodes = nullptr;


}

GameGridObject::GameGridObject()
{

	gridHeight = 0;
	gridWidth = 0;

	nodes = nullptr;

}

GameObjectType GameGridObject::GetType() const
{

	return GameObjectType_GRIDROUTE;

}

void GameGridObject::sizeGrid()
{

	if (gridHeight > 0 && gridWidth > 0)
	{

		nodes = new unsigned int[gridWidth*gridHeight];

	}
	else
	{

		gridWidth = 0;
		gridHeight = 0;

	}
}

void GameGridObject::SizeGrid(float nodeWidth)
{

	gridWidth = GetScale().x / nodeWidth;
	gridHeight = GetScale().z / nodeWidth;

	sizeGrid();

}

void GameGridObject::PopulateGrid(unsigned int node, unsigned int x, unsigned int y)
{

	nodes[x + gridWidth * y] = node;

}

unsigned int GameGridObject::GetNodeWidth() const
{

	return gridWidth;

}
unsigned int GameGridObject::GetNodeHeigth() const
{

	return gridHeight;

}

unsigned int GameGridObject::GetNode(unsigned int x, unsigned int y)
{

	if (nodes != nullptr)
	{

		return nodes[x + gridWidth * y];

	}

	return 0;

}

Vector3 GameGridObject::GetOrthogonalNormalOfNode(unsigned int x, unsigned int y) const
{

	Vector3 dir;

	if (x > 0 && x < gridWidth - 1)//Node is on the upper or lower edge
	{

		Vector3 up = Vector3(0.0f, 1.0f, 0.0f);

		if (y == 0)//Node is on the upper edge in the direction of the object
		{

			dir = VectorCross(up, GetDirection());

		}
		else//Object is on the lower edge in the reverse direction of the object
		{

			dir = VectorCross(GetDirection(), up);

		}
	}
	else//Node is on the right or the left edge, or on the corners
	{

		if (x == 0)
		{

			dir = GetDirection();

		}
		else
		{

			dir = -GetDirection();

		}
	}

	return dir / sqrt(VectorDot(dir));

}

unsigned int* GameGridObject::GetNodes() const
{

	return nodes;

}

Intersection GameGridObject::ContainsPoint(Vector3 point)
{

	BoundingBox* box = GetBox();
	point.y = GetPosition().y;
	Intersection retVal = box->ContainsPoint(point);

	for (unsigned int i = 0; i < GetSubobjects() && retVal == Intersection_BACK; i++)
	{

		GameObject* sub = (GameObject*)GetSubobject(i);

		if (sub != nullptr)
		{

			box = sub->GetBox();
			retVal = box->ContainsPoint(point);

		}
	}

	return retVal;

}

//Serializable Interface
char* GameGridObject::Serialize(unsigned int &size)
{

	unsigned int subSize = 0;
	char* subSerial = GameObject::Serialize(subSize);
	size = subSize + 1 + sizeof(unsigned int) * (2 + (gridWidth*gridHeight));
	char* byteVal = new char[size];

	byteVal[0] = SerializableType_GAMEOBJECTROUTE;

	unsigned int offset = 1;
	memcpy(&byteVal[offset], &gridWidth, sizeof(unsigned int));
	offset += sizeof(unsigned int);
	memcpy(&byteVal[offset], &gridHeight, sizeof(unsigned int));
	offset += sizeof(unsigned int);

	for (unsigned int i = 0; i < gridWidth; i++)
	{

		for (unsigned int k = 0; k < gridHeight; k++)
		{

			unsigned int node = GetNode(i, k);
			memcpy(&byteVal[offset], &node, sizeof(unsigned int));
			offset += sizeof(unsigned int);

		}
	}

	memcpy(&byteVal[offset], subSerial, subSize);
	delete[] subSerial;

	return byteVal;

}

char* GameGridObject::Unserialize(char* data)
{

	unsigned int offset = 0;

	memcpy(&gridWidth, data, sizeof(unsigned int));
	offset += sizeof(unsigned int);
	memcpy(&gridHeight, &data[offset], sizeof(unsigned int));
	sizeGrid();
	
	for (unsigned int i = 0; i < gridWidth; i++)
	{

		for (unsigned int k = 0; k < gridHeight; k++)
		{

			unsigned int node;
			memcpy(&node, &data[offset], sizeof(unsigned int));
			offset += sizeof(unsigned int);

			PopulateGrid(node, i, k);

		}
	}

	if (data[offset] == SerializableType_GAMEOBJECTSCENERY)
	{

		return GameObject::Unserialize(&data[offset + 1]);

	}

	return nullptr;

}

GameGridObject::~GameGridObject()
{

	if (nodes != nullptr)
	{

		delete[] nodes;

	}
}
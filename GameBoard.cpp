#include "stdafx.h"
#include "GameBoard.h"

using namespace Resources;
using namespace Entities;
using namespace CrossHandlers;

GameBoard::GameBoard(unsigned int cells, CelMesh* gridObject, unsigned char maxFlips, unsigned int maxInstances) : ScriptableObject()
{

	this->cells = cells;
	drawingBoard = new DrawingBoard(maxFlips,maxInstances);
	drawingBoard->AddMesh(gridObject);
	objectRoot = new ObjectTree(cells, 32, CelestialMath::Vector2(0.0f, 0.0f), ((BaseObject*)gridObject)->GetId());
	travelObjects = new CelestialSlicedList<GameObject*>(32, nullptr);
	activeObjects = new CelestialSlicedList<GameObject*>(32, nullptr);
	camera = nullptr;
	boardObject = nullptr;
	boardNormal = Vector3(0.0f, 1.0f, 0.0f);
	travelObjectsAmounts = 0;
	routing = new RoutingManager();

}

void GameBoard::ReCalcPaths(unsigned int time)
{

	for (unsigned int i = 0; i < travelObjects->GetHighest(); i++)
	{

		GameTravelObject* travelObj = (GameTravelObject*)travelObjects->GetValue(i);
		
		if (travelObj != nullptr &&
			travelObj->GetNode() != travelObj->GetFinalGoalNode())
		{

			routing->Travel(travelObj, travelObj->GetFinalGoalNode(), time);

		}
	}

}

RoutingManager* GameBoard::GetRoutingManager() const
{

	return routing;

}

char* GameBoard::Serialize(unsigned int &size)
{

	char** subs = nullptr;
	unsigned int* subSizes = nullptr;
	unsigned int subTot = 0;
	unsigned int subSize = 0;

	if (activeObjects->GetHighest() > 0)
	{

		subs = new char*[activeObjects->GetHighest()];
		subSizes = new unsigned int[activeObjects->GetHighest()];

		for (unsigned int i = 0; i < activeObjects->GetHighest(); i++)
		{

			GameObject* object = activeObjects->GetValue(i);

			if (object != nullptr 
				&& !object->IsChild()
				&& object->ShouldSave())
			{

				subs[subTot] = object->Serialize(subSizes[subTot]);
				char* temp = new char[subSizes[subTot] + sizeof(unsigned int)];

				memcpy(temp, &subSizes[subTot], sizeof(unsigned int));
				memcpy(&temp[4], subs[subTot], subSizes[subTot]);

				subSizes[subTot] += sizeof(unsigned int);

				delete[] subs[subTot];
				subs[subTot] = temp;

				subSize += subSizes[subTot];
				subTot++;

			}
		}
	
	}

	unsigned int routingSize = 0;
	char* routeBytes = routing->Serialize(routingSize);
	unsigned int subObjectSize;
	char* subSer = ScriptableObject::Serialize(subObjectSize);

	size = 1 + subSize + subObjectSize + sizeof(unsigned int) + routingSize;

	char* byteVal = new char[size];
	byteVal[0] = SerializableType_GAMEBOARD;
	unsigned int offset = 1;

	memcpy(&byteVal[offset], &subSize, 4);
	offset += 4;

	if (subs != nullptr)
	{

		for (unsigned int i = 0; i < subTot; i++)
		{

			memcpy(&byteVal[offset], subs[i], subSizes[i]);
			offset += subSizes[i];
			delete[] subs[i];

		}

		delete[] subSizes;
		delete[] subs;

	}

	memcpy(&byteVal[offset], routeBytes, routingSize);
	delete[] routeBytes;
	offset += routingSize;

	memcpy(&byteVal[offset], subSer, subObjectSize);
	delete[] subSer;
	return byteVal;

}

char* GameBoard::Unserialize(char* data)
{

	unsigned int offset = 1;

	if (data[0] == SerializableType_ROUTEMANAGER)
	{

		unsigned int totalNodes;
		memcpy(&totalNodes, &data[offset], sizeof(unsigned int));
		offset += sizeof(unsigned int);

		unsigned int nodes = totalNodes / 16;

		for (unsigned int i = 0; i < nodes; i++)
		{

			Vector3 nodePos;
			float width;
			memcpy(&nodePos.x, &data[offset], 4);
			offset += 4;
			memcpy(&nodePos.y, &data[offset], 4);
			offset += 4;
			memcpy(&nodePos.z, &data[offset], 4);
			offset += 4;
			memcpy(&width, &data[offset], 4);
			offset += 4;

			AddRouteNode(nodePos, width);

		}
	}


	if (data[offset] == SerializableType_SCRIPTABLE)
	{

		ScriptableObject::Unserialize(&data[offset + 1]);

	}

	return data;

}

void GameBoard::ClearObjects()
{

	objectRoot->ClearObjects();
	travelObjects->Reset();
	activeObjects->Reset();
	travelObjectsAmounts = 0;
	routing->ClearNodes();

}

bool GameBoard::GetBoardPosition(Vector3 origin, Vector3 unitDirection, Vector3 &position, float floor) const
{

	if (VectorDot(unitDirection, boardNormal) > 0)
	{

		return false;

	}

	Vector3 point(0, 0.5f + floor, 0);

	Vector3 toLineOrigin = point - origin;
	float scalar = VectorDot(toLineOrigin, boardNormal);
	Vector3 projectedLine = boardNormal *scalar;
	float dotProduct = VectorDot(unitDirection, projectedLine);
	float distance = 0;

	if (dotProduct <= CELESTIAL_EPSILON)
	{

		distance = VectorDot(origin, boardNormal);

	}
	else
	{

		float lineMag = sqrt(VectorDot(projectedLine, projectedLine));
		float angle = acos(VectorDot(unitDirection, projectedLine) / lineMag);

		distance = (1 / cos(angle)) *lineMag;

	}

	position = origin + unitDirection*distance;
	return true;

}

bool GameBoard::GetBoardPosition(Vector3 origin, Vector3 unitDirection, Vector3 &position) const
{

	return GetBoardPosition(origin, unitDirection, position, 0);

}

void GameBoard::Update(CrossHandlers::Message* mess)
{

	if (mess->type == MessageType_OBJECT)
	{

		unsigned int param1;
		bool found = false;
		unsigned int i;

		switch (mess->mess)
		{

		case ObjectMess_REMOVECHILD:
			param1 = mess->params[0] | ((int)mess->params[1] << 8) | ((int)mess->params[2] << 16) | ((int)mess->params[3] << 24);
			found = false;

			for (i = 0; i < travelObjectsAmounts && !found; i++)
			{

				found = travelObjects->GetValue(i) != nullptr && travelObjects->GetValue(i)->GetId() == param1;

				if (found)
				{

					travelObjects->Remove(i);

				}
			}
			
			if (!found)
			{

				objectRoot->RemoveObject(param1);

			}

			break;
		default:
			ScriptableObject::Update(mess);

		}
	}
}

void GameBoard::FillInstanceBuffer(GameObject* trackingObject)
{

	if (camera != nullptr)
	{

		camera->GetView()->ResetInstances();
		drawingBoard->StartAddingInstances();

		if (trackingObject != nullptr)
		{

			drawingBoard->AddInstance(trackingObject);

		}

		for (unsigned int i = 0; i < travelObjectsAmounts; i++)
		{

			GameObject* go = travelObjects->GetValue(i);

			if (go != nullptr)
			{

				drawingBoard->AddInstance(go);

			}
		}

		if (boardObject != nullptr)
		{

			drawingBoard->AddInstance(boardObject);

		}

		unsigned int checkedBoxes = objectRoot->AddInstance(camera->GetView(), drawingBoard);
		drawingBoard->FinalizeInstances(camera->GetView());

	}
}

void GameBoard::AddObject(GameObject* object)
{

	if (object->GetScale().x <= CELESTIAL_EPSILON ||
		object->GetScale().y <= CELESTIAL_EPSILON ||
		object->GetScale().z <= CELESTIAL_EPSILON ||
		object->GetActiveId() != 0)
	{

		return;

	}

	object->SetActiveId(activeObjects->Add(object) + 1);

	if (object->GetType() != GameObjectType_TRAVELING)
	{

		objectRoot->AddObject(object);

	}
	else
	{

		unsigned int id = travelObjects->Add(object);
		travelObjectsAmounts = id + 1 > travelObjectsAmounts ? id + 1 : travelObjectsAmounts;

	}

	object->SetParent(this);

}

unsigned int GameBoard::AddRouteNode(CelestialMath::Vector3 position, float width, unsigned int* collided, unsigned int &collAmounts)
{

	return routing->AddNode(position, collided, collAmounts);

}

unsigned int GameBoard::AddRouteNode(Vector3 position, float width)
{

	BoundingSphere sphere = BoundingSphere(position.x, position.y, position.z, width / 2.0f);
	unsigned int amount = 0;

	unsigned int* collided = GetCollidedObject(&sphere, GameObjectType_ROUTE, amount);
	return routing->AddNode(position, collided, amount);

}

CelestialSlicedList<GameObject*>* GameBoard::GetActiveObjects() const
{

	return activeObjects;

}

GameObject* GameBoard::GetBoardObject() const
{

	return boardObject;

}

void GameBoard::SetBoardObject(GameObject* object)
{

	object->SetScale(Vector3(cells, 0.1f, cells));
	object->UpdateMatrix();
	boardObject = object;

}

void GameBoard::SetCamera(CameraObject* cam)
{
	
	this->camera = cam;

}

CameraObject* GameBoard::GetCam() const
{

	return camera;

}

DrawingBoard* GameBoard::GetDrawingBoard() const
{

	return drawingBoard;

}

unsigned int GameBoard::GetClosestObject(Vector3 origin, Vector3 unitDirection, float &smallestDistance) const
{

	return objectRoot->GetClosestObject(origin, unitDirection, smallestDistance);

}

unsigned int* GameBoard::GetCollidedObject(GameObject* object,unsigned int &amount) const
{

	return objectRoot->GetCollidedObject(object,amount);

}

unsigned int* GameBoard::GetCollidedObject(BoundingSphere* sphere, GameObjectType filter, unsigned int &amount) const
{

	return objectRoot->GetCollidedObject(sphere, filter, amount);

}

GameBoard::~GameBoard()
{

	delete drawingBoard;
	delete objectRoot;
	delete travelObjects;
	delete activeObjects;
	delete routing;

}
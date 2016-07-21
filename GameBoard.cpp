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
	camera = nullptr;
	boardObject = nullptr;
	boardNormal = Vector3(0.0f, 1.0f, 0.0f);
	travelObjectsAmounts = 0;

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

}
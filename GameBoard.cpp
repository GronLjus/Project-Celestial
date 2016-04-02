#include "stdafx.h"
#include "GameBoard.h"

using namespace Resources;
using namespace Entities;

GameBoard::GameBoard(unsigned int cells) : ScriptableObject()
{

	this->cells = cells;
	drawingBoard = new DrawingBoard();
	objectRoot = new ObjectTree(cells, 32, CelestialMath::Vector2(0.0f, 0.0f));
	camera = nullptr;
	boardObject = nullptr;

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
			objectRoot->RemoveObject(param1);
			break;
		default:
			ScriptableObject::Update(mess);

		}
	}
}

void GameBoard::FillInstanceBuffer()
{

	if (camera != nullptr)
	{

		camera->GetView()->ResetInstances();
		drawingBoard->StartAddingInstances();

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

	objectRoot->AddObject(object);
	object->SetParent(this);

}

GameObject* GameBoard::GetBoardObject() const
{

	return boardObject;

}

void GameBoard::SetBoardObject(GameObject* object)
{

	Message mess = Message();
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_SCALE;

	float scaleX = cells;
	float scaleY = 1.0f;
	float scaleZ = cells;

	unsigned char tempBuff[12];

	memcpy(&tempBuff, &scaleX, 4);
	memcpy(&tempBuff[4], &scaleY, 4);
	memcpy(&tempBuff[8], &scaleZ, 4);

	mess.SetParams(tempBuff, 0, 12);
	object->Update(&mess);

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

GameBoard::~GameBoard()
{

	delete drawingBoard;
	delete objectRoot;

}
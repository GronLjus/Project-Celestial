#include "stdafx.h"
#include "GameBoard.h"

using namespace Resources;
using namespace Entities;

GameBoard::GameBoard(unsigned int cells)
{

	drawingBoard = new DrawingBoard();
	objectRoot = new ObjectTree(cells, 32, CelestialMath::Vector2(0.0f, 0.0f));
	camera = nullptr;

}

void GameBoard::FillInstanceBuffer()
{

	if (camera != nullptr)
	{

		camera->GetView()->ResetInstances();
		drawingBoard->StartAddingInstances();
		unsigned int checkedBoxes = objectRoot->AddInstance(camera->GetView(), drawingBoard);
		drawingBoard->FinalizeInstances(camera->GetView());

	}
}

void GameBoard::AddObject(GameObject* object)
{

	objectRoot->AddObject(object);

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
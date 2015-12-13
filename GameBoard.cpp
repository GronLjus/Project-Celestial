#include "stdafx.h"
#include "GameBoard.h"

using namespace Resources;
using namespace Entities;

GameBoard::GameBoard(unsigned int cells)
{

	drawingBoard = new DrawingBoard();
	objectRoot = new ObjectTree(cells, 16, 1, CelestialMath::Vector2(0.0f, 0.0f));
	camera = nullptr;

}

void GameBoard::FillInstanceBuffer()
{

	if (camera != nullptr)
	{

		drawingBoard->StartAddingInstances();
		objectRoot->AddInstance(camera->GetView(), drawingBoard);
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

GameBoard::~GameBoard()
{

	delete drawingBoard;
	delete objectRoot;

}
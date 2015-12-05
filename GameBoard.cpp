#include "stdafx.h"
#include "GameBoard.h"

using namespace Resources;
using namespace Entities;

GameBoard::GameBoard(unsigned int cells)
{

	drawingBoard = new DrawingBoard();
	objectRoot = new ObjectTree(cells, 16, 1, CelestialMath::Vector2(0.0f, 0.0f));

}

void GameBoard::AddObject(GameObject* object)
{

	objectRoot->AddObject(object);

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
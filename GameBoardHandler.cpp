#include "stdafx.h"
#include "GameBoardHandler.h"

using namespace Entities;
using namespace Resources;
using namespace CrossHandlers;

GameBoardHandler::GameBoardHandler() : IHandleMessages(200, MessageSource_ENTITIES)
{

	localGameBoard = nullptr;
	filter = MessageType_ENTITIES;

}

void GameBoardHandler::Init(CelestialSlicedList<BaseObject*>* gameObjects)
{

	this->gameObjects = gameObjects;

}

void GameBoardHandler::Update(unsigned int time)
{
	Message* currentMessage = inQueue->PopMessage();

	while (currentMessage->type != MessageType_NA)
	{

		unsigned int param1 = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);

		if (currentMessage->mess == GameBoardMess_ADDMESH && localGameBoard != nullptr)
		{

			localGameBoard->GetDrawingBoard()->AddMesh((MeshObject*)(gameObjects->GetValue(param1)));

			messageBuffer[this->currentMessage].timeSent = time;
			messageBuffer[this->currentMessage].destination = MessageSource_GRAPHICS;
			messageBuffer[this->currentMessage].type = MessageType_GRAPHICS;
			messageBuffer[this->currentMessage].mess = GraphicMess_UPDATEGAMEBOARDBUFFERS;
			messageBuffer[this->currentMessage].read = false;
			outQueue->PushMessage(&messageBuffer[this->currentMessage]);
			this->currentMessage = (this->currentMessage + 1) % outMessages;

		}
		else if (currentMessage->mess == GameBoardMess_ADDOBJECT && localGameBoard != nullptr)
		{

			localGameBoard->AddObject((GameObject*)(gameObjects->GetValue(param1)));

		}
		else if (currentMessage->mess == GameBoardMess_SETCAM && localGameBoard != nullptr)
		{

			localGameBoard->SetCamera((CameraObject*)(gameObjects->GetValue(param1)));

			messageBuffer[this->currentMessage].timeSent = time;
			messageBuffer[this->currentMessage].destination = MessageSource_GRAPHICS;
			messageBuffer[this->currentMessage].type = MessageType_GRAPHICS;
			messageBuffer[this->currentMessage].mess = GraphicMess_SETCAMERA;
			messageBuffer[this->currentMessage].read = false;
			messageBuffer[this->currentMessage].SetParams(currentMessage->params,0,4);
			outQueue->PushMessage(&messageBuffer[this->currentMessage]);
			this->currentMessage = (this->currentMessage + 1) % outMessages;

		}
		else if (currentMessage->mess == GameBoardMess_SETGAMEBOARD)
		{

			localGameBoard = (GameBoard*)(gameObjects->GetValue(param1));

			messageBuffer[this->currentMessage].timeSent = time;
			messageBuffer[this->currentMessage].destination = MessageSource_GRAPHICS;
			messageBuffer[this->currentMessage].type = MessageType_GRAPHICS;
			messageBuffer[this->currentMessage].mess = GraphicMess_SETGAMEBOARD;
			messageBuffer[this->currentMessage].read = false;
			messageBuffer[this->currentMessage].SetParams(currentMessage->params, 0, 4);
			outQueue->PushMessage(&messageBuffer[this->currentMessage]);
			this->currentMessage = (this->currentMessage + 1) % outMessages;
			
		}

		currentMessage->read = true;
		currentMessage = inQueue->PopMessage();

	}

	if (localGameBoard != nullptr)
	{

		localGameBoard->FillInstanceBuffer();

	}
}

GameBoardHandler::~GameBoardHandler()
{

}

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

void GameBoardHandler::UpdateMessages(unsigned int time)
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
		else if (currentMessage->mess == GameBoardMess_SELECTOBJECT && localGameBoard != nullptr && localGameBoard->GetCam() != nullptr)
		{

			CameraObject* cam = localGameBoard->GetCam();
			int mouseX = currentMessage->params[4] | ((int)currentMessage->params[5] << 8) | ((int)currentMessage->params[6] << 16) | ((int)currentMessage->params[7] << 24);
			int mouseY = currentMessage->params[8] | ((int)currentMessage->params[9] << 8) | ((int)currentMessage->params[10] << 16) | ((int)currentMessage->params[11] << 24);
			float screenX = ((float)mouseX) / (float)(cam->GetView()->GetPort().width);
			float screenY = ((float)mouseY) / (float)(cam->GetView()->GetPort().height);
			screenX = 2 * screenX - 1;
			screenY = 2 * screenX - 1;
			float screenZ = cam->GetView()->GetPort().minDepth;

			Vector3 worldPoint = VectorTransform(Vector3(screenX, screenY, screenZ), cam->GetView()->GetInverseViewProjection(cam->GetFlip()));
			Vector3 direction = worldPoint - cam->GetPosition();
			direction /= sqrt(abs(VectorDot(direction, direction)));
			float minDist = 0;
			unsigned int selectedObject = localGameBoard->GetClosestObject(cam->GetPosition(), direction, minDist);

			char tempBuff[]{param1 >> 0, param1 >> 8, param1 >> 16, param1 >> 24,
				selectedObject >> 0, selectedObject >> 8, selectedObject >> 16, selectedObject >> 24};

			messageBuffer[this->currentMessage].timeSent = time;
			messageBuffer[this->currentMessage].destination = MessageSource_CELSCRIPT;
			messageBuffer[this->currentMessage].type = MessageType_SCRIPT;
			messageBuffer[this->currentMessage].mess = ScriptMess_ADDPARNUM;
			messageBuffer[this->currentMessage].read = false;
			messageBuffer[this->currentMessage].SetParams(tempBuff, 0, 8);
			outQueue->PushMessage(&messageBuffer[this->currentMessage]);
			this->currentMessage = (this->currentMessage + 1) % outMessages;

			messageBuffer[this->currentMessage].timeSent = time;
			messageBuffer[this->currentMessage].destination = MessageSource_CELSCRIPT;
			messageBuffer[this->currentMessage].type = MessageType_SCRIPT;
			messageBuffer[this->currentMessage].mess = ScriptMess_RUN;
			messageBuffer[this->currentMessage].read = false;
			messageBuffer[this->currentMessage].SetParams(tempBuff, 0, 4);
			outQueue->PushMessage(&messageBuffer[this->currentMessage]);
			this->currentMessage = (this->currentMessage + 1) % outMessages;

		}
		else if (currentMessage->mess == GameBoardMess_SETCAM && localGameBoard != nullptr)
		{

			localGameBoard->SetCamera((CameraObject*)(gameObjects->GetValue(param1)));

			messageBuffer[this->currentMessage].timeSent = time;
			messageBuffer[this->currentMessage].destination = MessageSource_GRAPHICS;
			messageBuffer[this->currentMessage].type = MessageType_GRAPHICS;
			messageBuffer[this->currentMessage].mess = GraphicMess_SETCAMERA;
			messageBuffer[this->currentMessage].read = false;
			messageBuffer[this->currentMessage].SetParams(currentMessage->params, 0, 4);
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
}

void GameBoardHandler::Update(unsigned int time)
{

	UpdateMessages(time);

	if (localGameBoard != nullptr)
	{

		localGameBoard->FillInstanceBuffer();

	}
}

GameBoardHandler::~GameBoardHandler()
{

}

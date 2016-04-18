#include "stdafx.h"
#include "GameBoardHandler.h"

using namespace Entities;
using namespace Resources;
using namespace CrossHandlers;

GameBoardHandler::GameBoardHandler() : IHandleMessages(200, MessageSource_ENTITIES)
{

	localGameBoard = nullptr;
	filter = MessageType_ENTITIES;
	dragScript = 0;

}

void GameBoardHandler::Init(CelestialSlicedList<BaseObject*>* gameObjects)
{

	this->gameObjects = gameObjects;

}

void GameBoardHandler::sendCommonScriptParams(unsigned int script, unsigned int objectId, unsigned int time, int mouseX, int mouseY)
{
	
	unsigned char tempBuff[]{ script >> 0, script >> 8, script >> 16, script >> 24,
		objectId >> 0, objectId >> 8, objectId >> 16, objectId >> 24 };

	unsigned char tempBuff2[]{ script >> 0, script >> 8, script >> 16, script >> 24,
		mouseX >> 0, mouseX >> 8, mouseX >> 16, mouseX >> 24
	};
	unsigned char tempBuff3[]{ script >> 0, script >> 8, script >> 16, script >> 24,
		mouseY >> 0, mouseY >> 8, mouseY >> 16, mouseY >> 24
	};

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
	messageBuffer[this->currentMessage].mess = ScriptMess_ADDPARNUM;
	messageBuffer[this->currentMessage].read = false;
	messageBuffer[this->currentMessage].SetParams(tempBuff2, 0, 8);
	outQueue->PushMessage(&messageBuffer[this->currentMessage]);
	this->currentMessage = (this->currentMessage + 1) % outMessages;

	messageBuffer[this->currentMessage].timeSent = time;
	messageBuffer[this->currentMessage].destination = MessageSource_CELSCRIPT;
	messageBuffer[this->currentMessage].type = MessageType_SCRIPT;
	messageBuffer[this->currentMessage].mess = ScriptMess_ADDPARNUM;
	messageBuffer[this->currentMessage].read = false;
	messageBuffer[this->currentMessage].SetParams(tempBuff3, 0, 8);
	outQueue->PushMessage(&messageBuffer[this->currentMessage]);
	this->currentMessage = (this->currentMessage + 1) % outMessages;

	messageBuffer[this->currentMessage].timeSent = time;
	messageBuffer[this->currentMessage].destination = MessageSource_CELSCRIPT;
	messageBuffer[this->currentMessage].type = MessageType_SCRIPT;
	messageBuffer[this->currentMessage].mess = ScriptMess_ADDPARFLOAT;
	messageBuffer[this->currentMessage].read = false;
	memcpy(&(tempBuff[4]), &(boardPos.x), 4);
	messageBuffer[this->currentMessage].SetParams(tempBuff, 0, 8);
	outQueue->PushMessage(&messageBuffer[this->currentMessage]);
	this->currentMessage = (this->currentMessage + 1) % outMessages;
	messageBuffer[this->currentMessage].timeSent = time;
	messageBuffer[this->currentMessage].destination = MessageSource_CELSCRIPT;
	messageBuffer[this->currentMessage].type = MessageType_SCRIPT;
	messageBuffer[this->currentMessage].mess = ScriptMess_ADDPARFLOAT;
	messageBuffer[this->currentMessage].read = false;
	memcpy(&(tempBuff[4]), &(boardPos.y), 4);
	messageBuffer[this->currentMessage].SetParams(tempBuff, 0, 8);
	outQueue->PushMessage(&messageBuffer[this->currentMessage]);
	this->currentMessage = (this->currentMessage + 1) % outMessages;
	messageBuffer[this->currentMessage].timeSent = time;
	messageBuffer[this->currentMessage].destination = MessageSource_CELSCRIPT;
	messageBuffer[this->currentMessage].type = MessageType_SCRIPT;
	messageBuffer[this->currentMessage].mess = ScriptMess_ADDPARFLOAT;
	messageBuffer[this->currentMessage].read = false;
	memcpy(&(tempBuff[4]), &(boardPos.z), 4);
	messageBuffer[this->currentMessage].SetParams(tempBuff, 0, 8);
	outQueue->PushMessage(&messageBuffer[this->currentMessage]);
	this->currentMessage = (this->currentMessage + 1) % outMessages;

}

void GameBoardHandler::runScript(unsigned int script, unsigned int time)
{

	unsigned char tempBuff[]{ script >> 0, script >> 8, script >> 16, script >> 24 };
	messageBuffer[this->currentMessage].timeSent = time;
	messageBuffer[this->currentMessage].destination = MessageSource_CELSCRIPT;
	messageBuffer[this->currentMessage].type = MessageType_SCRIPT;
	messageBuffer[this->currentMessage].mess = ScriptMess_RUN;
	messageBuffer[this->currentMessage].read = false;
	messageBuffer[this->currentMessage].SetParams(tempBuff, 0, 4);
	outQueue->PushMessage(&messageBuffer[this->currentMessage]);
	this->currentMessage = (this->currentMessage + 1) % outMessages;

}

void GameBoardHandler::triggerMouseScript(unsigned int script, unsigned int objectId, unsigned int time, int mouseX, int mouseY)
{

	sendCommonScriptParams(script, objectId, time, mouseX, mouseY);
	runScript(script, time);

}

void GameBoardHandler::triggerMouseScript(unsigned int script, unsigned int objectId, unsigned int time, int mouseX, int mouseY, unsigned int dragStatus)
{


	sendCommonScriptParams(script, objectId, time, mouseX, mouseY);

	unsigned char tempBuff[]{ script >> 0, script >> 8, script >> 16, script >> 24,
		dragStatus >> 0, dragStatus >> 8, dragStatus >> 16, dragStatus >> 24
	};

	messageBuffer[this->currentMessage].timeSent = time;
	messageBuffer[this->currentMessage].destination = MessageSource_CELSCRIPT;
	messageBuffer[this->currentMessage].type = MessageType_SCRIPT;
	messageBuffer[this->currentMessage].mess = ScriptMess_ADDPARNUM;
	messageBuffer[this->currentMessage].read = false;
	messageBuffer[this->currentMessage].SetParams(tempBuff, 0, 8);
	outQueue->PushMessage(&messageBuffer[this->currentMessage]);
	this->currentMessage = (this->currentMessage + 1) % outMessages;

	runScript(script, time);

}

Vector3 GameBoardHandler::getMouseWorldLine(unsigned int mouseX, unsigned int mouseY) const
{

	CameraObject* cam = localGameBoard->GetCam();
	float screenX = ((float)mouseX) / (float)(cam->GetView()->GetPort().width);
	float screenY = ((float)mouseY) / (float)(cam->GetView()->GetPort().height);
	screenX = 2 * screenX - 1;
	screenY = 2 * screenY - 1;
	float screenZ = 0;// cam->GetView()->GetPort().minDepth;

	Vector3 worldPoint = VectorTransform(Vector3(screenX, screenY, screenZ), cam->GetView()->GetInverseViewProjection(cam->GetFlip()));
	Vector3 direction = worldPoint - cam->GetPosition();
	direction /= sqrt(abs(VectorDot(direction, direction)));
	return direction;

}

ScriptableObject * GameBoardHandler::getMouseObject(CelestialMath::Vector3 direction) const
{

	CameraObject* cam = localGameBoard->GetCam();
	float minDist = 0;
	unsigned int selectedObject = localGameBoard->GetClosestObject(cam->GetPosition(), direction, minDist);
	ScriptableObject* obj = localGameBoard;

	if (minDist != 0)
	{

		obj = (ScriptableObject*)gameObjects->GetValue(selectedObject);

	}
	return obj;

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
		else if (currentMessage->mess == GameBoardMess_CLICKOBJECT && 
			localGameBoard != nullptr && localGameBoard->GetCam() != nullptr)
		{

			CameraObject* cam = localGameBoard->GetCam();
			unsigned int mouseX = currentMessage->params[1] | ((int)currentMessage->params[2] << 8) | ((int)currentMessage->params[3] << 16) | ((int)currentMessage->params[4] << 24);
			unsigned int mouseY = currentMessage->params[5] | ((int)currentMessage->params[6] << 8) | ((int)currentMessage->params[7] << 16) | ((int)currentMessage->params[8] << 24);
			
			Vector3 direction = getMouseWorldLine(mouseX, mouseY);
			ScriptableObject* obj = getMouseObject(direction);

			unsigned int script = currentMessage->params[0] == 0 ? obj->GetLeftClickScript() :
				currentMessage->params[0] == 1 ? obj->GetMiddleClickScript() :
				obj->GetRightClickScript();

			if (script != 0)
			{

				script--;
				localGameBoard->GetBoardPosition(cam->GetPosition(), direction, boardPos);
				triggerMouseScript(script, obj->GetId(), time, 0, 0);

			}
		}
		else if (currentMessage->mess == GameBoardMess_STARTDRAGGING &&
			localGameBoard != nullptr && localGameBoard->GetCam() != nullptr)
		{

			unsigned int mouseX = currentMessage->params[1] | ((int)currentMessage->params[2] << 8) | ((int)currentMessage->params[3] << 16) | ((int)currentMessage->params[4] << 24);
			unsigned int mouseY = currentMessage->params[5] | ((int)currentMessage->params[6] << 8) | ((int)currentMessage->params[7] << 16) | ((int)currentMessage->params[8] << 24);

			Vector3 direction = getMouseWorldLine(mouseX, mouseY);
			ScriptableObject* obj = getMouseObject(direction);

			unsigned int script = currentMessage->params[0] == 0 ? obj->GetLeftDragScript() :
				currentMessage->params[0] == 1 ? obj->GetMiddleDragScript() :
				obj->GetRightDragScript();

			if (script != 0)
			{

				dragScript = script;
				dragId = obj->GetId();
				script--;
				localGameBoard->GetBoardPosition(localGameBoard->GetCam()->GetPosition(), direction, boardPos);
				triggerMouseScript(script, dragId, time, mouseX, mouseY, 0);

			}
		}
		else if (currentMessage->mess == GameBoardMess_DRAGOBJECT &&
			localGameBoard != nullptr && localGameBoard->GetCam() != nullptr)
		{

			unsigned int mouseX = currentMessage->params[1] | ((int)currentMessage->params[2] << 8) | ((int)currentMessage->params[3] << 16) | ((int)currentMessage->params[4] << 24);
			unsigned int mouseY = currentMessage->params[5] | ((int)currentMessage->params[6] << 8) | ((int)currentMessage->params[7] << 16) | ((int)currentMessage->params[8] << 24);

			Vector3 direction = getMouseWorldLine(mouseX, mouseY);

			if (dragScript != 0)
			{

				localGameBoard->GetBoardPosition(localGameBoard->GetCam()->GetPosition(), direction, boardPos);
				triggerMouseScript(dragScript-1, dragId, time, mouseX, mouseY, 1);

			}
		}
		else if (currentMessage->mess == GameBoardMess_ORBITOBJECT &&
			localGameBoard != nullptr && localGameBoard->GetCam() != nullptr)
		{

			unsigned int obj = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);
			PositionableObject* object = (PositionableObject*)gameObjects->GetValue(obj);

			Vector3 objectDirection = object->GetDirection();
			Vector3 gameBoardPos(object->GetPosition() + objectDirection*10);

			localGameBoard->GetBoardPosition(object->GetPosition(), objectDirection, gameBoardPos);

			messageBuffer[this->currentMessage].timeSent = time;
			messageBuffer[this->currentMessage].destination = MessageSource_OBJECT;
			messageBuffer[this->currentMessage].type = MessageType_OBJECT;
			messageBuffer[this->currentMessage].mess = ObjectMess_ORBIT;
			messageBuffer[this->currentMessage].read = false;

			unsigned char tempBuff[16];
			memcpy(tempBuff, &gameBoardPos.x, 4);
			memcpy(&tempBuff[4], &gameBoardPos.y, 4);
			memcpy(&tempBuff[8], &gameBoardPos.z, 4);
			memcpy(&tempBuff[12], &currentMessage->params[4], 4);
			messageBuffer[this->currentMessage].SetParams(tempBuff, 0, 16);
			object->Update(&messageBuffer[this->currentMessage]);

		}
		else if (currentMessage->mess == GameBoardMess_STOPDRAGGING &&
			localGameBoard != nullptr && localGameBoard->GetCam() != nullptr)
		{

			unsigned int mouseX = currentMessage->params[1] | ((int)currentMessage->params[2] << 8) | ((int)currentMessage->params[3] << 16) | ((int)currentMessage->params[4] << 24);
			unsigned int mouseY = currentMessage->params[5] | ((int)currentMessage->params[6] << 8) | ((int)currentMessage->params[7] << 16) | ((int)currentMessage->params[8] << 24);

			Vector3 direction = getMouseWorldLine(mouseX, mouseY);

			if (dragScript != 0)
			{

				localGameBoard->GetBoardPosition(localGameBoard->GetCam()->GetPosition(), direction, boardPos);
				triggerMouseScript(dragScript - 1, dragId, time, mouseX, mouseY, 2);
				dragScript = 0;

			}
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

			GameBoard* gB = (GameBoard*)(gameObjects->GetValue(param1));

			messageBuffer[this->currentMessage].timeSent = time;
			messageBuffer[this->currentMessage].destination = MessageSource_GRAPHICS;
			messageBuffer[this->currentMessage].type = MessageType_GRAPHICS;
			messageBuffer[this->currentMessage].mess = GraphicMess_SETGAMEBOARD;
			messageBuffer[this->currentMessage].read = false;
			messageBuffer[this->currentMessage].SetParams(currentMessage->params, 0, 4);
			outQueue->PushMessage(&messageBuffer[this->currentMessage]);
			this->currentMessage = (this->currentMessage + 1) % outMessages;

			if (gB->GetBoardObject() != nullptr)
			{

				gB->GetDrawingBoard()->AddMesh((MeshObject*)(gameObjects->GetValue(gB->GetBoardObject()->GetMeshId())));
				messageBuffer[this->currentMessage].timeSent = time;
				messageBuffer[this->currentMessage].destination = MessageSource_GRAPHICS;
				messageBuffer[this->currentMessage].type = MessageType_GRAPHICS;
				messageBuffer[this->currentMessage].mess = GraphicMess_UPDATEGAMEBOARDBUFFERS;
				messageBuffer[this->currentMessage].read = false;
				outQueue->PushMessage(&messageBuffer[this->currentMessage]);
				this->currentMessage = (this->currentMessage + 1) % outMessages;

			}

			localGameBoard = gB;

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

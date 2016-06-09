#include "stdafx.h"
#include "GameBoardHandler.h"

using namespace Entities;
using namespace Resources;
using namespace CrossHandlers;

GameBoardHandler::GameBoardHandler() : IHandleMessages(200, MessageSource_ENTITIES)
{

	localGameBoard = nullptr;
	trackedObject = nullptr;
	filter = MessageType_ENTITIES;
	dragScript = 0;
	hookObject = false;

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

void GameBoardHandler::triggerMouseScript(unsigned int script, unsigned int objectId, unsigned int time, int mouseX, int mouseY, short delta)
{

	sendCommonScriptParams(script, objectId, time, mouseX, mouseY);

	unsigned char tempBuff[]{ script >> 0, script >> 8, script >> 16, script >> 24,
		delta >> 0, delta >> 8, delta >> 16, delta >> 24
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
	screenY = -screenY;
	float screenZ = 0;// cam->GetView()->GetPort().minDepth;

	Vector3 worldPoint = VectorTransform(Vector3(screenX, screenY, screenZ), cam->GetView()->GetInverseViewProjection(cam->GetFlip()));
	Vector3 direction = worldPoint - cam->GetPosition();
	direction /= sqrt(abs(VectorDot(direction, direction)));
	return direction;

}

ScriptableObject* GameBoardHandler::getMouseObject(CelestialMath::Vector3 direction) const
{

	if (trackedObject != nullptr)
	{

		return trackedObject;

	}

	CameraObject* cam = localGameBoard->GetCam();
	float minDist = 0;
	unsigned int selectedObject = localGameBoard->GetClosestObject(cam->GetPosition(), direction, minDist);
	ScriptableObject* obj = nullptr;

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
			unsigned int mouseX = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);
			unsigned int mouseY = currentMessage->params[4] | ((int)currentMessage->params[5] << 8) | ((int)currentMessage->params[6] << 16) | ((int)currentMessage->params[7] << 24);
			
			Vector3 direction = getMouseWorldLine(mouseX, mouseY);
			ScriptableObject* obj = getMouseObject(direction);
			unsigned int script = 0;

			if (obj != nullptr)
			{

				script = currentMessage->params[8] == 0 ? obj->GetLeftClickScript() :
					currentMessage->params[8] == 1 ? obj->GetMiddleClickScript() :
					obj->GetRightClickScript();

			}

			if (obj == nullptr || script == 0)
			{

				obj = localGameBoard;
				script = currentMessage->params[8] == 0 ? obj->GetLeftClickScript() :
					currentMessage->params[8] == 1 ? obj->GetMiddleClickScript() :
					obj->GetRightClickScript();

			}

			if (script != 0)
			{

				script--;
				localGameBoard->GetBoardPosition(cam->GetPosition(), direction, boardPos);
				triggerMouseScript(script, obj->GetId(), time, mouseX, mouseY);

			}
		}
		else if (currentMessage->mess == GameBoardMess_WHEELOBJECT &&
			localGameBoard != nullptr && localGameBoard->GetCam() != nullptr)
		{

			CameraObject* cam = localGameBoard->GetCam();
			unsigned int mouseX = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);
			unsigned int mouseY = currentMessage->params[4] | ((int)currentMessage->params[5] << 8) | ((int)currentMessage->params[6] << 16) | ((int)currentMessage->params[7] << 24);
			short wheelDelta = currentMessage->params[8] | ((int)currentMessage->params[9] << 8);

			Vector3 direction = getMouseWorldLine(mouseX, mouseY);
			ScriptableObject* obj = getMouseObject(direction);
			unsigned int script = 0;

			if (obj != nullptr)
			{

				script = obj->GetWheelScript();

			}

			if (obj == nullptr || script == 0)
			{

				obj = localGameBoard;
				script = obj->GetWheelScript();

			}

			if (script != 0)
			{

				script--;
				localGameBoard->GetBoardPosition(cam->GetPosition(), direction, boardPos);
				triggerMouseScript(script, obj->GetId(), time, mouseX, mouseY, wheelDelta);

			}
		}
		else if (currentMessage->mess == GameBoardMess_STARTDRAGGING &&
			localGameBoard != nullptr && localGameBoard->GetCam() != nullptr)
		{

			unsigned int mouseX = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);
			unsigned int mouseY = currentMessage->params[4] | ((int)currentMessage->params[5] << 8) | ((int)currentMessage->params[6] << 16) | ((int)currentMessage->params[7] << 24);
			
			Vector3 direction = getMouseWorldLine(mouseX, mouseY);
			ScriptableObject* obj = getMouseObject(direction);
			unsigned int script = 0;

			if (obj != nullptr)
			{

				script = currentMessage->params[8] == 0 ? obj->GetLeftDragScript() :
					currentMessage->params[8] == 1 ? obj->GetMiddleDragScript() :
					obj->GetRightDragScript();

			}

			if (obj == nullptr || script == 0)
			{

				obj = localGameBoard;
				script = currentMessage->params[8] == 0 ? obj->GetLeftDragScript() :
					currentMessage->params[8] == 1 ? obj->GetMiddleDragScript() :
					obj->GetRightDragScript();

			}

			if (script != 0)
			{

				dragScript = script;
				dragId = obj->GetId();
				script--;
				localGameBoard->GetBoardPosition(localGameBoard->GetCam()->GetPosition(), direction, boardPos);
				triggerMouseScript(script, dragId, time, mouseX, mouseY, (unsigned int)0);

			}
		}
		else if (currentMessage->mess == GameBoardMess_DRAGOBJECT &&
			localGameBoard != nullptr && localGameBoard->GetCam() != nullptr)
		{

			unsigned int mouseX = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);
			unsigned int mouseY = currentMessage->params[4] | ((int)currentMessage->params[5] << 8) | ((int)currentMessage->params[6] << 16) | ((int)currentMessage->params[7] << 24);

			Vector3 direction = getMouseWorldLine(mouseX, mouseY);

			if (dragScript != 0)
			{

				localGameBoard->GetBoardPosition(localGameBoard->GetCam()->GetPosition(), direction, boardPos);
				triggerMouseScript(dragScript-1, dragId, time, mouseX, mouseY, (unsigned int)1);

			}
		}
		else if (currentMessage->mess == GameBoardMess_ORBITOBJECT &&
			localGameBoard != nullptr && localGameBoard->GetCam() != nullptr)
		{

			unsigned int obj = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);
			PositionableObject* object = (PositionableObject*)gameObjects->GetValue(obj);

			Vector3 objectDirection = object->GetDirection();
			Vector3 gameBoardPos(object->GetPosition() + objectDirection*10);

			bool result = localGameBoard->GetBoardPosition(object->GetPosition(), objectDirection, gameBoardPos);

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
				triggerMouseScript(dragScript - 1, dragId, time, mouseX, mouseY, (unsigned int)2);
				dragScript = 0;

			}
		}
		else if (currentMessage->mess == GameBoardMess_SETTRACKING)
		{

			unsigned int obj = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);
			trackedObject = (GameObject*)gameObjects->GetValue(obj);

		}
		else if (currentMessage->mess == GameBoardMess_CLEARTRACK)
		{

			trackedObject->SetPosition(hookPos);
			trackedObject->SetScale(hookScale);
			trackedObject->SetRotation(hookRot);
			trackedObject = nullptr;
			hookObject = false;

		}
		else if ((currentMessage->mess == GameBoardMess_HOOKTRACK || currentMessage->mess == GameBoardMess_UNHOOKTRACK ) 
			&& trackedObject != nullptr)
		{

			hookObject = currentMessage->mess == GameBoardMess_HOOKTRACK;

			if (hookObject)
			{

				hookPos = trackedObject->GetPosition();
				hookScale = trackedObject->GetScale();
				hookRot = trackedObject->GetRotation();

			}
			else
			{

				trackedObject->SetPosition(hookPos);
				trackedObject->SetScale(hookScale);
				trackedObject->SetRotation(hookRot);

			}
		}
		else if (currentMessage->mess == GameBoardMess_MOUSEMOVE && trackedObject != nullptr &&
			localGameBoard != nullptr && localGameBoard->GetCam() != nullptr)
		{

			unsigned int mouseX = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);
			unsigned int mouseY = currentMessage->params[4] | ((int)currentMessage->params[5] << 8) | ((int)currentMessage->params[6] << 16) | ((int)currentMessage->params[7] << 24);
			handleMouseMovement(mouseX, mouseY);

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

void GameBoardHandler::handleMouseMovement(unsigned int mouseX, unsigned int mouseY)
{

	Vector3 direction = getMouseWorldLine(mouseX, mouseY);
	float halfHeight = trackedObject->GetScale().y / 2;
	localGameBoard->GetBoardPosition(localGameBoard->GetCam()->GetPosition(), direction, boardPos, halfHeight);
	Vector3 scale = trackedObject->GetScale();
	unsigned char tempBuff[12];
	Message mess;

	if (!hookObject)
	{

		boardPos.x = floor(boardPos.x) + scale.x / 2;
		boardPos.z = floor(boardPos.z) + scale.z / 2;
		memcpy(&tempBuff[0], &boardPos.x, 4);
		memcpy(&tempBuff[4], &boardPos.y, 4);
		memcpy(&tempBuff[8], &boardPos.z, 4);

		mess.destination = MessageSource_OBJECT;
		mess.mess = ObjectMess_POS;
		mess.type = MessageType_OBJECT;
		mess.SetParams(tempBuff, 0, 12);
		trackedObject->Update(&mess);

		unsigned int collidedObj = localGameBoard->GetCollidedObject(trackedObject);

		if (collidedObj > 0)
		{

			PositionableObject* obj = (PositionableObject*)gameObjects->GetValue(collidedObj);
			Vector3 newPos = obj->GetObjectCenterLine(boardPos);
			trackedObject->SetPosition(newPos);
			trackedObject->UpdateMatrix();

		}

	}
	else
	{

		boardPos.x = floor(boardPos.x) + hookScale.x / 2;
		boardPos.z = floor(boardPos.z) + hookScale.z / 2;
		Vector3 dist = boardPos - hookPos;
		scale.z = hookScale.z + sqrt(VectorDot(dist,dist));
		trackedObject->SetScale(scale);

		if (scale.z - hookScale.z > 0)
		{

			Vector3 forward = Vector3(0.0f, 0.0f, 1.0f);
			float dotProduct = VectorDot(forward, dist);
			float yAngle = acos(dotProduct / (scale.z - hookScale.z));
			float sideDot = VectorDot(Vector3(1.0f, 0.0f, 0.0f), dist);
			yAngle *= sideDot >= CELESTIAL_EPSILON ? 1.0 : -1.0;
			Vector3 rot = Vector3(hookRot.x, yAngle, hookRot.z);
			trackedObject->SetRotation(rot);

		}

		dist /= 2;
		boardPos = hookPos + dist;
		trackedObject->SetPosition(boardPos);
		trackedObject->UpdateMatrix();

	}
}

void GameBoardHandler::Update(unsigned int time)
{

	UpdateMessages(time);

	if (localGameBoard != nullptr)
	{

		localGameBoard->FillInstanceBuffer(trackedObject);

	}
}

GameBoardHandler::~GameBoardHandler()
{

}

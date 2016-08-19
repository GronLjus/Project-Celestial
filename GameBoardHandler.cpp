#include "stdafx.h"
#include "GameBoardHandler.h"
#include "GameRouteObject.h"

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
	routing = new RoutingManager();
	hookColl = new unsigned int[128];

}

void GameBoardHandler::Init(CelestialSlicedList<BaseObject*>* gameObjects)
{

	this->gameObjects = gameObjects;
	routing->Init(gameObjects);

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

	addScriptParamNum(script, objectId, time);
	addScriptParamNum(script, mouseX, time);
	addScriptParamNum(script, mouseY, time);

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

void GameBoardHandler::addScriptParamNum(unsigned int script, unsigned int num, unsigned int time)
{

	unsigned char tempBuff[]{ script >> 0, script >> 8, script >> 16, script >> 24,
		num >> 0, num >> 8, num >> 16, num >> 24
	};

	messageBuffer[this->currentMessage].timeSent = time;
	messageBuffer[this->currentMessage].destination = MessageSource_CELSCRIPT;
	messageBuffer[this->currentMessage].type = MessageType_SCRIPT;
	messageBuffer[this->currentMessage].mess = ScriptMess_ADDPARNUM;
	messageBuffer[this->currentMessage].read = false;
	messageBuffer[this->currentMessage].SetParams(tempBuff, 0, 8);
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
	addScriptParamNum(script, dragStatus, time);
	unsigned char tempBuff[]{ script >> 0, script >> 8, script >> 16, script >> 24,
		dragStatus >> 0, dragStatus >> 8, dragStatus >> 16, dragStatus >> 24
	};

	runScript(script, time);

}

void GameBoardHandler::triggerMouseScript(unsigned int script, unsigned int objectId, unsigned int time, int mouseX, int mouseY, short delta)
{

	sendCommonScriptParams(script, objectId, time, mouseX, mouseY);
	addScriptParamNum(script, delta, time);
	runScript(script, time);

}

void GameBoardHandler::triggerNodeScript(unsigned int script, unsigned int obj, unsigned int goalNode, unsigned int currentNode, unsigned int time)
{

	addScriptParamNum(script, obj, time);
	addScriptParamNum(script, currentNode, time);
	addScriptParamNum(script, goalNode, time);
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

			localGameBoard->GetDrawingBoard()->AddMesh((CelMesh*)(gameObjects->GetValue(param1)));

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
			unsigned int mouseX = trackedObject != nullptr && hookObject ?
				trackedMX :
				currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);
			unsigned int mouseY = trackedObject != nullptr && hookObject ?
				trackedMY : 
				currentMessage->params[4] | ((int)currentMessage->params[5] << 8) | ((int)currentMessage->params[6] << 16) | ((int)currentMessage->params[7] << 24);
			
			Vector3 direction = getMouseWorldLine(mouseX, mouseY);
			ScriptableObject* obj = getMouseObject(direction);
			unsigned int script = 0;

			if (obj == trackedObject && hookObject)
			{

				unsigned char tempBuff[]{ mouseX >> 0, mouseX >> 8, mouseY >> 0, mouseY >> 8 };
				messageBuffer[this->currentMessage].timeSent = time;
				messageBuffer[this->currentMessage].destination = MessageSource_SYSTEM;
				messageBuffer[this->currentMessage].type = MessageType_SYSTEM;
				messageBuffer[this->currentMessage].mess = SystemMess_MOVECURSOR;
				messageBuffer[this->currentMessage].read = false;
				messageBuffer[this->currentMessage].SetParams(tempBuff, 0, 4);
				outQueue->PushMessage(&messageBuffer[this->currentMessage]);
				this->currentMessage = (this->currentMessage + 1) % outMessages;

			}

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
			handleMouseMovement(mouseX, mouseY,time);

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

				gB->GetDrawingBoard()->AddMesh((CelMesh*)(gameObjects->GetValue(gB->GetBoardObject()->GetMeshId())));
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
		else if (currentMessage->mess == GameBoardMess_ADDROUTE && localGameBoard != nullptr)
		{

			Vector3 pos;
			memcpy(&(pos.x), &(currentMessage->params[0]), 4);
			memcpy(&(pos.y), &(currentMessage->params[4]), 4);
			memcpy(&(pos.z), &(currentMessage->params[8]), 4);

			float width = 0.49f;

			pos.x = floor(pos.x) + 0.5f;
			pos.z = floor(pos.z) + 0.5f;

			BoundingSphere sphere = BoundingSphere(pos.x, pos.y, pos.z, width);
			unsigned int amount = 0;

			unsigned int* collided = localGameBoard->GetCollidedObject(&sphere, GameObjectType_ROUTE, amount);
			unsigned int retVal = routing->AddNode(pos,collided,amount);

			if (currentMessage->source == MessageSource_CELSCRIPT && currentMessage->returnParam > 0)
			{

				messageBuffer[this->currentMessage].timeSent = time;
				messageBuffer[this->currentMessage].destination = MessageSource_CELSCRIPT;
				messageBuffer[this->currentMessage].type = MessageType_SCRIPT;
				messageBuffer[this->currentMessage].mess = ScriptMess_RESUME;
				unsigned char tempBuff[]{ currentMessage->senderId >> 0, currentMessage->senderId >> 8, currentMessage->senderId >> 16, currentMessage->senderId >> 24,
					currentMessage->returnParam >> 0, currentMessage->returnParam >> 8, currentMessage->returnParam >> 16, currentMessage->returnParam >> 24,
					retVal >> 0, retVal >> 8, retVal >> 16, retVal >> 24
				};
				messageBuffer[this->currentMessage].SetParams(tempBuff, 0, 12);
				messageBuffer[this->currentMessage].read = false;
				outQueue->PushMessage(&messageBuffer[this->currentMessage]);
				this->currentMessage = (this->currentMessage + 1) % outMessages;

			}
		}
		else if (currentMessage->mess == GameBoardMess_TRAVEL)
		{

			unsigned int param2 = currentMessage->params[4] |
				((int)currentMessage->params[5] << 8) |
				((int)currentMessage->params[6] << 16) |
				((int)currentMessage->params[7] << 24);

			if (param2 > 0)
			{

				routing->Travel((GameTravelObject*)gameObjects->GetValue(param1), param2-1, time);

			}
		}
		else if (currentMessage->mess == GameBoardMess_SPAWN)
		{

			unsigned int param2 = currentMessage->params[4] |
				((int)currentMessage->params[5] << 8) |
				((int)currentMessage->params[6] << 16) |
				((int)currentMessage->params[7] << 24);

			if (param2 > 0)
			{

				routing->Spawn((GameTravelObject*)gameObjects->GetValue(param1), param2-1);

			}
		}
		else if (currentMessage->mess == GameBoardMess_SPLITOBJECT)
		{

			RouteNodeObject* node = routing->GetNode(param1 -1);
			float width;
			memcpy(&width, &currentMessage->params[4], 4);
			BoundingSphere bs = BoundingSphere(node->GetPosition().x,
				node->GetPosition().y,
				node->GetPosition().z,
				width / 2);

			unsigned int amounts;
			unsigned int* objects = localGameBoard->GetCollidedObject(&bs, GameObjectType_ROUTE, amounts);

			for (unsigned int i = 0; i < amounts; i++)
			{

				splitObject((GameObject*)gameObjects->GetValue(objects[i]), node->GetPosition(), width, time);

			}
		}

		currentMessage->read = true;

		if (currentMessage->killWhenDone)
		{

			delete currentMessage;

		}

		currentMessage = inQueue->PopMessage();

	}
}

void GameBoardHandler::splitObject(GameObject* object, Vector3 position, float width, unsigned int time)
{

	float objectLength = object->GetScale().z;
	position.y = object->GetPosition().y;

	Vector3 endPoint1 = object->GetPosition() + (object->GetDirection() * (objectLength / 2.0f));
	Vector3 endPoint2 = object->GetPosition() - (object->GetDirection() * (objectLength / 2.0f));
	float halfWidthSquared = pow(width / 2.0f, 2);
	float dist1 = VectorDot(endPoint1 - position);
	float dist2 = VectorDot(endPoint2 - position);

	float dir1 = VectorDot(endPoint1 - position, object->GetDirection());
	float dir2 = VectorDot(endPoint2 - position, object->GetDirection());

	if (dir1 < 0.0f ||
		dir2 > 0.0f ||
		dist1 <= halfWidthSquared+CELESTIAL_EPSILON ||
		dist2 <= halfWidthSquared+CELESTIAL_EPSILON)
	{

		return;

	}

	Vector3 startPoint1 = position + (object->GetDirection() * (width / 2.0f));
	Vector3 startPoint2 = position - (object->GetDirection() * (width / 2.0f));

	Vector3 diff1 = startPoint1 - endPoint1;
	float length1 = sqrt(VectorDot(diff1));
	float length2 = objectLength - length1 - width;

	Vector3 pos1 = startPoint1 + (object->GetDirection() * (length1 / 2.0f));
	Vector3 pos2 = startPoint2 - (object->GetDirection() * (length2 / 2.0f));

	Vector3 newScale = object->GetScale();
	newScale.z = length1;

	object->SetScale(newScale);
	object->SetPosition(pos1);
	object->UpdateMatrix();

	messageBuffer[this->currentMessage].timeSent = time;
	messageBuffer[this->currentMessage].destination = MessageSource_RESOURCES;
	messageBuffer[this->currentMessage].type = MessageType_RESOURCES;
	messageBuffer[this->currentMessage].mess = ResourceMess_LOADCOPYOBJECTAT;
	unsigned char tempBuff[]{ object->GetId() >> 0, object->GetId() >> 8, object->GetId() >> 16, object->GetId() >> 24,
		0,0,0,0,
		0,0,0,0,
		0,0,0,0,
		0,0,0,0
	};

	memcpy(&tempBuff[4], &length2, 4);
	memcpy(&tempBuff[8], &(pos2.x), 4);
	memcpy(&tempBuff[12], &(pos2.y), 4);
	memcpy(&tempBuff[16], &(pos2.z), 4);

	messageBuffer[this->currentMessage].SetParams(tempBuff, 0, 20);
	messageBuffer[this->currentMessage].read = false;
	outQueue->PushMessage(&messageBuffer[this->currentMessage]);
	this->currentMessage = (this->currentMessage + 1) % outMessages;

}

void GameBoardHandler::transformHookedObject(Vector3 mousePos)
{

	Vector3 scale = trackedObject->GetScale();
	Vector3 dist = mousePos - hookPos;
	float distL = sqrt(VectorDot(dist, dist));
	Vector3 normalDist = dist / distL;
	scale.z = hookScale.z + distL;
	trackedObject->SetScale(scale);

	if (scale.z - hookScale.z > 0)
	{

		Vector3 forward = Vector3(0.0f, 0.0f, 1.0f);
		float dotProduct = VectorDot(forward, normalDist);
		float yAngle = acos(dotProduct);
		float sideDot = VectorDot(Vector3(1.0f, 0.0f, 0.0f), normalDist);
		yAngle *= sideDot >= CELESTIAL_EPSILON ? 1.0 : -1.0;
		Vector3 rot = Vector3(hookRot.x, yAngle, hookRot.z);
		trackedObject->SetRotation(rot);

	}

	dist /= 2;
	trackedObject->SetPosition(hookPos + dist);
	trackedObject->UpdateMatrix();

}

void GameBoardHandler::handleMouseMovement(unsigned int mouseX, unsigned int mouseY, unsigned int time)
{

	Vector3 direction = getMouseWorldLine(mouseX, mouseY);
	float halfHeight = trackedObject->GetScale().y / 2;
	localGameBoard->GetBoardPosition(localGameBoard->GetCam()->GetPosition(), direction, boardPos, halfHeight);
	Vector3 scale = trackedObject->GetScale();
	unsigned char tempBuff[12];
	Vector3 mouse = boardPos;
	Message mess;

	if (abs(floor(boardPos.x) - mouseCell.x) > CELESTIAL_EPSILON ||
		abs(floor(boardPos.z) - mouseCell.y) > CELESTIAL_EPSILON ||
		resetMouse)
	{

		resetMouse = false;
		mouseCell.x = floor(boardPos.x);
		mouseCell.y = floor(boardPos.z);

		if (!hookObject)
		{

			boardPos.x = floor(boardPos.x) + 0.5f;
			boardPos.z = floor(boardPos.z) + 0.5f;
			trackedObject->SetPosition(boardPos);
			trackedObject->UpdateMatrix();

			hookTargets = 0;
			unsigned int* collidedObjects = localGameBoard->GetCollidedObject(trackedObject, hookTargets);

			if (hookTargets > 0)
			{

				memcpy(hookColl, collidedObjects, min(hookTargets, 128) * sizeof(unsigned int));
				PositionableObject* lastObj = nullptr;

				for (unsigned int i = 0; i < hookTargets; i++)
				{
				
					PositionableObject* obj = (PositionableObject*)gameObjects->GetValue(collidedObjects[i]);
					Vector3 newPos = lastObj == nullptr ?
						obj->GetObjectCenterLine(boardPos) :
						obj->GetObjectCenterLine(boardPos, lastObj->GetDirection());

					boardPos.x = newPos.x;
					boardPos.y = newPos.y;
					boardPos.z = newPos.z;
					lastObj = obj;

				}
			}

			trackedObject->SetPosition(boardPos);
			trackedObject->UpdateMatrix();

		}
		else
		{

			boardPos.x = floor(boardPos.x) + 0.5f;
			boardPos.z = floor(boardPos.z) + 0.5f;
			transformHookedObject(boardPos);

			unsigned int amountOfCollidedObjects = 0;
			unsigned int* collidedObjects = localGameBoard->GetCollidedObject(trackedObject, amountOfCollidedObjects);

			if (amountOfCollidedObjects > hookTargets)
			{

				Vector3 distVect1 = boardPos - hookPos;
				float currentDist = VectorDot(distVect1, distVect1);

				float smallest = -1.0f;
				Vector3 smallestVect = boardPos;
				float cellToMouse = 0.0f;

				for (unsigned int i = 0; i < amountOfCollidedObjects; i++)
				{

					bool safe = true;

					for (unsigned int k = 0; k < hookTargets && safe; k++)
					{

						safe = !(collidedObjects[i] == hookColl[k]);

					}

					if (safe)
					{
					
						PositionableObject* obj = (PositionableObject*)gameObjects->GetValue(collidedObjects[i]);

						Vector3 newPos = obj->GetObjectCenterLine(boardPos, distVect1);
						Vector3 distVect = newPos - hookPos;
						float dist = VectorDot(distVect, distVect);

						if (dist != dist)
						{

							int brk = 0;

						}

						if (smallest < 0 || dist < smallest)
						{

							smallest = dist;
							smallestVect = newPos;
							Vector3 mToC = mouse - newPos;
							cellToMouse = VectorDot(mToC);

						}
					}
				}

				boardPos = smallestVect;
				transformHookedObject(boardPos);

				CameraObject* cam = localGameBoard->GetCam();
				Vector3 onScreen = VectorTransform(boardPos,
					cam->GetView()->GetViewProjection(cam->GetFlip()));
				trackedMX = ((onScreen.x + 1) / 2) * cam->GetView()->GetPort().width;
				trackedMY = ((-onScreen.y + 1) / 2) * cam->GetView()->GetPort().height;

				if (smallest < currentDist && cellToMouse >= 2.0f)
				{


					unsigned char tempBuff[]{ trackedMX >> 0, trackedMX >> 8, trackedMY >> 0, trackedMY >> 8 };
					messageBuffer[this->currentMessage].timeSent = time;
					messageBuffer[this->currentMessage].destination = MessageSource_SYSTEM;
					messageBuffer[this->currentMessage].type = MessageType_SYSTEM;
					messageBuffer[this->currentMessage].mess = SystemMess_MOVECURSOR;
					messageBuffer[this->currentMessage].read = false;
					messageBuffer[this->currentMessage].SetParams(tempBuff, 0, 4);
					outQueue->PushMessage(&messageBuffer[this->currentMessage]);
					this->currentMessage = (this->currentMessage + 1) % outMessages;

					mouseCell.x = floor(boardPos.x);
					mouseCell.y = floor(boardPos.z);

				}
			}
			else
			{

				CameraObject* cam = localGameBoard->GetCam();
				Vector3 onScreen = VectorTransform(boardPos,
					cam->GetView()->GetViewProjection(cam->GetFlip()));
				trackedMX = ((onScreen.x + 1) / 2) * cam->GetView()->GetPort().width;
				trackedMY = ((-onScreen.y + 1) / 2) * cam->GetView()->GetPort().height;

			}
		}
	}
}

void GameBoardHandler::Update(unsigned int time)
{

	UpdateMessages(time);

	if (localGameBoard != nullptr)
	{

		localGameBoard->FillInstanceBuffer(trackedObject);

	}

	unsigned int scripts = 0;
	unsigned int* scriptsToRun = routing->Update(time, scripts);

	for (unsigned int i = 0; i < scripts; i++)
	{

		GameTravelObject* obj = (GameTravelObject*)gameObjects->GetValue(scriptsToRun[i]);
		triggerNodeScript(
			obj->GetTravelNodeScript()-1,
			scriptsToRun[i],
			obj->GetFinalGoalNode() + 1,
			obj->GetNode() + 1,
			time);

	}
}

GameBoardHandler::~GameBoardHandler()
{

	delete routing;
	delete[] hookColl;

}

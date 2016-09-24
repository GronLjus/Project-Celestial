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
	hookObject = false;
	hookColl = new unsigned int[128];
	mH = new MouseHandler();

}

void GameBoardHandler::Init(CelestialSlicedList<BaseObject*>* gameObjects)
{

	this->gameObjects = gameObjects;
	mH->Init(gameObjects,
		messageBuffer,
		currentMessage,
		outQueue,
		outMessages);

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
		0.0f, 0.0f,0.0f,0.0f
	};
	memcpy(&(tempBuff[4]), &(num), 4);

	messageBuffer[this->currentMessage].timeSent = time;
	messageBuffer[this->currentMessage].destination = MessageSource_CELSCRIPT;
	messageBuffer[this->currentMessage].type = MessageType_SCRIPT;
	messageBuffer[this->currentMessage].mess = ScriptMess_ADDPARNUM;
	messageBuffer[this->currentMessage].read = false;
	messageBuffer[this->currentMessage].SetParams(tempBuff, 0, 8);
	outQueue->PushMessage(&messageBuffer[this->currentMessage]);
	this->currentMessage = (this->currentMessage + 1) % outMessages;

}

void GameBoardHandler::triggerNodeScript(unsigned int script, unsigned int obj, unsigned int goalNode, unsigned int currentNode, unsigned int time)
{

	addScriptParamNum(script, obj, time);
	addScriptParamNum(script, currentNode, time);
	addScriptParamNum(script, goalNode, time);
	runScript(script, time);

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

			mH->Click(currentMessage->params[0], time, trackedObject);

		}
		else if (currentMessage->mess == GameBoardMess_WHEELOBJECT &&
			localGameBoard != nullptr && localGameBoard->GetCam() != nullptr)
		{

			short wheelDelta = currentMessage->params[0] | ((int)currentMessage->params[1] << 8);
			mH->Wheel(time, wheelDelta, trackedObject);

		}
		else if (currentMessage->mess == GameBoardMess_STARTDRAGGING &&
			localGameBoard != nullptr && localGameBoard->GetCam() != nullptr)
		{

			mH->StartDrag(currentMessage->params[0], time, trackedObject);

		}
		else if (currentMessage->mess == GameBoardMess_DRAGOBJECT &&
			localGameBoard != nullptr && localGameBoard->GetCam() != nullptr)
		{

			mH->Drag(time);

		}
		else if (currentMessage->mess == GameBoardMess_STOPDRAGGING &&
			localGameBoard != nullptr && localGameBoard->GetCam() != nullptr)
		{
			
			mH->StopDrag(time);

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
		else if (currentMessage->mess == GameBoardMess_MOUSEMOVE &&
			localGameBoard != nullptr && localGameBoard->GetCam() != nullptr)
		{

			unsigned int mouseX = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);
			unsigned int mouseY = currentMessage->params[4] | ((int)currentMessage->params[5] << 8) | ((int)currentMessage->params[6] << 16) | ((int)currentMessage->params[7] << 24);

			mH->MoveMouse(vectorUI2(mouseX, mouseY));

			if (trackedObject != nullptr)
			{

				mH->SetWorldMouse(handleTracked(time));

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
			mH->SetBoard(gB);
			routing = gB->GetRoutingManager();
			routing->Init(gameObjects);

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

			float width = 0.98f;

			BoundingSphere sphere = BoundingSphere(pos.x, pos.y, pos.z, width / 2.0f);
			unsigned int amount = 0;

			unsigned int* collided = localGameBoard->GetCollidedObject(&sphere, GameObjectType_ROUTE, amount);
			unsigned int retVal = localGameBoard->AddRouteNode(pos, width, collided, amount);

			for (unsigned int i = 0; i < amount; i++)
			{

				GameRouteObject* routeObj = (GameRouteObject*)gameObjects->GetValue(collided[i]);

				if (routeObj->GetMiddleNode() != 0)
				{

					RouteNodeObject* middleNode = routing->GetNode(routeObj->GetMiddleNode() - 1);
					splitObject(routeObj, middleNode->GetPosition(), width, time);

				}
			}

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

PositionableObject* GameBoardHandler::snapMouse(unsigned int amounts, unsigned int* collidedObjects, PositionableObject* lastObj, Vector3 &worldMouse)
{

	for (unsigned int i = 0; i < amounts; i++)
	{

		PositionableObject* obj = (PositionableObject*)gameObjects->GetValue(collidedObjects[i]);
		Vector3 newPos = lastObj == nullptr ?
			obj->GetObjectCenterLine(worldMouse) :
			obj->GetObjectCenterLine(worldMouse, lastObj->GetDirection());

		worldMouse.x = newPos.x;
		worldMouse.y = newPos.y;
		worldMouse.z = newPos.z;
		lastObj = obj;

	}

	return lastObj;

}

Vector3 GameBoardHandler::handleTracked(unsigned int time)
{


	Vector3 worldMouse = mH->GetWorldMouse();
	worldMouse.y += trackedObject->GetScale().y / 2;
	Message mess;

	if (abs(floor(worldMouse.x) - mouseCell.x) > CELESTIAL_EPSILON ||
		abs(floor(worldMouse.z) - mouseCell.y) > CELESTIAL_EPSILON ||
		resetMouse)
	{

		resetMouse = false;
		mouseCell.x = floor(worldMouse.x);
		mouseCell.y = floor(worldMouse.z);
		worldMouse.x = floor(worldMouse.x) + 0.5f;
		worldMouse.z = floor(worldMouse.z) + 0.5f;
		BoundingSphere cursor = BoundingSphere(worldMouse.x,
			trackedObject->GetScale().y / 2,
			worldMouse.z, 
			0.5f);

		if (!hookObject)
		{

			Vector3 midPos = worldMouse;
			unsigned int amounts = 0;
			unsigned int* collidedObjects = localGameBoard->GetCollidedObject(&cursor, GameObjectType_ROUTE, amounts);
			PositionableObject* lastObject = nullptr;
			hookTargets = 0;

			if (amounts > 0)
			{

				lastObject = snapMouse(amounts, collidedObjects, lastObject, worldMouse);
				memcpy(hookColl, collidedObjects, min(amounts, 128) * sizeof(unsigned int));
				hookTargets += amounts;

			}

			amounts = 0;
			collidedObjects = localGameBoard->GetCollidedObject(&cursor, GameObjectType_SCENERY, amounts);

			if (amounts > 0)
			{

				snapMouse(amounts, collidedObjects, lastObject, worldMouse);
				memcpy(&hookColl[hookTargets], collidedObjects, min(amounts, 128 - hookTargets) * sizeof(unsigned int));
				hookTargets += amounts;

			}

			trackedObject->SetPosition(worldMouse);
			trackedObject->UpdateMatrix();

		}
		else
		{

			transformHookedObject(worldMouse);

			unsigned int amountOfCollidedObjects = 0;
			unsigned int* collidedObjects = localGameBoard->GetCollidedObject(trackedObject, amountOfCollidedObjects);

			if (amountOfCollidedObjects > hookTargets)
			{

				Vector3 distVect1 = worldMouse - hookPos;
				float currentDist = VectorDot(distVect1, distVect1);

				float smallest = -1.0f;
				Vector3 smallestVect = worldMouse;
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

						Vector3 newPos = obj->GetObjectCenterLine(worldMouse, distVect1);
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
							Vector3 mToC = worldMouse - newPos;
							cellToMouse = VectorDot(mToC);

						}
					}
				}

				worldMouse = smallestVect;
				transformHookedObject(worldMouse);

				CameraObject* cam = localGameBoard->GetCam();
				Vector3 onScreen = VectorTransform(worldMouse,
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

					mouseCell.x = floor(worldMouse.x);
					mouseCell.y = floor(worldMouse.z);

				}
			}
			else
			{

				CameraObject* cam = localGameBoard->GetCam();
				Vector3 onScreen = VectorTransform(worldMouse,
					cam->GetView()->GetViewProjection(cam->GetFlip()));
				trackedMX = ((onScreen.x + 1) / 2) * cam->GetView()->GetPort().width;
				trackedMY = ((-onScreen.y + 1) / 2) * cam->GetView()->GetPort().height;

			}
		}

		mouseCache = worldMouse;

	}
	else
	{

		worldMouse = mouseCache;

	}

	return worldMouse;

}

void GameBoardHandler::Update(unsigned int time)
{

	UpdateMessages(time);

	if (localGameBoard != nullptr)
	{


		localGameBoard->FillInstanceBuffer(trackedObject);

		unsigned int scripts = 0;
		unsigned int* scriptsToRun = routing->Update(time, scripts);

		for (unsigned int i = 0; i < scripts; i++)
		{

			GameTravelObject* obj = (GameTravelObject*)gameObjects->GetValue(scriptsToRun[i]);
			triggerNodeScript(
				obj->GetTravelNodeScript() - 1,
				scriptsToRun[i],
				obj->GetFinalGoalNode() + 1,
				obj->GetNode() + 1,
				time);

		}
	}
}

GameBoardHandler::~GameBoardHandler()
{

	delete mH;
	delete[] hookColl;

}

#include "stdafx.h"
#include "GameBoardHandler.h"
#include "GameRouteObject.h"
#include "GameGridObject.h"

#include <algorithm>

using namespace Entities;
using namespace Resources;
using namespace CrossHandlers;

GameBoardHandler::GameBoardHandler() : IHandleMessages(200, MessageSource_ENTITIES)
{

	snap = true;
	localGameBoard = nullptr;
	trackedObject = nullptr;
	filter = MessageType_ENTITIES;
	hookStatus = 0;
	mH = new MouseHandler();
	lastTime = 0;

	hookOccupied = false;
	trackLock = false;
	mouseCache = Vector3(0.0f, 0.0f, 0.0f);

	trackedRotation = Vector3();

}

void GameBoardHandler::Init(CelestialSlicedList<BaseObject*>* gameObjects)
{

	this->gameObjects = gameObjects;
	mH->Init(gameObjects,
		mBuffer);

}

void GameBoardHandler::runScript(unsigned int script, unsigned int time)
{

	Message* msg = mBuffer->GetCurrentMess();

	unsigned char tempBuff[]{ script >> 0, script >> 8, script >> 16, script >> 24 };
	msg->timeSent = time;
	msg->destination = MessageSource_CELSCRIPT;
	msg->type = MessageType_SCRIPT;
	msg->mess = ScriptMess_RUN;
	msg->read = false;
	msg->SetParams(tempBuff, 0, 4);

	mBuffer->PushMessageOut();

}

void GameBoardHandler::addScriptParamNum(unsigned int script, unsigned int num, unsigned int time)
{

	Message* msg = mBuffer->GetCurrentMess();

	unsigned char tempBuff[]{ script >> 0, script >> 8, script >> 16, script >> 24,
		0.0f, 0.0f,0.0f,0.0f
	};
	memcpy(&(tempBuff[4]), &(num), 4);

	msg->timeSent = time;
	msg->destination = MessageSource_CELSCRIPT;
	msg->type = MessageType_SCRIPT;
	msg->mess = ScriptMess_ADDPARNUM;
	msg->read = false;
	msg->SetParams(tempBuff, 0, 8);

	mBuffer->PushMessageOut();

}

void GameBoardHandler::addScriptParamFloat(unsigned int script, float num, unsigned int time)
{

	Message* msg = mBuffer->GetCurrentMess();

	unsigned char tempBuff[]{ script >> 0, script >> 8, script >> 16, script >> 24,
		0.0f, 0.0f,0.0f,0.0f
	};
	memcpy(&(tempBuff[4]), &(num), 4);

	msg->timeSent = time;
	msg->destination = MessageSource_CELSCRIPT;
	msg->type = MessageType_SCRIPT;
	msg->mess = ScriptMess_ADDPARFLOAT;
	msg->read = false;
	msg->SetParams(tempBuff, 0, 8);

	mBuffer->PushMessageOut();

}

void GameBoardHandler::triggerNodeScript(unsigned int script, unsigned int obj, unsigned int goalNode, unsigned int currentNode, unsigned int time)
{

	addScriptParamNum(script, obj, time);
	addScriptParamNum(script, currentNode, time);
	addScriptParamNum(script, goalNode, time);
	runScript(script, time);

}

void GameBoardHandler::triggerSplitScript(unsigned int script, unsigned int obj, Vector3 pos, unsigned int time)
{

	addScriptParamNum(script, obj, time);

	addScriptParamFloat(script, pos.x, time);
	addScriptParamFloat(script, pos.y, time);
	addScriptParamFloat(script, pos.z, time);

	runScript(script, time);

}

void  GameBoardHandler::handleInput(CrossHandlers::Message* currentMessage, unsigned int time)
{

	if (currentMessage->mess == GameBoardMess_CLICKOBJECT)
	{

		mH->Click(currentMessage->params[0], time, trackedObject, hookOccupied);

	}
	else if (currentMessage->mess == GameBoardMess_WHEELOBJECT)
	{

		short wheelDelta = currentMessage->params[0] | ((int)currentMessage->params[1] << 8);
		mH->Wheel(time, wheelDelta, trackedObject);

	}
	else if (currentMessage->mess == GameBoardMess_STARTDRAGGING)
	{

		mH->StartDrag(currentMessage->params[0], time, trackedObject);

	}
	else if (currentMessage->mess == GameBoardMess_DRAGOBJECT)
	{

		mH->Drag(time);

	}
	else if (currentMessage->mess == GameBoardMess_STOPDRAGGING)
	{

		mH->StopDrag(time);

	}
	else if (currentMessage->mess == GameBoardMess_MOUSEUD)
	{

		mH->UpDown(currentMessage->params[0], currentMessage->params[1] == 1 ,time);

	}
	else if (currentMessage->mess == GameBoardMess_HANDLEKEY)
	{

		mH->HandleKey(Input::CelestialKeyCategories(currentMessage->params[0]), currentMessage->params[1], currentMessage->params[2] == 1 , time, trackedObject);

	}
	else if (currentMessage->mess == GameBoardMess_MOUSEMOVE)
	{

		unsigned int mouseX = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);
		unsigned int mouseY = currentMessage->params[4] | ((int)currentMessage->params[5] << 8) | ((int)currentMessage->params[6] << 16) | ((int)currentMessage->params[7] << 24);

		mH->MoveMouse(vectorUI2(mouseX, mouseY), time);

		if (trackedObject != nullptr)
		{

			mH->SetWorldMouse(handleTracked(time));

		}
	}
}

void GameBoardHandler::lockObjectNodes(unsigned int object)
{

	GameObject* obj = (GameObject*)gameObjects->GetValue(object);

	unsigned int amount;
	unsigned int* collidedObjects = localGameBoard->GetCollidedObject(obj, GameObjectType_GRIDROUTE, amount);

	for (unsigned int i = 0; i < amount; i++)
	{

		GameGridObject* grid = (GameGridObject*)gameObjects->GetValue(collidedObjects[i]);

		for (unsigned int x = 0; x < grid->GetNodeWidth(); x++)
		{

			for (unsigned int y = 0; y < grid->GetNodeHeigth(); y++)
			{

				RouteNodeObject* node = localGameBoard->GetRoutingManager()->GetNode(grid->GetNode(x, y));
				float rad = (float)node->GetWidth() / 2.0f;

				BoundingSphere bs = BoundingSphere(node->GetPosition().x, node->GetPosition().y, node->GetPosition().z, rad);

				if (obj->GetBox()->IntersectsBounding(&bs, Shape_SPHERE) != Intersection_BACK)
				{

					node->Lock(object);
					localGameBoard->GetRoutingManager()->ClearRoads(node);

				}
			}
		}
	}
}

void GameBoardHandler::UpdateMessages(unsigned int time, unsigned int clock)
{

	Message* currentMessage = inQueue->PopMessage();

	while (currentMessage->type != MessageType_NA)
	{

		unsigned int outId = 0;
		unsigned int param1 = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);

		if (currentMessage->mess == GameBoardMess_ADDMESH && localGameBoard != nullptr)
		{

			localGameBoard->GetDrawingBoard()->AddMesh((CelMesh*)(gameObjects->GetValue(param1)));

			Message* msg = mBuffer->GetCurrentMess();

			msg->timeSent = time;
			msg->destination = MessageSource_GRAPHICS;
			msg->type = MessageType_GRAPHICS;
			msg->mess = GraphicMess_UPDATEGAMEBOARDBUFFERS;
			msg->read = false;

			mBuffer->PushMessageOut();

		}
		else if (currentMessage->mess == GameBoardMess_ADDOBJECT && localGameBoard != nullptr)
		{

			localGameBoard->AddObject((GameObject*)(gameObjects->GetValue(param1)));

		}
		else if (currentMessage->mess == GameBoardMess_GLUEOBJECT && localGameBoard != nullptr)
		{

			unsigned int param2;
			Vector3 pos;

			memcpy(&param2, &currentMessage->params[4], sizeof(unsigned int));
			memcpy(&pos.x, &currentMessage->params[8], sizeof(float));
			memcpy(&pos.y, &currentMessage->params[12], sizeof(float));
			memcpy(&pos.z, &currentMessage->params[16], sizeof(float));

			GameObject* parent = (GameObject*)(gameObjects->GetValue(param1));
			GameObject* child = (GameObject*)(gameObjects->GetValue(param2));

			localGameBoard->AddObject(child);
			parent->AddSubObject(child, pos);

		}
		else if (currentMessage->mess == GameBoardMess_CLICKOBJECT ||
			currentMessage->mess == GameBoardMess_WHEELOBJECT ||
			currentMessage->mess == GameBoardMess_STARTDRAGGING ||
			currentMessage->mess == GameBoardMess_DRAGOBJECT ||
			currentMessage->mess == GameBoardMess_STOPDRAGGING ||
			currentMessage->mess == GameBoardMess_MOUSEMOVE ||
			currentMessage->mess == GameBoardMess_MOUSEUD ||
			currentMessage->mess == GameBoardMess_HANDLEKEY)
		{

			handleInput(currentMessage, time);

		}
		else if (currentMessage->mess == GameBoardMess_LOCKONGRID && localGameBoard != nullptr)
		{

			lockObjectNodes(param1);

		}

		else if (currentMessage->mess == GameBoardMess_ORBITOBJECT &&
			localGameBoard != nullptr && localGameBoard->GetCam() != nullptr)
		{

			unsigned int obj = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);
			PositionableObject* object = (PositionableObject*)gameObjects->GetValue(obj);

			Vector3 objectDirection = object->GetDirection();
			Vector3 gameBoardPos(object->GetPosition() + objectDirection*10);

			bool result = localGameBoard->GetBoardPosition(object->GetPosition(), objectDirection, gameBoardPos);

			Message* msg = mBuffer->GetCurrentMess();

			msg->timeSent = time;
			msg->destination = MessageSource_OBJECT;
			msg->type = MessageType_OBJECT;
			msg->mess = ObjectMess_ORBIT;
			msg->read = false;

			unsigned char tempBuff[16];
			memcpy(tempBuff, &gameBoardPos.x, 4);
			memcpy(&tempBuff[4], &gameBoardPos.y, 4);
			memcpy(&tempBuff[8], &gameBoardPos.z, 4);
			memcpy(&tempBuff[12], &currentMessage->params[4], 4);
			msg->SetParams(tempBuff, 0, 16);

			object->Update(msg);

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
			trackLock = false;
			hookStatus = 0;

		}
		else if ((currentMessage->mess == GameBoardMess_HOOKTRACK || currentMessage->mess == GameBoardMess_UNHOOKTRACK ) 
			&& trackedObject != nullptr)
		{

			if (currentMessage->mess == GameBoardMess_HOOKTRACK)
			{

				hookStatus++;
				hookPos = trackedObject->GetPosition();
				hookScale = trackedObject->GetScale();
				hookRot = trackedObject->GetRotation();

				hookSide = VectorCross(trackedObject->GetDirection(), Vector3(0.0f, 1.0f, 0.0f));
				hookSide /= sqrt(VectorDot(hookSide));

				if (hookStatus == 2)
				{

					hookPos -= hookSide*(trackedObject->GetScale().x / 2);
				}
			}
			else
			{

				hookStatus = 0;
				trackedObject->SetPosition(hookPos);
				trackedObject->SetScale(hookScale);
				trackedObject->SetRotation(hookRot);

			}
		}
		else if (currentMessage->mess == GameBoardMess_SETCAM && localGameBoard != nullptr)
		{

			localGameBoard->SetCamera((CameraObject*)(gameObjects->GetValue(param1)));

			Message* msg = mBuffer->GetCurrentMess();

			msg->timeSent = time;
			msg->destination = MessageSource_GRAPHICS;
			msg->type = MessageType_GRAPHICS;
			msg->mess = GraphicMess_SETCAMERA;
			msg->read = false;
			msg->SetParams(currentMessage->params, 0, 4);

			mBuffer->PushMessageOut();

		}
		else if (currentMessage->mess == GameBoardMess_SETUI)
		{

			GUILayout* layout = (GUILayout*)gameObjects->GetValue(param1);
			mH->SetLayout(layout);

		}
		else if (currentMessage->mess == GameBoardMess_SETGAMEBOARD)
		{

			GameBoard* gB = (GameBoard*)(gameObjects->GetValue(param1));
			mH->SetBoard(gB);
			routing = gB->GetRoutingManager();
			routing->Init(gameObjects);

			Message* msg = mBuffer->GetCurrentMess();

			msg->timeSent = time;
			msg->destination = MessageSource_GRAPHICS;
			msg->type = MessageType_GRAPHICS;
			msg->mess = GraphicMess_SETGAMEBOARD;
			msg->read = false;
			msg->SetParams(currentMessage->params, 0, 4);

			mBuffer->PushMessageOut();

			if (gB->GetBoardObject() != nullptr)
			{

				gB->GetDrawingBoard()->AddMesh((CelMesh*)(gameObjects->GetValue(gB->GetBoardObject()->GetMeshId())));

				msg = mBuffer->GetCurrentMess();

				msg->timeSent = time;
				msg->destination = MessageSource_GRAPHICS;
				msg->type = MessageType_GRAPHICS;
				msg->mess = GraphicMess_UPDATEGAMEBOARDBUFFERS;
				msg->read = false;

				mBuffer->PushMessageOut();

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

			unsigned int* collided = localGameBoard->GetCollidedObject(&sphere, GameObjectType_ROUTE | GameObjectType_GRIDROUTE, amount);
			outId = localGameBoard->AddRouteNode(pos, width, collided, amount) + 1;

			for (unsigned int i = 0; i < amount; i++)
			{

				GameObject* obj = (GameObject*)gameObjects->GetValue(collided[i]);

				if(obj->GetType() == GameObjectType_ROUTE)
				{

					GameRouteObject* routeObj = (GameRouteObject*)gameObjects->GetValue(collided[i]);

					if (routeObj->GetMiddleNode() != 0)
					{

						RouteNodeObject* middleNode = routing->GetNode(routeObj->GetMiddleNode() - 1);

						if (routeObj->GetSplitNodeScript() != 0)
						{

							triggerSplitScript(routeObj->GetSplitNodeScript() - 1, routeObj->GetId(), middleNode->GetPosition(), time);

						}
						else
						{

							splitObject(routeObj, middleNode->GetPosition(), 0.0f, time);

						}
					}
				}
			}
		}
		else if (currentMessage->mess == GameBoardMess_GETROUTE && localGameBoard != nullptr)
		{

			Vector3 pos;
			memcpy(&(pos.x), &(currentMessage->params[0]), 4);
			memcpy(&(pos.y), &(currentMessage->params[4]), 4);
			memcpy(&(pos.z), &(currentMessage->params[8]), 4);

			float width = 0.98f;

			BoundingSphere sphere = BoundingSphere(pos.x, pos.y, pos.z, width / 2.0f);
			unsigned int amount = 0;

			unsigned int* collided = localGameBoard->GetCollidedObject(&sphere, GameObjectType_ROUTE | GameObjectType_GRIDROUTE, amount);
			outId = localGameBoard->GetRoutingManager()->FindNode(pos, collided, amount) + 1;

		}
		else if (currentMessage->mess == GameBoardMess_TRAVEL)
		{

			unsigned int param2 = currentMessage->params[4] |
				((int)currentMessage->params[5] << 8) |
				((int)currentMessage->params[6] << 16) |
				((int)currentMessage->params[7] << 24);

			if (param2 > 0)
			{

				routing->Travel((GameTravelObject*)gameObjects->GetValue(param1), param2-1, clock);

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
		else if (currentMessage->mess == GameBoardMess_ADDROUTEGROUP)
		{

			outId = routing->AddGroup((GameObject*)gameObjects->GetValue(param1)) + 1;

		}
		else if (currentMessage->mess == GameBoardMess_SPLITOBJECT)
		{

			GameRouteObject* routeObj = (GameRouteObject*)gameObjects->GetValue(param1);

			Vector3 pos;
			memcpy(&pos.x, &currentMessage->params[4], 4);
			memcpy(&pos.y, &currentMessage->params[8], 4);
			memcpy(&pos.z, &currentMessage->params[12], 4);

			float width;
			memcpy(&width, &currentMessage->params[16], 4);

			splitObject(routeObj, pos, width, time);


		}
		else if (currentMessage->mess == GameBoardMess_GETPARENT)
		{

			BaseObject* obj = gameObjects->GetValue(param1);
			outId = obj->GetParentId() + 1;

		}
		else if (currentMessage->mess == GameBoardMess_POPULATEGRID && localGameBoard != nullptr)
		{

			unsigned int gridObject;
			float nodeWidth;

			memcpy(&gridObject, currentMessage->params, sizeof(unsigned int));
			memcpy(&nodeWidth, &currentMessage->params[4], sizeof(float));

			GameGridObject* grid = (GameGridObject*)gameObjects->GetValue(gridObject);

			localGameBoard->GetRoutingManager()->PopulateGrid(grid, nodeWidth);

		}
		else if (currentMessage->mess == GameBoardMess_LOCKTRACK)
		{

			trackLock = !trackLock;

		}
		else if (currentMessage->mess == GameBoardMess_ROUTEOBJECT && localGameBoard != nullptr)
		{

			unsigned int param2;
			memcpy(&param2, &currentMessage->params[4], sizeof(unsigned int));
			localGameBoard->GetRoutingManager()->AddRoad(param1 - 1, param2 - 1);

		}
		else
		{

			int dbg = 0;

		}

		if (currentMessage->source == MessageSource_CELSCRIPT && currentMessage->returnParam > 0 &&
			outId > 0)
		{

			outId--;

			Message* msg = mBuffer->GetCurrentMess();

			msg->timeSent = time;
			msg->destination = MessageSource_CELSCRIPT;
			msg->type = MessageType_SCRIPT;
			msg->mess = ScriptMess_RESUME;
			unsigned char tempBuff[]{ currentMessage->senderId >> 0, currentMessage->senderId >> 8, currentMessage->senderId >> 16, currentMessage->senderId >> 24,
				currentMessage->returnParam >> 0, currentMessage->returnParam >> 8, currentMessage->returnParam >> 16, currentMessage->returnParam >> 24,
				outId >> 0, outId >> 8, outId >> 16, outId >> 24,
				currentMessage->returnMess >> 0, currentMessage->returnMess >> 8, currentMessage->returnMess >> 16, currentMessage->returnMess >> 24
			};
			msg->SetParams(tempBuff, 0, 16);
			msg->read = false;

			mBuffer->PushMessageOut();

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

	Message* msg = mBuffer->GetCurrentMess();

	msg->timeSent = time;
	msg->destination = MessageSource_RESOURCES;
	msg->type = MessageType_RESOURCES;
	msg->mess = ResourceMess_LOADCOPYOBJECTAT;
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

	msg->SetParams(tempBuff, 0, 20);
	msg->read = false;

	mBuffer->PushMessageOut();

}

void GameBoardHandler::transformHookedObject(Vector3 mousePos)
{

	if (trackLock)
	{

		mousePos = trackedObject->GetObjectCenterLineNoCap(mousePos);

	}

	Vector3 scale = trackedObject->GetScale();
	mousePos.y = hookPos.y;
	Vector3 dist = mousePos - hookPos;
	float distL = sqrt(VectorDot(dist, dist));
	Vector3 normalDist = dist / distL;
	scale.z = hookScale.z + distL;
	trackedObject->SetScale(scale);

	if (scale.z - hookScale.z > 0 && !trackLock)
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

Vector3 GameBoardHandler::getClosestPositionOnObj(GameGridObject* grid, Vector3 mouse, Vector3 origin)
{

	BoundingBox* box = grid->GetBox();

	if (hookStatus == 1 && !trackLock)
	{

		Vector3 dir = mouse - origin;
		dir /= sqrt(VectorDot(dir));

		float dist = 0.0f;
		Intersection inter = box->IntersectsLine(origin, dir, dist);

		dist = sqrt(dist);

		mouse = origin + (dir * dist);

	}

	unsigned int closestObj = 0;
	unsigned int closestNode = 0;
	float closest = -1.0f;
	Vector3 closestPos;

	CelestialMath::vectorUI2 selectedNode;
	bool corner = false;

	for (unsigned int x = 0; x < grid->GetNodeWidth(); x++)
	{

		for (unsigned int y = 0; y < grid->GetNodeHeigth(); y++)
		{

			RouteNodeObject* node = localGameBoard->GetRoutingManager()->GetNode(grid->GetNode(x, y));

			if (node->GetLock() > 0 ||
				(x == 0 || x == grid->GetNodeWidth() - 1) ||
				(y == 0 || y == grid->GetNodeHeigth() - 1))//Filter out all nodes not on the edge of the grid or are locked to an object
			{

				//Check the distance from the mouseposition to the node
				float dist = VectorDot(node->GetPosition() - mouse);

				if (dist < closest || closest < 0)
				{

					closest = dist;
					closestNode = node->GetId();
					closestPos = node->GetPosition();

					selectedNode.x = x;
					selectedNode.y = y;

					trackedCorner = (x == 0 || x == grid->GetNodeWidth() - 1) &&
						(y == 0 || y == grid->GetNodeHeigth() - 1);

					closestObj = node->GetLock();

				}
			}
		}
	}

	if (closest >= 0)//We found a node
	{

		trackedRotation = grid->GetOrthogonalNormalOfNode(selectedNode.x, selectedNode.y);

		if (trackedCorner)
		{

			Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
			//Since up and tracedRotation are orthogonal and normalized vectors the cross products is also normalized
			trackedSideRotation = VectorCross(trackedRotation, up);

			if ((selectedNode.x == 0 && selectedNode.y == 0) ||
				(selectedNode.x == grid->GetNodeWidth() - 1 && selectedNode.y == grid->GetNodeHeigth() - 1))
			{

				trackedSideRotation = -trackedSideRotation;

			}

			if (closestObj != 0)
			{

				PositionableObject* obj = (PositionableObject*)gameObjects->GetValue(closestObj);
				Vector3 objDir = obj->GetDirection();

				if (abs(VectorDot(objDir, trackedRotation)) <= CELESTIAL_EPSILON)//The rotation is orthogonal to the object
				{

					trackedRotation = trackedSideRotation;

				}
				else
				{

					trackedSideRotation = trackedRotation;

				}
			}
		}
	}

	return closestPos;

}

Vector3 GameBoardHandler::snapMouse(unsigned int amounts, unsigned int* collidedObjects, PositionableObject* lastObj, Vector3 worldMouse)
{

	for (unsigned int i = 0; i < amounts; i++)
	{

		GameObject* obj = (GameObject*)gameObjects->GetValue(collidedObjects[i]);

		if (obj->GetType() == GameObjectType_ROUTE)
		{

			Vector3 newPos = lastObj == nullptr ?
				obj->GetObjectCenterLine(worldMouse) :
				obj->GetObjectCenterLine(worldMouse, lastObj->GetDirection());

			worldMouse.x = newPos.x;
			worldMouse.y = newPos.y;
			worldMouse.z = newPos.z;
			lastObj = obj;

		}
		else if (obj->GetType() == GameObjectType_GRIDROUTE)
		{

			GameGridObject* gridObj = (GameGridObject*)obj;

			worldMouse = getClosestPositionOnObj(gridObj, worldMouse, hookPos);
			worldMouse.y -= gridObj->GetScale().y / 2;

		}
	}

	return worldMouse;

}

bool GameBoardHandler::filterObjects(unsigned int* objects, unsigned int amounts, std::string filters)
{

	if( filters == " ")
	{
		
		return true;

	}

	bool objectGood = false;
	bool skip = false;
	bool accept = false;

	std::string tempStr = "";

	for (unsigned int i = 0; i < filters.size() && !objectGood; i++)
	{

		bool breakInput = filters[i] == ',' || filters[i] == '&';

		if (!breakInput)
		{

			tempStr += filters[i];

		}

		if(breakInput || i == filters.size()-1)
		{

			bool hasObject = false;

			if (!skip)
			{

				if (tempStr == "none")
				{

					hasObject = amounts == 0;

				}
				else
				{

					for (unsigned int i = 0; i < amounts && !hasObject; i++)
					{

						GameObject* obj = (GameObject*)gameObjects->GetValue(objects[i]);
						hasObject = obj->GetObjectName() == tempStr;

					}
				}

				accept = hasObject;
				skip = !hasObject;

			}

			tempStr = "";

			if (filters[i] == ',')
			{

				skip = false;
				
				if (accept)
				{

					return true;

				}
			}
		}
	}

	return accept;

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

		trackedObject->SetRotationLimit(Vector3(CELESTIAL_PI, CELESTIAL_PI, CELESTIAL_PI), 
			Vector3(-CELESTIAL_PI, -CELESTIAL_PI, -CELESTIAL_PI), true);
		trackedCorner = false;
		trackedRotation = Vector3(0.0f, 0.0f, 0.0f);

		resetMouse = false;
		worldMouse.x = floor(worldMouse.x) + 0.5f;
		worldMouse.z = floor(worldMouse.z) + 0.5f;
		BoundingSphere cursor = BoundingSphere(worldMouse.x,
			trackedObject->GetScale().y / 2,
			worldMouse.z, 
			0.51f);

		unsigned int cursorAmounts = 0;

		if (snap && hookStatus < 2)
		{

			Vector3 midPos = worldMouse;
			unsigned int* collidedObjects = localGameBoard->GetCollidedObject(&cursor, GameObjectType_ROUTE | GameObjectType_GRIDROUTE | GameObjectType_SCENERY, cursorAmounts);
			PositionableObject* lastObject = nullptr;
			worldMouse = snapMouse(cursorAmounts, collidedObjects, nullptr, worldMouse);

		}

		if (hookStatus == 0)
		{

			trackedObject->SetPosition(worldMouse);

			if (VectorDot(trackedRotation) != 0)
			{

				Vector3 forwardPoint = worldMouse + trackedRotation;
				Vector3 right = Vector3(1.0f, 0.0f, 0.0f);
				Vector3 forward = Vector3(0.0f, 0.0f, 1.0f);
				float fDot = VectorDot(trackedRotation, forward);
				float fAngle = acos(fDot) * (VectorDot(right, trackedRotation) >= 0 ? 1 : -1);

				if (trackedCorner)//The object is at a corner of a grid, we need extra calculations to snap the rotation
				{

					float sDot = VectorDot(trackedSideRotation, forward);
					float sAngle = acos(sDot) * (VectorDot(right, trackedSideRotation) >= 0 ? 1 : -1) ;

					Vector3 uprLimit = Vector3(0.0f, fAngle > sAngle ? fAngle : sAngle, 0);
					Vector3 lwrLimit = Vector3(0.0f, fAngle < sAngle ? fAngle : sAngle, 0);

					trackedObject->SetRotationLimit(uprLimit, lwrLimit, false);

					Vector3 objectDir = trackedObject->GetDirection();
					objectDir /= sqrt(VectorDot(objectDir));

					float forwardRot = VectorDot(trackedRotation, objectDir);
					float sideRot = VectorDot(trackedSideRotation, objectDir);


					//Only spin the object if needed
					if (abs(forwardRot - 1) > CELESTIAL_EPSILON &&
						abs(sideRot - 1) > CELESTIAL_EPSILON)
					{

						trackedObject->Point(forwardPoint);//This method updates the matrix

					}
					else
					{

						trackedObject->UpdateMatrix();

					}
				}
				else if (VectorDot(trackedRotation) > 0)
				{

					trackedObject->SetRotationLimit(Vector3(0.0f, fAngle, 0.0f), Vector3(0.0f, fAngle, 0.0f), false);

					trackedObject->Point(forwardPoint);//This method updates the matrix

				}
				else
				{

					trackedObject->UpdateMatrix();

				}
			}
			else
			{

				trackedObject->UpdateMatrix();

			}

			hookTargets = cursorAmounts;

		}
		else if (hookStatus == 1)//Fill one dimension
		{

			transformHookedObject(worldMouse);

		}
		else if (hookStatus == 2)//Fill the second dimension
		{


			Vector3 scale = trackedObject->GetScale();
			Vector3 pos = hookPos;

			Vector3 projectedLine = Vector3(0.0f, 0.0f, 0.0f);

			Vector3 line = worldMouse - hookPos;
			float vd1 = VectorDot(line, hookSide);
			float vd2 = VectorDot(hookSide, hookSide);

			if (vd2 > CELESTIAL_EPSILON)
			{

				projectedLine = hookSide * (vd1 / vd2);

			}

			float dist = sqrt(VectorDot(projectedLine)) + (hookScale.x / 2);
			float projectedDot = VectorDot(projectedLine, hookSide);

			dist *= projectedDot < 0 ? -1 : 1;

			if (dist < 0)
			{

				pos += hookSide*hookScale.x;

			}

			pos += hookSide * (dist / 2.0f);
			scale.x = max(abs(dist), hookScale.x);

			trackedObject->SetPosition(pos);
			trackedObject->SetScale(scale);
			trackedObject->UpdateMatrix();

		}

		unsigned int trackedObjCollidedAmounts = 0;
		unsigned int* trackedCollided = localGameBoard->GetCollidedObject(trackedObject, GameObjectType_ROUTE | GameObjectType_GRIDROUTE | GameObjectType_SCENERY, trackedObjCollidedAmounts);
		hookOccupied = false;

		if (hookStatus > 0)
		{

			//The tracked object intersects with more then the end and start
			hookOccupied = trackedObjCollidedAmounts > (cursorAmounts + hookTargets);

		}

		if (!hookOccupied)
		{

			hookOccupied = !(filterObjects(trackedCollided, trackedObjCollidedAmounts, trackedObject->GetCollisionFilter()));

		}

		mouseCache = worldMouse;
		mouseCell.x = floor(worldMouse.x);
		mouseCell.y = floor(worldMouse.z);

	}
	else
	{

		worldMouse = mouseCache;

	}

	return worldMouse;

}

void GameBoardHandler::Update(unsigned int time, unsigned int clock, float timeDiff)
{

	mH->Update(time);

	UpdateMessages(time, clock);

	if (localGameBoard != nullptr)
	{

		localGameBoard->FillInstanceBuffer(trackedObject);

		if (lastTime == time)
		{

			return;

		}

		lastTime = time;
		unsigned int scripts = 0;
		unsigned int* scriptsToRun = routing->Update(clock, scripts, timeDiff);

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

}

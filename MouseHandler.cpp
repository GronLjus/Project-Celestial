#include "stdafx.h"
#include "MouseHandler.h"

using namespace Entities;
using namespace CrossHandlers;
using namespace Resources;
using namespace CelestialMath;

MouseHandler::MouseHandler()
{

	hoverObject = nullptr;
	screenMouse = vectorUI2(0, 0);
	worldMouse = Vector3(0.0f, 0.0f, 0.0f);

}

void MouseHandler::Init(CelestialSlicedList<BaseObject*>* gameObjects,
	Message* mBuffer,
	unsigned int &currentMessage,
	MessageQueue* outQueue,
	unsigned int maxMess)
{


	this->gameObjects = gameObjects;
	this->mBuffer = mBuffer;
	this->outQueue = outQueue;
	this->currentMessage = currentMessage;
	outMessages = maxMess;

}

void MouseHandler::MoveMouse(vectorUI2 newMouse)
{

	if (newMouse.x != screenMouse.x ||
		newMouse.y != screenMouse.y)
	{

		screenMouse = newMouse; 
		Vector3 direction = getMouseWorldLine(screenMouse);
		board->GetBoardPosition(board->GetCam()->GetPosition(), direction, worldMouse);
		vectorUI2 cells(floor(worldMouse.x), floor(worldMouse.z));

	}
}

void MouseHandler::runScript(unsigned int script, unsigned int time)
{

	unsigned char tempBuff[]{ script >> 0, script >> 8, script >> 16, script >> 24 };
	mBuffer[this->currentMessage].timeSent = time;
	mBuffer[this->currentMessage].destination = MessageSource_CELSCRIPT;
	mBuffer[this->currentMessage].type = MessageType_SCRIPT;
	mBuffer[this->currentMessage].mess = ScriptMess_RUN;
	mBuffer[this->currentMessage].read = false;
	mBuffer[this->currentMessage].SetParams(tempBuff, 0, 4);
	outQueue->PushMessage(&mBuffer[this->currentMessage]);
	this->currentMessage = (this->currentMessage + 1) % outMessages;

}

void MouseHandler::addScriptParamNum(unsigned int script, unsigned int num, unsigned int time)
{

	unsigned char tempBuff[]{ script >> 0, script >> 8, script >> 16, script >> 24,
		0.0f, 0.0f,0.0f,0.0f
	};
	memcpy(&(tempBuff[4]), &(num), 4);

	mBuffer[this->currentMessage].timeSent = time;
	mBuffer[this->currentMessage].destination = MessageSource_CELSCRIPT;
	mBuffer[this->currentMessage].type = MessageType_SCRIPT;
	mBuffer[this->currentMessage].mess = ScriptMess_ADDPARNUM;
	mBuffer[this->currentMessage].read = false;
	mBuffer[this->currentMessage].SetParams(tempBuff, 0, 8);
	outQueue->PushMessage(&mBuffer[this->currentMessage]);
	this->currentMessage = (this->currentMessage + 1) % outMessages;

}

void MouseHandler::addScriptParamFloat(unsigned int script, float num, unsigned int time)
{

	unsigned char tempBuff[]{ script >> 0, script >> 8, script >> 16, script >> 24,
		0.0f, 0.0f,0.0f,0.0f
	};
	memcpy(&(tempBuff[4]), &(num), 4);

	mBuffer[this->currentMessage].timeSent = time;
	mBuffer[this->currentMessage].destination = MessageSource_CELSCRIPT;
	mBuffer[this->currentMessage].type = MessageType_SCRIPT;
	mBuffer[this->currentMessage].mess = ScriptMess_ADDPARFLOAT;
	mBuffer[this->currentMessage].read = false;
	mBuffer[this->currentMessage].SetParams(tempBuff, 0, 8);
	outQueue->PushMessage(&mBuffer[this->currentMessage]);
	this->currentMessage = (this->currentMessage + 1) % outMessages;

}

void MouseHandler::sendCommonScriptParams(unsigned int script, unsigned int objectId, unsigned int time)
{

	addScriptParamNum(script, objectId, time);
	addScriptParamNum(script, screenMouse.x, time);
	addScriptParamNum(script, screenMouse.y, time);

	addScriptParamFloat(script, worldMouse.x, time);
	addScriptParamFloat(script, worldMouse.y, time);
	addScriptParamFloat(script, worldMouse.z, time);

}

Vector3 MouseHandler::getMouseWorldLine(vectorUI2 mouse) const
{

	CameraObject* cam = board->GetCam();
	float screenX = ((float)mouse.x) / (float)(cam->GetView()->GetPort().width);
	float screenY = ((float)mouse.y) / (float)(cam->GetView()->GetPort().height);
	screenX = 2 * screenX - 1;
	screenY = 2 * screenY - 1;
	screenY = -screenY;
	float screenZ = 0;// cam->GetView()->GetPort().minDepth;

	Vector3 worldPoint = VectorTransform(Vector3(screenX, screenY, screenZ), cam->GetView()->GetInverseViewProjection(cam->GetFlip()));
	Vector3 direction = worldPoint - cam->GetPosition();
	direction /= sqrt(abs(VectorDot(direction, direction)));
	return direction;

}

ScriptableObject* MouseHandler::getMouseObject(Vector3 direction) const
{

	CameraObject* cam = board->GetCam();
	float minDist = 0;
	unsigned int selectedObject = board->GetClosestObject(cam->GetPosition(), direction, minDist);
	ScriptableObject* obj = nullptr;

	if (minDist != 0)
	{

		obj = (ScriptableObject*)gameObjects->GetValue(selectedObject);

	}

	return obj;

}

unsigned int MouseHandler::getClickScript(char button, Resources::ScriptableObject* obj) const
{

	return button == 0 ? obj->GetLeftClickScript() :
		button == 1 ? obj->GetMiddleClickScript() :
		obj->GetRightClickScript();

}

unsigned int MouseHandler::getDragScript(char button, Resources::ScriptableObject* obj) const
{

	return button == 0 ? obj->GetLeftDragScript() :
		button == 1 ? obj->GetMiddleDragScript() :
		obj->GetRightDragScript();

}

void MouseHandler::Click(char button, unsigned int time, PositionableObject* trackedObject)
{

	ScriptableObject* clickOb = trackedObject;

	if (trackedObject == nullptr)
	{

		clickOb = hoverObject;

	}

	if (clickOb == nullptr)
	{

		clickOb = board;

	}

	unsigned int script = getClickScript(button, clickOb);

	if (script == 0 && clickOb != board)
	{

		script = getClickScript(button, board);

	}

	if (script != 0)
	{

		script--;
		sendCommonScriptParams(script, clickOb->GetId(), time);
		runScript(script, time);

	}
}

void MouseHandler::Wheel(unsigned int time, short delta, Resources::PositionableObject* trackedObject)
{

	ScriptableObject* wheelObj = trackedObject;

	if (trackedObject == nullptr)
	{

		wheelObj = hoverObject;

	}

	if (wheelObj == nullptr)
	{

		wheelObj = board;

	}

	unsigned int script = wheelObj->GetWheelScript();

	if (script == 0 && wheelObj != board)
	{

		script = board->GetWheelScript();

	}

	if (script != 0)
	{

		script--;
		sendCommonScriptParams(script, wheelObj->GetId(), time);
		addScriptParamNum(script, delta, time);
		runScript(script, time);

	}
}

void MouseHandler::StartDrag(char button, unsigned int time, Resources::PositionableObject* trackedObject)
{

	ScriptableObject* dragObj = trackedObject;

	if (dragObj == nullptr)
	{

		dragObj = hoverObject;

	}

	if (dragObj == nullptr)
	{

		dragObj = board;

	}

	unsigned int script = getDragScript(button, dragObj);

	if (script == 0 && dragObj != board)
	{

		script = getDragScript(button, board);

	}

	if (script != 0)
	{

		dragScript = script;
		dragId = dragObj->GetId();
		script--;
		sendCommonScriptParams(script, dragId, time);
		addScriptParamNum(script, 0, time);
		runScript(script, time);

	}
}

void MouseHandler::Drag(unsigned int time)
{

	if (dragScript > 0)
	{

		sendCommonScriptParams(dragScript - 1, dragId, time);
		addScriptParamNum(dragScript - 1, 1, time);
		runScript(dragScript - 1, time);

	}
}

void MouseHandler::StopDrag(unsigned int time)
{

	if (dragScript > 0)
	{

		sendCommonScriptParams(dragScript - 1, dragId, time);
		addScriptParamNum(dragScript - 1, 2, time);
		runScript(dragScript - 1, time);
		dragScript = 0;

	}
}

Vector3 MouseHandler::GetWorldMouse() const
{

	return worldMouse;

}

void MouseHandler::SetWorldMouse(Vector3 pos)
{

	this->worldMouse = pos;

}

void MouseHandler::SetBoard(Resources::GameBoard* board)
{

	this->board = board;

}

void MouseHandler::Update(unsigned int time)
{

	Vector3 direction = getMouseWorldLine(screenMouse);
	ScriptableObject* obj = getMouseObject(direction);

}
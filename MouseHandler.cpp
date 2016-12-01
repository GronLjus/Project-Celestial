#include "stdafx.h"
#include "MouseHandler.h"
#include "InputHandler.h"

using namespace Entities;
using namespace CrossHandlers;
using namespace Resources;
using namespace CelestialMath;

MouseHandler::MouseHandler()
{

	dragScript = 0;
	board = nullptr;
	hoverObject = nullptr;
	lastTarget = nullptr;
	gameObject = nullptr;
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

unsigned int MouseHandler::getKeyScript(Input::CelestialKeyCategories cat, unsigned key, Resources::ScriptableObject* obj) const
{

	if (obj == nullptr)
	{

		return 0;

	}

	return obj->GetKeyScript(cat, key);

}

unsigned int MouseHandler::getClickScript(char button, ScriptableObject* obj) const
{

	return obj == nullptr ? 0 :
		button == 0 ? obj->GetLeftClickScript() :
		button == 1 ? obj->GetMiddleClickScript() :
		obj->GetRightClickScript();

}

unsigned int MouseHandler::getWheelScript(ScriptableObject* obj) const
{

	return obj == nullptr ? 0 : obj->GetWheelScript();

}

unsigned int MouseHandler::getDragScript(char button, ScriptableObject* obj) const
{

	return obj == nullptr ? 0 : 
		button == 0 ? obj->GetLeftDragScript() :
		button == 1 ? obj->GetMiddleDragScript() :
		obj->GetRightDragScript();

}

ScreenTarget* MouseHandler::getScreenTarget(vectorUI2 mouse, GUILayout* base)
{

	ScreenTarget* firstTarget = nullptr;

	if (base != nullptr)
	{

		unsigned char topLayer = 0;

		for (unsigned int i = 0; i < base->GetChildren() && firstTarget == nullptr; i++)
		{

			GUIObject* object = base->GetChild(i);

			if (object != nullptr)
			{

				ScreenTarget* target = object->GetScreenTarget();

				if (target != nullptr)
				{

					float l = target->GetPosition().x;
					float r = target->GetPosition().x + target->GetScale().x;
					float t = target->GetPosition().y;
					float b = target->GetPosition().y + target->GetScale().y;

					if ((l <= mouse.x && r >= mouse.x &&
						t <= mouse.y && b >= mouse.y) &&
						target->GetLayer() > topLayer)
					{

						topLayer = target->GetLayer();
						firstTarget = target;

					}
				}

				if (firstTarget != nullptr && object->GetType() == GUIObjects_LAYOUT)
				{

					firstTarget = getScreenTarget(mouse, (GUILayout*)object);

				}
			}
		}
	}

	if (firstTarget != nullptr && !firstTarget->IsVisible())
	{

		firstTarget = nullptr;

	}

	if (firstTarget == nullptr
		&& base != nullptr &&
		base->GetScreenTarget() != nullptr &&
		!base->GetScreenTarget()->IsLocked())
	{

		return base->GetScreenTarget();

	}

	return firstTarget;

}

void MouseHandler::setCursor(Logic::CursorCode code, unsigned int time)
{

	unsigned char cd = code;
	mBuffer[this->currentMessage].timeSent = time;
	mBuffer[this->currentMessage].destination = MessageSource_SYSTEM;
	mBuffer[this->currentMessage].type = MessageType_SYSTEM;
	mBuffer[this->currentMessage].mess = SystemMess_SETCURSOR;
	mBuffer[this->currentMessage].read = false;
	mBuffer[this->currentMessage].SetParams(&cd, 0, 8);
	outQueue->PushMessage(&mBuffer[this->currentMessage]);
	this->currentMessage = (this->currentMessage + 1) % outMessages;

}

void MouseHandler::HandleKey(Input::CelestialKeyCategories cat, unsigned char key, bool up, unsigned int time, PositionableObject* trackedObject)
{

	ScriptableObject* clickObj = trackedObject;

	if (getKeyScript(cat, key, clickObj) == 0)
	{

		clickObj = board;

	}

	unsigned int script = getKeyScript(cat, key, clickObj);

	if (script != 0)
	{

		script--;

		addScriptParamNum(script, clickObj->GetTargetId(), time);
		addScriptParamNum(script, Input::ConvertCode(cat, key), time);
		addScriptParamNum(script, up ? 1 : 0, time);
		runScript(script, time);

	}
}

void MouseHandler::MoveMouse(vectorUI2 newMouse, unsigned int time)
{

	bool skip = false;

	if (dragScript == 0 && screenLayout != nullptr)
	{

		ScreenTarget* gui = getScreenTarget(newMouse, screenLayout);
		skip = gui != nullptr;

		if (gui == nullptr || gui->GetLeftClickScript() == 0)
		{

			setCursor(Logic::CursorCode_POINT, time);

		}

		if (gui != lastTarget && lastTarget != nullptr && lastTarget->IsHovering())
		{

			lastTarget->SetHovering(false);


			if (lastTarget->GetExitScript() != 0)
			{

				unsigned int script = lastTarget->GetExitScript() - 1;
				sendCommonScriptParams(script, lastTarget->GetTargetId(), time);
				runScript(script, time);

			}
		}

		if (gui != nullptr)
		{

			if (!gui->IsHovering() && gui->GetEnterScript() != 0)
			{

				unsigned int script = gui->GetEnterScript() - 1;
				sendCommonScriptParams(script, gui->GetTargetId(), time);
				runScript(script, time);

			}

			gui->SetHovering(true);

			if (gui->GetHoverScript() != 0)
			{

				unsigned int hoverScript = gui->GetHoverScript() - 1;
				sendCommonScriptParams(hoverScript, gui->GetTargetId(), time);
				runScript(hoverScript, time);

			}

			if (gui->GetLeftClickScript() != 0)
			{

				setCursor(Logic::CursorCode_HAND, time);

			}
		}

		hoverObject = gui;
		lastTarget = gui;

	}

	if (!skip && (newMouse.x != screenMouse.x ||
		newMouse.y != screenMouse.y))
	{

		hoverObject = nullptr;
		screenMouse = newMouse; 

		if (board != nullptr && board->GetCam() != nullptr)
		{

			Vector3 direction = getMouseWorldLine(screenMouse);
			board->GetBoardPosition(board->GetCam()->GetPosition(), direction, worldMouse);

		}
	}
}

void MouseHandler::UpDown(char button, bool up, unsigned int time)
{

	if (lastTarget != nullptr && lastTarget->GetUpdownScript() > 0)
	{

		unsigned int script = lastTarget->GetUpdownScript() - 1;

		if ((lastTarget->GetLeftClickScript() != 0 &&
			button == Input::InputHandler::keyCodeMouseL)
			|| (lastTarget->GetMiddleClickScript() != 0 &&
				button == Input::InputHandler::keyCodeMouseM)
			|| (lastTarget->GetRightClickScript() != 0 &&
				button == Input::InputHandler::keyCodeMouseR))
		{

			sendCommonScriptParams(script, lastTarget->GetTargetId(), time);
			addScriptParamNum(script, up ? 1 : 0, time);
			runScript(script, time);

		}
	}
}

void MouseHandler::Click(char button, unsigned int time, PositionableObject* trackedObject)
{

	ScriptableObject* clickOb = hoverObject;

	if (getClickScript(button, clickOb) == 0)
	{

		clickOb = trackedObject;

	}

	if (getClickScript(button, clickOb) == 0)
	{

		clickOb = gameObject;

	}

	if (getClickScript(button, clickOb) == 0)
	{

		clickOb = board;

	}

	unsigned int script = getClickScript(button, clickOb);

	if (script != 0)
	{

		script--;
		sendCommonScriptParams(script, clickOb->GetTargetId(), time);
		runScript(script, time);

	}
}

void MouseHandler::Wheel(unsigned int time, short delta, PositionableObject* trackedObject)
{

	ScriptableObject* wheelObj = hoverObject;

	if (getWheelScript(wheelObj) == 0)
	{

		wheelObj = trackedObject;

	}

	if (getWheelScript(wheelObj) == 0)
	{

		wheelObj = gameObject;

	}

	if (getWheelScript(wheelObj) == 0)
	{

		wheelObj = board;

	}

	unsigned int script = getWheelScript(wheelObj);

	if (script != 0)
	{

		script--;
		sendCommonScriptParams(script, wheelObj->GetTargetId(), time);
		addScriptParamNum(script, delta, time);
		runScript(script, time);

	}
}

void MouseHandler::StartDrag(char button, unsigned int time, Resources::PositionableObject* trackedObject)
{

	ScriptableObject* dragObj = hoverObject;


	if (getDragScript(button, dragObj) == 0)
	{

		dragObj = trackedObject;

	}

	if (getDragScript(button, dragObj) == 0)
	{

		dragObj = gameObject;

	}

	if (getDragScript(button, dragObj) == 0)
	{

		dragObj = board;

	}

	unsigned int script = getDragScript(button, dragObj);

	if (script != 0)
	{

		dragScript = script;
		dragId = dragObj->GetTargetId();
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

void MouseHandler::SetLayout(Resources::GUILayout* layout)
{

	this->screenLayout = layout;

}

void MouseHandler::Update(unsigned int time)
{

	if (board != nullptr && board->GetCam() != nullptr)
	{
	
		Vector3 direction = getMouseWorldLine(screenMouse);
		gameObject = getMouseObject(direction);

	}
}
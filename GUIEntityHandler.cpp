#include "stdafx.h"
#include "GUIEntityHandler.h"
#include "GUITextBox.h"
#include "InputHandler.h"
#include "CursorCodes.h"

using namespace Entities;
using namespace CrossHandlers;
using namespace Resources;

GUIEntityHandler::GUIEntityHandler() : IHandleMessages(20,MessageSource_GUIENTITIES)
{

	filter = MessageType_GUIENTITIES;
	focusedObject = nullptr;
	keyMessage.type = MessageType_OBJECT;
	keyMessage.mess = ObjectMess_HANDLEKEY;

	dragTarget = nullptr;
	lastTarget = nullptr;
	screenLayout = nullptr;
	
}

void GUIEntityHandler::Init(CelestialSlicedList<BaseObject*>* gameObjects)
{

	this->gameObjects = gameObjects;

}

void GUIEntityHandler::setCommonScriptParameters(unsigned int script, unsigned int time, unsigned int targetId, CelestialMath::vectorUI2 mouse)
{

	unsigned char tempBuff[]{ script >> 0, script >> 8, script >> 16, script >> 24,
		targetId >> 0, targetId >> 8, targetId >> 16, targetId >> 24
	};
	unsigned char tempBuff2[]{ script >> 0, script >> 8, script >> 16, script >> 24,
		mouse.x >> 0, mouse.x >> 8, mouse.x >> 16, mouse.x >> 24
	};
	unsigned char tempBuff3[]{ script >> 0, script >> 8, script >> 16, script >> 24,
		mouse.y >> 0, mouse.y >> 8, mouse.y >> 16, mouse.y >> 24
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

}

void GUIEntityHandler::runScript(unsigned int script, unsigned int time)
{

	unsigned char tempBuff[]{ script >> 0, script >> 8, script >> 16, script >> 24
	};

	messageBuffer[this->currentMessage].SetParams(tempBuff, 0, 4);
	messageBuffer[this->currentMessage].timeSent = time;
	messageBuffer[this->currentMessage].destination = MessageSource_CELSCRIPT;
	messageBuffer[this->currentMessage].type = MessageType_SCRIPT;
	messageBuffer[this->currentMessage].mess = ScriptMess_RUN;
	messageBuffer[this->currentMessage].read = false;
	outQueue->PushMessage(&messageBuffer[this->currentMessage]);
	this->currentMessage = (this->currentMessage + 1) % outMessages;

}

void GUIEntityHandler::triggerScript(unsigned int script, unsigned int time, unsigned int targetId, CelestialMath::vectorUI2 mouse, unsigned int dragStatus)
{

	setCommonScriptParameters(script, time, targetId, mouse);

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

void GUIEntityHandler::triggerScript(unsigned int script, unsigned int time, unsigned int targetId, CelestialMath::vectorUI2 mouse, short delta)
{

	setCommonScriptParameters(script, time, targetId, mouse);

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

void GUIEntityHandler::triggerScript(unsigned int script, unsigned int time, unsigned int targetId, CelestialMath::vectorUI2 mouse)
{

	setCommonScriptParameters(script, time, targetId, mouse);
	runScript(script, time);

}

ScreenTarget* GUIEntityHandler::getScreenTarget(unsigned int time, CelestialMath::vectorUI2 mouse, Resources::GUILayout* base)
{

	ScreenTarget* firstTarget = nullptr;

	if (base != nullptr)
	{

		unsigned char topLayer = 0;

		for (unsigned int i = 0; i < base->GetChildren() && firstTarget == nullptr; i++)
		{

			GUIObject* object = base->GetChild(i);

			if(object != nullptr)
			{

				ScreenTarget* target = object->GetScreenTarget();

				if(target != nullptr)
				{

					float l = target->GetPosition().x;
					float r = target->GetPosition().x + target->GetScale().x;
					float t = target->GetPosition().y;
					float b = target->GetPosition().y + target->GetScale().y;

					if ((l<= mouse.x && r >= mouse.x &&
							t <= mouse.y && b >= mouse.y) &&
						target->GetLayer() > topLayer)
					{

						topLayer = target->GetLayer();
						firstTarget = target;

					}
				}

				if (firstTarget != nullptr && object->GetType() == GUIObjects_LAYOUT)
				{

					firstTarget = getScreenTarget(time, mouse, (GUILayout*)object);

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

void GUIEntityHandler::handleMouseMovement(Resources::ScreenTarget* target, unsigned int time, CelestialMath::vectorUI2 mouse)
{

	if (!target->IsHovering() && target->GetEnterScript() != 0)
	{

		unsigned int script = target->GetEnterScript() - 1;
		triggerScript(script, time, target->GetTargetId(), mouse);

	}

	target->SetHovering(true);

	if (target->GetHoverScript() != 0)
	{

		unsigned int hoverScript = target->GetHoverScript() - 1;
		triggerScript(hoverScript, time, target->GetTargetId(), mouse);

	}

	if (target->GetLeftClickScript() != 0)
	{

		unsigned char code = Logic::CursorCode_HAND;
		messageBuffer[this->currentMessage].timeSent = time;
		messageBuffer[this->currentMessage].destination = MessageSource_SYSTEM;
		messageBuffer[this->currentMessage].type = MessageType_SYSTEM;
		messageBuffer[this->currentMessage].mess = SystemMess_SETCURSOR;
		messageBuffer[this->currentMessage].read = false;
		messageBuffer[this->currentMessage].SetParams(&code, 0, 8);
		outQueue->PushMessage(&messageBuffer[this->currentMessage]);
		this->currentMessage = (this->currentMessage + 1) % outMessages;
	}
}

void GUIEntityHandler::handleMouseClick(Resources::ScreenTarget* target, unsigned int time, CelestialMath::vectorUI2 mouse, unsigned char key)
{

	if (target->GetLeftClickScript() != 0 &&
		key == Input::InputHandler::keyCodeMouseL)
	{

		unsigned int lcScript = target->GetLeftClickScript() - 1;
		triggerScript(lcScript, time, target->GetTargetId(), mouse);

	}

	if (target->GetMiddleClickScript() != 0 &&
		key == Input::InputHandler::keyCodeMouseM)
	{

		unsigned int mcScript = target->GetMiddleClickScript() - 1;
		triggerScript(mcScript, time, target->GetTargetId(), mouse);

	}

	if (target->GetRightClickScript() != 0 &&
		key == Input::InputHandler::keyCodeMouseR)
	{

		unsigned int rcScript = target->GetRightClickScript() - 1;
		triggerScript(rcScript, time, target->GetTargetId(), mouse);

	}
}

void GUIEntityHandler::handleMouseWheel(Resources::ScreenTarget* target, unsigned int time, CelestialMath::vectorUI2 mouse, short delta)
{

	if (target->GetWheelScript() != 0)
	{

		unsigned int whScript = target->GetWheelScript() - 1;
		triggerScript(whScript, time, target->GetTargetId(), mouse,delta);

	}
}

void GUIEntityHandler::handleMouseDrag(Resources::ScreenTarget* target, unsigned int time, CelestialMath::vectorUI2 mouse, unsigned char key, unsigned int dragStatus)
{

	if (target->GetLeftDragScript() != 0 &&
		key == Input::InputHandler::keyCodeMouseL)
	{

		unsigned int lcScript = target->GetLeftDragScript() - 1;
		triggerScript(lcScript, time, target->GetTargetId(), mouse, dragStatus);

	}

	if (target->GetMiddleDragScript() != 0 &&
		key == Input::InputHandler::keyCodeMouseM)
	{

		unsigned int mcScript = target->GetMiddleDragScript() - 1;
		triggerScript(mcScript, time, target->GetTargetId(), mouse, dragStatus);

	}

	if (target->GetRightDragScript() != 0 &&
		key == Input::InputHandler::keyCodeMouseR)
	{

		unsigned int rcScript = target->GetRightDragScript() - 1;
		triggerScript(rcScript, time, target->GetTargetId(), mouse, dragStatus);

	}
}

void GUIEntityHandler::handleMouseUpdown(ScreenTarget* target, unsigned int time, CelestialMath::vectorUI2 mouse, unsigned char key, bool up)
{

	if (target->GetUpdownScript() > 0)
	{

		unsigned int script = target->GetUpdownScript() -1;

		if ((target->GetLeftClickScript() != 0 &&
			key == Input::InputHandler::keyCodeMouseL)
			|| (target->GetMiddleClickScript() != 0 &&
			key == Input::InputHandler::keyCodeMouseM)
			|| (target->GetRightClickScript() != 0 &&
			key == Input::InputHandler::keyCodeMouseR))
		{

			triggerScript(script, time, target->GetTargetId(), mouse, up ? (unsigned int)1 : (unsigned int)0);

		}
	}
}

void GUIEntityHandler::handleMouseAction(Message* currentMessage, unsigned int time, CelestialMath::vectorUI2 mouse)
{

	ScreenTarget* target = dragTarget;

	if (currentMessage->mess != GUIMess_DRAGOBJECT && currentMessage->mess != GUIMess_STOPDRAGGING)
	{

		target = getScreenTarget(time, mouse, screenLayout);

		if (target == nullptr || target->GetLeftClickScript() == 0)
		{

			unsigned char code = Logic::CursorCode_POINT;
			messageBuffer[this->currentMessage].timeSent = time;
			messageBuffer[this->currentMessage].destination = MessageSource_SYSTEM;
			messageBuffer[this->currentMessage].type = MessageType_SYSTEM;
			messageBuffer[this->currentMessage].mess = SystemMess_SETCURSOR;
			messageBuffer[this->currentMessage].read = false;
			messageBuffer[this->currentMessage].SetParams(&code, 0, 8);
			outQueue->PushMessage(&messageBuffer[this->currentMessage]);
			this->currentMessage = (this->currentMessage + 1) % outMessages;

		}

		if (target != lastTarget && lastTarget != nullptr && lastTarget->IsHovering())
		{

			target->SetHovering(false);


			if (target->GetExitScript() != 0)
			{

				unsigned int script = target->GetExitScript() - 1;
				triggerScript(script, time, target->GetTargetId(), mouse);

			}
		}
	}

	if (target != nullptr)
	{

		if (currentMessage->mess == GUIMess_MOVEMOUSE)
		{

			handleMouseMovement(target, time, mouse);

		}
		else if (currentMessage->mess == GUIMess_MOUSEDOWN)
		{
			Input::InputHandler::keyCode code = Input::InputHandler::keyCode(currentMessage->params[8]);
			handleMouseUpdown(target, time, mouse, code, false);
		}
		else if (currentMessage->mess == GUIMess_MOUSEUP)
		{
			Input::InputHandler::keyCode code = Input::InputHandler::keyCode(currentMessage->params[8]);
			handleMouseUpdown(target, time, mouse, code, true);
		}
		else if (currentMessage->mess == GUIMess_CLICKOBJECT)
		{

			Input::InputHandler::keyCode code = Input::InputHandler::keyCode(currentMessage->params[8]);
			handleMouseClick(target, time, mouse, code);

		}
		else if (currentMessage->mess == GUIMess_WHEELOBJECT)
		{

			short wheelDelta = currentMessage->params[8] | ((int)currentMessage->params[9] << 8);
			handleMouseWheel(target, time, mouse, wheelDelta);

		}
		else
		{

			Input::InputHandler::keyCode code = Input::InputHandler::keyCode(currentMessage->params[8]);

			if (currentMessage->mess == GUIMess_STARTDRAGGING)
			{

				dragTarget = target;

			}

			handleMouseDrag(target, time, mouse, code,
				currentMessage->mess == GUIMess_STARTDRAGGING ? 0 :
				currentMessage->mess == GUIMess_DRAGOBJECT ? 1 :
				2);

			if (currentMessage->mess == GUIMess_STOPDRAGGING)
			{

				dragTarget = nullptr;

			}
		}
	}
	else if (currentMessage->mess != GUIMess_MOVEMOUSE &&
		currentMessage->mess != GUIMess_MOUSEDOWN && 
		currentMessage->mess != GUIMess_MOUSEUP)
	{

		currentMessage->destination = MessageSource_ENTITIES;
		currentMessage->type = MessageType_ENTITIES;
		currentMessage->mess = currentMessage->mess == GUIMess_CLICKOBJECT ? GameBoardMess_CLICKOBJECT :
			currentMessage->mess == GUIMess_DRAGOBJECT ? GameBoardMess_DRAGOBJECT :
			currentMessage->mess == GUIMess_STARTDRAGGING ? GameBoardMess_STARTDRAGGING :
			currentMessage->mess == GUIMess_STOPDRAGGING ? GameBoardMess_STOPDRAGGING :
			GameBoardMess_WHEELOBJECT;

		outQueue->PushMessage(currentMessage);

	}
}

void GUIEntityHandler::Update(unsigned int time)
{

	keyMessage.params[0] = 0;
	keyMessage.read = true;
	keyMessage.timeSent = time;

	Message* currentMessage = inQueue->PopMessage();

	while (currentMessage->type != MessageType_NA)
	{
		
		if (currentMessage->mess == GUIMess_SETUI)
		{

			unsigned int param1 = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);
			this->screenLayout = (GUILayout*)(gameObjects->GetValue(param1));

			messageBuffer[this->currentMessage].timeSent = time;
			messageBuffer[this->currentMessage].destination = MessageSource_GRAPHICS;
			messageBuffer[this->currentMessage].type = MessageType_GRAPHICS;
			messageBuffer[this->currentMessage].mess = GraphicMess_SETUI;
			messageBuffer[this->currentMessage].SetParams(currentMessage->params, 0, 4);
			messageBuffer[this->currentMessage].read = false;
			outQueue->PushMessage(&messageBuffer[this->currentMessage]);
			this->currentMessage = (this->currentMessage + 1) % outMessages;

		}
		else if (currentMessage->mess == GUIMess_CLICKOBJECT ||
			currentMessage->mess == GUIMess_DRAGOBJECT ||
			currentMessage->mess == GUIMess_MOVEMOUSE ||
			currentMessage->mess == GUIMess_STARTDRAGGING ||
			currentMessage->mess == GUIMess_STOPDRAGGING ||
			currentMessage->mess == GUIMess_WHEELOBJECT ||
			currentMessage->mess == GUIMess_MOUSEDOWN||
			currentMessage->mess == GUIMess_MOUSEUP)
		{

			unsigned int mouseX = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);
			unsigned int mouseY = currentMessage->params[4] | ((int)currentMessage->params[5] << 8) | ((int)currentMessage->params[6] << 16) | ((int)currentMessage->params[7] << 24);
			
			handleMouseAction(currentMessage, time, CelestialMath::vectorUI2(mouseX, mouseY));
			
		}
		else if ((currentMessage->mess == GUIMess_POST || 
			currentMessage->mess == GUIMess_APPEND || 
			currentMessage->mess == GUIMess_CLEAR || 
			currentMessage->mess == GUIMess_ERASE) && focusedObject != nullptr && focusedObject->GetType() == GUIObjects_TEXTBOX)
		{

			GUITextBox* obj = ((GUITextBox*)focusedObject);
			unsigned int param1 = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);

			if (currentMessage->mess == GUIMess_POST)
			{

				std::string stringParam((char*)(currentMessage->params));
				obj->GetText()->AddTextLine(stringParam);

			}
			else if (currentMessage->mess == GUIMess_APPEND)
			{

				std::string stringParam((char*)(currentMessage->params));
				obj->GetText()->AppendText(stringParam);

			}
			else if (currentMessage->mess == GUIMess_CLEAR)
			{

				obj->GetText()->Clear();

			}
			else if (currentMessage->mess == GUIMess_ERASE)
			{

				obj->GetText()->EraseText(param1);

			}
		}
		else if (currentMessage->mess == GUIMess_FOCUS)
		{

			unsigned int param1 = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);
			
			if (focusedObject != nullptr)
			{

				focusedObject->SetFocus(false);

			}

			focusedObject = (GUIObject*)gameObjects->GetValue(param1);

			focusedObject->SetFocus(true);

		}
		else if (currentMessage->mess == GUIMess_HANDLECHAR && focusedObject != nullptr)
		{

			currentMessage->type = MessageType_OBJECT;
			currentMessage->mess = ObjectMess_HANDLECHAR;
			currentMessage->timeSent = time;
			focusedObject->Update(currentMessage);

		}
		else if (currentMessage->mess == GUIMess_HANDLEKEY && focusedObject != nullptr)
		{

			keyMessage.read = false;
			keyMessage.params[keyMessage.params[0] * 2 + 1] = currentMessage->params[0];
			keyMessage.params[keyMessage.params[0] * 2 + 2] = currentMessage->params[1];
			keyMessage.params[0] += 1;

		}

		currentMessage->read = currentMessage->destination != MessageSource_GUIENTITIES;
		currentMessage = inQueue->PopMessage();

	}

	if (!keyMessage.read)
	{

		focusedObject->Update(&keyMessage);

	}

	int dbg = 0;
}

GUIEntityHandler::~GUIEntityHandler()
{


}
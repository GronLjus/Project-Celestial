#include "stdafx.h"
#include "InputHandler.h"
#include "KeyCodes.h"

using namespace Input;
using namespace CrossHandlers;
using namespace Resources;
using namespace CelestialMath;

InputHandler::InputHandler() : IHandleMessages(300,MessageSource_INPUT)
{

	draggedTarget = 0;
	draggedScript = 0;
	clickTime = 125;
	filter = MessageType_INPUT;
	screenTargets = new CelestialSlicedList<ScreenTarget*>(32, nullptr);
	gameObjects = nullptr;
	maxScreenTargets = 0;
	mouse = vectorUI2(0, 0);
	keyStates = new keyState[keyCodeNA];
	pressedKeys = new CelestialStack<key>(false);
	dragging = keyCodeNA;

	keyStatus = new bool*[CelestialKeyCategories_NA];
	keyStatus[CelestialKeyCategories_CHAR] = new bool[CelestialCharKeyCodes_NA];
	keyStatus[CelestialKeyCategories_MOD] = new bool[CelestialModKeyCodes_NA];
	keyStatus[CelestialKeyCategories_SPEC] = new bool[CelestialSpecKeyCodes_NA];

	for (unsigned char i = 0; i < CelestialCharKeyCodes_NA; i++)
	{

		keyStatus[CelestialKeyCategories_CHAR][i] = false;

	}

	for (unsigned char i = 0; i < CelestialModKeyCodes_NA; i++)
	{

		keyStatus[CelestialKeyCategories_MOD][i] = false;

	}

	for (unsigned char i = 0; i < CelestialSpecKeyCodes_NA; i++)
	{

		keyStatus[CelestialKeyCategories_SPEC][i] = false;

	}
}

void InputHandler::Init(CelestialSlicedList<BaseObject*>* gameObjects)
{

	this->gameObjects = gameObjects;

}

void InputHandler::triggerScript(unsigned int script, unsigned int time, unsigned int targetId)
{

	unsigned char tempBuff[]{script >> 0, script >> 8, script >> 16, script >> 24, 
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

	messageBuffer[this->currentMessage].SetParams(tempBuff, 0, 4);
	messageBuffer[this->currentMessage].timeSent = time;
	messageBuffer[this->currentMessage].destination = MessageSource_CELSCRIPT;
	messageBuffer[this->currentMessage].type = MessageType_SCRIPT;
	messageBuffer[this->currentMessage].mess = ScriptMess_RUN;
	messageBuffer[this->currentMessage].read = false;
	outQueue->PushMessage(&messageBuffer[this->currentMessage]);
	this->currentMessage = (this->currentMessage + 1) % outMessages;
}

void InputHandler::handleKeys(unsigned int time)
{

	while (pressedKeys->GetCount() > 0)
	{

		key pressedKey = pressedKeys->PeekElement();

		if (keyStatus[pressedKey.cat][pressedKey.keyVal])
		{

			messageBuffer[this->currentMessage].params[0] = pressedKey.cat;
			messageBuffer[this->currentMessage].params[1] = pressedKey.keyVal;
			messageBuffer[this->currentMessage].timeSent = time;
			messageBuffer[this->currentMessage].destination = MessageSource_GUIENTITIES;
			messageBuffer[this->currentMessage].type = MessageType_GUIENTITIES;
			messageBuffer[this->currentMessage].mess = GUIMess_HANDLEKEY;
			messageBuffer[this->currentMessage].read = false;
			outQueue->PushMessage(&messageBuffer[this->currentMessage]);
			this->currentMessage = (this->currentMessage + 1) % outMessages;
			pressedKeys->PopElement();

		}
		else
		{
			
			pressedKeys->Remove();

		}
	}

	pressedKeys->Rewind();

}

ScreenTarget * InputHandler::checkScreenTargets(unsigned int time)
{

	ScreenTarget* firstTarget = nullptr;
	unsigned char topLayer = 1;

	for (unsigned int i = 0; i < maxScreenTargets; i++)
	{

		ScreenTarget* st = screenTargets->GetValue(i);

		if (st != nullptr)
		{

			if (st->IsVisible() && !st->ShouldRemove() &&
				(st->GetPosition().x <= mouse.x && st->GetPosition().x + st->GetScale().x >= mouse.x &&
					st->GetPosition().y <= mouse.y && st->GetPosition().y + st->GetScale().y >= mouse.y) &&
				(st->GetLayer() > topLayer))
			{

				if (firstTarget != nullptr && firstTarget->IsHovering())
				{

					firstTarget->SetHovering(false);

					if (firstTarget->GetExitScript() != 0)
					{

						unsigned int script = firstTarget->GetExitScript() - 1;
						triggerScript(script, time, firstTarget->GetTargetId());

					}
				}

				topLayer = st->GetLayer();
				firstTarget = st;

			}
			else if (st->ShouldRemove())
			{

				unsigned char tempBuff[]{ st->GetId() >> 0, st->GetId() >> 8, st->GetId() >> 16, st->GetId() >> 24 };
				messageBuffer[this->currentMessage].SetParams(tempBuff, 0, 4);
				messageBuffer[this->currentMessage].timeSent = time;
				messageBuffer[this->currentMessage].destination = MessageSource_RESOURCES;
				messageBuffer[this->currentMessage].type = MessageType_RESOURCES;
				messageBuffer[this->currentMessage].mess = ResourceMess_UNLOADOBJECT;
				messageBuffer[this->currentMessage].read = false;
				outQueue->PushMessage(&messageBuffer[this->currentMessage]);
				this->currentMessage = (this->currentMessage + 1) % outMessages;

				screenTargets->Add(nullptr, i);

			}
			else if (st->IsHovering())
			{

				st->SetHovering(false);

				if (st->GetExitScript() != 0)
				{

					unsigned int script = st->GetExitScript() - 1;
					triggerScript(script, time, st->GetTargetId());

				}
			}
		}
	}

	return firstTarget;

}

bool InputHandler::checkMouseClick(unsigned int time, keyCode ks) const
{

	if (keyStates[ks].state && !keyStates[ks].lastState &&
		keyStates[ks].thisTime - keyStates[ks].lastTime <= clickTime)
	{

		keyStates[ks].lastState = true;
		return true;

	}

	return false;

}

bool InputHandler::checkDrag(unsigned int time)
{

	if (dragging == keyCodeNA)
	{

		for (unsigned char i = 0; i < keyCodeNA && dragging == keyCodeNA; i++)
		{

			if (!keyStates[i].state &&
				time - keyStates[i].thisTime > clickTime)
			{


				dragging = keyCode(i);

				ScreenTarget* firstTarget = checkScreenTargets(time);

				if (firstTarget != nullptr)
				{

					draggedScript = i == keyCodeMouseL ? firstTarget->GetLeftDragScript() :
						i == keyCodeMouseM ? firstTarget->GetMiddleClickScript() :
						firstTarget->GetRightDragScript();

					if (draggedScript != 0)
					{

						draggedTarget = firstTarget->GetTargetId();
						triggerScript(draggedScript-1, time, draggedTarget);

					}
				}
				else
				{

					sendMessageToGB(GameBoardMess_STARTDRAGGING, time, dragging);

				}

				for (unsigned char k = 0; k < keyCodeNA; k++)
				{

					if (i != dragging)
					{

						keyStates[i].state = true;
						keyStates[i].lastState = true;
						keyStates[i].lastTime = time;
						keyStates[i].thisTime = time;

					}
				}
			}
		}
	}
	else
	{

		if (keyStates[dragging].state)
		{

			if (draggedScript != 0)
			{

				triggerScript(draggedScript-1, time, draggedTarget);
				draggedScript = 0;
				draggedTarget = 0;

			}
			else if(draggedTarget == 0)
			{

				sendMessageToGB(GameBoardMess_STOPDRAGGING, time, dragging);
			
			}

			dragging = keyCodeNA;

		}
		else
		{

			if (draggedScript != 0)
			{

				triggerScript(draggedScript-1, time, draggedTarget);

			}
			else if (draggedTarget == 0)
			{

				sendMessageToGB(GameBoardMess_DRAGOBJECT, time, dragging);

			}
		}
	}

	return dragging != keyCodeNA;

}

void InputHandler::sendMessageToGB(unsigned int mess, unsigned int time, keyCode key)
{

	unsigned char tempBuff[]{ key,
		mouse.x >> 0, mouse.x >> 8, mouse.x >> 16, mouse.x >> 24,
		mouse.y >> 0, mouse.y >> 8, mouse.y >> 16, mouse.y >> 24
	};

	messageBuffer[this->currentMessage].SetParams(tempBuff, 0, 12);
	messageBuffer[this->currentMessage].timeSent = time;
	messageBuffer[this->currentMessage].destination = MessageSource_ENTITIES;
	messageBuffer[this->currentMessage].type = MessageType_ENTITIES;
	messageBuffer[this->currentMessage].mess = mess;
	messageBuffer[this->currentMessage].read = false;
	outQueue->PushMessage(&messageBuffer[this->currentMessage]);
	this->currentMessage = (this->currentMessage + 1) % outMessages;

}

void InputHandler::handleMouse(unsigned int time)
{

	if (checkDrag(time))
	{

		return;

	}

	ScreenTarget* firstTarget = checkScreenTargets(time);
	
	if (firstTarget != nullptr)
	{

		if (!firstTarget->IsHovering() && firstTarget->GetEnterScript() != 0)
		{

			unsigned int script = firstTarget->GetEnterScript() - 1;
			triggerScript(script, time, firstTarget->GetTargetId());

		}

		firstTarget->SetHovering(true);

		if (firstTarget->GetHoverScript() != 0)
		{

			unsigned int hoverScript = firstTarget->GetHoverScript() - 1;
			triggerScript(hoverScript, time, firstTarget->GetTargetId());

		}

		if (firstTarget->GetLeftClickScript() != 0 &&
			checkMouseClick(time,keyCodeMouseL))
		{

			unsigned int lcScript = firstTarget->GetLeftClickScript() - 1;
			triggerScript(lcScript, time, firstTarget->GetTargetId());

		}
		
		if (firstTarget->GetMiddleClickScript() != 0 &&
			checkMouseClick(time, keyCodeMouseM))
		{

			unsigned int mcScript = firstTarget->GetMiddleClickScript() - 1;
			triggerScript(mcScript, time, firstTarget->GetTargetId());

		}

		if (firstTarget->GetRightClickScript() != 0 &&
			checkMouseClick(time, keyCodeMouseR))
		{

			unsigned int rcScript = firstTarget->GetRightClickScript() - 1;
			triggerScript(rcScript, time, firstTarget->GetTargetId());

		}
	}
	else
	{

		for (unsigned char i = 0; i < keyCodeNA; i++)
		{

			if (checkMouseClick(time, keyCode(i)))
			{

				sendMessageToGB(GameBoardMess_CLICKOBJECT, time, keyCode(i));

			}
		}
	}
}

void InputHandler::Update(unsigned int time)
{

	Message* currentMessage = inQueue->PopMessage();
	bool mouseMoved = false;

	while (currentMessage->type != MessageType_NA)
	{

		messageBuffer[this->currentMessage].timeSent = time;

		short param10 = currentMessage->params[0] | ((int)currentMessage->params[1] << 8);
		short param20 = currentMessage->params[2] | ((int)currentMessage->params[3] << 8);

		unsigned int param1 = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);

		if (currentMessage->mess == InputMess_ADDSCREENTARGET)
		{
				
			unsigned int stId = screenTargets->Add((ScreenTarget*)gameObjects->GetValue(param1));

			if (stId >= maxScreenTargets)
			{

				maxScreenTargets = stId+1;

			}
		}
		else if (currentMessage->mess == InputMess_MOUSEMOVE)
		{

			mouse.x = currentMessage->params[0] | ((int)currentMessage->params[1] << 8);
			mouse.y = currentMessage->params[2] | ((int)currentMessage->params[3] << 8);

		}
		else if ((currentMessage->mess == InputMess_MOUSEUP && !keyStates[param1].state) || (currentMessage->mess == InputMess_MOUSEDWN && keyStates[param1].state))
		{

			keyStates[param1].lastState = keyStates[param1].state;
			keyStates[param1].lastState = keyStates[param1].state;
			keyStates[param1].state = currentMessage->mess == InputMess_MOUSEUP;
			keyStates[param1].lastTime = keyStates[param1].thisTime;
			keyStates[param1].thisTime = time;

		}
		else if (currentMessage->mess == InputMess_KEYDWN || currentMessage->mess == InputMess_KEYUP)
		{

			keyStatus[currentMessage->params[0]][currentMessage->params[1]] = currentMessage->mess == InputMess_KEYDWN;

			if (currentMessage->mess == InputMess_KEYDWN)
			{

				pressedKeys->PushElement(key(CelestialKeyCategories(currentMessage->params[0]), currentMessage->params[1]));

			}
			else
			{

				int dbg = 0;
			}
		}
		else if (currentMessage->mess == InputMess_CHAR)
		{

			messageBuffer[this->currentMessage].SetParams(currentMessage->params, 0, 1);
			messageBuffer[this->currentMessage].timeSent = time;
			messageBuffer[this->currentMessage].destination = MessageSource_GUIENTITIES;
			messageBuffer[this->currentMessage].type = MessageType_GUIENTITIES;
			messageBuffer[this->currentMessage].mess = GUIMess_HANDLECHAR;
			messageBuffer[this->currentMessage].read = false;
			outQueue->PushMessage(&messageBuffer[this->currentMessage]);
			this->currentMessage = (this->currentMessage + 1) % outMessages;

		}

		currentMessage->read = true;
		currentMessage = inQueue->PopMessage();

	}

	handleMouse(time);
	handleKeys(time);

}

InputHandler::~InputHandler()
{

	delete[] keyStates;
	delete screenTargets;
	delete pressedKeys;

	delete[] keyStatus[CelestialKeyCategories_CHAR];
	delete[] keyStatus[CelestialKeyCategories_MOD];
	delete[] keyStatus[CelestialKeyCategories_SPEC];
	delete[] keyStatus;

}
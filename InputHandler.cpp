#include "stdafx.h"
#include "InputHandler.h"
#include "KeyCodes.h"

using namespace Input;
using namespace CrossHandlers;
using namespace Resources;
using namespace CelestialMath;

InputHandler::InputHandler() : IHandleMessages(300,MessageSource_INPUT)
{

	filter = MessageType_INPUT;
	screenTargets = new CelestialSlicedList<ScreenTarget*>(32, nullptr);
	gameObjects = nullptr;
	maxScreenTargets = 0;
	mouse = vectorUI2(0, 0);
	keyStates = new keyState[keyCodeNA];
	pressedKeys = new CelestialStack<key>(false);

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

	messageBuffer[this->currentMessage].timeSent = time;
	messageBuffer[this->currentMessage].destination = MessageSource_CELSCRIPT;
	messageBuffer[this->currentMessage].type = MessageType_SCRIPT;
	messageBuffer[this->currentMessage].mess = ScriptMess_ADDPARNUM;
	messageBuffer[this->currentMessage].read = false;
	messageBuffer[this->currentMessage].SetParams(tempBuff, 0, 8);
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

void InputHandler::handleMouse(unsigned int time)
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

				unsigned char tempBuff[]{st->GetId() >> 0, st->GetId() >> 8, st->GetId() >> 16, st->GetId() >> 24};
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
			keyStates[keyCodeMouseL].thisTime == time &&
			keyStates[keyCodeMouseL].state &&
			keyStates[keyCodeMouseL].thisTime - keyStates[keyCodeMouseL].lastTime <= 250)
		{

			unsigned int lcScript = firstTarget->GetLeftClickScript() - 1;
			triggerScript(lcScript, time, firstTarget->GetTargetId());

		}

		if (firstTarget->GetRightClickScript() != 0 &&
			keyStates[keyCodeMouseR].thisTime == time &&
			keyStates[keyCodeMouseR].state &&
			keyStates[keyCodeMouseR].thisTime - keyStates[keyCodeMouseR].lastTime <= 250)
		{

			unsigned int rcScript = firstTarget->GetRightClickScript() - 1;
			triggerScript(rcScript, time, firstTarget->GetTargetId());

		}
	}
	else
	{

		if (keyStates[keyCodeMouseR].thisTime == time &&
			keyStates[keyCodeMouseR].state &&
			keyStates[keyCodeMouseR].thisTime - keyStates[keyCodeMouseR].lastTime <= 250)
		{

			unsigned char tempBuff[]{1 >> 0, 1 >> 8, 1 >> 16, 1 >> 24,
				mouse.x >> 0, mouse.x >> 8, mouse.x >> 16, mouse.x >> 24,
				mouse.y >> 0, mouse.y >> 8, mouse.y >> 16, mouse.y >> 24
			};
			messageBuffer[this->currentMessage].SetParams(tempBuff, 0, 12);
			messageBuffer[this->currentMessage].timeSent = time;
			messageBuffer[this->currentMessage].destination = MessageSource_ENTITIES;
			messageBuffer[this->currentMessage].type = MessageType_ENTITIES;
			messageBuffer[this->currentMessage].mess = GameBoardMess_SELECTOBJECT;
			messageBuffer[this->currentMessage].read = false;
			outQueue->PushMessage(&messageBuffer[this->currentMessage]);
			this->currentMessage = (this->currentMessage + 1) % outMessages;
		}

		if (keyStates[keyCodeMouseL].thisTime == time &&
			keyStates[keyCodeMouseL].state &&
			keyStates[keyCodeMouseL].thisTime - keyStates[keyCodeMouseL].lastTime <= 250)
		{

			unsigned char tempBuff[]{0 >> 0, 0 >> 8, 0 >> 16, 0 >> 24,
				mouse.x >> 0, mouse.x >> 8, mouse.x >> 16, mouse.x >> 24,
				mouse.y >> 0, mouse.y >> 8, mouse.y >> 16, mouse.y >> 24
			};
			messageBuffer[this->currentMessage].SetParams(tempBuff, 0, 12);
			messageBuffer[this->currentMessage].SetParams(tempBuff, 0, 4);
			messageBuffer[this->currentMessage].timeSent = time;
			messageBuffer[this->currentMessage].destination = MessageSource_ENTITIES;
			messageBuffer[this->currentMessage].type = MessageType_ENTITIES;
			messageBuffer[this->currentMessage].mess = GameBoardMess_SELECTOBJECT;
			messageBuffer[this->currentMessage].read = false;
			outQueue->PushMessage(&messageBuffer[this->currentMessage]);
			this->currentMessage = (this->currentMessage + 1) % outMessages;

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
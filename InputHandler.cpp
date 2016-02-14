#include "stdafx.h"
#include "InputHandler.h"
#include <windowsx.h>

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

}

void InputHandler::Init(CelestialSlicedList<BaseObject*>* gameObjects)
{

	this->gameObjects = gameObjects;

}

void InputHandler::triggerScript(unsigned int script, unsigned int time)
{

	unsigned char tempBuff[]{script >> 0, script >> 8, script >> 16, script >> 24
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

void InputHandler::handleMouse(unsigned int time)
{

	bool isOverST = false;

	for (unsigned int i = 0; i < maxScreenTargets; i++)
	{

		ScreenTarget* st = screenTargets->GetValue(i);

		if (st != nullptr)
		{

			if (st->GetPosition().x <= mouse.x && st->GetPosition().x + st->GetScale().x >= mouse.x &&
				st->GetPosition().y <= mouse.y && st->GetPosition().y + st->GetScale().y >= mouse.y)
			{

				isOverST = true;

				if (!st->IsHovering() && st->GetEnterScript() != 0)
				{

					unsigned int script = st->GetEnterScript() - 1;
					triggerScript(script, time);

				}

				st->SetHovering(true);

				if (st->GetHoverScript() != 0)
				{

					unsigned int hoverScript = st->GetHoverScript() - 1;
					triggerScript(hoverScript, time);

				}

				if (st->GetLeftClickScript() != 0 && 
					keyStates[keyCodeMouseL].thisTime == time && 
					keyStates[keyCodeMouseL].state && 
					keyStates[keyCodeMouseL].thisTime - keyStates[keyCodeMouseL].lastTime <= 250)
				{

					unsigned int lcScript = st->GetLeftClickScript() - 1;
					triggerScript(lcScript, time);

				}

				if (st->GetRightClickScript() != 0 &&
					keyStates[keyCodeMouseR].thisTime == time &&
					keyStates[keyCodeMouseR].state &&
					keyStates[keyCodeMouseR].thisTime - keyStates[keyCodeMouseR].lastTime <= 250)
				{

					unsigned int rcScript = st->GetRightClickScript() - 1;
					triggerScript(rcScript, time);

				}
			}
			else if (st->IsHovering())
			{

				st->SetHovering(false);

				if (st->GetExitScript() != 0)
				{

					unsigned int script = st->GetExitScript() - 1;
					triggerScript(script, time);

				}
			}
		}
	}

	if (!isOverST)
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

			if (stId > maxScreenTargets)
			{

				maxScreenTargets = stId;

			}
		}
		else if (currentMessage->mess == InputMess_MOUSEMOVE)
		{

			mouse.x = currentMessage->params[0] | ((int)currentMessage->params[1] << 8);
			mouse.y = currentMessage->params[2] | ((int)currentMessage->params[3] << 8);

		}
		else if ((currentMessage->mess == InputMess_KEYUP && !keyStates[param1].state) || (currentMessage->mess == InputMess_KEYDWN && keyStates[param1].state))
		{

			keyStates[param1].lastState = keyStates[param1].state;
			keyStates[param1].state = currentMessage->mess == InputMess_KEYUP;
			keyStates[param1].lastTime = keyStates[param1].thisTime;
			keyStates[param1].thisTime = time;

		}

		currentMessage->read = true;
		currentMessage = inQueue->PopMessage();

	}

	handleMouse(time);

}

InputHandler::~InputHandler()
{

	delete[] keyStates;
	delete screenTargets;

}
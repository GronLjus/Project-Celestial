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
	gameObjects = nullptr;
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

		keyStatus[CelestialKeyCategories_CHAR][i] = true;

	}

	for (unsigned char i = 0; i < CelestialModKeyCodes_NA; i++)
	{

		keyStatus[CelestialKeyCategories_MOD][i] = true;

	}

	for (unsigned char i = 0; i < CelestialSpecKeyCodes_NA; i++)
	{

		keyStatus[CelestialKeyCategories_SPEC][i] = true;

	}
}

void InputHandler::Init(CelestialSlicedList<BaseObject*>* gameObjects)
{

	this->gameObjects = gameObjects;

}

void InputHandler::handleKeys(unsigned int time)
{

	while (pressedKeys->GetCount() > 0)
	{

		key pressedKey = pressedKeys->PopElement();

		if (keyStatus[pressedKey.cat][pressedKey.keyVal] != pressedKey.up)
		{

			keyStatus[pressedKey.cat][pressedKey.keyVal] = pressedKey.up;

			messageBuffer[this->currentMessage].params[0] = pressedKey.cat;
			messageBuffer[this->currentMessage].params[1] = pressedKey.keyVal;
			messageBuffer[this->currentMessage].params[2] = pressedKey.up ?  1 : 0;
			messageBuffer[this->currentMessage].timeSent = time;
			messageBuffer[this->currentMessage].destination = MessageSource_GUIENTITIES;
			messageBuffer[this->currentMessage].type = MessageType_GUIENTITIES;
			messageBuffer[this->currentMessage].mess = GUIMess_HANDLEKEY;
			messageBuffer[this->currentMessage].read = false;
			outQueue->PushMessage(&messageBuffer[this->currentMessage]);
			this->currentMessage = (this->currentMessage + 1) % outMessages;

		}
	}
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
				sendMessage(GameBoardMess_STARTDRAGGING, time, dragging);

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

			sendMessage(GameBoardMess_STOPDRAGGING, time, dragging);
			dragging = keyCodeNA;

		}
		else
		{

			sendMessage(GameBoardMess_DRAGOBJECT, time, dragging);

		}
	}

	return dragging != keyCodeNA;

}

void InputHandler::checkMouseWheel(unsigned int time)
{

	if (mouseWheel != 0)
	{

		sendMessage(GameBoardMess_WHEELOBJECT, time, mouseWheel);
		mouseWheel = 0;

	}
}

void InputHandler::sendMessage(unsigned int mess, unsigned int time, short delta)
{

	unsigned char tempBuff[]{
		delta >> 0, delta >> 8
	};

	messageBuffer[this->currentMessage].SetParams(tempBuff, 0, 2);
	messageBuffer[this->currentMessage].timeSent = time;
	messageBuffer[this->currentMessage].destination = MessageSource_ENTITIES;
	messageBuffer[this->currentMessage].type = MessageType_ENTITIES;
	messageBuffer[this->currentMessage].mess = mess;
	messageBuffer[this->currentMessage].read = false;
	outQueue->PushMessage(&messageBuffer[this->currentMessage]);
	this->currentMessage = (this->currentMessage + 1) % outMessages;

}

void InputHandler::sendMessage(unsigned int mess, unsigned int time, keyCode key)
{

	unsigned char tempBuff[]{
		key
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

void InputHandler::sendMessage(unsigned int mess, unsigned int time, keyCode key, bool up)
{

	unsigned char tempBuff[]{
		key,
		up ? 1 : 0
	};

	messageBuffer[this->currentMessage].SetParams(tempBuff, 0, 2);
	messageBuffer[this->currentMessage].timeSent = time;
	messageBuffer[this->currentMessage].destination = MessageSource_ENTITIES;
	messageBuffer[this->currentMessage].type = MessageType_ENTITIES;
	messageBuffer[this->currentMessage].mess = mess;
	messageBuffer[this->currentMessage].read = false;
	outQueue->PushMessage(&messageBuffer[this->currentMessage]);
	this->currentMessage = (this->currentMessage + 1) % outMessages;

}

void InputHandler::sendMessage(unsigned int mess, unsigned int time)
{

	unsigned char tempBuff[]{
		mouse.x >> 0, mouse.x >> 8, mouse.x >> 16, mouse.x >> 24,
		mouse.y >> 0, mouse.y >> 8, mouse.y >> 16, mouse.y >> 24
	};

	messageBuffer[this->currentMessage].SetParams(tempBuff, 0, 8);
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

	for (unsigned char i = 0; i < keyCodeNA; i++)
	{

		if (checkMouseClick(time, keyCode(i)))
		{

			sendMessage(GameBoardMess_CLICKOBJECT, time, keyCode(i));

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

		if (currentMessage->mess == InputMess_MOUSEMOVE)
		{

			mouse.x = currentMessage->params[0] | ((int)currentMessage->params[1] << 8);
			mouse.y = currentMessage->params[2] | ((int)currentMessage->params[3] << 8);
			sendMessage(GameBoardMess_MOUSEMOVE, time);

		}
		else if (currentMessage->mess == InputMess_MOUSEWHEEL)
		{

			sendMessage(GameBoardMess_WHEELOBJECT, time, param10);

		}
		else if ((currentMessage->mess == InputMess_MOUSEUP && !keyStates[param1].state) || (currentMessage->mess == InputMess_MOUSEDWN && keyStates[param1].state))
		{

			keyStates[param1].lastState = keyStates[param1].state;
			keyStates[param1].lastState = keyStates[param1].state;
			keyStates[param1].state = currentMessage->mess == InputMess_MOUSEUP;
			keyStates[param1].lastTime = keyStates[param1].thisTime;
			keyStates[param1].thisTime = time;

			sendMessage(GameBoardMess_MOUSEUD, time, keyCode(param1), keyStates[param1].state);

		}
		else if (currentMessage->mess == InputMess_KEYDWN || currentMessage->mess == InputMess_KEYUP)
		{

			pressedKeys->PushElement(key(CelestialKeyCategories(currentMessage->params[0]), currentMessage->params[1], currentMessage->mess == InputMess_KEYUP));

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
		
		if (currentMessage->killWhenDone)
		{

			delete currentMessage;

		}

		currentMessage = inQueue->PopMessage();

	}

	handleMouse(time);
	handleKeys(time);

}

InputHandler::~InputHandler()
{

	delete[] keyStates;
	delete pressedKeys;

	delete[] keyStatus[CelestialKeyCategories_CHAR];
	delete[] keyStatus[CelestialKeyCategories_MOD];
	delete[] keyStatus[CelestialKeyCategories_SPEC];
	delete[] keyStatus;

}
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

	dragTarget = nullptr;
	lastTarget = nullptr;
	screenLayout = nullptr;

	pressedKeys = new CelestialStack<key>(false);
	
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

void GUIEntityHandler::triggerScript(unsigned int script, unsigned int time, unsigned int targetId, unsigned char key)
{

	unsigned char tempBuff[]{ script >> 0, script >> 8, script >> 16, script >> 24,
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

	messageBuffer[this->currentMessage].timeSent = time;
	messageBuffer[this->currentMessage].destination = MessageSource_CELSCRIPT;
	messageBuffer[this->currentMessage].type = MessageType_SCRIPT;
	messageBuffer[this->currentMessage].mess = ScriptMess_ADDPARNUM;
	messageBuffer[this->currentMessage].read = false;
	messageBuffer[this->currentMessage].SetParams(&key, 0, 1);
	outQueue->PushMessage(&messageBuffer[this->currentMessage]);
	this->currentMessage = (this->currentMessage + 1) % outMessages;

	runScript(script, time);

}

void GUIEntityHandler::Update(unsigned int time)
{

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

			messageBuffer[this->currentMessage].timeSent = time;
			messageBuffer[this->currentMessage].destination = MessageSource_ENTITIES;
			messageBuffer[this->currentMessage].type = MessageType_ENTITIES;
			messageBuffer[this->currentMessage].mess = GameBoardMess_SETUI;
			messageBuffer[this->currentMessage].SetParams(currentMessage->params, 0, 4);
			messageBuffer[this->currentMessage].read = false;
			outQueue->PushMessage(&messageBuffer[this->currentMessage]);
			this->currentMessage = (this->currentMessage + 1) % outMessages;

		}
		else if (currentMessage->mess == GUIMess_ADDCHILD)
		{

			unsigned int parent = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);
			unsigned int child = currentMessage->params[4] | ((int)currentMessage->params[5] << 8) | ((int)currentMessage->params[6] << 16) | ((int)currentMessage->params[7] << 24);

			GUIObject* pObject = (GUIObject*)gameObjects->GetValue(parent);
			GUIObject* cObject = (GUIObject*)gameObjects->GetValue(child);

			if (pObject->GetType() == GUIObjects_LAYOUT)
			{

				((GUILayout*)pObject)->AddChild(cObject);

			}
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

			if (param1 > 0)
			{

				focusedObject = (GUIObject*)gameObjects->GetValue(param1);
				focusedObject->SetFocus(true);

			}
			else
			{

				focusedObject = nullptr;

			}
		}
		else if (currentMessage->mess == GUIMess_HANDLECHAR && focusedObject != nullptr)
		{

			currentMessage->type = MessageType_OBJECT;
			currentMessage->mess = ObjectMess_HANDLECHAR;
			currentMessage->timeSent = time;
			focusedObject->Update(currentMessage);

		}
		else if (currentMessage->mess == GUIMess_HANDLEKEY)
		{

			if (focusedObject == nullptr || 
				currentMessage->params[0] == Input::CelestialKeyCategories_SPEC)
			{

				messageBuffer[this->currentMessage].timeSent = time;
				messageBuffer[this->currentMessage].destination = MessageSource_ENTITIES;
				messageBuffer[this->currentMessage].type = MessageType_ENTITIES;
				messageBuffer[this->currentMessage].mess = GameBoardMess_HANDLEKEY;
				messageBuffer[this->currentMessage].SetParams(currentMessage->params, 0, 4);
				messageBuffer[this->currentMessage].read = false;
				outQueue->PushMessage(&messageBuffer[this->currentMessage]);
				this->currentMessage = (this->currentMessage + 1) % outMessages;

			}

			if (currentMessage->params[2] == 1)
			{

				key ky = key(Input::CelestialKeyCategories(currentMessage->params[0]), currentMessage->params[1]);
				pressedKeys->PushElement(ky);

			}
			else
			{

				bool stop = false;

				while (!stop && pressedKeys->GetCount() > 0)
				{

					key ky = pressedKeys->PeekElement();

					if (ky.cat == currentMessage->params[0] &&
						ky.keyVal == currentMessage->params[1])
					{

						pressedKeys->Remove();
						stop = true;

					}
					else
					{

						pressedKeys->PopElement();

					}
				}

				pressedKeys->Rewind();

			}
		}

		currentMessage->read = currentMessage->destination != MessageSource_GUIENTITIES;
		currentMessage = inQueue->PopMessage();

	}

	Message keyMessage;
	keyMessage.type = MessageType_OBJECT;
	keyMessage.mess = ObjectMess_HANDLEKEY;

	while (pressedKeys->GetCount() > 0)
	{

		key ky = pressedKeys->PopElement();

		keyMessage.params[0] = ky.cat;
		keyMessage.params[1] = ky.keyVal;

	}

	pressedKeys->Rewind();

}

GUIEntityHandler::~GUIEntityHandler()
{

	delete pressedKeys;

}
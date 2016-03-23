#include "stdafx.h"
#include "GUIEntityHandler.h"
#include "GUITextBox.h"

using namespace Entities;
using namespace CrossHandlers;
using namespace Resources;

GUIEntityHandler::GUIEntityHandler() : IHandleMessages(20,MessageSource_GUIENTITIES)
{

	filter = MessageType_GUIENTITIES;
	focusedObject = nullptr;
	keyMessage.type = MessageType_OBJECT;
	keyMessage.mess = ObjectMess_HANDLEKEY;
	
}

void GUIEntityHandler::Init(CelestialSlicedList<BaseObject*>* gameObjects)
{

	this->gameObjects = gameObjects;

}

void GUIEntityHandler::Update(unsigned int time)
{

	keyMessage.params[0] = 0;
	keyMessage.read = true;
	keyMessage.timeSent = time;

	Message* currentMessage = inQueue->PopMessage();

	while (currentMessage->type != MessageType_NA)
	{

		if ((currentMessage->mess == GUIMess_POST || 
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

		currentMessage->read = true;
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
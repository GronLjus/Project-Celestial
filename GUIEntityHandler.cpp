#include "stdafx.h"
#include "GUIEntityHandler.h"
#include "GUITextBox.h"

using namespace Entities;
using namespace CrossHandlers;
using namespace Resources;

GUIEntityHandler::GUIEntityHandler() : IHandleMessages(20,MessageSource_GUIENTITIES)
{

	filter = MessageType_GUIENTITIES;
	
}

void GUIEntityHandler::Init(CelestialSlicedList<BaseObject*>* gameObjects)
{

	this->gameObjects = gameObjects;

}

void GUIEntityHandler::Update(unsigned int time)
{

	Message* currentMessage = inQueue->PopMessage();

	while (currentMessage->type != MessageType_NA)
	{

		if (currentMessage->mess == GUIMess_POST || 
			currentMessage->mess == GUIMess_APPEND || 
			currentMessage->mess == GUIMess_CLEAR || 
			currentMessage->mess == GUIMess_ERASE)
		{

			unsigned int param1 = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);

			if (GUIObjects_TEXTBOX == ((GUIObject*)gameObjects->GetValue(param1))->GetType())
			{

				GUITextBox* obj = ((GUITextBox*)gameObjects->GetValue(param1));

				if (currentMessage->mess == GUIMess_POST)
				{

					std::string stringParam((char*)currentMessage->params);
					obj->AddText(stringParam);

				}
				else if (currentMessage->mess == GUIMess_APPEND)
				{

					std::string stringParam((char*)currentMessage->params);
					obj->AppendText(stringParam);

				}
				else if (currentMessage->mess == GUIMess_CLEAR)
				{

					obj->Clear();

				}
				else if (currentMessage->mess == GUIMess_ERASE)
				{

					obj->EraseText(param1);

				}
			}
		}
		
		currentMessage->read = true;
		currentMessage = inQueue->PopMessage();

	}

	int dbg = 0;
}

GUIEntityHandler::~GUIEntityHandler()
{

}
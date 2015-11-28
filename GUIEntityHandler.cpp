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

void GUIEntityHandler::Init(CelestialSlicedList<GUIObject*>* guiObjects)
{

	this->guiObjects = guiObjects;

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

			if (GUIObjects_TEXTBOX == guiObjects->GetValue(currentMessage->param1)->GetType())
			{

				GUITextBox* obj = ((GUITextBox*)guiObjects->GetValue(currentMessage->param1));

				if (currentMessage->mess == GUIMess_POST)
				{

					obj->AddText(currentMessage->stringParam);

				}
				else if (currentMessage->mess == GUIMess_APPEND)
				{

					obj->AppendText(currentMessage->stringParam);

				}
				else if (currentMessage->mess == GUIMess_CLEAR)
				{

					obj->Clear();

				}
				else if (currentMessage->mess == GUIMess_ERASE)
				{

					obj->EraseText(currentMessage->param1);

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
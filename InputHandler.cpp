#include "stdafx.h"
#include "InputHandler.h"
#include <windowsx.h>

using namespace Input;
using namespace CrossHandlers;

InputHandler::InputHandler() : IHandleMessages(300,MessageSource_INPUT)
{

	triggers = new CelestialList<keyTrigger>();
	filter = MessageType_INPUT;

	mousePos = new float[2];
	lastMousePos = new float[2];
	mouseMoved = false;
	contain = false;

}

void InputHandler::Init(HWND hWnd)
{

	this->hWnd = hWnd;

}

void InputHandler::Update(unsigned int time)
{

	Message* currentMessage = inQueue->PopMessage();

	while (currentMessage->type != MessageType_NA)
	{

		messageBuffer[this->currentMessage].timeSent = time;

		if (currentMessage->mess == InputMess_CHARDOWN || currentMessage->mess == InputMess_NONCHARDOWN)
		{
				
			CelestialListNode<keyTrigger>* trig = triggers->GetFirstNode();
			bool breakOut = false;

			while (trig != nullptr && !breakOut)
			{

				breakOut = trig->GetNodeObject().filter == currentMessage->param1 && 
					((currentMessage->mess == InputMess_CHARDOWN) == trig->GetNodeObject().charTrigg);

				if (!breakOut)
				{

					trig = trig->GetNext();

				}
			}

			if (breakOut)
			{

				for (int i = 0; i < trig->GetNodeObject().scriptAmount; i++)
				{

					messageBuffer[this->currentMessage].destination = MessageSource_CELSCRIPT;
					messageBuffer[this->currentMessage].mess = ScriptMess_ADDPARNUM;
					messageBuffer[this->currentMessage].param1 = trig->GetNodeObject().scripts[i];
					messageBuffer[this->currentMessage].param2 = trig->GetNodeObject().obj;
					outQueue->PushMessage(&messageBuffer[this->currentMessage]);
					this->currentMessage = (this->currentMessage + 1) % outMessages;

					messageBuffer[this->currentMessage].destination = MessageSource_CELSCRIPT;
					messageBuffer[this->currentMessage].param1 = trig->GetNodeObject().scripts[i];

					if (trig->GetNodeObject().charTrigg)
					{

						messageBuffer[this->currentMessage].mess = ScriptMess_ADDPARASTR;
						messageBuffer[this->currentMessage].stringParam = ((char)trig->GetNodeObject().filter);

					}
					else
					{

						messageBuffer[this->currentMessage].mess = ScriptMess_ADDPARNUM;
						messageBuffer[this->currentMessage].param2 = trig->GetNodeObject().filter;

					}

					outQueue->PushMessage(&messageBuffer[this->currentMessage]);
					this->currentMessage = (this->currentMessage + 1) % outMessages;

					messageBuffer[this->currentMessage].destination = MessageSource_CELSCRIPT;
					messageBuffer[this->currentMessage].mess = ScriptMess_RUN;
					messageBuffer[this->currentMessage].param1 = trig->GetNodeObject().scripts[i];;
					outQueue->PushMessage(&messageBuffer[this->currentMessage]);
					this->currentMessage = (this->currentMessage + 1) % outMessages;

				}
			}
		}
		else if (currentMessage->mess == InputMess_MOUSEMOVE)
		{

			if (mouseTriggerId > 0)
			{

				messageBuffer[this->currentMessage].destination = MessageSource_CELSCRIPT;
				messageBuffer[this->currentMessage].param1 = mouseTriggerId-1;
				messageBuffer[this->currentMessage].param2 = currentMessage->param1;
				messageBuffer[this->currentMessage].param3 = currentMessage->param2;
				messageBuffer[this->currentMessage].read = false;
				outQueue->PushMessage(&messageBuffer[this->currentMessage]);
				this->currentMessage = (this->currentMessage + 1) % outMessages;

			}

			float epsilon = 0.000000f;
			RECT rc;
			GetClientRect( hWnd, &rc );
			float screenWidth = (float)(rc.right - rc.left);
			float screenHeight = (float)(rc.bottom - rc.top);
			float x = ((float)(currentMessage->param1) / screenWidth) * 2 - 1;
			float y = ((float)(currentMessage->param2) / screenHeight) * 2 - 1;

			//mouseMoved = (keys[Key_SHIFT] && !contain) || (contain && ((x > epsilon || x < -epsilon) || (y > epsilon || y < -epsilon)));

			if(mouseMoved)
			{

				int debug = 0;
				debug++;

			}

			if(!contain)
			{
			
				lastMousePos[0] = mousePos[0];
				lastMousePos[1] = mousePos[1];

			}

			mousePos[0] = (float)x; 
			mousePos[1] = (float)y; 

		}
		else if (currentMessage->mess == InputMess_TOGGLESCRIPTTRIGGER_KEYPRESS)
		{

			short filterVal = currentMessage->param1 >> 0;
			bool charFilter = currentMessage->param1 >> 16;
			bool add = currentMessage->param1 >> 17;

			if (add)
			{

				bool breakOut = false;
				CelestialListNode<keyTrigger>* trig = triggers->GetFirstNode();

				while (trig != nullptr && !breakOut)
				{

					breakOut = trig->GetNodeObject().filter == filterVal && trig->GetNodeObject().charTrigg == charFilter;
					trig = trig->GetNext();

				}

				if (!breakOut)
				{

					keyTrigger tr;
					tr.obj = currentMessage->param2;
					tr.filter = filterVal;
					tr.maxScripts = 10;
					tr.scriptAmount = 1;
					tr.scripts = new unsigned int[tr.maxScripts];
					tr.scripts[0] = currentMessage->param3;
					tr.charTrigg = charFilter;
					triggers->AddElement(tr);

				}
				else
				{

					keyTrigger tr = trig->GetNodeObject();

					if (tr.scriptAmount == tr.maxScripts)
					{

						tr.maxScripts += 10;
						unsigned int* temp = new unsigned int[tr.maxScripts];

						for (unsigned int i = 0; i < tr.scriptAmount; i++)
						{

							temp[i] = tr.scripts[i];

						}

						delete tr.scripts;
						tr.scripts = temp;

					}
		
					tr.scripts[tr.scriptAmount] = currentMessage->param3;
					tr.scriptAmount++;
					triggers->AddElement(tr);

				}
			}
			else
			{
				CelestialListNode<keyTrigger>* trig = triggers->GetFirstNode();
				bool breakOut = false;

				while (trig->GetNext() != nullptr && !breakOut)
				{

					breakOut = trig->GetNodeObject().filter == filterVal && trig->GetNodeObject().charTrigg == charFilter;
					trig = trig->GetNext();

				}

				if (breakOut)
				{

					keyTrigger trigg = trig->GetNodeObject();

					bool sortLoop = false;

					for (unsigned int i = 0; i < trigg.scriptAmount; i++)
					{

						if (!sortLoop)
						{

							sortLoop = trigg.scripts[i] == currentMessage->param3;

						}
						else
						{

							trigg.scripts[i - 1] = trigg.scripts[i];

						}
					}

					if (sortLoop)
					{

						trigg.scriptAmount--;

					}

					trig->SetNodeObject(trigg);

				}
			}
		}
		else if (currentMessage->mess == InputMess_TOGGLESCRIPTTRIGGER_MOUSEMOVE)
		{

			mouseTriggerId = currentMessage->param2 == 1 ? currentMessage->param3 + 1 : 0;

		}

		currentMessage->read = true;
		currentMessage = inQueue->PopMessage();

	}
}

int* InputHandler::GetMovement()//0=x,1=y,2=z
{

	int* retVal = new int[3];
	retVal[0] = 0;
	retVal[1] = 0;
	retVal[2] = 0;

	bool forw = IsKeyDown(Key_FORWARD);
	bool back = IsKeyDown(Key_BACKWARD);
	bool right = IsKeyDown(Key_RIGHT);
	bool left = IsKeyDown(Key_LEFT);

	if(forw && !back)
	{

		retVal[2] = 1;

	}
	else if(!forw && back)
	{

		retVal[2] = -1;

	}

	if(left && !right)
	{

		retVal[0] = -1;

	}
	else if(!left && right)
	{

		retVal[0] = 1;

	}

	return retVal;

}

float* InputHandler::GetMousePos()
{

	return mousePos;

}

float* InputHandler::GetLastMousePos()
{

	return lastMousePos;

}

bool InputHandler::HasMouseMoved()
{

	bool retVal = mouseMoved;

	if(contain)
	{
		
		centerMouse();

	}
	
	mouseMoved = false;
	return retVal;

}

void InputHandler::ToggleContainMouse()
{

	contain = !contain;

	if(contain)
	{
		
		ShowCursor(false);
		lastMousePos[0] = 0;
		lastMousePos[1] = 0;
		centerMouse();

	}
	else
	{

		ShowCursor(true);

	}
}

void InputHandler::centerMouse()
{
	
	POINT point = POINT();
	point.x = 0;
	point.y = 0;
	RECT rc;
	GetClientRect( hWnd, &rc );
	ClientToScreen(hWnd,&point);

	SetCursorPos((int)(point.x+(rc.right - rc.left)*0.5f),(int)(point.y+(rc.bottom - rc.top)*0.5f));

}

int InputHandler::GetNumberKeyDown()
{
	
	int retVal = -1;
	bool stop = false;

	return retVal;

}

bool InputHandler::IsKeyDown(Key key)
{

	bool retVal = false;
	return retVal;

}

InputHandler::~InputHandler()
{

	delete[] mousePos;
	delete[] lastMousePos;
	delete triggers;

}
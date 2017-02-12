#include "stdafx.h"
#include "GUITextBox.h"
#include "CelestialMath.h"

using namespace Input;
using namespace std;
using namespace Resources;

GUITextBox::GUITextBox(GUISnap h, GUISnap v) : GUIObject()
{

	enabled = false;
	hSnap = h;
	vSnap = v;
	type = GUIObjects_TEXTBOX;
	textContainer = new TextContainer();
	caretCol = 0;
	caretRow = 0;
	lastTime = 0;
	acceptKeys = false;

}

unsigned int GUITextBox::GetCaretPosition() const
{

	return textContainer->GetCaretPos();

}

bool GUITextBox::HasCaret() const
{

	return acceptKeys;

}

unsigned char* GUITextBox::Update(Message* mess)
{

	if (mess->type == MessageType_OBJECT)
	{

		std::string stringParam;
		unsigned char keys = mess->params[0];
		bool shift = false;

		switch (mess->mess)
		{

		case ObjectMess_SETTEXT:
			setLastUpdated(mess->timeSent);
			stringParam = string((char*)mess->params);
			textContainer->SetText(stringParam);
			break;
		case ObjectMess_APPENDTEXT:
			setLastUpdated(mess->timeSent);
			stringParam = string((char*)mess->params);
			textContainer->AppendText(stringParam);
			break;
		case ObjectMess_APPENDTEXTLINE:
			setLastUpdated(mess->timeSent);
			stringParam = string((char*)mess->params);
			textContainer->AddTextLine(stringParam);
			break;
		case ObjectMess_LOCKINPUT:
			PositionableObject::Update(mess);
		case ObjectMess_LOCKKEYS:
			acceptKeys = mess->params[0] == 1;
			break;
		case ObjectMess_HANDLECHAR:
			if (acceptKeys)
			{

				setLastUpdated(mess->timeSent);
				switch (mess->params[0])
				{

				case '\b':
					textContainer->EraseText(1);
					break;
				case '\n':
					textContainer->AddTextLine("");
					break;
				default:
					textContainer->AppendText(mess->params[0]);
					break;

				}
			}
		case ObjectMess_HANDLEKEY:

			if (mess->timeSent - lastTime >= 100 && acceptKeys)
			{

				setLastUpdated(mess->timeSent);
				lastTime = mess->timeSent;

				if (mess->params[0] == CelestialKeyCategories_SPEC)
				{

					if (mess->params[1] == CelestialSpecKeyCodes_RGHT)
					{
						textContainer->MoveCaret(0, 1);
					}

					if (mess->params[1] == CelestialSpecKeyCodes_LFT)
					{
						textContainer->MoveCaret(0, -1);
					}

					if (mess->params[1] == CelestialSpecKeyCodes_UP)
					{
						textContainer->MoveCaret(-1, 0);
					}

					if (mess->params[1] == CelestialSpecKeyCodes_DWN)
					{
						textContainer->MoveCaret(1, 0);
					}

				}
			}
		default:
			return GUIObject::Update(mess);

		}
	}

	return nullptr;

}

TextContainer* GUITextBox::GetText() const
{

	return textContainer;

}

void GUITextBox::SetText(TextContainer* textCont)
{

	delete textContainer;
	textContainer = textCont;

}

float GUITextBox::GetAngle()
{

	return (3.0f*CelestialMath::CELESTIAL_PI)*0.5f;

}

GUITextBox::~GUITextBox()
{

	delete textContainer;

}
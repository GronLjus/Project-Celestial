#include "stdafx.h"
#include "GUITextBox.h"
#include "CelestialMath.h"
#include "KeyCodes.h"

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

}

unsigned int GUITextBox::GetCaretPosition() const
{

	return textContainer->GetCaretPos();

}

void GUITextBox::Update(Message* mess)
{

	if (mess->type == MessageType_OBJECT)
	{

		std::string stringParam;
		unsigned char keys = mess->params[0];
		bool shift = false;

		switch (mess->mess)
		{

		case ObjectMess_SETTEXT:
			stringParam = string((char*)mess->params);
			textContainer->SetText(stringParam);
			break;
		case ObjectMess_APPENDTEXT:
			stringParam = string((char*)mess->params);
			textContainer->AppendText(stringParam);
			break;
		case ObjectMess_APPENDTEXTLINE:
			stringParam = string((char*)mess->params);
			textContainer->AddTextLine(stringParam);
			break;
		case ObjectMess_HANDLECHAR:
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
		case ObjectMess_HANDLEKEY:

			for (unsigned char i = 0; i < keys; i+=2)
			{

				if (mess->params[i + 1] == CelestialKeyCategories_SPEC)
				{

					if (mess->params[i + 2] == CelestialSpecKeyCodes_RGHT)
					{
						textContainer->MoveCaret(0, 1);
					}

					if (mess->params[i + 2] == CelestialSpecKeyCodes_LFT)
					{
						textContainer->MoveCaret(0, -1);
					}

					if (mess->params[i + 2] == CelestialSpecKeyCodes_UP)
					{
						textContainer->MoveCaret(-1, 0);
					}

					if (mess->params[i + 2] == CelestialSpecKeyCodes_DWN)
					{
						textContainer->MoveCaret(1, 0);
					}
				}

			}
		default:
			GUIObject::Update(mess);

		}
	}
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
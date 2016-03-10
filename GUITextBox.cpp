#include "stdafx.h"
#include "GUITextBox.h"
#include "CelestialMath.h"

using namespace std;
using namespace Resources;

GUITextBox::GUITextBox(GUISnap h, GUISnap v) : GUIObject()
{

	enabled = false;
	hSnap = h;
	vSnap = v;
	type = GUIObjects_TEXTBOX;
	textContainer = new TextContainer();

}

void GUITextBox::Update(Message* mess)
{

	if (mess->type == MessageType_OBJECT)
	{

		std::string stringParam;
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
		default:
			PositionableObject::Update(mess);

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
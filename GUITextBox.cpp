#include "stdafx.h"
#include "GUITextBox.h"
#include "CelestialMath.h"

using namespace std;
using namespace Resources;

GUITextBox::GUITextBox(bool multi, GUISnap h, GUISnap v) : TextContainer(!multi ? 1 : 100) , GUIObject()
{

	enabled = false;
	isMulti = multi;
	hSnap = h;
	vSnap = v;
	type = GUIObjects_TEXTBOX;

}

bool GUITextBox::IsMultiLine()
{

	return isMulti;

}

int GUITextBox::GetTextLines()
{
	
	return lines;

}

float GUITextBox::GetAngle()
{

	return (3.0f*CelestialMath::CELESTIAL_PI)*0.5f;

}

void GUITextBox::SetText(int line, string text)
{

	if (line < lines)
	{

		textLines[line] = text;

	}
}

GUITextBox::~GUITextBox()
{

}
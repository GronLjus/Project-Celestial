#include "stdafx.h"
#include "GUIObject.h"

using namespace Resources;
using namespace CelestialMath;

GUIObject::GUIObject() : PositionableObject()
{ 

	parentGUI = 0;
	borderBrush = 0;
	contentBrush = 0;

}

void GUIObject::Update(Message* mess)
{

	if (mess->type == MessageType_OBJECT)
	{

		unsigned char hor;
		GUISnap snap;

		switch (mess->mess)
		{

		case ObjectMess_SETSNAP:
			hor = mess->params[0];
			snap = GUISnap(mess->params[1]);

			if (hor == 0)
			{

				hSnap = snap;

			}
			else
			{

				vSnap = snap;

			}

			break;
		default:
			PositionableObject::Update(mess);

		}
	}
}

GUISnap GUIObject::GetHorizontalSnap() const
{ 

	return hSnap; 

}

GUISnap GUIObject::GetVerticalSnap() const
{ 
	
	return vSnap; 

}

GUIObjects GUIObject::GetType() const
{
	
	return type; 

}

unsigned int GUIObject::GetParentID() const
{ 
	
	return parentGUI; 

}

void GUIObject::SetParentID(unsigned int parent, unsigned int childId)
{ 
	
	parentGUI = parent; 
	this->childId = childId; 

}

void GUIObject::ToggleVisibility(bool visible)
{

	isVisible = visible;

}

bool GUIObject::IsVisible()
{

	return isVisible;

}

unsigned int GUIObject::GetContentBrush() const
{

	return contentBrush;

}

void GUIObject::SetContentBrush(unsigned int brush)
{

	contentBrush = brush;

}

unsigned int GUIObject::GetBorderBrush() const
{

	return borderBrush;

}

void GUIObject::SetBorderBrush(unsigned int brush)
{

	borderBrush = brush;

}

GUIObject::~GUIObject()
{ 


}
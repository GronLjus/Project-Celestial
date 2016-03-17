#include "stdafx.h"
#include "GUIObject.h"
#include <thread>

using namespace Resources;
using namespace CelestialMath;

GUIObject::GUIObject() : PositionableObject()
{ 

	parentGUI = nullptr;
	borderBrush = 0;
	contentBrush = 0;
	enabled = false;
	pause = false;
	paused = false;
	isVisible = true;

}

void GUIObject::ShouldPause()
{

	while (pause)
	{

		std::this_thread::yield();
		paused = true;

	}
}

void GUIObject::pauseRendering()
{

	paused = false;
	pause = true;

	while (!paused)
	{
		
		std::this_thread::yield();

	}
}

void GUIObject::resumeRendering()
{

	pause = false;

}

void GUIObject::Enable()
{

	enabled = true;

}

void GUIObject::Disable()
{

	enabled = true;

}

void GUIObject::Update(Message* mess)
{

	if (mess->type == MessageType_OBJECT)
	{

		unsigned char hor;
		GUISnap snap;
		unsigned char tempBuff[]{childId >> 0, childId >> 8, childId >> 16, childId >> 24};

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
		case ObjectMess_HIDE:
			isVisible = false;
			break;
		case ObjectMess_SHOW:
			isVisible = true;
			break;
		case ObjectMess_REMOVE:
			
			if (parentGUI != nullptr)
			{

				mess->mess = ObjectMess_REMOVECHILD;
				mess->SetParams(tempBuff, 0, 4);
				parentGUI->Update(mess);

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
	
	return parentGUI->GetId();

}

void GUIObject::SetParent(GUIObject* parent, unsigned int childId)
{ 
	
	this->parentGUI = parent;
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

bool GUIObject::IsEnabled()
{

	return enabled;

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
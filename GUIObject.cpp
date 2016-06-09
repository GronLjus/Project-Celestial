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
	focused = false;
	target = new ScreenTarget(Vector4(0, 0, 0, 0));

}

void GUIObject::UpdateScreenTarget()
{


	if (killMessage == nullptr)
	{

		killMessage = new Message();
		unsigned char tempBuff[]{ target->GetId() >> 0, target->GetId() >> 8, target->GetId() >> 16, target->GetId() >> 24 };
		killMessage->SetParams(tempBuff, 0, 4);
		killMessage->destination = MessageSource_RESOURCES;
		killMessage->type = MessageType_RESOURCES;
		killMessage->mess = ResourceMess_UNLOADOBJECT;
		killMessage->read = false;

	}
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

void GUIObject::SetFocus(bool focus)
{
	
	this->focused = focus;

}

bool GUIObject::GetFocus() const
{

	return focused;

}

void GUIObject::resumeRendering()
{

	pause = false;

}

void GUIObject::Enable()
{

	enabled = true;
	target->SetVisible(isVisible);

}

void GUIObject::Disable()
{

	enabled = false;
	target->SetVisible(false);

}

void GUIObject::Update(Message* mess)
{

	if (mess->type == MessageType_OBJECT)
	{

		unsigned char hor;
		GUISnap snap;
		unsigned char tempBuff[]{childId >> 0, childId >> 8, childId >> 16, childId >> 24};
		Vector2 pos = Vector2(GetPosition().x,GetPosition().y);
		Vector3 scale = GetScale();

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
			target->SetVisible(false);
			break;
		case ObjectMess_SHOW:
			isVisible = true;
			target->SetVisible(enabled);
			break;
		case ObjectMess_REMOVE:
			
			if (parentGUI != nullptr)
			{

				mess->mess = ObjectMess_REMOVECHILD;
				mess->SetParams(tempBuff, 0, 4);
				parentGUI->Update(mess);
				target->SetVisible(false);

			}

			break;
		case ObjectMess_LOCKMOUSE:
		case ObjectMess_LOCKINPUT:

			if (mess->params[0] == 0)
			{

				target->Lock();
			}
			else
			{

				target->Unlock();

			}

			break;
		case ObjectMess_MOVE:
		case ObjectMess_SCALE:
		case ObjectMess_POS:
		case ObjectMess_SIZE:
			PositionableObject::Update(mess);
			pos = GetTopLeft();
			scale = GetScale();
			target->Refresh(Vector4(pos.x ,pos.y,scale.x,scale.y));
			break;
		case ObjectMess_SETENTRSCRPT:
		case ObjectMess_SETEXTSCRPT:
		case ObjectMess_SETHVRSCRPT:
		case ObjectMess_SETLCSCRPT:
		case ObjectMess_SETMCSCRPT:
		case ObjectMess_SETRCSCRPT:
		case ObjectMess_SETLDSCRPT:
		case ObjectMess_SETMDSCRPT:
		case ObjectMess_SETRDSCRPT:
		case ObjectMess_SETWHLSCRPT:
		case ObjectMess_SETUPDWNSCRPT:
		case ObjectMess_SETSCRPTTRGT:
			target->Update(mess);
			break;
		case ObjectMess_INCREMENTLAYER:
		case ObjectMess_DECREMENTLAYER:
		case ObjectMess_SETLAYER:
			PositionableObject::Update(mess);
			target->SetLayer(GetLayer());
			break;
		default:
			PositionableObject::Update(mess);
			break;

		}
	}
}

Vector2 GUIObject::GetTopLeft() const
{

	Vector2 parentTopLeft(0, 0);
	Vector2 objSize = Vector2(GetScale().x, GetScale().y);
	Vector3 parentSize(objSize,0);
	Vector2 parentMidPoint = objSize/2;

	if (parentGUI != nullptr)
	{

		parentTopLeft = parentGUI->GetTopLeft();
		parentSize = parentGUI->GetScale();
		parentMidPoint = parentTopLeft + Vector2(parentSize.x / 2,parentSize.y / 2);

	}

	return Vector2(
		GetHorizontalSnap() == GUISnap_LEFT ? parentTopLeft.x + GetPosition().x :
		GetHorizontalSnap() == GUISnap_MIDDLE ? parentMidPoint.x - objSize.x*0.5f :
		parentTopLeft.x + parentSize.x - (GetPosition().x + GetScale().x),

		GetVerticalSnap() == GUISnap_TOP ? parentTopLeft.y + GetPosition().y :
		GetVerticalSnap() == GUISnap_MIDDLE ? parentMidPoint.y - objSize.y*0.5f :
		parentTopLeft.y + parentSize.y - (GetPosition().y + GetScale().y));

}

void GUIObject::SetLayer(unsigned char layer)
{

	setLayer(layer);
	target->SetLayer(layer);

}

void GUIObject::SetId(unsigned int id)
{

	target->SetTargetId(id);
	PositionableObject::SetId(id);

}

void GUIObject::toggleScreenTarget(bool enabled)
{

	if (enabled)
	{


		target->Lock();

	}
	else
	{


		target->Unlock();

	}
}

ScreenTarget* GUIObject::GetScreenTarget() const
{

	return target;

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

unsigned int GUIObject::GetParentId() const
{
	
	return GetParentID();

}

void GUIObject::SetParent(GUIObject* parent, unsigned int childId)
{ 
	
	setLayer(parent->GetLayer() + 1);
	target->SetLayer(GetLayer());
	this->parentGUI = parent;
	this->childId = childId; 

}

void GUIObject::ToggleVisibility(bool visible)
{

	isVisible = visible;
	target->SetVisible(visible && enabled);

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

	if (killMessage != nullptr)
	{

		delete killMessage;
		killMessage = nullptr;

	}

}
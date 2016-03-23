#include "stdafx.h"
#include "ScreenTarget.h"

using namespace Resources;
using namespace CelestialMath;
using namespace CrossHandlers;

ScreenTarget::ScreenTarget(Vector4 target) : PositionableObject(Vector3(target.x,target.y,0),Vector3(target.z,target.w,0))
{

	hoverScript = 0;
	enterScript = 0;
	exitScript = 0;
	isHovering = true;
	isVisible = true;
	locked = true;
	remove = false;
	targetId = 0;

}

void ScreenTarget::Update(Message* mess)
{

	if (mess->type == MessageType_OBJECT)
	{
		switch (mess->mess)
		{

		case ObjectMess_SETENTRSCRPT:
			enterScript = mess->params[0] | ((int)mess->params[1] << 8) | ((int)mess->params[2] << 16) | ((int)mess->params[3] << 24);
			break;
		case ObjectMess_SETEXTSCRPT:
			exitScript = mess->params[0] | ((int)mess->params[1] << 8) | ((int)mess->params[2] << 16) | ((int)mess->params[3] << 24);
			break;
		case ObjectMess_SETHVRSCRPT:
			hoverScript = mess->params[0] | ((int)mess->params[1] << 8) | ((int)mess->params[2] << 16) | ((int)mess->params[3] << 24);
			break;
		default:
			PositionableObject::Update(mess);
		}
	}
}

void ScreenTarget::SetLayer(unsigned char layer)
{

	setLayer(layer);

}

unsigned int ScreenTarget::GetTargetId() const
{

	return targetId;

}

void ScreenTarget::SetTargetId(unsigned int id)
{

	targetId = id;

}

void ScreenTarget::Lock()
{

	locked = true;

}

void ScreenTarget::Unlock()
{

	locked = false;

}

bool ScreenTarget::ShouldRemove() const
{

	return remove;

}

void ScreenTarget::Remove()
{

	remove = true;

}

bool ScreenTarget::IsVisible() const
{

	return isVisible && !locked;

}

void ScreenTarget::SetVisible(bool vis)
{

	isVisible = vis;

}

void ScreenTarget::Refresh(CelestialMath::Vector4 target)
{

	refresh(Vector3(target.x, target.y, 0), Vector3(target.z, target.w, 0));
	
}

unsigned int ScreenTarget::GetHoverScript() const
{

	return hoverScript;

}

unsigned int ScreenTarget::GetEnterScript() const
{

	return enterScript;

}

unsigned int ScreenTarget::GetExitScript() const
{

	return exitScript;

}

bool ScreenTarget::IsHovering() const
{

	return isHovering;

}

void ScreenTarget::SetHovering(bool val)
{

	isHovering = val;

}
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
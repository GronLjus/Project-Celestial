#include "stdafx.h"
#include "ScriptableObject.h"

using namespace Resources;
using namespace CrossHandlers;


ScriptableObject::ScriptableObject()
{

	rightClickScript = 0;
	leftClickScript = 0;
	middleClickScript = 0;

	rightDragScript = 0;
	leftDragScript = 0;
	middleDragScript = 0;

	wheelScript = 0;
	upDownScript = 0;

}


void ScriptableObject::Update(Message* mess)
{

	if (mess->type == MessageType_OBJECT)
	{

		unsigned int param1 = mess->params[0] | ((int)mess->params[1] << 8) | ((int)mess->params[2] << 16) | ((int)mess->params[3] << 24);

		switch (mess->mess)
		{
		case ObjectMess_SETRCSCRPT:
			rightClickScript = param1 + 1;
			break;
		case ObjectMess_SETMCSCRPT:
			middleClickScript = param1 + 1;
			break;
		case ObjectMess_SETLCSCRPT:
			leftClickScript = param1 + 1;
			break;
		case ObjectMess_SETRDSCRPT:
			rightDragScript = param1 + 1;
			break;
		case ObjectMess_SETMDSCRPT:
			middleDragScript = param1 + 1;
			break;
		case ObjectMess_SETLDSCRPT:
			leftDragScript = param1 + 1;
			break;
		case ObjectMess_SETWHLSCRPT:
			wheelScript = param1 + 1;
			break;
		case ObjectMess_SETUPDWNSCRPT:
			upDownScript = param1 + 1;
			break;
		}
	}
}

unsigned int ScriptableObject::GetWheelScript() const
{

	return wheelScript;

}

unsigned int ScriptableObject::GetRightClickScript() const
{

	return rightClickScript;

}

unsigned int ScriptableObject::GetMiddleClickScript() const
{

	return middleClickScript;

}

unsigned int Resources::ScriptableObject::GetRightDragScript() const
{

	return rightDragScript;

}

unsigned int Resources::ScriptableObject::GetLeftDragScript() const
{

	return leftDragScript;

}

unsigned int Resources::ScriptableObject::GetMiddleDragScript() const
{

	return middleDragScript;

}

unsigned int ScriptableObject::GetUpdownScript() const
{

	return upDownScript;

}

unsigned int ScriptableObject::GetLeftClickScript() const
{

	return leftClickScript;

}
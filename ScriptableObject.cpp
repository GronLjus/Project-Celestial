#include "stdafx.h"
#include "ScriptableObject.h"

using namespace Resources;
using namespace CrossHandlers;


ScriptableObject::ScriptableObject()
{

	rightClickScript = 0;
	leftClickScript = 0;
	middleClickScript = 0;

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
		}
	}
}
unsigned int ScriptableObject::GetRightClickScript() const
{

	return rightClickScript;

}

unsigned int ScriptableObject::GetMiddleClickScript() const
{

	return middleClickScript;

}

unsigned int ScriptableObject::GetLeftClickScript() const
{

	return leftClickScript;

}
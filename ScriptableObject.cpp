#include "stdafx.h"
#include "ScriptableObject.h"

using namespace Resources;
using namespace CrossHandlers;


ScriptableObject::ScriptableObject() : SerializableObject(), BaseObject()
{

	rightClickScript = 0;
	leftClickScript = 0;
	middleClickScript = 0;

	rightDragScript = 0;
	leftDragScript = 0;
	middleDragScript = 0;

	wheelScript = 0;
	upDownScript = 0;

	travelArrivedNodeScript = 0;
	serial = 36;

}

bool ScriptableObject::TranslateScripts(CrossHandlers::Dictionary* dict)
{

	if (rightClickScript != 0)
	{

		unsigned int newScript = dict->GetTranslation(rightClickScript - 1);

		if (newScript == 0)
		{

			return false;

		}

		rightClickScript = newScript + 1;

	}

	if (leftClickScript != 0)
	{

		unsigned int newScript = dict->GetTranslation(leftClickScript - 1);

		if (newScript == 0)
		{

			return false;

		}

		leftClickScript = newScript + 1;

	}

	if (middleClickScript != 0)
	{

		unsigned int newScript = dict->GetTranslation(middleClickScript - 1);

		if (newScript == 0)
		{

			return false;

		}

		middleClickScript = newScript + 1;

	}

	if (rightDragScript != 0)
	{

		unsigned int newScript = dict->GetTranslation(rightDragScript - 1);

		if (newScript == 0)
		{

			return false;

		}

		rightDragScript = newScript + 1;

	}

	if (leftDragScript != 0)
	{

		unsigned int newScript = dict->GetTranslation(leftDragScript - 1);

		if (newScript == 0)
		{

			return false;

		}

		leftDragScript = newScript + 1;

	}

	if (middleDragScript != 0)
	{

		unsigned int newScript = dict->GetTranslation(middleDragScript - 1);

		if (newScript == 0)
		{

			return false;

		}

		middleDragScript = newScript + 1;

	}

	if (wheelScript != 0)
	{

		unsigned int newScript = dict->GetTranslation(wheelScript - 1);

		if (newScript == 0)
		{

			return false;

		}

		wheelScript = newScript + 1;

	}

	if (upDownScript != 0)
	{

		unsigned int newScript = dict->GetTranslation(upDownScript - 1);

		if (newScript == 0)
		{

			return false;

		}

		upDownScript = newScript + 1;

	}

	if (travelArrivedNodeScript != 0)
	{

		unsigned int newScript = dict->GetTranslation(travelArrivedNodeScript - 1);

		if (newScript == 0)
		{

			return false;

		}

		travelArrivedNodeScript = newScript + 1;

	}

	return true;

}

char* ScriptableObject::Unserialize(char* data)
{

	memcpy(&rightClickScript, &data[0], 4);
	memcpy(&leftClickScript, &data[4], 4);
	memcpy(&middleClickScript, &data[8], 4);

	memcpy(&rightDragScript, &data[12], 4);
	memcpy(&leftDragScript, &data[16], 4);
	memcpy(&middleDragScript, &data[20], 4);

	memcpy(&wheelScript, &data[24], 4);
	memcpy(&upDownScript, &data[28], 4);
	memcpy(&travelArrivedNodeScript, &data[32], 4);
	return nullptr;

}

char* ScriptableObject::Serialize(unsigned int &size)
{

	size = serial + 1;
	char* byteVal = new char[size];

	byteVal[0] = SerializableType_SCRIPTABLE;

	memcpy(&byteVal[1], &rightClickScript, sizeof(unsigned int));
	memcpy(&byteVal[5], &leftClickScript, sizeof(unsigned int));
	memcpy(&byteVal[9], &middleClickScript, sizeof(unsigned int));

	memcpy(&byteVal[13], &rightDragScript, sizeof(unsigned int));
	memcpy(&byteVal[17], &leftDragScript, sizeof(unsigned int));
	memcpy(&byteVal[21], &middleDragScript, sizeof(unsigned int));

	memcpy(&byteVal[25], &wheelScript, sizeof(unsigned int));
	memcpy(&byteVal[29], &upDownScript, sizeof(unsigned int));
	memcpy(&byteVal[33], &travelArrivedNodeScript, sizeof(unsigned int));

	return byteVal;

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
		case ObjectMess_SETTRVLNDESCRPT:
			travelArrivedNodeScript = param1 + 1;
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

unsigned int ScriptableObject::GetTravelNodeScript() const
{

	return travelArrivedNodeScript;

}
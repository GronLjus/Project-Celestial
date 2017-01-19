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
	splitNodeScript = 0;

	serial = 40;

	currentTriggers = 0;
	maxTriggers = 0;
	triggers = nullptr;

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

	if (currentTriggers > 0)
	{

		for (unsigned int i = 0; i < currentTriggers; i++)
		{

			unsigned int newScript = dict->GetTranslation(triggers[i].script);

			if (newScript == 0)
			{

				return false;

			}

			trigger trg = triggers[i];
			trg.script = newScript;
			triggers[i] = trg;

		}

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

	memcpy(&currentTriggers, &data[36], 4);

	maxTriggers = currentTriggers;

	if (maxTriggers > 0)
	{

		triggers = new trigger[maxTriggers];
		unsigned int offset = 40;

		for (unsigned int i = 0; i < currentTriggers; i++)
		{

			memcpy((&triggers[i].cat), &data[offset], 1);
			offset++;
			memcpy((&triggers[i].code), &data[offset], 1);
			offset++;
			memcpy((&triggers[i].script), &data[offset], 4);
			offset+=4;

		}
	}
	return nullptr;

}

char* ScriptableObject::Serialize(unsigned int &size)
{

	size = serial + currentTriggers * 6 + 1;

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

	memcpy(&byteVal[37], &currentTriggers, sizeof(unsigned int));

	unsigned int offset = 38;

	for (unsigned int i = 0; i < currentTriggers; i++)
	{

		memcpy(&byteVal[offset], &(triggers[i].cat), 1);
		offset++;
		memcpy(&byteVal[offset], &(triggers[i].code), 1);
		offset++;
		memcpy(&byteVal[offset], &(triggers[i].script), sizeof(unsigned int));
		offset += 4;


	}

	return byteVal;

}

void ScriptableObject::Update(Message* mess)
{

	if (mess->type == MessageType_OBJECT)
	{

		unsigned int param1 = mess->params[0] | ((int)mess->params[1] << 8) | ((int)mess->params[2] << 16) | ((int)mess->params[3] << 24);
		unsigned int param2 = mess->params[4] | ((int)mess->params[5] << 8) | ((int)mess->params[6] << 16) | ((int)mess->params[7] << 24);
		trigger trg;
		

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
		case ObjectMess_SETSPLTSCRPT:
			splitNodeScript = param1 + 1;
			break;
		case ObjectMess_ADDKEYSCRPT:
			trg.cat = Input::GetCat(param2);
			trg.code = Input::GetCode(param2, trg.cat);
			trg.script = param1 + 1;
			addTrigger(trg);
			break;
		}
	}
}

void ScriptableObject::addTrigger(trigger trigg)
{

	if (currentTriggers == maxTriggers)
	{

		maxTriggers += 5;

		trigger* newTriggers = new trigger[maxTriggers];

		if (triggers != nullptr)
		{

			memcpy(newTriggers, triggers, currentTriggers * 6);
			delete[] triggers;

		}

		triggers = newTriggers;

	}

	triggers[currentTriggers] = trigg;
	currentTriggers++;

}

unsigned int ScriptableObject::GetKeyScript(Input::CelestialKeyCategories cat, unsigned char code) const
{

	for (unsigned int i = 0; i < currentTriggers; i++)
	{

		if (triggers[i].cat == cat &&
			triggers[i].code == code)
		{

			return triggers[i].script;

		}
	}

	return 0;

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

unsigned int ScriptableObject::GetSplitNodeScript() const
{

	return splitNodeScript;

}

ScriptableObject::~ScriptableObject()
{

	if (triggers != nullptr)
	{

		delete[] triggers;

	}
}
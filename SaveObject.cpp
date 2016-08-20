#include "stdafx.h"
#include "SaveObject.h"

using namespace Resources;
using namespace CrossHandlers;

SaveObject::SaveObject()
{

	save = true;

}

void SaveObject::setSaveState(bool save)
{

	this->save = save;

}

bool SaveObject::ShouldSave() const
{

	return save;

}

bool SaveObject::UpdateSaveObject(Message* mess)
{

	bool retVal = false;

	if (mess->type == MessageType_OBJECT)
	{

		switch (mess->mess)
		{

			case ObjectMess_SAVESCOPE:
				retVal = true;
				save = mess->params[0] == 1;

		}
	}

	return retVal;

}
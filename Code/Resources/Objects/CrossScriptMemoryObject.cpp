#include "../../../stdafx.h"
#include "CrossScriptMemoryObject.h"

using namespace std;
using namespace Resources;

CrossScriptMemoryObject::CrossScriptMemoryObject()
{

	maxObjects = 20;
	totalObjects = 0;
	objects = new MemObject[maxObjects];

}

unsigned int CrossScriptMemoryObject::getIndex(string val, unsigned int start, unsigned int end)
{

	unsigned int middle = start + (end - start) / 2;

	if (middle == start || objects[middle].name.compare(val) == 0)
	{

		if (objects[middle].name.compare(val) > 0)
		{

			return middle + 1;

		}

		return middle;

	}

	if (objects[middle].name.compare(val) > 0)
	{

		return getIndex(val, middle, end);

	}
	else
	{

		return getIndex(val, start, middle);

	}
}

void CrossScriptMemoryObject::addMem(MemObject object, unsigned int index)
{

	MemObject* addObjects = objects;

	if (totalObjects + 1 >= maxObjects)
	{

		maxObjects += 20;
		addObjects = new MemObject[maxObjects];

		for (unsigned int i = 0; i < index; i++)
		{

			addObjects[i] = objects[i];
			objects[i].value = nullptr;

		}
	}

	totalObjects++;

	for (unsigned int i = totalObjects; i > index; i--)
	{

		addObjects[i] = objects[i - 1];

		if (addObjects != objects)
		{

			objects[i].value = nullptr;

		}
	}

	addObjects[index] = object;

	if (addObjects != objects)
	{

		delete[] objects;
		objects = addObjects;

	}
}

void CrossScriptMemoryObject::SetMem(string name, unsigned char* value, unsigned int valLength)
{

	unsigned int index = getIndex(name, 0, totalObjects);

	if (objects[index].name == name)
	{

		if (objects[index].totalLength < valLength)
		{

			delete[] objects[index].value;
			objects[index].totalLength = valLength;
			objects[index].value = new unsigned char[valLength];

		}

		memcpy(objects[index].value, value, valLength);
		objects[index].valLength = valLength;

	}
	else
	{
		MemObject obj;
		obj.name = name;
		obj.valLength = valLength;
		obj.totalLength = valLength;

		addMem(obj, index);
		objects[index].value = new unsigned char[valLength];

		memcpy(objects[index].value, value, valLength);

	}
}

unsigned char* CrossScriptMemoryObject::GetMem(string name, unsigned int &length)
{

	unsigned int index = getIndex(name, 0, totalObjects);

	if (objects[index].name == name)
	{

		length = objects[index].valLength;
		return objects[index].value;

	}
	else
	{

		return nullptr;

	}
}

CrossScriptMemoryObject::~CrossScriptMemoryObject()
{
	
	delete[] objects;

}
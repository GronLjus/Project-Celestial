#include "stdafx.h"
#include "ResourceDictionary.h"

using namespace Resources;
using namespace CrossHandlers;

ResourceDictionary::ResourceDictionary()
{

	resourceDictionary = new CelestialSlicedList<resourceId*>(32, nullptr);

}

void ResourceDictionary::AddResource(unsigned int id, std::string name)
{

	resourceId* rId = new resourceId();
	rId->id = id;
	rId->name = name;
	resourceDictionary->Add(rId);

}

void ResourceDictionary::RemoveResource(unsigned int id)
{

	for (unsigned int i = 0; i < resourceDictionary->GetHighest(); i++)
	{

		resourceId* rId = resourceDictionary->GetValue(i);

		if (rId != nullptr && rId->id == id)
		{

			resourceDictionary->Remove(i);
			return;

		}
	}
}

unsigned int ResourceDictionary::FindId(std::string name) const
{

	for (unsigned int i = 0; i < resourceDictionary->GetHighest(); i++)
	{

		resourceId* rId = resourceDictionary->GetValue(i);

		if (rId != nullptr && rId->name == name)
		{

			return rId->id;

		}
	}

	return 0;

}

Dictionary* ResourceDictionary::Translate(ResourceDictionary* rd)
{

	Dictionary* translation = new Dictionary();

	for (unsigned int i = 0; i < resourceDictionary->GetHighest(); i++)
	{

		resourceId* rId = resourceDictionary->GetValue(i);

		if (rId != nullptr)
		{

			unsigned int otherId = rd->FindId(rId->name);
			translation->AddTranslation(rId->id, otherId);

		}
	}

	return translation;

}

//Serializable Interface
char* ResourceDictionary::Serialize(unsigned int &size)
{

	char** subSers = new char*[resourceDictionary->GetHighest()];
	unsigned int* subSize = new unsigned int[resourceDictionary->GetHighest()];
	unsigned int subTot = 0;
	unsigned int totalSize = 0;

	for (unsigned int i = 0; i < resourceDictionary->GetHighest(); i++)
	{

		resourceId* rId = resourceDictionary->GetValue(i);

		if (rId != nullptr)
		{

			subSize[subTot] = sizeof(unsigned int) + rId->name.length() + 1;
			char* local = new char[subSize[subTot]];
			memcpy(local, &rId->id, 4);
			memcpy(&local[4], (rId->name.c_str()), rId->name.length() + 1);
			totalSize += subSize[subTot];
			subSers[subTot] = local;
			subTot++;

		}
	}

	size = totalSize + 4 + 1;
	char* byteVal = new char[size];
	byteVal[0] = SerializableType_RESOURCEDICTIONARY;

	unsigned int offset = 1;

	for (unsigned i = 0; i < subTot; i++)
	{

		memcpy(&byteVal[offset], subSers[i], subSize[i]);
		delete[] subSers[i];
		offset += subSize[i];

	}

	unsigned tail = 0;
	delete[] subSers;
	delete[] subSize;

	memcpy(&byteVal[offset], &tail, 4);
	return byteVal;

}

char* ResourceDictionary::Unserialize(char* data)
{

	unsigned int offset = 0;
	unsigned int id = 0;
	memcpy(&id, data, 4);

	while (id != 0)
	{

		resourceId* rId = new resourceId();
		rId->id = id,
		offset += 4;
		rId->name = std::string(&data[offset]);
		resourceDictionary->Add(rId);
		offset += rId->name.length()+1;
		memcpy(&id, &data[offset], 4);

	}

	return nullptr;

}

ResourceDictionary::~ResourceDictionary()
{

	resourceDictionary->KillList();
	delete resourceDictionary;

}
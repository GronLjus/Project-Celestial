#include "stdafx.h"
#include "ResourceContainer.h"

using namespace Resources;
using namespace CrossHandlers;

ResourceContainer::ResourceContainer()
{
	
	theList = new CelestialSlicedList<IContainable*>(50,nullptr);

}

int ResourceContainer::AddObject(IContainable* object)
{

	int id = theList->Add(object);
	object->SetId(id);
	return id;

}

void ResourceContainer::DeleteObject(int id)
{

	theList->Kill(id);

}

IContainable* ResourceContainer::GetDataObject(int id)
{

	return theList->GetSlice(id)[theList->TransformId(id)];

}

ResourceContainer::~ResourceContainer()
{

	theList->KillList();
	delete theList;

}
#include "stdafx.h"
#include "GameObject.h"

#include <algorithm>

using namespace Resources;
using namespace CrossHandlers;
using namespace CelestialMath;

GameObject::GameObject(BoundingBox* box, BoundingSphere* baseSphere, unsigned int meshId, std::string name) : PositionableObject(), SaveObject()
{

	this->box = box;
	this->sphere = baseSphere;
	this->mesh = meshId;
	flipInit = false;
	parent = nullptr;

	this->objectName = name;
	collisionFilter = " ";

}

GameObject::GameObject() : PositionableObject(), SaveObject()
{

	flipInit = false;
	parent = nullptr;

	this->box = nullptr;
	this->sphere = nullptr;
	this->mesh = 0;

	this->objectName = "nameless";
	collisionFilter = " ";

}

void GameObject::SetMesh(CrossHandlers::BoundingBox* baseBox, CrossHandlers::BoundingSphere* baseSphere, unsigned int meshId)
{

	this->box = baseBox;
	this->sphere = baseSphere;
	this->mesh = meshId;

}

char* GameObject::Unserialize(char* data)
{

	memcpy(&mesh, data, 4);

	if (data[4] == SerializableType_POSITIONABLE)
	{

		return PositionableObject::Unserialize(&data[5]);

	}

	return nullptr;

}

char* GameObject::Serialize(unsigned int &size)
{

	unsigned int subSize = 0;
	char* subSerial = PositionableObject::Serialize(subSize);
	size = subSize + 5;
	char* byteVal = new char[size];

	byteVal[0] = SerializableType_GAMEOBJECTSCENERY;
	byteVal[1] = mesh << 0;
	byteVal[2] = mesh << 8;
	byteVal[3] = mesh << 16;
	byteVal[4] = mesh << 24;

	memcpy(&byteVal[5], subSerial, subSize);

	delete[] subSerial;
	return byteVal;

}

void GameObject::SetParent(BaseObject* parent)
{

	this->parent = parent;

}

GameObjectType GameObject::GetType() const
{
	
	return GameObjectType_SCENERY;

}

std::string GameObject::GetCollisionFilter() const
{

	return collisionFilter;

}

std::string GameObject::GetObjectName() const
{

	return objectName;

}

void GameObject::Update(Message* mess)
{

	if (mess->type == MessageType_OBJECT)
	{

		unsigned char tempBuff[]{GetId() >> 0, GetId() >> 8, GetId() >> 16, GetId() >> 24};

		switch (mess->mess)
		{

		case ObjectMess_SETCOLLFILTER:
			collisionFilter = std::string((char*)(mess->params));
			collisionFilter.erase(std::remove(collisionFilter.begin(), collisionFilter.end(), ' '), collisionFilter.end());
			break;
		case ObjectMess_REMOVE:

			if (parent != nullptr)
			{

				mess->mess = ObjectMess_REMOVECHILD;
				mess->SetParams(tempBuff, 0, 4);
				parent->Update(mess);

			}

			break;
		default:
			if (!UpdateSaveObject(mess))
			{

				PositionableObject::Update(mess);

			}
		}
	}
}

bool GameObject::IsFlipBuffered(unsigned char flip)
{

	if (!flipInit || flip != flipBuffered)
	{

		flipInit = true;
		flipBuffered = flip;
		return false;

	}

	return true;

}

Matrix GameObject::PopTransformation()
{
	
	lastMatrix = this->GetMatrix();
	return lastMatrix;

}

Matrix GameObject::GetLastTransformation() const
{

	return lastMatrix;

}

unsigned int GameObject::GetMeshId() const
{

	return mesh;

}

unsigned int GameObject::GetScriptId() const
{

	return script;

}

BoundingBox* GameObject::GetBox()
{

	if (hasMatrixChanged())
	{

		box->Transform(GetMatrix());
		sphere->Transform(GetMatrix());

	}

	return box;

}

BoundingSphere* GameObject::GetSphere()
{

	if (hasMatrixChanged())
	{

		box->Transform(GetMatrix());
		sphere->Transform(GetMatrix());

	}

	return sphere;

}

GameObject::~GameObject()
{

	delete box;
	delete sphere;

}
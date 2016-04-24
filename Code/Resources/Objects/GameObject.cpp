#include "stdafx.h"
#include "GameObject.h"

using namespace Resources;
using namespace CrossHandlers;
using namespace CelestialMath;

GameObject::GameObject(BoundingBox* box, BoundingSphere* baseSphere, unsigned int meshId) : PositionableObject()
{

	this->box = box;
	this->sphere = baseSphere;
	this->mesh = meshId;
	flipInit = false;
	parent = nullptr;

}

void GameObject::SetParent(BaseObject* parent)
{

	this->parent = parent;

}

void GameObject::Update(Message* mess)
{

	if (mess->type == MessageType_OBJECT)
	{

		unsigned char tempBuff[]{GetId() >> 0, GetId() >> 8, GetId() >> 16, GetId() >> 24};

		switch (mess->mess)
		{

		case ObjectMess_REMOVE:

			if (parent != nullptr)
			{

				mess->mess = ObjectMess_REMOVECHILD;
				mess->SetParams(tempBuff, 0, 4);
				parent->Update(mess);

			}

			break;
		default:
			PositionableObject::Update(mess);

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

BoundingBox* GameObject::GetBox() const
{

	return box;

}

BoundingSphere* GameObject::GetSphere() const
{

	return sphere;

}

GameObject::~GameObject()
{

	delete box;
	delete sphere;

}
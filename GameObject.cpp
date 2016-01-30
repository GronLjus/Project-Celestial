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

}

void GameObject::Update(Message* mess)
{

	//DOSTUFF

}bool GameObject::IsFlipBuffered(unsigned char flip)
{

	if (!flipInit || flip != flipBuffered)
	{

		flipInit = true;
		flipBuffered = flip;
		return false;

	}

	return true;

}

Matrix GameObject::GetInverseTransformation() const
{

	return MatrixInverse(this->GetMatrix());

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
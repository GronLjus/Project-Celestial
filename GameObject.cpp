#include "stdafx.h"
#include "GameObject.h"

using namespace Resources;
using namespace CrossHandlers;
using namespace CelestialMath;

GameObject::GameObject(BoundingBox* box, BoundingSphere* baseSphere)
{

	this->box = box;
	this->sphere = baseSphere;

	scale = Vector3(1.0f, 1.0f, 1.0f);
	position = Vector3(0.0f, 0.0f, 0.0f);
	rotation = Vector3(0.0f, 0.0f, 0.0f);

}

void GameObject::Update(Message* mess)
{
	//DOSTUFF

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

Vector3 GameObject::GetPosition() const
{

	return position;

}

Vector3 GameObject::GetRotation() const
{

	return rotation;

}

Vector3 GameObject::GetScale() const
{

	return scale;

}

Matrix GameObject::GetMatrix() const
{

	Matrix s = MatrixScaling(scale.x, scale.y, scale.z);
	Matrix t = MatrixTranslation(position.x, position.y, position.z);
	Matrix r = MatrixRotationYawPitchRoll(rotation.y, rotation.x, rotation.z);
	Matrix transformation = MatrixMultiply(MatrixMultiply(r, t), s);

	return transformation;

}

GameObject::~GameObject()
{

	delete box;
	delete sphere;

}
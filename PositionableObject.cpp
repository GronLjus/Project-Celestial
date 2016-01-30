#include "stdafx.h"
#include "PositionableObject.h"

using namespace Resources;
using namespace CelestialMath;
using namespace CrossHandlers;

PositionableObject::PositionableObject()
{

	scale = Vector3(1.0f, 1.0f, 1.0f);
	position = Vector3(0.0f, 0.0f, 0.0f);
	rotation = Vector3(0.0f, 0.0f, 0.0f);

}

void PositionableObject::Update(Message* mess)
{

	if (mess->type == MessageType_OBJECT)
	{
		switch (mess->mess)
		{

		case ObjectMess_MOVE:
			position += Vector3(mess->param1, mess->param2, mess->param3);
			break;

		}
	}
}

Vector3 PositionableObject::GetPosition() const
{

	return position;

}

Vector3 PositionableObject::GetRotation() const
{

	return rotation;

}

Vector3 PositionableObject::GetScale() const
{

	return scale;

}

Matrix PositionableObject::GetMatrix() const
{

	Matrix s = MatrixScaling(scale.x, scale.y, scale.z);
	Matrix t = MatrixTranslation(position.x, position.y, position.z);
	Matrix r = MatrixRotationYawPitchRoll(rotation.y, rotation.x, rotation.z);
	Matrix transformation = MatrixMultiply(MatrixMultiply(r, t), s);

	return transformation;

}
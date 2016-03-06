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

	rightClickScript = 0;
	leftClickScript = 0;

}

PositionableObject::PositionableObject(Vector3 position, Vector3 scale)
{

	this->position = position;
	this->scale = scale;

}

void PositionableObject::Update(Message* mess)
{

	if (mess->type == MessageType_OBJECT)
	{

		unsigned int param1 = mess->params[0] | ((int)mess->params[1] << 8) | ((int)mess->params[2] << 16) | ((int)mess->params[3] << 24);
		unsigned int param2 = mess->params[4] | ((int)mess->params[5] << 8) | ((int)mess->params[6] << 16) | ((int)mess->params[7] << 24);
		unsigned int param3 = mess->params[8] | ((int)mess->params[9] << 8) | ((int)mess->params[10] << 16) | ((int)mess->params[11] << 24);

		switch (mess->mess)
		{

		case ObjectMess_MOVE:
			position += Vector3(param1, param2, param3);
			break;
		case ObjectMess_SCALE:
			memcpy(&scale.x, mess->params, 4);
			memcpy(&scale.y, &mess->params[4], 4);
			memcpy(&scale.z, &mess->params[8], 4);
			break;
		case ObjectMess_SETRCSCRPT:
			rightClickScript = param1+1;
			break;
		case ObjectMess_SETLCSCRPT:
			leftClickScript = param1+1;
			break;

		}
	}
}

unsigned int PositionableObject::GetRightClickScript() const
{

	return rightClickScript;

}

unsigned int PositionableObject::GetLeftClickScript() const
{

	return leftClickScript;

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
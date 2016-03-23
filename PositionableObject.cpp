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
	layer = 0;

}

PositionableObject::PositionableObject(Vector3 position, Vector3 scale)
{

	this->position = position;
	this->scale = scale;
	rotation = Vector3(0.0f, 0.0f, 0.0f);

	rightClickScript = 0;
	leftClickScript = 0;
	layer = 0;

}

void PositionableObject::refresh(Vector3 position, Vector3 scale)
{

	this->position = position;
	this->scale = scale;

}

void PositionableObject::Update(Message* mess)
{

	if (mess->type == MessageType_OBJECT)
	{

		unsigned int param1 = mess->params[0] | ((int)mess->params[1] << 8) | ((int)mess->params[2] << 16) | ((int)mess->params[3] << 24);
		Vector3 newVec;

		switch (mess->mess)
		{

		case ObjectMess_MOVE:
			memcpy(&newVec.x, mess->params, 4);
			memcpy(&newVec.y, &mess->params[4], 4);
			memcpy(&newVec.z, &mess->params[8], 4);
			position += newVec;
			break;
		case ObjectMess_SCALE:
			memcpy(&newVec.x, mess->params, 4);
			memcpy(&newVec.y, &mess->params[4], 4);
			memcpy(&newVec.z, &mess->params[8], 4);
			scale += newVec;
			break;
		case ObjectMess_POS:
			memcpy(&position.x, mess->params, 4);
			memcpy(&position.y, &mess->params[4], 4);
			memcpy(&position.z, &mess->params[8], 4);
			break;
		case ObjectMess_SIZE:
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
		case ObjectMess_INCREMENTLAYER:
			if (layer < 255)
			{

				layer++;

			}
			break;
		case ObjectMess_DECREMENTLAYER:
			if (layer > 0)
			{

				layer--;

			}
			break;
		case ObjectMess_SETLAYER:
			layer = mess->params[0];
			break;

		}
	}
}

void PositionableObject::setLayer(unsigned char layer)
{

	this->layer = layer;

}

unsigned char PositionableObject::GetLayer() const
{

	return layer;

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
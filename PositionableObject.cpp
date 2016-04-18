#include "stdafx.h"
#include "PositionableObject.h"

using namespace Resources;
using namespace CelestialMath;
using namespace CrossHandlers;

PositionableObject::PositionableObject() : ScriptableObject()
{

	scale = Vector3(1.0f, 1.0f, 1.0f);
	position = Vector3(0.0f, 0.0f, 0.0f);
	rotation = Vector3(0.0f, 0.0f, 0.0f);
	direction = Vector3(0.0f, 0.0f, 1.0f);
	layer = 0;

}

PositionableObject::PositionableObject(Vector3 position, Vector3 scale) : ScriptableObject()
{

	this->position = position;
	this->scale = scale;
	rotation = Vector3(0.0f, 0.0f, 0.0f);
	direction = Vector3(0.0f, 0.0f, 1.0f);
	layer = 0;

}

void PositionableObject::refresh(Vector3 position, Vector3 scale)
{

	this->position = position;
	this->scale = scale;

}

void PositionableObject::createMatrix()
{

	Matrix s = MatrixScaling(scale.x, scale.y, scale.z);
	Matrix t = MatrixTranslation(position.x, position.y, position.z);
	Matrix r = MatrixRotationYawPitchRoll(rotation.y, rotation.x, rotation.z);
	transformMatrix = MatrixMultiply(MatrixMultiply(r, t), s);
	transformInvTrMatrix = MatrixInverse(MatrixTranspose(transformMatrix));

}

void PositionableObject::rotateObjectToPoint(Vector3 point)
{

	Vector3 targetToObject = point - position;
	float targetMagnitude = sqrt(VectorDot(targetToObject, targetToObject));

	if (targetMagnitude > CELESTIAL_EPSILON)
	{

		float xAngle = acos(VectorDot(targetToObject, Vector3(0.0f, 1.0f, 0.0f)));
		xAngle -= CELESTIAL_PI*0.5f;
		rotation.x = xAngle;

		Vector3 projectedLine = Vector3(targetToObject.x, 0, targetToObject.z);
		float projectedMagnitude = sqrt(VectorDot(projectedLine, projectedLine));
		projectedLine /= projectedMagnitude;

		if (projectedMagnitude > CELESTIAL_EPSILON)
		{

			float yAngle = acos(VectorDot(projectedLine, Vector3(0.0f, 1.0f, 0.0f)) / projectedMagnitude);
			yAngle *= VectorDot(projectedLine, Vector3(0, 0, 1)) >= 0 ? 1 : -1;
			rotation.y = yAngle;

		}

		direction = targetToObject / targetMagnitude;

	}
}

void PositionableObject::orbitAroundPoint(Vector3 point, float arc)
{

	Vector3 circleCenter = Vector3(point.x, position.y, point.x);
	Vector3 circleRad = circleCenter - position;
	float circleRadMagnitude = sqrt(VectorDot(circleRad, circleRad));
	float circleCirc = CELESTIAL_PI*circleRadMagnitude*circleRadMagnitude;
	float angle = circleRadMagnitude / circleCirc * (2 * CELESTIAL_PI);

	rotation.y -= angle;

	Matrix r = MatrixRotationYawPitchRoll(rotation.y, rotation.x, rotation.z);
	direction = VectorTransform(Vector3(0.0f,0.0f,1.0f), MatrixInverse(MatrixTranspose(r)));

	Vector3 pointToPos = point - position;
	float pointToPosMag = sqrt(VectorDot(pointToPos, pointToPos));
	position = point + (-direction)*pointToPosMag;
	createMatrix();

}

void PositionableObject::Update(Message* mess)
{

	if (mess->type == MessageType_OBJECT)
	{

		unsigned int param1 = mess->params[0] | ((int)mess->params[1] << 8) | ((int)mess->params[2] << 16) | ((int)mess->params[3] << 24);
		Vector3 newVec;
		float factor;

		switch (mess->mess)
		{

		case ObjectMess_MOVE:
			memcpy(&newVec.x, mess->params, 4);
			memcpy(&newVec.y, &mess->params[4], 4);
			memcpy(&newVec.z, &mess->params[8], 4);
			position += newVec;
			createMatrix();
			break;
		case ObjectMess_SCALE:
			memcpy(&newVec.x, mess->params, 4);
			memcpy(&newVec.y, &mess->params[4], 4);
			memcpy(&newVec.z, &mess->params[8], 4);
			scale += newVec;
			createMatrix();
			break;
		case ObjectMess_POS:
			memcpy(&position.x, mess->params, 4);
			memcpy(&position.y, &mess->params[4], 4);
			memcpy(&position.z, &mess->params[8], 4);
			createMatrix();
			break;
		case ObjectMess_SIZE:
			memcpy(&scale.x, mess->params, 4);
			memcpy(&scale.y, &mess->params[4], 4);
			memcpy(&scale.z, &mess->params[8], 4);
			createMatrix();
			break;
		case ObjectMess_POINT:
			memcpy(&newVec.x, mess->params, 4);
			memcpy(&newVec.y, &mess->params[4], 4);
			memcpy(&newVec.z, &mess->params[8], 4);
			rotateObjectToPoint(newVec);
			createMatrix();
			break;
		case ObjectMess_ORBIT:
			memcpy(&factor, mess->params, 4);
			memcpy(&newVec.x, &mess->params[4], 4);
			memcpy(&newVec.y, &mess->params[8], 4);
			memcpy(&newVec.z, &mess->params[12], 4);
			orbitAroundPoint(newVec, factor);
			break;
		case ObjectMess_ROTATE:
			memcpy(&newVec.x, mess->params, 4);
			memcpy(&newVec.y, &mess->params[4], 4);
			memcpy(&newVec.z, &mess->params[8], 4);
			rotation += newVec;
			createMatrix();
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
		default:
			ScriptableObject::Update(mess);

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

Vector3 PositionableObject::GetDirection() const
{

	return direction;

}

Matrix PositionableObject::GetMatrix() const
{

	return transformMatrix;

}

Matrix PositionableObject::GetInvTrnMatrix() const
{

	return transformInvTrMatrix;

}
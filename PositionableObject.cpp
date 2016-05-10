#include "stdafx.h"
#include "PositionableObject.h"

using namespace Resources;
using namespace CelestialMath;
using namespace CrossHandlers;

PositionableObject::PositionableObject() : PositionableObject(Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f))
{


}

PositionableObject::PositionableObject(Vector3 position, Vector3 scale) : ScriptableObject()
{

	parent = nullptr;
	this->position = position;
	this->scale = scale;
	rotation = Vector3(0.0f, 0.0f, 0.0f);
	direction = Vector3(0.0f, 0.0f, 1.0f);
	layer = 0;
	subObjects = new CelestialSlicedList<PositionableObject*>(32, nullptr);
	subObjectAmount = 0;
	childId = 0;

}

void PositionableObject::SetPosition(Vector3 pos)
{

	this->position = pos;

}

void PositionableObject::SetScale(Vector3 scale)
{

	this->scale = scale;

}

void PositionableObject::SetRotation(Vector3 rotation)
{

	this->rotation = rotation;

}

void PositionableObject::refresh(Vector3 position, Vector3 scale)
{

	this->position = position;
	this->scale = scale;

}


void PositionableObject::AddSubObject(PositionableObject* object, Vector3 relativePosition)
{

	unsigned int subId = subObjects->Add(object);
	object->SetObjectParent(this,subId,relativePosition);
	object->UpdateMatrix();

	if (subId >= subObjectAmount)
	{

		subObjectAmount = subId + 1;

	}
}

void PositionableObject::SetObjectParent(PositionableObject* parent, unsigned int childId, Vector3 relativePosition)
{

	this->parent = parent;
	this->childId = childId;
	this->relativePosition = relativePosition;

}

void PositionableObject::createMatrix()
{

	Matrix t;

	if (parent != nullptr)
	{

		Vector3 parentalPosition = parent->GetPosition();
		Vector3 relativePlace = (parent->GetScale()/2 + scale/2) * relativePosition;
		Vector3 parentRotation = parent->GetRotation();

		Matrix relativePlaceMat = MatrixTranslation(relativePlace.x, relativePlace.y, relativePlace.z);
		Matrix parentRotMat = MatrixRotationYawPitchRoll(parentRotation.y, parentRotation.x, parentRotation.z);
		Matrix parentalPlaceMat = MatrixTranslation(parentalPosition.x, parentalPosition.y, parentalPosition.z);

		t = MatrixMultiply(MatrixMultiply(relativePlaceMat, parentRotMat), parentalPlaceMat);

	}
	else
	{

		t = MatrixTranslation(position.x, position.y, position.z);

		for (unsigned int i = 0; i < subObjectAmount; i++)
		{

			PositionableObject* subObject = subObjects->GetValue(i);

			if (subObject != nullptr)
			{

				subObject->UpdateMatrix();

			}
		}
	}

	Matrix s = MatrixScaling(scale.x, scale.y, scale.z);
	Matrix r = MatrixRotationYawPitchRoll(rotation.y, rotation.x, rotation.z);

	Matrix tr = MatrixMultiply(s , r);
	transformMatrix = MatrixMultiply(tr, t);
	transformInvTrMatrix = MatrixInverse(MatrixTranspose(transformMatrix));
	direction = VectorTransform(Vector3(0.0f, 0.0f, 1.0f), MatrixInverse(MatrixTranspose(r)));

}

void PositionableObject::rotateObjectToPoint(Vector3 point)
{

	Vector3 targetToObject = point - position;
	float targetMagnitude = sqrt(VectorDot(targetToObject, targetToObject));

	if (targetMagnitude > CELESTIAL_EPSILON)
	{

		float dot = VectorDot(targetToObject, Vector3(0.0f, 1.0f, 0.0f));
		float xAngle = acos(dot/targetMagnitude);
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
	}
}

void PositionableObject::orbitAroundPoint(Vector3 point, float arc)
{

	Vector3 circleCenter = Vector3(point.x, position.y, point.z);
	Vector3 circleRad = circleCenter - position;
	float circleRadMagnitude = sqrt(VectorDot(circleRad, circleRad));


	if (circleRadMagnitude >= CELESTIAL_EPSILON)
	{

		float circleCirc = CELESTIAL_PI*circleRadMagnitude*circleRadMagnitude;
		float angle = ((arc*circleCirc) / circleCirc) * (2 * CELESTIAL_PI);

		rotation.y -= angle;

		Matrix r = MatrixRotationYawPitchRoll(rotation.y, rotation.x, rotation.z);
		direction = VectorTransform(Vector3(0.0f, 0.0f, 1.0f), MatrixInverse(MatrixTranspose(r)));

		Vector3 pointToPos = point - position;
		float pointToPosMag = sqrt(VectorDot(pointToPos, pointToPos));
		position = point + (-direction)*pointToPosMag;
		createMatrix();

	}
}

void PositionableObject::Update(Message* mess)
{

	if (mess->type == MessageType_OBJECT)
	{

		unsigned char tempBuff[]{ childId >> 0, childId >> 8, childId >> 16, childId >> 24 };
		unsigned int param1 = mess->params[0] | ((int)mess->params[1] << 8) | ((int)mess->params[2] << 16) | ((int)mess->params[3] << 24);
		Vector3 newVec;
		float factor;

		switch (mess->mess)
		{
		case ObjectMess_COPY:
			memcpy(&position.x, &mess->params[0], 4);
			memcpy(&position.y, &mess->params[4], 4);
			memcpy(&position.z, &mess->params[8], 4);
			memcpy(&scale.x, &mess->params[12], 4);
			memcpy(&scale.y, &mess->params[16], 4);
			memcpy(&scale.z, &mess->params[20], 4);
			memcpy(&rotation.x, &mess->params[24], 4);
			memcpy(&rotation.y, &mess->params[28], 4);
			memcpy(&rotation.z, &mess->params[32], 4);
			createMatrix();
			break;
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
			memcpy(&newVec.x, mess->params, 4);
			memcpy(&newVec.y, &mess->params[4], 4);
			memcpy(&newVec.z, &mess->params[8], 4);
			memcpy(&factor, &mess->params[12], 4);
			orbitAroundPoint(newVec, factor);
			break;
		case ObjectMess_PROPEL:
			memcpy(&factor, mess->params, 4);
			position += direction*factor;
			createMatrix();
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
		case ObjectMess_REMOVE:
			if (parent != nullptr)
			{

				mess->mess = ObjectMess_REMOVECHILD;
				mess->SetParams(tempBuff, 0, 4);
				parent->Update(mess);

			}
			break;
		case ObjectMess_REMOVECHILD:
			if (subObjects->GetValue(param1) != nullptr)
			{

				subObjects->Remove(param1);

			}
			break;
		default:
			ScriptableObject::Update(mess);

		}
	}
}

void PositionableObject::UpdateMatrix()
{

	createMatrix();

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

PositionableObject::~PositionableObject()
{

	delete subObjects;

}
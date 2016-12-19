#include "stdafx.h"
#include "PositionableObject.h"
#include "BoundingSphere.h"

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
	subObjects = new CelestialSlicedList<PositionableObject*>(32);
	subObjectAmount = 0;
	childId = 0;
	serial = 40;

	absOffset = Vector3(0, 0, 0);
	absScale = scale;

}

char* PositionableObject::Unserialize(char* data)
{

	memcpy(&position.x ,&data[0], 4);
	memcpy(&position.y,&data[4], 4);
	memcpy(&position.z, &data[8], 4);

	memcpy(&scale.x, &data[12], 4);
	memcpy(&scale.y, &data[16], 4);
	memcpy(&scale.z, &data[20], 4);

	memcpy(&rotation.x, &data[24], 4);
	memcpy(&rotation.y, &data[28], 4);
	memcpy(&rotation.z, &data[32], 4);

	unsigned int subSize = 0;
	memcpy(&subSize, &data[36], 4);

	if (data[40 + subSize] == SerializableType_SCRIPTABLE)
	{

		ScriptableObject::Unserialize(&data[40 + subSize + 1]);

	}

	return &data[36];

}

char* PositionableObject::Serialize(unsigned int &size)
{

	char** subsSer = nullptr;
	unsigned int* subSizes = nullptr;
	unsigned int subVal = 0;
	unsigned int subSize = 0;

	if (subObjectAmount > 0)
	{

		subsSer = new char*[subObjectAmount];
		subSizes = new unsigned int[subObjectAmount];

		for (unsigned int i = 0; i < subObjectAmount; i++)
		{

			PositionableObject* sub = subObjects->GetValue(i);

			if (sub != nullptr)
			{

				subsSer[subVal] = sub->Serialize(subSizes[subVal]);

				unsigned int tempSize = subSizes[subVal] + sizeof(unsigned int) + sizeof(float) * 3;

				char* temp = new char[tempSize];

				unsigned int offset = 0;
				Vector3 subPos = sub->GetRelativePosition();

				memcpy(&temp[offset], &(subPos.x), sizeof(float));
				offset += sizeof(float);
				memcpy(&temp[offset], &(subPos.y), sizeof(float));
				offset += sizeof(float);
				memcpy(&temp[offset], &(subPos.z), sizeof(float));
				offset += sizeof(float);
				memcpy(&temp[offset], &subSizes[subVal], sizeof(unsigned int));
				offset += sizeof(unsigned int);
				memcpy(&temp[offset], subsSer[subVal], subSizes[subVal]);

				delete[] subsSer[subVal];
				subsSer[subVal] = temp;
				subSizes[subVal] = tempSize;

				subSize += subSizes[subVal];
				subVal++;

			}
		}
	}

	unsigned int subObjectSize;
	char* subSerial = ScriptableObject::Serialize(subObjectSize);

	unsigned int standardSize = serial+1;
	size = 41 + subSize + subObjectSize;

	char* byteVal = new char[size];

	byteVal[0] = SerializableType_POSITIONABLE;
	Vector3 pos = GetPosition();
	memcpy(&byteVal[1], &pos.x, 4);
	memcpy(&byteVal[5], &pos.y, 4);
	memcpy(&byteVal[9], &pos.z, 4);

	Vector3 scale = GetScale();
	memcpy(&byteVal[13], &scale.x, 4);
	memcpy(&byteVal[17], &scale.y, 4);
	memcpy(&byteVal[21], &scale.z, 4);

	Vector3 rot = GetRotation();
	memcpy(&byteVal[25], &rot.x, 4);
	memcpy(&byteVal[29], &rot.y, 4);
	memcpy(&byteVal[33], &rot.z, 4);
	
	memcpy(&byteVal[37], &subSize, sizeof(unsigned int));

	unsigned int offset = 41;

	if (subsSer != nullptr)
	{

		for (unsigned int i = 0; i < subVal; i++)
		{

			memcpy(&byteVal[offset], subsSer[i], subSizes[i]);
			offset += subSizes[i];
			delete[] subsSer[i];

		}

		delete[] subSizes;
		delete[] subsSer;

	}

	memcpy(&byteVal[offset], subSerial, subObjectSize);
	delete[] subSerial;
	return byteVal;

}

void PositionableObject::Move(float distance)
{

	this->position += direction*distance;

}

Vector3 PositionableObject::getPlane(Vector3 point)
{

	Vector3 point1 = position;
	Vector3 point2 = point1 + direction;

	Vector3 line1 = point1 - point2;
	Vector3 line2 = point1 - point;

	Vector3 plane1 = VectorCross(line1, line2);

	Vector3 point4 = point1 + plane1;
	Vector3 line3 = point1 - point4;

	return VectorCross(line1, line3);

}

Vector3 PositionableObject::GetObjectCenterLine(Vector3 startPoint, Vector3 endPoint, float offSetFromEdge)
{

	if (offSetFromEdge > (scale.x / 2))
	{

		return GetObjectCenterLine(startPoint, startPoint - endPoint);

	}

	Vector3 newPoint = GetObjectCenterLine(startPoint);

	Vector3 plane = getPlane(endPoint);
	float normPlane = sqrt(VectorDot(plane));

	if (abs(normPlane) <= CELESTIAL_EPSILON)
	{

		normPlane = 1;
		plane.x = -1;

	}

	plane /= -normPlane;

	float newScal = (scale.x / 2) - offSetFromEdge;
	Vector3 toEdge = plane * newScal;

	return newPoint + toEdge;

}

Vector3 PositionableObject::GetObjectCenterLine(Vector3 startPoint, Vector3 direction)
{

	Vector3 plane = getPlane(startPoint);
	float d = -VectorDot(plane, position);

	float vn = VectorDot(direction, plane);

	if (abs(vn) <= CELESTIAL_EPSILON)
	{

		return capLine(startPoint);

	}

	float t = -(VectorDot(startPoint, plane) + d) / vn;
	return capLine(startPoint + direction*t);

}

Vector3 PositionableObject::GetObjectCenterLine(Vector3 point3)
{

	Vector3 plane2 = getPlane(point3);
	Vector3 line2 = position - point3;
	float vd1 = VectorDot(line2, plane2);
	float vd2 = VectorDot(plane2, plane2);
	Vector3 centerPoint = point3;

	if (vd2 > CELESTIAL_EPSILON)
	{


		Vector3 projectedLine = plane2 * (vd1 / vd2);
		centerPoint = point3 + projectedLine;
	
	}

	return capLine(centerPoint);

}

Vector3 PositionableObject::capLine(Vector3 point)
{

	Vector3 scl = subObjectAmount > 0 ? absScale : scale;

	Vector3 centerLine = point - (position + absOffset);
	float distSqr = VectorDot(centerLine, centerLine);

	if (distSqr > (scl.z / 2)*(scl.z / 2))
	{

		float dist = (scl.z / 2);
		dist -= 0.5f;
		dist = floor(dist);
		dist += 0.5f;

		Vector3 halfDist = (direction * dist);
		return VectorDot(centerLine, direction) > 0 ?
			(position + absOffset) + halfDist :
			(position + absOffset) - halfDist;

	}

	return point;

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

	if (subId >= subObjectAmount)
	{

		subObjectAmount = subId + 1;

	}

	object->UpdateMatrix();
	figureOutAbsoluteSize();

}

void PositionableObject::SetObjectParent(PositionableObject* parent, unsigned int childId, Vector3 relativePosition)
{

	this->parent = parent;
	this->childId = childId;
	this->relativePosition = relativePosition;
	position = Vector3(0.0f, 0.0f, 0.0f);
	rotateObjectToPoint(relativePosition);

}

bool PositionableObject::IsChild() const
{

	return parent != nullptr;

}

Vector3 PositionableObject::GetRelativePosition() const
{

	return relativePosition;

}

void PositionableObject::figureOutAbsoluteSize()
{

	absOffset = Vector3(0, 0, 0);

	float leftest = -scale.x/2;
	float rightest = scale.x/2;
	float toppest = scale.y/2;
	float bottomest = -scale.y/2;
	float nearest = scale.z/2;
	float furthest = -scale.z/2;

	for (unsigned int i = 0; i < subObjectAmount; i++)
	{

		PositionableObject* sub = subObjects->GetValue(i);

		if (sub != nullptr)
		{

			Vector3 subScale = sub->GetScale() / 2;
			Vector3 subPos = (scale/2 + subScale)*sub->GetRelativePosition();

			leftest = min(leftest, subPos.x - subScale.x);
			rightest = max(rightest, subPos.x + subScale.x);
			
			bottomest = min(bottomest, subPos.y - subScale.y);
			toppest = max(toppest, subPos.y + subScale.y);

			nearest = max(nearest, subPos.z + subScale.z);
			furthest = min(furthest, subPos.z - subScale.z);

		}
	}

	if (leftest < -scale.x / 2)
	{

		absOffset.x -= abs(leftest) - (scale.x / 2);

	}

	if (rightest > scale.x / 2)
	{

		absOffset.x += rightest - (scale.x / 2);

	}

	if (bottomest < -scale.y / 2)
	{

		absOffset.y -= abs(bottomest) - (scale.y / 2);

	}

	if (toppest > scale.y / 2)
	{

		absOffset.y += toppest - (scale.y / 2);

	}

	if (furthest < -scale.z / 2)
	{

		absOffset.z -= abs(furthest) - (scale.z / 2);

	}

	if (nearest > scale.z / 2)
	{

		absOffset.z += nearest - (scale.z / 2);

	}

	absScale.x = rightest - leftest;
	absScale.y = toppest - bottomest;
	absScale.z = nearest - furthest;

}

void PositionableObject::createMatrix()
{

	Matrix t;
	Vector3 boundingScale(0, 0, 0);

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
	matrixChanged = true;

}

unsigned int PositionableObject::GetTargetId() const
{

	return parent == nullptr ? GetId() : parent->GetId();

}

unsigned int PositionableObject::GetSubobjects() const
{

	return subObjectAmount;

}

PositionableObject* PositionableObject::GetSubobject(unsigned int object) const
{

	return subObjects->GetValue(object);

}

Matrix PositionableObject::GetBoundingMatrix() const
{

	return boundingMatrix;

}

bool PositionableObject::hasMatrixChanged()
{

	bool old = matrixChanged;
	matrixChanged = false;
	return old;

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

		if (projectedMagnitude > CELESTIAL_EPSILON)
		{

			float yAngle = acos(VectorDot(projectedLine, Vector3(0.0f, 0.0f, 1.0f)) / projectedMagnitude);
			yAngle *= VectorDot(projectedLine, Vector3(1.0f, 0, 0)) >= 0 ? 1 : -1;
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

void PositionableObject::Point(CelestialMath::Vector3 point)
{

	rotateObjectToPoint(point);
	createMatrix();

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
				figureOutAbsoluteSize();

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
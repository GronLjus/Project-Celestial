#include "stdafx.h"
#include "CameraObject.h"

using namespace Resources;
using namespace CrossHandlers;
using namespace Entities;
using namespace CelestialMath;

CameraObject::CameraObject(unsigned int width, unsigned int height, float depth, unsigned int flips) : PositionableObject()
{

	Vector3 sidePoint (1, 0, 0);
	Vector3 up(0, 0, 1);
	Vector3 lookAtPoint(0, 0, 0);
	Vector3 direction(0, -1, 0);
	fov = CELESTIAL_PI*0.25f;

	ViewObject::ViewPort port;
	port.height = height;
	port.maxDepth = depth;
	port.minDepth = 0.1f;
	port.topX = 0;
	port.topY = 0;
	port.width = width;

	Message mess = Message();
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_MOVE;
	Vector3 moveVal(0.0f,15.0f,0.0f);
	unsigned char tempBuff[12];
	memcpy(tempBuff, &moveVal.x, 4);
	memcpy(&tempBuff[4], &moveVal.y, 4);
	memcpy(&tempBuff[8], &moveVal.z, 4);
	mess.SetParams(tempBuff, 0, 12);
	PositionableObject::Update(&mess);

	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_POINT;
	memcpy(tempBuff, &direction.x, 4);
	memcpy(&tempBuff[4], &direction.y, 4);
	memcpy(&tempBuff[8], &direction.z, 4);
	mess.SetParams(tempBuff, 0, 12);
	PositionableObject::Update(&mess);

	theView = new ViewObject(this->GetPosition(),sidePoint,lookAtPoint,up,fov, flips, port);

}

void CameraObject::IncrementFlipBuff()
{

	theView->IncrementInstances();

}

unsigned int CameraObject::PeekNextFlip() const
{

	return theView->PeekNextFlip();

}

unsigned int CameraObject::GetFlip() const
{

	return theView->GetFlip();

}

void CameraObject::Update(Message* mess)
{
	if (mess->type == MessageType_OBJECT)
	{

		Vector3 newVec;
		PositionableObject::Update(mess);

		switch (mess->mess)
		{

		case ObjectMess_PROPEL:
		case ObjectMess_MOVE:
		case ObjectMess_POS:
			theView->Update(GetPosition());
			break;
		case ObjectMess_POINT:
		case ObjectMess_ORBIT:
		case ObjectMess_ROTATE:
			theView->Rotate(GetRotation());
			theView->Update(GetPosition());
			break;

		}
	}
}

ViewObject* CameraObject::GetView() const
{

	return theView;

}

CameraObject::~CameraObject()
{

	delete theView;

}
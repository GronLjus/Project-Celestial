#include "stdafx.h"
#include "CameraObject.h"

using namespace Resources;
using namespace CrossHandlers;
using namespace Entities;
using namespace CelestialMath;

CameraObject::CameraObject(unsigned int width, unsigned int height, float depth, unsigned int flips) : PositionableObject()
{

	Vector3 sidePoint (1, 0, 0);
	Vector3 up(0, 1, 0);
	Vector3 lookAtPoint(0, 0, 1);
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
	mess.param1 = 5;
	mess.param2 = 5;
	mess.param3 = 5;
	PositionableObject::Update(&mess);

	theView = new ViewObject(this->GetPosition(),sidePoint,lookAtPoint,up,fov, flips, port);

}

void CameraObject::IncrementFlipBuff()
{

	
	theView->IncrementInstances();

}

unsigned int CameraObject::GetFlip() const
{

	return theView->GetFlip();

}

void CameraObject::Update(Message* mess)
{
}

ViewObject* CameraObject::GetView() const
{

	return theView;

}

CameraObject::~CameraObject()
{

	delete theView;

}
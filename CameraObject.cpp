#include "stdafx.h"
#include "CameraObject.h"

using namespace Resources;
using namespace CrossHandlers;
using namespace Entities;
using namespace CelestialMath;

CameraObject::CameraObject(unsigned int width, unsigned int height, float depth, unsigned int flips) : PositionableObject()
{

	sidePoint = Vector3(1, 0, 0);
	up = Vector3(0, 1, 0);
	lookAtPoint = Vector3(0, 0, 1);
	this->width = width;
	this->height = height;
	fov = CELESTIAL_PI*0.25f;
	Matrix view = MatrixLookAtLH(this->GetPosition(), lookAtPoint, up);

	Matrix projection = MatrixPerspectiveFovLH(fov, width / height, 0.1f, depth);

	viewProjection = MatrixMultiply(view, projection);
	invViewProjection = MatrixInverse(viewProjection);

	theView = new ViewObject(viewProjection,flips);

}

void CameraObject::Update(Message* mess)
{
}

ViewObject* CameraObject::GetView() const
{

	return theView;

}

Matrix CameraObject::GetViewProjection() const
{

	return viewProjection;

}

CameraObject::~CameraObject()
{

	delete theView;

}
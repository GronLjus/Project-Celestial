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
	flip = 0;
	this->flips = flips;
	viewProjections = new Matrix[flips];
	invViewProjections = new Matrix[flips];

	for (unsigned int i = 0; i < flips; i++)
	{

		viewProjections[i] = MatrixMultiply(view, projection);
		invViewProjections[i] = MatrixInverse(viewProjections[i]);

	}

	theView = new ViewObject(viewProjections[0],flips);

}

void CameraObject::IncrementFlipBuff()
{

	flip++;
	flip %= flips;

}

void CameraObject::Update(Message* mess)
{
}

ViewObject* CameraObject::GetView() const
{

	return theView;

}

Matrix CameraObject::GetViewProjection(unsigned int flipBuffer) const
{

	return viewProjections[flipBuffer];

}

CameraObject::~CameraObject()
{

	delete[] viewProjections;
	delete[] invViewProjections;
	delete theView;

}
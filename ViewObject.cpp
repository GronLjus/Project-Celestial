#include "stdafx.h"
#include "ViewObject.h"

using namespace Entities;
using namespace CelestialMath;
using namespace CrossHandlers;

ViewObject::ViewObject(CelestialMath::Vector3 pos,Vector3 sidePoint, Vector3 lookAtPoint, Vector3 up, float fov, unsigned int bufferFlips, ViewPort port)
{

	this->vp = port;
	flips = bufferFlips;
	flip = 0;

	this->sidePoint = sidePoint;
	this->lookAtPoint = lookAtPoint;
	this->up = up;
	this->fov = fov;

	views = new Matrix[flips];
	projections = new Matrix[flips];
	viewProjections = new Matrix[flips];
	invViewProjections = new Matrix[flips];

	instances = new CelestialStack<Fragment>*[flips];
	this->pos = new Vector3[flips];
		
	for (unsigned int i = 0; i < flips; i++)
	{

		instances[i] = new CelestialStack<Fragment>(false);
		views[i] = MatrixLookAtLH(pos, lookAtPoint, up);
		projections[i] = MatrixPerspectiveFovLH(fov, (float)port.width / (float)port.height, port.minDepth, port.maxDepth);

		viewProjections[i] = MatrixMultiply(views[i], projections[i]);
		invViewProjections[i] = MatrixInverse(viewProjections[i]);

		this->pos[i] = pos;

	}

	localFrustum = new Frustum(viewProjections[0], invViewProjections[0]);

}

void ViewObject::IncrementInstances()
{

	unsigned int oldFlip = flip;
	flip++;
	flip %= flips;
	pos[flip] = pos[oldFlip];

}

unsigned int ViewObject::PeekNextFlip() const
{

	unsigned int nextFlip = flip;
	nextFlip++;
	nextFlip %= flips;
	return nextFlip;

}

void ViewObject::Update(Matrix transform)
{

	Vector4 newPoint(lookAtPoint, 0);
	Vector4 newUp(up, 0);
	Vector4 newSide(sidePoint, 0);

	newPoint = VectorTransform(newPoint, transform);
	newUp = VectorTransform(newUp, transform);
	newSide = VectorTransform(newSide, transform);

	lookAtPoint = Vector3(newPoint.x, newPoint.y, newPoint.z);
	pos[flip] += Vector3(transform._41, transform._42, transform._43);

	views[flip] = MatrixLookAtLH(pos[flip], lookAtPoint, up);
	projections[flip] = MatrixPerspectiveFovLH(fov, vp.width / vp.height, vp.minDepth, vp.maxDepth);
	viewProjections[flip] = MatrixMultiply(views[flip], projections[flip]);
	invViewProjections[flip] = MatrixInverse(viewProjections[flip]);

	localFrustum->Construct(viewProjections[flip], invViewProjections[flip], false);

}

unsigned int ViewObject::GetFlip() const
{

	return flip;

}

void ViewObject::AddInstanceFragment(unsigned int mesh, unsigned int start, unsigned int length)
{

	instances[flip]->PushElement(Fragment(mesh,start,length));

}

void ViewObject::ResetInstances()
{
	
	instances[flip]->Reset();

}

Frustum* ViewObject::GetFrustum() const
{

	return localFrustum;

}

CelestialStack<ViewObject::Fragment>* ViewObject::GetInstanceStack(unsigned int bufferFlip) const
{

	return instances[bufferFlip];

}

Matrix ViewObject::GetView(unsigned int flip) const
{

	return views[flip];

}

Matrix ViewObject::GetProjection(unsigned int flip) const
{

	return projections[flip];

}

Matrix ViewObject::GetViewProjection(unsigned int flip) const
{

	return viewProjections[flip];

}

Matrix ViewObject::GetInverseViewProjection(unsigned int flip) const
{

	return invViewProjections[flip];

}

Vector3 ViewObject::GetPosition(unsigned int flip) const
{

	return pos[flip];

}

float ViewObject::GetFov() const
{

	return fov;

}

ViewObject::ViewPort ViewObject::GetPort() const
{

	return vp;

}

ViewObject::~ViewObject()
{

	delete[] pos;
	delete[] views;
	delete[] projections;
	delete[] viewProjections;
	delete[] invViewProjections;

	delete localFrustum;

	for (unsigned int i = 0; i < flips; i++)
	{

		delete instances[i];

	}

	delete[] instances;

}
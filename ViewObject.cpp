#include "stdafx.h"
#include "ViewObject.h"

using namespace Entities;
using namespace CelestialMath;
using namespace CrossHandlers;

ViewObject::ViewObject(Matrix matrix)
{

	localFrustum = new Frustum(matrix, MatrixInverse(matrix));
	instances = new CelestialStack<Fragment>(false);

}

void ViewObject::Update(Matrix matrix)
{

	localFrustum->Construct(matrix, MatrixInverse(matrix),false);

}

void ViewObject::AddInstanceFragment(unsigned int mesh, unsigned int start, unsigned int length)
{

	instances->PushElement(Fragment(mesh,start,length));

}

Frustum* ViewObject::GetFrustum() const
{

	return localFrustum;

}

CelestialStack<ViewObject::Fragment>* ViewObject::GetInstanceStack() const
{

	return instances;

}

ViewObject::~ViewObject()
{

	delete localFrustum;
	delete instances;

}
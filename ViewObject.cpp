#include "stdafx.h"
#include "ViewObject.h"

using namespace Entities;
using namespace CelestialMath;
using namespace CrossHandlers;

ViewObject::ViewObject(Matrix matrix, unsigned int bufferFlips)
{

	flips = bufferFlips;
	flip = 0;
	localFrustum = new Frustum(matrix, MatrixInverse(matrix));
	instances = new CelestialStack<Fragment>*[flips];
		
	for (unsigned int i = 0; i < flips; i++)
	{

		instances[i] = new CelestialStack<Fragment>(false);

	}
}

void ViewObject::IncrementInstances()
{

	flip++;
	flip %= flips;

}

void ViewObject::Update(Matrix matrix)
{

	localFrustum->Construct(matrix, MatrixInverse(matrix),false);

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

ViewObject::~ViewObject()
{

	delete localFrustum;

	for (unsigned int i = 0; i < flips; i++)
	{

		delete instances[i];

	}

	delete[] instances;

}
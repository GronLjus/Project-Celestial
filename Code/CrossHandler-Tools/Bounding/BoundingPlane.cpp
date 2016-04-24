#include "stdafx.h"
#include "BoundingPlane.h"
#include <cmath>

using namespace CrossHandlers;
using namespace CelestialMath;

BoundingPlane::BoundingPlane()
{

}

BoundingPlane::BoundingPlane(float a,float b,float c,float d)
{

	normals = Vector3(a,b,c);
	dComponent = d;

	normalLength = sqrt(a*a+b*b+c*c);
	unitNormals = Vector3(a / normalLength, b / normalLength, c / normalLength);

	pConstant = d/normalLength;

}

BoundingPlane::BoundingPlane(Vector3 abc, float d)
{

	normals = abc;
	dComponent = d;

	normalLength = sqrt(abc.x*abc.x + abc.y*abc.y + abc.z*abc.z);
	unitNormals = abc/normalLength;

	pConstant = d / normalLength;

}

BoundingPlane::BoundingPlane(Vector3 abc, float d, float length)
{

	normals = abc;
	dComponent = d;

	normalLength = length;
	unitNormals = abc / normalLength;

	pConstant = d / normalLength;

}

CelestialMath::Vector3 BoundingPlane::GetNormal() const
{

	return normals;

}

CelestialMath::Vector3 BoundingPlane::GetUnitNormal() const
{

	return unitNormals;

}

float BoundingPlane::GetD() const
{

	return dComponent;

}

float BoundingPlane::GetP() const
{

	return pConstant;

}

float BoundingPlane::GetNormalLength() const
{

	return normalLength;

}

BoundingPlane::~BoundingPlane()
{

}
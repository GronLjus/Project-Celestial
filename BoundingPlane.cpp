#include "stdafx.h"
#include "BoundingPlane.h"
#include <cmath>

using namespace CrossHandlers;

BoundingPlane::BoundingPlane()
{

	normals = nullptr;
	unitNormals = nullptr;

}

BoundingPlane::BoundingPlane(float a,float b,float c,float d)
{

	normals = new float[3];
	normals[0] = a;
	normals[1] = b;
	normals[2] = c;
	dComponent = d;

	unitNormals = new float[3];
	normalLength = sqrt(a*a+b*b+c*c);

	for(int i=0;i<3;i++)
	{

		unitNormals[i] = normals[i]/normalLength;

	}

	pConstant = d/normalLength;

}

BoundingPlane::BoundingPlane(const BoundingPlane* plane)
{
	
	normals = new float[3];
	unitNormals = new float[3];

	for(int i=0;i<3;i++)
	{

		normals[i] = plane->GetNormal()[i];
		unitNormals[i] = plane->GetUnitNormal()[i];

	}

	dComponent = plane->GetD();
	pConstant = plane->GetP();

}

BoundingPlane::BoundingPlane(const BoundingPlane& plane)
{
	
	normals = new float[3];
	unitNormals = new float[3];

	for(int i=0;i<3;i++)
	{

		normals[i] = plane.GetNormal()[i];
		unitNormals[i] = plane.GetUnitNormal()[i];

	}

	dComponent = plane.GetD();
	pConstant = plane.GetP();

}

float* BoundingPlane::GetNormal() const
{

	return normals;

}

float* BoundingPlane::GetUnitNormal() const
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

	if(normals != nullptr)
	{

		delete[] normals;
		normals = nullptr;
	
	}

	if(unitNormals)
	{

		delete[] unitNormals;
		unitNormals = nullptr;

	}
}
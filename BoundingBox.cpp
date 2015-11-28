#include "stdafx.h"
#include "CelestialMath.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"

using namespace CrossHandlers;
using namespace CelestialMath;

BoundingBox::BoundingBox(){epsilon = 0.00002f;}
		
BoundingBox::BoundingBox(float width,float heigth,float depth,float centerX,float centerY,float centerZ)
{

	dimensions = Vector3(width*0.5f,heigth*0.5f,depth*0.5f);
	nrOfPoints = 8;
	basePoints = new Point[nrOfPoints];
	points = new Point[nrOfPoints];

	points[0] = Point(centerX-dimensions.x,centerY-dimensions.y,centerZ-dimensions.z);
	points[1] = Point(centerX-dimensions.x,centerY-dimensions.y,centerZ+dimensions.z);
	points[2] = Point(centerX+dimensions.x,centerY-dimensions.y,centerZ-dimensions.z);
	points[3] = Point(centerX+dimensions.x,centerY-dimensions.y,centerZ+dimensions.z);
	
	points[4] = Point(centerX-dimensions.x,centerY+dimensions.y,centerZ-dimensions.z);
	points[5] = Point(centerX-dimensions.x,centerY+dimensions.y,centerZ+dimensions.z);
	points[6] = Point(centerX+dimensions.x,centerY+dimensions.y,centerZ-dimensions.z);
	points[7] = Point(centerX+dimensions.x,centerY+dimensions.y,centerZ+dimensions.z);

	for(int i=0;i<nrOfPoints;i++)
	{

		basePoints[i] = Point(points[i].GetX()-centerX,points[i].GetY()-centerY,points[i].GetZ()-centerZ);

	}

	pos = Vector3(centerX,centerY,centerZ);
	orgPos = Vector3(centerX,centerY,centerZ);
	epsilon = 0.00002f;//Small epsilon value 

}

float BoundingBox::GetX() const
{

	return pos.x;

}

float BoundingBox::GetY() const
{

	return pos.y;

}

float BoundingBox::GetZ() const
{

	return pos.z;

}

void BoundingBox::SetX(float x)
{

	pos.x = x;

}

void BoundingBox::SetY(float y)
{

	pos.y = y;

}

void BoundingBox::SetZ(float z)
{

	pos.z = z;

}

ShapeComplexity BoundingBox::GetComplexity()
{

	return ShapeComplexity_BOX;

}

IBounding* BoundingBox::GetCopy() const
{

	return new BoundingBox(dimensions.x*2,dimensions.y*2,dimensions.z*2,pos.x,pos.y,pos.z);

}

Intersection BoundingBox::checkPlaneSide(BoundingPlane* bp,Point p)
{

	float* planeNormal = bp->GetUnitNormal();
	float dot = planeNormal[0]*p.GetX()+planeNormal[1]*p.GetY()+planeNormal[2]*p.GetZ();
	float distance = dot+bp->GetP();

	if(distance < epsilon && distance > -epsilon)
	{

		return Intersection_ON;

	}
	else if(distance > 0)
	{

		return Intersection_FRONT;

	}
	else if(distance < 0)
	{

		return Intersection_BACK;

	}

	return Intersection_NA;

}

Intersection BoundingBox::IntersectsPlane(BoundingPlane* bp)
{

	bool allOnOneSide = true;
	Intersection lastResult = Intersection_NA;
	Intersection result = Intersection_NA;

	for(int i=0;i<nrOfPoints && allOnOneSide;i++)//If two points are on seperate sides they must go through the plane
	{

		lastResult = result;
		result = checkPlaneSide(bp,points[i]);
		allOnOneSide = lastResult == Intersection_NA || lastResult == result; 

	}

	if(!allOnOneSide)
	{

		return Intersection_THROUGH;

	}
	else
	{

		return result;

	}
}

Intersection BoundingBox::IntersectsBetweenPlanes(BoundingPlane* p1, BoundingPlane* p2)
{

	Intersection p1Result = IntersectsPlane(p1);
	Intersection p2Result = IntersectsPlane(p2);

	if (p1Result == Intersection_BACK || p2Result == Intersection_BACK)
	{

		return Intersection_BACK;

	}
	else if((p1Result == Intersection_FRONT || p1Result == Intersection_ON) && (p2Result == Intersection_FRONT || p2Result == Intersection_ON))
	{

		return Intersection_FRONT;

	}
	else
	{

		return Intersection_THROUGH;

	}
}

Intersection BoundingBox::IntersectsBounding(IBounding* bounding,Shape shape)
{

	if(shape == Shape_SPHERE)
	{

		BoundingSphere* sph = (BoundingSphere*)bounding;
		Vector3 localSpherePoint = Vector3(sph->GetX()-pos.x,sph->GetY()-pos.y,sph->GetZ()-pos.z);
		float distance = 0;

		if(localSpherePoint.x > dimensions.x)
		{

			distance += (localSpherePoint.x - dimensions.x)*(localSpherePoint.x - dimensions.x);

		}
		else if(localSpherePoint.x < -dimensions.x)
		{
		
			distance += (localSpherePoint.x + dimensions.x)*(localSpherePoint.x + dimensions.x);
		}

		if(localSpherePoint.y > dimensions.y)
		{

			distance += (localSpherePoint.y - dimensions.y)*(localSpherePoint.y - dimensions.y);

		}
		else if(localSpherePoint.y < -dimensions.y)
		{
		
			distance += (localSpherePoint.y + dimensions.y)*(localSpherePoint.y + dimensions.y);
		}

		if(localSpherePoint.z > dimensions.z)
		{

			distance += (localSpherePoint.z - dimensions.z)*(localSpherePoint.z - dimensions.z);

		}
		else if(localSpherePoint.z < -dimensions.z)
		{
		
			distance += (localSpherePoint.z + dimensions.z)*(localSpherePoint.z + dimensions.z);
		}

		if(distance-sph->GetRadi()*sph->GetRadi() < epsilon && distance-sph->GetRadi()*sph->GetRadi() > -epsilon)
		{

			return Intersection_ON;

		}
		else if(distance < sph->GetRadi()*sph->GetRadi())
		{

			return Intersection_THROUGH;

		}
		else
		{

			return Intersection_FRONT;

		}
	}

	return Intersection_NA;

}

void BoundingBox::Transform(Matrix mat)
{

	
	Vector4 tempVec = VectorTransform(Vector4(0,0,0,1),mat);
	pos.x = orgPos.x+tempVec.x;
	pos.y = orgPos.y+tempVec.y;
	pos.z = orgPos.z+tempVec.z;

	for(int i=0;i<nrOfPoints;i++)
	{

		Vector4 localPoint = Vector4(basePoints[i].GetX(),basePoints[i].GetY(),basePoints[i].GetZ(),1);
		Vector4 temp = VectorTransform(localPoint, mat);
		points[i] = Point(orgPos.x+temp.x,orgPos.y+temp.y,orgPos.z+temp.z);

	}

	int debugHere = 0;
}

BoundingBox::~BoundingBox()
{

	delete[] points;
	delete[] basePoints;

}
#include "stdafx.h"
#include "BoundingSphere.h"
#include "CelestialMath.h"

using namespace CrossHandlers;
using namespace CelestialMath;

BoundingSphere::BoundingSphere(){epsilon = 0.00002f;}

BoundingSphere::BoundingSphere(float posX,float posY,float posZ,float r)
{

	pos = Vector3(posX,posY,posZ);
	orgPos = Vector3(posX,posY,posZ);
	radi = r;
	epsilon = 0.00002f;//Small epsilon value 

}

float BoundingSphere::GetRadi() const
{

	return radi;

}

float BoundingSphere::GetX() const
{

	return pos.x;

}

float BoundingSphere::GetY() const
{

	return pos.y;

}

float BoundingSphere::GetZ() const
{

	return pos.z;

}

void BoundingSphere::SetRadi(float r)
{

	radi = r;

}

void BoundingSphere::SetX(float x)
{

	pos.x = x;

}

void BoundingSphere::SetY(float y)
{

	pos.y = y;

}

void BoundingSphere::SetZ(float z)
{

	pos.z = z;

}

ShapeComplexity BoundingSphere::GetComplexity()
{

	return ShapeComplexity_BOX;

}

Intersection BoundingSphere::IntersectsLine(Vector3 origin, Vector3 direction, float& smallestDistanceSquare)
{

	Vector3 pointSphere = pos - origin;
	float scalarProjection = VectorDot(pointSphere, direction);
	float distance = VectorDot(pointSphere, pointSphere)-(radi*radi);

	if (smallestDistanceSquare != 0 && smallestDistanceSquare < distance)
	{

		return Intersection_BACK;

	}

	Vector3 projectedLine = direction * scalarProjection;
	Vector3 sphereToLine = projectedLine - pointSphere;
	float smallestDistanceSquared = VectorDot(sphereToLine, sphereToLine);

	if (smallestDistanceSquared > radi*radi)
	{

		return Intersection_BACK;

	}

	smallestDistanceSquare = VectorDot(projectedLine, projectedLine);

	if (smallestDistanceSquared == radi*radi)
	{

		return Intersection_ON;

	}

	return Intersection_THROUGH;

}

Intersection BoundingSphere::IntersectsPlane(BoundingPlane* bp)
{

	Vector3 planeNormal = bp->GetUnitNormal();
	float dot = VectorDot(planeNormal, pos);
	float distance = dot+bp->GetP();

	if((distance < 0 && distance+radi > -epsilon && distance+radi < epsilon) || (distance > 0 && distance-radi > -epsilon && distance-radi < epsilon))
	{

		return Intersection_ON;

	}
	else if(distance < radi && distance > -radi)
	{

		return Intersection_THROUGH;

	}
	else if(distance > radi)
	{

		return Intersection_FRONT;

	}
	else if(distance < 0 && distance < -radi)
	{

		return Intersection_BACK;

	}

	return Intersection_NA;//Should not get here

}

Intersection BoundingSphere::IntersectsBetweenPlanes(BoundingPlane* p1, BoundingPlane* p2)
{

	Intersection p1R = IntersectsPlane(p1);
	Intersection p2R = IntersectsPlane(p2);

	if(p1R == Intersection_BACK || p2R == Intersection_BACK)
	{

		return Intersection_BACK;

	}
	else if((p1R == Intersection_FRONT || p1R == Intersection_ON) && (p2R == Intersection_FRONT || p2R == Intersection_ON))
	{

		return Intersection_FRONT;

	}
	else
	{

		return Intersection_THROUGH;

	}
}

Intersection BoundingSphere::IntersectsBounding(IBounding* bounding,Shape shape)
{

	if(shape == Shape_SPHERE)
	{
		
		BoundingSphere* sph = (BoundingSphere*)bounding;
		float totalRadius = radi+sph->GetRadi();
		float x = sph->GetX()-pos.x;
		float y = sph->GetY()-pos.y;
		float z = sph->GetZ()-pos.z;
		float distance = x*x+y*y+z*z;
		totalRadius*=totalRadius;

		if(totalRadius-distance < epsilon && totalRadius-distance > -epsilon)
		{

			return Intersection_ON;

		}
		else if(totalRadius > distance)
		{

			return Intersection_THROUGH;

		}
		else if(totalRadius < distance)
		{

			return Intersection_FRONT;

		}
	}
	else if(shape == Shape_BOX)
	{

		return bounding->IntersectsBounding(this,Shape_SPHERE);

	}

	return Intersection_NA;

}

Intersection BoundingSphere::ContainsPoint(Vector3 point)
{

	Vector3 diff = pos - point;
	float distSQR = VectorDot(diff);

	if (distSQR - epsilon < radi*radi)
	{

		return Intersection_FRONT;

	}
	else if(distSQR + epsilon > radi*radi)
	{

		return Intersection_BACK;

	}

	return Intersection_ON;

}

void BoundingSphere::Transform(Matrix mat)
{

	Vector4 out = VectorTransform(Vector4(orgPos,1),mat);
	pos.x = out.x;
	pos.y = out.y;
	pos.z = out.z;

	radi = abs(VectorTransform(Vector4(orgPos, 1) + Vector4(radi, radi, radi, 1),mat).x-pos.x);

}

IBounding* BoundingSphere::GetCopy() const
{

	return new BoundingSphere(pos.x,pos.y,pos.z,radi);

}
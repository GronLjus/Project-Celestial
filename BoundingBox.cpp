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
	pos = Vector3(centerX,centerY,centerZ);

	Vector3 x1(pos + Vector3(dimensions.x,0,0));
	Vector3 x2(pos - Vector3(dimensions.x, 0, 0));

	Vector3 y1(pos + Vector3(0,dimensions.y, 0));
	Vector3 y2(pos - Vector3(0,dimensions.y, 0));

	Vector3 z1(pos + Vector3(0,0, dimensions.z));
	Vector3 z2(pos - Vector3(0,0, dimensions.z));

	constructBox(x1, x2, y1, y2, z1, z2);
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

Intersection BoundingBox::getPlaneDistSquare(Vector3 origin, Vector3 direction, BoundingPlane plane, float& distSquare)
{

	Vector3 point(0, 0, 0);

	if (plane.GetNormal().x != 0)
	{
		point = Vector3((-plane.GetD()) / plane.GetNormal().x, 0, 0);

	}
	else if (plane.GetNormal().y != 0)
	{
		point = Vector3(0, -plane.GetD() / plane.GetNormal().y, 0);

	}
	else
	{

		point = Vector3(0, 0, (-plane.GetD()) / plane.GetNormal().z);

	}

	Vector3 toLineOrigin = point - origin;
	float scalar = VectorDot(toLineOrigin, plane.GetUnitNormal());
	Vector3 projectedLine = plane.GetUnitNormal() *scalar;
	float dotProduct = VectorDot(direction, projectedLine);

	if (dotProduct <= epsilon)
	{
		
		distSquare = VectorDot(point,plane.GetUnitNormal())+plane.GetP();
		Intersection retVal = distSquare > 0 ? 
		Intersection_FRONT : distSquare < 0 ? Intersection_BACK : Intersection_ON;
		distSquare *= distSquare;
		return retVal;

	}

	float lineMag = sqrt(VectorDot(projectedLine, projectedLine));
	float angle = acos(VectorDot(direction, projectedLine) / lineMag);

	float dist = (1 / cos(angle)) *lineMag;
	distSquare = dist*dist;

	return Intersection_THROUGH;

}

Intersection BoundingBox::IntersectsLine(Vector3 origin, Vector3 direction, float& smallestDistanceSquare)
{


	char dims[] = { 'x', 'x', 'y', 'y', 'z', 'z' };
	float distances[6];
	Intersection inters[6];
	Intersection returnVal = Intersection_FRONT;
	//Check line against all planes containing tha box
	inters[0] = getPlaneDistSquare(origin, direction, rightPlane, distances[0]);

	if (inters[0] != Intersection_BACK)
	{

		inters[1] = getPlaneDistSquare(origin, direction, leftPlane, distances[1]);

		if (inters[1] != Intersection_BACK)
		{

			inters[2] = getPlaneDistSquare(origin, direction, topPlane, distances[2]);

			if (inters[2] != Intersection_BACK)
			{

				inters[3] = getPlaneDistSquare(origin, direction, bottomPlane, distances[3]);

				if (inters[3] != Intersection_BACK)
				{

					inters[4] = getPlaneDistSquare(origin, direction, frontPlane, distances[4]);

					if (inters[4] != Intersection_BACK)
					{

						inters[5] = getPlaneDistSquare(origin, direction, rearPlane, distances[5]);

						if (inters[5] != Intersection_BACK)
						{

							//The line crosses all planes
							//Sort hits by distance or if the line starts in front of a plane 
							for (unsigned char i = 0; i < 6; i++)
							{

								for (unsigned char k = i+1; k < 6; k++)
								{

									if (inters[i] != Intersection_FRONT)
									{

										if (distances[i] < smallestDistanceSquare)
										{

											smallestDistanceSquare = distances[i];

										}

										if (inters[k] == Intersection_FRONT || distances[k] < distances[i]);
										{

											char dimTemp = dims[k];
											Intersection interTemp = inters[k];
											float distanceTemp = distances[k];

											dims[k] = dims[i];
											inters[k] = inters[i];
											distances[k] = inters[i];

											dims[i] = dimTemp;
											inters[i] = interTemp;
											distances[i] = distanceTemp;

										}
									}
								}

								//Figure out the order of hits
								bool hitx = false;
								bool hity = false;
								bool hitz = false;
								bool outside = false;

								for (unsigned char i = 2; i >= 0 && !outside; i--)
								{

									if (dims[i] == 'x')
									{

										outside = hitx;
										hitx = true;

									}
									else if (dims[i] == 'y')
									{

										outside = hity;
										hity = true;

									}
									else if (dims[i] == 'z')
									{

										outside = hitz;
										hitz = true;

									}
								}

								if (!outside)
								{

									returnVal = Intersection_THROUGH;

								}
							}
						}
					}
				}
			}
		}
	}

	return returnVal;

}

Intersection BoundingBox::IntersectsPlane(BoundingPlane* bp)
{

	Vector3 topLeftBack = pos + (rightPlane.GetUnitNormal()*(dimensions.x)) + (bottomPlane.GetUnitNormal()*(dimensions.y)) + (frontPlane.GetUnitNormal()*(dimensions.z));
	Vector3 bottomRightBack = pos + (leftPlane.GetUnitNormal()*(dimensions.x)) + (topPlane.GetUnitNormal()*(dimensions.y)) + (frontPlane.GetUnitNormal()*(dimensions.z));
	Vector3 topRightFront = pos + (leftPlane.GetUnitNormal()*(dimensions.x)) + (bottomPlane.GetUnitNormal()*(dimensions.y)) + (rearPlane.GetUnitNormal()*(dimensions.z));
	Vector3 bottomLeftFront = pos + (rightPlane.GetUnitNormal()*(dimensions.x)) + (topPlane.GetUnitNormal()*(dimensions.y)) + (rearPlane.GetUnitNormal()*(dimensions.z));

	float tlbx = 0;
	Intersection tlbxInter = getPlaneDistSquare(topLeftBack, leftPlane.GetUnitNormal(), *bp, tlbx);
	if (tlbxInter == Intersection_FRONT || tlbxInter == Intersection_THROUGH && tlbx <= dimensions.x * 2)
	{return tlbxInter;}
	float tlby = 0;
	Intersection tlbyInter = getPlaneDistSquare(topLeftBack, topPlane.GetUnitNormal(), *bp, tlby);
	if (tlbyInter == Intersection_FRONT || tlbyInter == Intersection_THROUGH && tlbx <= dimensions.y * 2)
	{return tlbyInter;}
	float tlbz = 0;
	Intersection tlbzInter = getPlaneDistSquare(topLeftBack, rearPlane.GetUnitNormal(), *bp, tlbz);
	if (tlbzInter == Intersection_FRONT || tlbzInter == Intersection_THROUGH && tlbz <= dimensions.z * 2)
	{return tlbzInter;}

	float brbx = 0;
	Intersection brbxInter = getPlaneDistSquare(bottomRightBack, rightPlane.GetUnitNormal(), *bp, brbx);
	if (brbxInter == Intersection_FRONT || brbxInter == Intersection_THROUGH && brbx <= dimensions.x * 2)
	{return brbxInter;}
	float brby = 0;
	Intersection brbyInter = getPlaneDistSquare(bottomRightBack, bottomPlane.GetUnitNormal(), *bp, brby);
	if (brbyInter == Intersection_FRONT || brbyInter == Intersection_THROUGH && brbx <= dimensions.y * 2)
	{return brbyInter;}
	float brbz = 0;
	Intersection brbzInter = getPlaneDistSquare(bottomRightBack, rearPlane.GetUnitNormal(), *bp, brbz);
	if (brbzInter == Intersection_FRONT || brbzInter == Intersection_THROUGH && brbz <= dimensions.z * 2)
	{return brbzInter;}
	
	float trfx = 0;
	Intersection trfxInter = getPlaneDistSquare(topRightFront, rightPlane.GetUnitNormal(), *bp, trfx);
	if (trfxInter == Intersection_FRONT || trfxInter == Intersection_THROUGH && trfx <= dimensions.x * 2)
	{return trfxInter;}
	float trfy = 0;
	Intersection trfyInter = getPlaneDistSquare(topRightFront, topPlane.GetUnitNormal(), *bp, trfy);
	if (trfyInter == Intersection_FRONT || trfyInter == Intersection_THROUGH && trfx <= dimensions.y * 2)
	{return trfyInter;}
	float trfz = 0;
	Intersection trfzInter = getPlaneDistSquare(topRightFront, frontPlane.GetUnitNormal(), *bp, trfz);
	if (trfzInter == Intersection_FRONT || trfzInter == Intersection_THROUGH && trfz <= dimensions.z * 2)
	{return trfzInter;}
	
	float blfx = 0;
	Intersection blfxInter = getPlaneDistSquare(bottomLeftFront, leftPlane.GetUnitNormal(), *bp, blfx);
	if (blfxInter == Intersection_FRONT || blfxInter == Intersection_THROUGH && blfx <= dimensions.x * 2)
	{return blfxInter;}
	float blfy = 0;
	Intersection blfyInter = getPlaneDistSquare(bottomLeftFront, bottomPlane.GetUnitNormal(), *bp, blfy);
	if (blfyInter == Intersection_FRONT || blfyInter == Intersection_THROUGH && blfx <= dimensions.y * 2)
	{return blfyInter;}
	float blfz = 0;
	Intersection blfzInter = getPlaneDistSquare(bottomLeftFront, frontPlane.GetUnitNormal(), *bp, blfz);
	if (blfzInter == Intersection_FRONT || blfzInter == Intersection_THROUGH && blfz <= dimensions.z * 2)
	{return blfzInter;}

	return Intersection_BACK;

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
	else if (shape == Shape_BOX)
	{

		BoundingBox* box = (BoundingBox*)bounding;
		Intersection interX = box->IntersectsBetweenPlanes(&rightPlane, &leftPlane);

		if (interX == Intersection_BACK)
		{
			
			return Intersection_BACK;

		}

		Intersection interY = box->IntersectsBetweenPlanes(&topPlane, &bottomPlane);

		if (interY == Intersection_BACK)
		{

			return Intersection_BACK;

		}

		Intersection interZ = box->IntersectsBetweenPlanes(&rearPlane, &frontPlane);

		if (interZ == Intersection_BACK)
		{

			return Intersection_BACK;

		}

		if ((interX == Intersection_FRONT || interX == Intersection_ON) &&
			(interY == Intersection_FRONT || interY == Intersection_ON) &&
			(interZ == Intersection_FRONT || interZ == Intersection_ON))
		{

			return Intersection_FRONT;

		}
		else
		{

			return Intersection_THROUGH;

		}
	}

	return Intersection_NA;

}

void BoundingBox::Transform(Matrix mat)
{

	pos = VectorTransform(Vector3(0,0,0),mat);

	Vector3 x1 = VectorTransform(Vector3(dimensions.x, 0, 0), mat);
	Vector3 y1 = VectorTransform(Vector3(0, dimensions.y, 0), mat);
	Vector3 z1 = VectorTransform(Vector3(0, 0, dimensions.z), mat);

	Vector3 x2 = VectorTransform(Vector3(-dimensions.x, 0, 0), mat);
	Vector3 y2 = VectorTransform(Vector3(0, -dimensions.y, 0), mat);
	Vector3 z2 = VectorTransform(Vector3(0, 0, -dimensions.z), mat);

	constructBox(x1, x2, y1, y2, z1, z2);
	dimensions = Vector3(leftPlane.GetNormalLength(), topPlane.GetNormalLength(), frontPlane.GetNormalLength());

}

void BoundingBox::constructBox(Vector3 x1, Vector3 x2, Vector3 y1, Vector3 y2, Vector3 z1, Vector3 z2)
{

	Vector3 xNormal = pos - x1;
	Vector3 yNormal = pos - y1;
	Vector3 zNormal = pos - z1;

	rightPlane = BoundingPlane(xNormal,
		-(VectorDot(x1, xNormal)));
	leftPlane = BoundingPlane(-xNormal,
		-(VectorDot(x2, -xNormal)),rightPlane.GetNormalLength());

	topPlane = BoundingPlane(yNormal,
		-(VectorDot(y1, yNormal)));
	bottomPlane = BoundingPlane(-yNormal,
		-(VectorDot(y2, -yNormal)), topPlane.GetNormalLength());

	rearPlane = BoundingPlane(zNormal,
		-(VectorDot(z1, zNormal)));
	frontPlane = BoundingPlane(-zNormal,
		-(VectorDot(z2, -zNormal)), rearPlane.GetNormalLength());

}

BoundingBox::~BoundingBox()
{


}
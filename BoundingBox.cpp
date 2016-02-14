#include "stdafx.h"
#include "CelestialMath.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"

#include <time.h>

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

Intersection BoundingBox::getPlaneDistSquare(Vector3 &point1, Vector3 &point2, BoundingPlane& plane)
{

	float distance1 = VectorDot(plane.unitNormals, point1);
	float distance2 = VectorDot(plane.unitNormals, point2);

	if (distance1 < 0 && distance2 >= 0)
	{
		return Intersection_THROUGH;

	}
	else if (distance1 >= 0 && distance2 >= 0)
	{

		return Intersection_FRONT;

	}

	return Intersection_BACK;

}

Intersection BoundingBox::getPlaneDistSquare(Vector3& origin, Vector3& direction, BoundingPlane& plane, float& distSquare)
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

		distSquare = VectorDot(origin, plane.GetUnitNormal()) + plane.GetP();
		Intersection retVal = distSquare >= -epsilon ?
		Intersection_FRONT : Intersection_BACK;
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

							bool foundSmallest = false;
							//The line crosses all planes
							//Sort hits by distance or if the line starts in front of a plane 
							for (unsigned char i = 0; i < 6; i++)
							{

								for (unsigned char k = i + 1; k < 6; k++)
								{

									if (inters[i] != Intersection_FRONT)
									{

										if (distances[i] < smallestDistanceSquare || smallestDistanceSquare == 0)
										{

											smallestDistanceSquare = distances[i];
											foundSmallest = true;

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
							}

							if (!foundSmallest)
							{

								return Intersection_BACK;

							}

							//Figure out the order of hits
							bool hitx = false;
							bool hity = false;
							bool hitz = false;
							bool outside = false;

							for (unsigned char i = 0; i < 6 && !outside && !hitx && !hity && hitz; i++)
							{

								if (dims[i] == 'x')
								{

									outside = hitx && inters[i] != Intersection_FRONT;
									hitx = true;

								}
								else if (dims[i] == 'y')
								{

									outside = hity && inters[i] != Intersection_FRONT;
									hity = true;

								}
								else if (dims[i] == 'z')
								{

									outside = hitz && inters[i] != Intersection_FRONT;
									hitz = true;

								}
							}

							if (!outside)
							{

								return Intersection_THROUGH;

							}
						}
					}
				}
			}
		}
	}

	return Intersection_BACK;

}

Intersection BoundingBox::IntersectsPlane(BoundingPlane* bp)
{

	float upCache = bottomPlane.unitNormals.y * dimensions.y;
	float downCache = -upCache;
	float frontCache = rearPlane.unitNormals.z * dimensions.z;
	float rearCache = -frontCache;

	Vector3 ltfCache = leftPoint + Vector3(0, upCache, frontCache);
	Vector3 ltrCache = leftPoint + Vector3(0, upCache, rearCache);
	Vector3 lbfCache = leftPoint + Vector3(0, downCache, frontCache);
	Vector3 lbrCache = leftPoint + Vector3(0, downCache, rearCache);

	Vector3 rtfCache = rightPoint + Vector3(0, upCache, frontCache);
	Vector3 rtrCache = rightPoint + Vector3(0, upCache, rearCache);
	Vector3 rbfCache = rightPoint + Vector3(0, downCache, frontCache);
	Vector3 rbrCache = rightPoint + Vector3(0, downCache, rearCache);

	Intersection ftxInter = getPlaneDistSquare(ltfCache, rtfCache, *bp);
	if (ftxInter == Intersection_THROUGH)
	{return Intersection_THROUGH;}
	Intersection fbxInter = getPlaneDistSquare(lbfCache, rbfCache, *bp);
	if (fbxInter == Intersection_THROUGH || fbxInter != ftxInter)
	{return Intersection_THROUGH;}
	Intersection flyInter = getPlaneDistSquare(ltfCache, lbfCache, *bp);
	if (fbxInter == Intersection_THROUGH || fbxInter != flyInter)
	{return Intersection_THROUGH;}
	Intersection fryInter = getPlaneDistSquare(rtfCache, rbfCache, *bp);
	if (fbxInter == Intersection_THROUGH || fryInter != flyInter)
	{return Intersection_THROUGH;}

	Intersection rtxInter = getPlaneDistSquare(ltrCache, rtrCache, *bp);
	if (rtxInter == Intersection_THROUGH || rtxInter != fryInter)
	{return Intersection_THROUGH;}
	Intersection rbxInter = getPlaneDistSquare(lbrCache, rbrCache, *bp);
	if (rbxInter == Intersection_THROUGH || rbxInter != rtxInter)
	{return Intersection_THROUGH;}
	Intersection rlyInter = getPlaneDistSquare(ltrCache, lbrCache, *bp);
	if (rbxInter == Intersection_THROUGH || rbxInter != rlyInter)
	{return Intersection_THROUGH;}
	Intersection rryInter = getPlaneDistSquare(rtrCache, rbrCache, *bp);
	if (rbxInter == Intersection_THROUGH || rryInter != rlyInter)
	{return Intersection_THROUGH;}

	Intersection rtzInter = getPlaneDistSquare(rtrCache, rtfCache, *bp);
	if (rtxInter == Intersection_THROUGH || rtzInter != rryInter)
	{return Intersection_THROUGH;}
	Intersection rbzInter = getPlaneDistSquare(rbfCache, rbrCache, *bp);
	if (rbxInter == Intersection_THROUGH || rtzInter != rbzInter)
	{return Intersection_THROUGH;}
	Intersection ltzInter = getPlaneDistSquare(ltrCache, ltfCache, *bp);
	if (rbxInter == Intersection_THROUGH || rbzInter != ltzInter)
	{return Intersection_THROUGH;}
	Intersection lbzInter = getPlaneDistSquare(lbfCache, lbrCache, *bp);
	if (rbxInter == Intersection_THROUGH || lbzInter != ltzInter)
	{return Intersection_THROUGH;}

	return ftxInter;

}

Intersection BoundingBox::IntersectsBetweenPlanes(BoundingPlane* p1, BoundingPlane* p2)
{

	unsigned int time1 = clock();
	Intersection p1Result = IntersectsPlane(p1);
	Intersection p2Result = IntersectsPlane(p2);
	unsigned int time2 = clock();

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

	rightPoint = x1;
	leftPoint = x2;

	topPoint = y1;
	bottomPoint = y2;

	frontPoint = z1;
	rearPoint = z2;

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
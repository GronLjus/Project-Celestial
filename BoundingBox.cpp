#include "stdafx.h"
#include "CelestialMath.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"

#include <time.h>

using namespace CrossHandlers;
using namespace CelestialMath;

BoundingBox::BoundingBox()
{

	epsilon = 0.00002f;
	cornerPoints = new Vector3[8];

}
		
BoundingBox::BoundingBox(float width,float heigth,float depth,float centerX,float centerY,float centerZ) : BoundingBox()
{

	dimensions = Vector3(width*0.5f,heigth*0.5f,depth*0.5f);
	startDimensions = dimensions;
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

Vector3 BoundingBox::GetPosition() const
{

	return pos;

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

Vector3 BoundingBox::GetPlaneNormal(unsigned char i) const
{

	return i == 0 ? leftPlane.GetUnitNormal() : i == 1 ? bottomPlane.GetUnitNormal(): frontPlane.GetUnitNormal();

}

Intersection BoundingBox::getPlaneDistSquare(Vector3 &point1, Vector3 &point2, BoundingPlane& plane)
{

	float distance1 = VectorDot(plane.unitNormals, point1) + plane.GetP();
	float distance2 = VectorDot(plane.unitNormals, point2) + plane.GetP();

	if (distance1 < 0 && distance2 >= 0 || distance1 >= 0 && distance2 < 0)
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

							for (unsigned char i = 0; i < 6; i++)
							{
								if (distances[i] < smallestDistanceSquare || smallestDistanceSquare == 0)
								{

									smallestDistanceSquare = distances[i];
									foundSmallest = true;

								}
							}

							//The line crosses all planes
							//Sort hits by distance or if the line starts in front of a plane 
							for (unsigned char i = 0; i < 6; i++)
							{

								for (unsigned char k = i + 1; k < 6 && (inters[i] != Intersection_FRONT); k++)
								{
									if (inters[k] == Intersection_FRONT || distances[k] < distances[i])
									{

										char dimTemp = dims[k];
										Intersection interTemp = inters[k];
										float distanceTemp = distances[k];

										dims[k] = dims[i];
										inters[k] = inters[i];
										distances[k] = distances[i];

										dims[i] = dimTemp;
										inters[i] = interTemp;
										distances[i] = distanceTemp;

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

							for (unsigned char i = 0; i < 6 && (!outside && ( !hitx || !hity || !hitz)); i++)
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

Intersection BoundingBox::checkSATEdges(Vector3 &b, Vector3 &D, Vector3* A, Vector3* B, float C[][3])
{

	Vector3 a = dimensions*2;
	float vda0 = VectorDot(A[0], D);
	float vda1 = VectorDot(A[1], D);
	float vda2 = VectorDot(A[2], D);

	 //Perform the nine checks on the edges of the boxes with a complicated formula found on the interner, I hope it works since I can't understand it.
	if ((a[1] * C[2][0] + a[2] * C[1][0]) + (b[1] * C[0][2] + b[2] * C[0][1]) < abs(C[1][0] * vda2 - C[2][0] * vda1) ||
		(a[1] * C[2][1] + a[2] * C[1][1]) + (b[0] * C[0][2] + b[2] * C[0][0]) < abs(C[1][1] * vda2 - C[2][1] * vda1) ||
		(a[1] * C[2][2] + a[2] * C[1][2]) + (b[0] * C[0][1] + b[1] * C[0][0]) < abs(C[1][2] * vda2 - C[2][2] * vda1) ||
		
		(a[0] * C[2][0] + a[2] * C[0][0]) + (b[1] * C[1][2] + b[2] * C[1][1]) < abs(C[2][0] * vda0 - C[0][0] * vda2) ||
		(a[0] * C[2][1] + a[2] * C[0][1]) + (b[0] * C[1][2] + b[2] * C[1][0]) < abs(C[2][1] * vda0 - C[0][1] * vda2) ||
		(a[0] * C[2][2] + a[2] * C[0][2]) + (b[0] * C[1][1] + b[1] * C[1][0]) < abs(C[2][2] * vda0 - C[0][2] * vda2) ||

		(a[0] * C[1][0] + a[1] * C[0][0]) + (b[1] * C[2][2] + b[2] * C[2][1]) < abs(C[0][0] * vda1 - C[1][0] * vda0) ||
		(a[0] * C[1][1] + a[1] * C[0][1]) + (b[0] * C[2][2] + b[2] * C[2][0]) < abs(C[0][1] * vda1 - C[1][1] * vda0) ||
		(a[0] * C[1][2] + a[1] * C[0][2]) + (b[0] * C[2][1] + b[1] * C[2][0]) < abs(C[0][2] * vda1 - C[1][2] * vda0))
	{

		return Intersection_BACK;

	}

	return Intersection_FRONT;

}

Intersection BoundingBox::IntersectsBounding(IBounding* bounding,Shape shape)
{

	if (shape == Shape_BOX)
	{

		//Use SAT to find a collison
		BoundingBox* otherBox = (BoundingBox*)bounding;
		Vector3 otherDim = otherBox->GetDimensions();
		Vector3 D = otherBox->GetPosition() - pos;

		Vector3 A[] = { leftPlane.GetUnitNormal(),
			bottomPlane.GetUnitNormal(),
			frontPlane.GetUnitNormal() };

		Vector3 B[] = { otherBox->GetPlaneNormal(0),
			otherBox->GetPlaneNormal(1),
			otherBox->GetPlaneNormal(2) };

		float C[3][3];

		//Check the first three axis
		for (char i = 0; i < 3;i++)
		{ 
	
			float R = abs(VectorDot(A[i], D));
			float r0 = dimensions[i];

			C[i][0] = abs(VectorDot(A[i], B[0]));
			C[i][1] = abs(VectorDot(A[i], B[1]));
			C[i][2] = abs(VectorDot(A[i], B[2]));
			float r1 = otherDim.x * C[i][0] +
				otherDim.y * C[i][1] +
				otherDim.z * C[i][2];

			if (R > r0 + r1)
			{

				return Intersection_BACK;

			}
		
		}

		//Check the second three axises
		for (char i = 0; i < 3; i++)
		{

			float R = abs(VectorDot(B[i], D));
			float r1 = otherDim[i];

			float r0 = dimensions.x * C[0][i] +
				dimensions.y * C[1][i] +
				dimensions.z * C[2][i];

			if (R > r0 + r1)
			{

				return Intersection_BACK;

			}

		}

		return checkSATEdges(otherDim, D, A, B, C);

	}

	Intersection interX = bounding->IntersectsBetweenPlanes(&rightPlane, &leftPlane);

	if (interX == Intersection_BACK)
	{
			
		return Intersection_BACK;

	}

	Intersection interY = bounding->IntersectsBetweenPlanes(&topPlane, &bottomPlane);

	if (interY == Intersection_BACK)
	{

		return Intersection_BACK;

	}

	Intersection interZ = bounding->IntersectsBetweenPlanes(&rearPlane, &frontPlane);

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

	return Intersection_NA;

}

void BoundingBox::Transform(Matrix mat)
{

	pos = VectorTransform(Vector3(0,0,0),mat);

	Vector3 x1 = VectorTransform(Vector3(startDimensions.x, 0, 0), mat);
	Vector3 y1 = VectorTransform(Vector3(0, startDimensions.y, 0), mat);
	Vector3 z1 = VectorTransform(Vector3(0, 0, startDimensions.z), mat);

	Vector3 x2 = VectorTransform(Vector3(-startDimensions.x, 0, 0), mat);
	Vector3 y2 = VectorTransform(Vector3(0, -startDimensions.y, 0), mat);
	Vector3 z2 = VectorTransform(Vector3(0, 0, -startDimensions.z), mat);

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


	float upCache = bottomPlane.unitNormals.y * dimensions.y;
	float downCache = -upCache;
	float frontCache = rearPlane.unitNormals.z * dimensions.z;
	float rearCache = -frontCache;

	cornerPoints[0] = leftPoint + Vector3(0, upCache, frontCache);
	cornerPoints[1] = leftPoint + Vector3(0, upCache, rearCache);
	cornerPoints[2] = leftPoint + Vector3(0, downCache, frontCache);
	cornerPoints[3] = leftPoint + Vector3(0, downCache, rearCache);

	cornerPoints[4] = rightPoint + Vector3(0, upCache, frontCache);
	cornerPoints[5] = rightPoint + Vector3(0, upCache, rearCache);
	cornerPoints[6] = rightPoint + Vector3(0, downCache, frontCache);
	cornerPoints[7] = rightPoint + Vector3(0, downCache, rearCache);

}

Vector3 BoundingBox::GetDimensions() const
{

	return dimensions;

}

BoundingBox::~BoundingBox()
{

	delete[] cornerPoints;

}
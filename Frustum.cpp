#include "stdafx.h"
#include "Frustum.h"
#include "CelestialMath.h"

using namespace CrossHandlers;
using namespace CelestialMath;

Frustum::Frustum(Matrix viewProjection,Matrix invViewProjection)
{
	
	this->sphere = nullptr;
	init();
	Construct(viewProjection,invViewProjection,true);

}


Frustum::Frustum(Frustum* copy)
{
	
	this->sphere = (BoundingSphere*)(copy->GetSphere()->GetCopy());
	init();

	for(int i=0;i<6;i++)
	{

		BoundingPlane* plane= copy->GetPlane(Plane(i));

		if(plane != nullptr)
		{
		
			planes[i] = new BoundingPlane(plane);

		}
	}

	Vector2* copCorners = copy->GetUpperCornerPoints(cornerSize);

	for(int i=0;i<cornerSize;i++)
	{
	
		topPoints[i] = copCorners[i];

	}
}

void Frustum::init()
{

	topPoints = new Vector2[6];
	neighBourTo = new char*[8];
	planes = new BoundingPlane*[6];
	planePoints = new char*[6];
	neighSides = new char**[8];
	planes[0] = nullptr;

	for(int i=0;i<6;i++)
	{

		planes[i] = nullptr;
		planePoints[i] = new char[4];

	}

	planePoints[Plane_NEAR][0] = 0;
	planePoints[Plane_NEAR][1] = 1;
	planePoints[Plane_NEAR][2] = 2;
	planePoints[Plane_NEAR][3] = 3;
	
	planePoints[Plane_FAR][0]  = 5;
	planePoints[Plane_FAR][1] = 6;
	planePoints[Plane_FAR][2] = 4;
	planePoints[Plane_FAR][3] = 7;

	planePoints[Plane_LEFT][0] = 1;
	planePoints[Plane_LEFT][1]  = 2;
	planePoints[Plane_LEFT][2]  = 4;
	planePoints[Plane_LEFT][3]  = 6;

	planePoints[Plane_RIGHT][0] = 0;
	planePoints[Plane_RIGHT][1] = 3;
	planePoints[Plane_RIGHT][2] = 7;
	planePoints[Plane_RIGHT][3] = 5;

	planePoints[Plane_UP][0] = 1;
	planePoints[Plane_UP][1] = 6;
	planePoints[Plane_UP][2] = 5;
	planePoints[Plane_UP][3] = 0;

	planePoints[Plane_DOWN][0] = 2;
	planePoints[Plane_DOWN][1] = 4;
	planePoints[Plane_DOWN][2] = 7;
	planePoints[Plane_DOWN][3] = 3;

	for(int i=0;i<8;i++)
	{

		neighBourTo[i] = new char[3];//each point has a maximum of three neighbours
		neighSides[i] = new char*[3];

		for(int k=0;k<3;k++)
		{

			neighSides[i][k] = new char[2];//Each neighbour-line can only exist on two faces
		
		}
	}

	//RightUpperNear = 0
	neighBourTo[0][0] = 1;//leftUpperNear
	neighSides[0][0][0] = Plane_NEAR;
	neighSides[0][0][1] = Plane_UP;
	neighBourTo[0][1] = 3;//rightLowerNear
	neighSides[0][1][0] = Plane_NEAR;
	neighSides[0][1][1] = Plane_RIGHT;
	neighBourTo[0][2] = 5;//rightUpperFar
	neighSides[0][2][0] = Plane_RIGHT;
	neighSides[0][2][1] = Plane_UP;
	
	//leftUpperNear = 1
	neighBourTo[1][0] = 0;//RightUpperNear
	neighSides[1][0][0] = Plane_NEAR;
	neighSides[1][0][1] = Plane_UP;
	neighBourTo[1][1] = 2;//leftLowerNear
	neighSides[1][1][0] = Plane_NEAR;
	neighSides[1][1][1] = Plane_LEFT;
	neighBourTo[1][2] = 6;//leftUpperFar
	neighSides[1][2][0] = Plane_LEFT;
	neighSides[1][2][1] = Plane_UP;
	
	//leftLowerNear = 2
	neighBourTo[2][0] = 3;//rightLowerNear
	neighSides[2][0][0] = Plane_NEAR;
	neighSides[2][0][1] = Plane_DOWN;
	neighBourTo[2][1] = 1;//leftUpperNear
	neighSides[2][1][0] = Plane_NEAR;
	neighSides[2][1][1] = Plane_LEFT;
	neighBourTo[2][2] = 4;//leftLowerFar
	neighSides[2][2][0] = Plane_LEFT;
	neighSides[2][2][1] = Plane_DOWN;
	
	//rightLowerNear = 3
	neighBourTo[3][0] = 2;//leftLowerNear
	neighSides[3][0][0] = Plane_NEAR;
	neighSides[3][0][1] = Plane_DOWN;
	neighBourTo[3][1] = 0;//RightUpperNear
	neighSides[3][1][0] = Plane_NEAR;
	neighSides[3][1][1] = Plane_RIGHT;
	neighBourTo[3][2] = 7;//rightLowerFar
	neighSides[3][2][0] = Plane_RIGHT;
	neighSides[3][2][1] = Plane_DOWN;
	
	//leftLowerFar = 4
	neighBourTo[4][0] = 6;//leftUpperFar
	neighSides[4][0][0] = Plane_FAR;
	neighSides[4][0][1] = Plane_LEFT;
	neighBourTo[4][1] = 7;//rightLowerFar
	neighSides[4][1][0] = Plane_FAR;
	neighSides[4][1][1] = Plane_DOWN;
	neighBourTo[4][2] = 2;//leftLowerNear
	neighSides[4][2][0] = Plane_DOWN;
	neighSides[4][2][1] = Plane_LEFT;
	
	//rightUpperFar = 5
	neighBourTo[5][0] = 7;//rightLowerFar
	neighSides[5][0][0] = Plane_FAR;
	neighSides[5][0][1] = Plane_RIGHT;
	neighBourTo[5][1] = 6;//leftUpperFar
	neighSides[5][1][0] = Plane_FAR;
	neighSides[5][1][1] = Plane_UP;
	neighBourTo[5][2] = 0;//RightUpperNear
	neighSides[5][2][0] = Plane_UP;
	neighSides[5][2][1] = Plane_RIGHT;
	
	//leftUpperFar = 6
	neighBourTo[6][0] = 4;//leftLowerFar
	neighSides[6][0][0] = Plane_FAR;
	neighSides[6][0][1] = Plane_LEFT;
	neighBourTo[6][1] = 5;//rightUpperFar
	neighSides[6][1][0] = Plane_FAR;
	neighSides[6][1][1] = Plane_UP;
	neighBourTo[6][2] = 1;//leftUpperNear
	neighSides[6][2][0] = Plane_UP;
	neighSides[6][2][1] = Plane_LEFT;
	
	//rightLowerFar = 7
	neighBourTo[7][0] = 5;//rightUpperFar
	neighSides[7][0][0] = Plane_FAR;
	neighSides[7][0][1] = Plane_RIGHT;
	neighBourTo[7][1] = 4;//leftLowerFar
	neighSides[7][1][0] = Plane_FAR;
	neighSides[7][1][1] = Plane_DOWN;
	neighBourTo[7][2] = 3;//rightLowerNear
	neighSides[7][2][0] = Plane_DOWN;
	neighSides[7][2][1] = Plane_RIGHT;

}

void Frustum::Construct(Matrix viewProjection,Matrix invViewProjection,bool projectionChanged)
{

	if(planes[0] != nullptr)
	{
		
		for(int i=0;i<6;i++)
		{

			delete planes[i];

		}
	}

	planes[Plane_LEFT] = new BoundingPlane(viewProjection._14+viewProjection._11,viewProjection._24+viewProjection._21,viewProjection._34+viewProjection._31,viewProjection._44+viewProjection._41);
	planes[Plane_RIGHT] = new BoundingPlane(viewProjection._14-viewProjection._11,viewProjection._24-viewProjection._21,viewProjection._34-viewProjection._31,viewProjection._44-viewProjection._41);
	planes[Plane_DOWN] = new BoundingPlane(viewProjection._14+viewProjection._12,viewProjection._24+viewProjection._22,viewProjection._34+viewProjection._32,viewProjection._44+viewProjection._42);
	planes[Plane_UP] = new BoundingPlane(viewProjection._14-viewProjection._12,viewProjection._24-viewProjection._22,viewProjection._34-viewProjection._32,viewProjection._44-viewProjection._42);
	planes[Plane_NEAR] = new BoundingPlane(viewProjection._13,viewProjection._23,viewProjection._33,viewProjection._43);
	planes[Plane_FAR] =  new BoundingPlane(viewProjection._14-viewProjection._13,viewProjection._24-viewProjection._23,viewProjection._34-viewProjection._33,viewProjection._44-viewProjection._43);
	

	Vector4 rightUpperNear = VectorTransform(Vector4(1, 1, 0, 1), invViewProjection);
	rightUpperNear /= rightUpperNear.w;
	Vector4 leftUpperNear = VectorTransform(Vector4(-1, 1, 0, 1), invViewProjection);
	leftUpperNear /= leftUpperNear.w;
	Vector4 leftLowerNear = VectorTransform(Vector4(-1, -1, 0, 1), invViewProjection);
	leftLowerNear /= leftLowerNear.w;
	Vector4 rightLowerNear = VectorTransform(Vector4(1, -1, 0, 1), invViewProjection);
	rightLowerNear /= rightLowerNear.w;
	
	Vector4 leftLowerFar = VectorTransform(Vector4(-1, -1, 1, 1), invViewProjection);
	leftLowerFar /= leftLowerFar.w;
	Vector4 rightUpperFar = VectorTransform(Vector4(1, 1, 1, 1), invViewProjection);
	rightUpperFar /= rightUpperFar.w;
	Vector4 leftUpperFar = VectorTransform(Vector4(-1, 1, 1, 1), invViewProjection);
	leftUpperFar /= leftUpperFar.w;
	Vector4 rightLowerFar = VectorTransform(Vector4(1, -1, 1, 1), invViewProjection);
	rightLowerFar /= rightLowerFar.w;
	
	bool* isFace = new bool[6];
	Vector2* points = new Vector2[8];
	char* pointIndices = new char[20];
	char* faces = new char[8];
	char** neighbours = new char*[8];
	char* currentNeighbours = new char[8];
	float epsilon = 0.0000002f;

	for(int i=0;i<8;i++)
	{

		faces[i] = 0;
		currentNeighbours[i] = 0;
		neighbours[i] = new char[3];//Each point can only have three neighbours

	}

	int indSize = 0;

	points[0] = Vector2(rightUpperNear.x,rightUpperNear.z);
	points[1] = Vector2(leftUpperNear.x,leftUpperNear.z);
	points[2] = Vector2(leftLowerNear.x,leftLowerNear.z);
	points[3] = Vector2(rightLowerNear.x,rightLowerNear.z);
	
	points[4] = Vector2(leftLowerFar.x,leftLowerFar.z);
	points[5] = Vector2(rightUpperFar.x,rightUpperFar.z);
	points[6] = Vector2(leftUpperFar.x,leftUpperFar.z);
	points[7] = Vector2(rightLowerFar.x,rightLowerFar.z);

	for(int i=0;i<6;i++)
	{
	
		isFace[i] = false;

		if(planes[i]->GetNormal()[1] < -epsilon)//See if the plane points upwards
		{

			isFace[i] = true;

			for(int k=0;k<4;k++)
			{

				pointIndices[indSize++] = planePoints[i][k];
				faces[planePoints[i][k]]++;

			}
		}
	}

	char* corners = new char[6];
	int cornPos = 0;

	for(int i=0;i<indSize;i++)//Cull out the points that aren't on the edge
	{

		if(faces[pointIndices[i]] < 3)//Any point attached to three or more faces is not on the edge
		{

			bool exists = false;

			for(int k=0;k<cornPos && !exists;k++)//Make sure the list is exclusive
			{

				exists = corners[k] == pointIndices[i];

			}

			if(!exists)
			{

				corners[cornPos] = pointIndices[i];
				cornPos++;

				for(int k=0;k<3;k++)
				{

					bool isOnFace = false;
					int point = neighBourTo[pointIndices[i]][k];

					for(int j=0;j<2 && !isOnFace;j++)//Check if this neighbour-pair is visible
					{

						isOnFace = isFace[neighSides[point][k][j]];

					}

					if(isOnFace)
					{

						neighbours[point][currentNeighbours[point]] = pointIndices[i];
						currentNeighbours[point]++;

					}
				}
			}
		}
	}

	delete[] faces;
	delete[] pointIndices;
	int firstIndex = corners[0];
	int index = corners[0];
	int lastIndex = index;
	int corner = 0;
	cornerSize = cornPos;

	do
	{

		int thisIndex = index;
		topPoints[corner] = points[index];
		corner++;
		index = neighbours[thisIndex][0];

		if(index == lastIndex && currentNeighbours[thisIndex] > 1)
		{

			index = neighbours[thisIndex][1];

		}

		lastIndex = thisIndex;

	}
	while(index != firstIndex);
	
	for(int i=0;i<8;i++)
	{

		delete[] neighbours[i];

	}

	delete[] neighbours;
	delete[] corners;
	delete[] currentNeighbours;
	delete[] points;
	delete[] isFace;

	float depth = abs(planes[Plane_FAR]->GetNormal()[0]*leftLowerNear.x+planes[Plane_FAR]->GetNormal()[1]*leftLowerNear.y+planes[Plane_FAR]->GetNormal()[2]*leftLowerNear.z+planes[Plane_FAR]->GetD())/planes[Plane_FAR]->GetNormalLength();

	//Find the center of the frustum
	Vector3 center = Vector3(leftUpperNear.x+(rightUpperNear.x-leftUpperNear.x)*0.5f,leftUpperNear.y+(rightUpperNear.y-leftUpperNear.y)*0.5f,leftUpperNear.z+(rightUpperNear.z-leftUpperNear.z)*0.5f);
	center.x += (leftLowerNear.x-leftUpperNear.x)*0.5f;
	center.y += (leftLowerNear.y-leftUpperNear.y)*0.5f;
	center.z += (leftLowerNear.z-leftUpperNear.z)*0.5f;
	center.z += depth*0.5f;

	if(projectionChanged)//Change the bounding sphere used by this frustum
	{
	
		Vector4 lineVecNear = rightLowerFar-leftUpperNear;
		Vector4 lineVecFar = rightLowerNear-leftUpperFar;
	
		Vector4 point1 = leftLowerFar;
		float depthConstant = 0;
		Vector4 point2 = rightUpperFar*depthConstant;

		if((leftLowerFar.x-rightUpperFar.x)*(leftLowerFar.x-rightUpperFar.x)+(leftLowerFar.y-rightUpperFar.y)*(leftLowerFar.y-rightUpperFar.y)+(leftLowerFar.z-rightUpperFar.z)*(leftLowerFar.z-rightUpperFar.z) < (leftLowerNear.x-rightUpperNear.x)*(leftLowerNear.x-rightUpperNear.x)+(leftLowerNear.y-rightUpperNear.y)*(leftLowerNear.y-rightUpperNear.y)+(leftLowerNear.z-rightUpperNear.z)*(leftLowerNear.z-rightUpperNear.z))
		{
	
			point1 = rightLowerNear;
			depthConstant = (lineVecNear.x*planes[Plane_FAR]->GetNormal()[0]+lineVecNear.y*planes[Plane_FAR]->GetNormal()[1]+lineVecNear.z*planes[Plane_FAR]->GetNormal()[2])/(planes[Plane_FAR]->GetNormalLength()*planes[Plane_FAR]->GetNormalLength());
			
			point2.x = point1.x+planes[Plane_FAR]->GetNormal()[0]*depthConstant;
			point2.y = point1.y+planes[Plane_FAR]->GetNormal()[1]*depthConstant;
			point2.z = point1.z+planes[Plane_FAR]->GetNormal()[2]*depthConstant;
			
			point1 = leftUpperNear;

		}
		else
		{

			point1 = rightLowerFar;
			depthConstant = (lineVecFar.x*planes[Plane_NEAR]->GetNormal()[0]+lineVecFar.y*planes[Plane_NEAR]->GetNormal()[1]+lineVecFar.z*planes[Plane_NEAR]->GetNormal()[2])/(planes[Plane_NEAR]->GetNormalLength()*planes[Plane_NEAR]->GetNormalLength());
			
			point2.x = planes[Plane_NEAR]->GetNormal()[0]*depthConstant;
			point2.y = planes[Plane_NEAR]->GetNormal()[1]*depthConstant;
			point2.z = planes[Plane_NEAR]->GetNormal()[2]*depthConstant;

			point2 = point1+point2;
			point1 = leftUpperFar;

		}

		float diameter = sqrt(pow(point1.x-point2.x,2)+pow(point1.y-point2.y,2)+pow(point1.z-point2.z,2));

		if(this->sphere != nullptr)
		{

			this->sphere->SetRadi(diameter*0.5f);

		}
		else
		{

			this->sphere = new BoundingSphere(center.x,center.y,center.z,diameter*0.5f);

		}
	}

	if(this->sphere != nullptr)
	{

		this->sphere->SetX(center.x);
		this->sphere->SetY(center.y);
		this->sphere->SetZ(center.z);

	}
}

Vector3 Frustum::getCommonPoint(BoundingPlane* plane1,BoundingPlane* plane2,BoundingPlane* plane3)
{

	//Solve by creating a system of linear equation
	float epsilon = 0.00000002f;
	float x1 = plane1->GetNormal()[0];
	float y1 = plane1->GetNormal()[1];
	float z1 = plane1->GetNormal()[2];
	float a1 = -plane1->GetD();
	
	float x2 = plane2->GetNormal()[0];
	float y2 = plane2->GetNormal()[1];
	float z2 = plane2->GetNormal()[2];
	float a2 = -plane2->GetD();

	float x3 = plane3->GetNormal()[0];
	float y3 = plane3->GetNormal()[1];
	float z3 = plane3->GetNormal()[2];
	float a3 = -plane3->GetD();
	
	//Change row 1 with row 2
	if(((x1 > -epsilon && x1 < epsilon) || (y2 > -epsilon && y2 < epsilon)) && ((x2 > epsilon || x2 < -epsilon)&&(y1 > epsilon || y1 < -epsilon)))
	{

		float temp = x1;
		x1 = x2;
		x2 = temp;
		temp = y1;
		y1 = y2;
		y2 = temp;
		temp = z1;
		z1 = z2;
		z2 = temp;
		temp = a1;
		a1 = a2;
		a2 = temp;

	}
	
	//change row 1 with row 3
	if(((x1 > -epsilon && x1 < epsilon)||(z3 > -epsilon && z3 < epsilon)) &&((x3 > epsilon || x3 < -epsilon)&&(z1 > epsilon || z1 < -epsilon)))
	{

		float temp = x1;
		x1 = x3;
		x3 = temp;
		temp = y1;
		y1 = y3;
		y3 = temp;
		temp = z1;
		z1 = z3;
		z3 = temp;
		temp = a1;
		a1 = a3;
		a3 = temp;

	}

	//Change row 2 with 3
	if(((y2 > -epsilon && y2 < epsilon)||(z3 > -epsilon && z3 < epsilon)) && ((y3 > epsilon || y3 < -epsilon)&&(z2 > epsilon || z2 < -epsilon)))
	{

		float temp = x2;
		x2 = x3;
		x3 = temp;
		temp = y2;
		y2 = y3;
		y3 = temp;
		temp = z2;
		z2 = z3;
		z3 = temp;
		temp = a2;
		a2 = a3;
		a3 = temp;

	}

	//Should not be possible with a frustum as all planes intersect
	if((x1 < epsilon && x1 > -epsilon) || (y2 < epsilon && y2 > -epsilon) || (z3 < epsilon && z3 > -epsilon))//We have a free value and an infinite number of points
	{
		
		return Vector3();

	}

	float ratio = x2/x1;
	x2 -= x1*ratio;//Will be zero
	y2 -= y1*ratio;
	z2 -= z1*ratio;
	a2 -= a1*ratio;

	ratio = x3/x1;
	x3 -= x1*ratio;//Will be zero
	y3 -= y1*ratio;
	z3 -= z1*ratio;
	a3 -= a1*ratio;

	ratio = y3/y2;
	x3 -= x2*ratio;//Will be zero
	y3 -= y2*ratio;//Will be zero
	z3 -= z2*ratio;
	a3 -= (a2*ratio);

	a3 /= z3;//Will be the z-value
	a2 -= z2*a3;
	a2 /= y2;//Will be y-value
	a1 -= z1*a3-y1*a2;
	a1 /=x1;//will be x-value

	return Vector3(a1,a2,a3);

}

Intersection Frustum::Check(IBounding* boundingObject,Dimension ignore)
{

	Intersection result = boundingObject->IntersectsBounding(this->sphere,Shape_SPHERE);//Check against the volume of the frustums sphere
	bool front = true;

	if(result != Intersection_BACK)
	{

		result= boundingObject->IntersectsBetweenPlanes(planes[Plane_LEFT],planes[Plane_RIGHT]);//Check the x-axis first
		front = (front && result != Intersection_THROUGH ) || (ignore == Dimension_X);

		if(result != Intersection_BACK)
		{
			
			result = boundingObject->IntersectsBetweenPlanes(planes[Plane_UP],planes[Plane_DOWN]);//Then check the y-axis
			front = (front && result != Intersection_THROUGH) || (front && ignore == Dimension_Y);

			if(result != Intersection_BACK)
			{

				result = boundingObject->IntersectsBetweenPlanes(planes[Plane_FAR],planes[Plane_NEAR]);//Then check the z-axis
				front = (front && result != Intersection_THROUGH) || (front && ignore == Dimension_Z);
	
			}
		}

		if(!front && result != Intersection_BACK)
		{

			result = Intersection_THROUGH;

		}
	}

	return result;

}

BoundingPlane* Frustum::GetPlane(Plane code)
{

	return planes[code];

}

BoundingSphere* Frustum::GetSphere()
{

	return this->sphere;

}

Vector2* Frustum::GetUpperCornerPoints(int &size)
{

	size = cornerSize;
	return topPoints;

}

Frustum::~Frustum()
{
	
	for(int i=0;i<6;i++)
	{

		if(planes[i] != nullptr)
		{
		
			delete planes[i];

		}

		delete[] planePoints[i];

	}

	if(this->sphere != nullptr)
	{

		delete this->sphere;

	}

	for(int i=0;i<8;i++)
	{


		for(int k=0;k<3;k++)
		{

			delete[] neighSides[i][k];

		}

		delete[] neighBourTo[i];
		delete[] neighSides[i];

	}

	delete[] planes;
	delete[] planePoints;
	delete[] neighBourTo;
	delete[] topPoints;
	delete[] neighSides;

}
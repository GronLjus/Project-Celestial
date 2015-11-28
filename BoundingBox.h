#pragma once

#include "IBounding.h"

namespace CrossHandlers
{

	///<summary>This class represents a volume spanned by a box</summary>
	class BoundingBox:public IBounding
	{
		private:

			///<summary>This struct represents a single point int a three-dimensional space</summary>
			struct Point
			{
		
				private:
					///<summary>The position of the point</summary>
					CelestialMath::Vector3 pos;

				public:
					Point(){}
					///<param name='xP'>[in]The x-value of the points position</param>
					///<param name='yP'>[in]The y-value of the points position</param>
					///<param name='zP'>[in]The z-value of the points position</param>
					Point(float xP,float yP,float zP)
					{

						pos = CelestialMath::Vector3(xP, yP, zP);

					}

					///<summary>Gets the points x-coordinate</summary>
					///<returns>THis points x-coordinate</returns>
					float GetX()
					{

						return pos.x;

					}
				
					///<summary>Gets the points y-coordinate</summary>
					///<returns>THis points y-coordinate</returns>
					float GetY()
					{

						return pos.y;

					}
				
					///<summary>Gets the points z-coordinate</summary>
					///<returns>THis points z-coordinate</returns>
					float GetZ()
					{

						return pos.z;

					}
			};

			///<summary>The epsilon-value used in comparisions</summary>
			float epsilon;
			///<summary>The nr of points used by this box</summary>
			int nrOfPoints;
			///<summary>The array of points that builds the box</summary>
			Point* points;
			///<summary>The array of points that builds the untransformed box</summary>
			Point* basePoints;
			///<summary>The centerpoint of the box</summary>
			CelestialMath::Vector3 pos;
			///<summary>The original position of the centerpoint</summary>
			CelestialMath::Vector3 orgPos;
			///<summary>The dimensions of the box</summary>
			CelestialMath::Vector3 dimensions;

			///<summary>Checks a point against a plane</summary>
			Intersection checkPlaneSide(BoundingPlane* bp,Point p);

		public:
			BoundingBox();
			///<param name='width'>[in]The width of the box</param>
			///<param name='heigth'>[in]The height of the box</param>
			///<param name='depth'>[in]The depth of the box</param>
			///<param name='centerX'>[in]The x-coordinate of the centerposition</param>
			///<param name='centerX'>[in]The y-coordinate of the centerposition</param>
			///<param name='centerX'>[in]The z-coordinate of the centerposition</param>
			BoundingBox(float width,float heigth,float depth,float centerX,float centerY,float centerZ);

			///<summary>Gets the x-coordinate of the box's center</summary>
			///<returns>A value representing the x-coordinate of the center</returns>
			virtual float GetX() const;
			///<summary>Gets the y-coordinate of the box's center</summary>
			///<returns>A value representing the y-coordinate of the center</returns>
			virtual float GetY() const;
			///<summary>Gets the z-coordinate of the box's center</summary>
			///<returns>A value representing the z-coordinate of the center</returns>
			virtual float GetZ() const;
			///<summary>Sets the x-coordinate of the box's center</summary>
			///<param name='x'>[in]A value representing the center-points x-coordinate</param>
			virtual void SetX(float x);
			///<summary>Sets the y-coordinate of the box's center</summary>
			///<param name='y'>[in]A value representing the center-points y-coordinate</param>
			virtual void SetY(float y);
			///<summary>Sets the x-coordinate of the box's center</summary>
			///<param name='z'>[in]A value representing the center-points z-coordinate</param>
			virtual void SetZ(float z);

			virtual ShapeComplexity GetComplexity();

			///<summary>Checks this box against a plane to see if they intersect</summary>
			///<param name='bp'>[in]A pointer to the boundingplane to check against</param>
			///<returns>How the box intersects with the plane</returns>
			virtual Intersection IntersectsPlane(BoundingPlane* bp);
			///<summary>Checks this box against two planes to see if the box is between them</summary>
			///<param name='p1'>[in]A pointer to the first plane to check against</param>
			///<param name='p2'>[in]A pointer to the second plane to check against</param>
			///<returns>How the box intersects with the two planes</returns>
			virtual Intersection IntersectsBetweenPlanes(BoundingPlane* p1, BoundingPlane* p2);
			///<summary>Checks this object against another object to see if they intersect</summary>
			///<param name='bounding'>[in]A pointer to the object to check against</param>
			///<param name='shape'>A shape that determines how the boundingobject is constructed</param>
			///<returns>How the two objects intersect, Intersection_THROUGH means they overlap</returns>
			virtual Intersection IntersectsBounding(IBounding* bounding,Shape shape);
			///<summary>Transforms the box according to a matrix</summary>
			///<param name='mat'>The matrix to transform the box by</param>
			virtual void Transform(CelestialMath::Matrix mat);
		
			///<summary>Gets a copy of this box</summary>
			///<returns>A pointer to a copy of this box</returns>
			virtual IBounding* GetCopy() const;
			virtual ~BoundingBox();

	};

}
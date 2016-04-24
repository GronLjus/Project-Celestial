#pragma once

#include "../IBounding.h"

namespace CrossHandlers
{

	///<summary>This class represents a volume spanned by a box</summary>
	class BoundingBox:public IBounding
	{
		private:

			///<summary>The epsilon-value used in comparisions</summary>
			float epsilon;
			///<summary>The centerpoint of the box</summary>
			CelestialMath::Vector3 pos;
			///<summary>The dimensions of the box</summary>
			CelestialMath::Vector3 dimensions;

			BoundingPlane topPlane;
			BoundingPlane bottomPlane;
			BoundingPlane rightPlane;
			BoundingPlane leftPlane;
			BoundingPlane frontPlane;
			BoundingPlane rearPlane;

			CelestialMath::Vector3 rightPoint;
			CelestialMath::Vector3 leftPoint;
			CelestialMath::Vector3 topPoint;
			CelestialMath::Vector3 bottomPoint;
			CelestialMath::Vector3 frontPoint;
			CelestialMath::Vector3 rearPoint;

			Intersection getPlaneDistSquare(CelestialMath::Vector3 &point1, CelestialMath::Vector3 &point2, BoundingPlane &plane); 
			Intersection getPlaneDistSquare(CelestialMath::Vector3 &origin, CelestialMath::Vector3 &direction, BoundingPlane &plane, float& distSquare);
			void constructBox(CelestialMath::Vector3 x1, CelestialMath::Vector3 x2, CelestialMath::Vector3 y1, CelestialMath::Vector3 y2, CelestialMath::Vector3 z1, CelestialMath::Vector3 z2);

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

			virtual Intersection IntersectsLine(CelestialMath::Vector3 origin, CelestialMath::Vector3 direction, float& smallestDistanceSquare);
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
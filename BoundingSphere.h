#pragma once

#include "IBounding.h"

namespace CrossHandlers
{

	///<summary>This class represents an area spanned by a sphere</summary>
	class BoundingSphere: public IBounding
	{
		private:
			///<summary>The epsilon value used to compare calculations against</summary>
			float epsilon;
			///<summary>The radius of the sphere</summary>
			float radi;
			///<summary>The position of this sphere</summary>
			CelestialMath::Vector3 pos;
			///<summary>The original position of this sphere</summary>
			CelestialMath::Vector3 orgPos;

		public:
			BoundingSphere();
			///<param name='posX'>[in]The x-coordinate of the centerposition</param>
			///<param name='posY'>[in]The y-coordinate of the centerposition</param>
			///<param name='posZ'>[in]The z-coordinate of the centerposition</param>
			///<param name='radi'>[in]The radius of this spere</param>
			BoundingSphere(float posX,float posY,float posZ,float radi);

			///<summary>Gets the radius of this sphere</summary>
			///<returns>The value meaning the radius of this sphere</returns>
			float GetRadi() const;
			///<summary>Sets the radius of this sphere</summary>
			///<param name='r'>The new radius of this sphere</param>
			void SetRadi(float r);

			///<summary>Gets the x-coordinate of the sphere's center</summary>
			///<returns>A value representing the x-coordinate of the center</returns>
			virtual float GetX() const;
			///<summary>Gets the y-coordinate of the sphere's center</summary>
			///<returns>A value representing the y-coordinate of the center</returns>
			virtual float GetY() const;
			///<summary>Gets the z-coordinate of the sphere's center</summary>
			///<returns>A value representing the z-coordinate of the center</returns>
			virtual float GetZ() const;
			///<summary>Sets the x-coordinate of the sphere's center</summary>
			///<param name='x'>[in]A value representing the center-points x-coordinate</param>
			virtual void SetX(float x);
			///<summary>Sets the y-coordinate of the sphere's center</summary>
			///<param name='y'>[in]A value representing the center-points y-coordinate</param>
			virtual void SetY(float y);
			///<summary>Sets the x-coordinate of the sphere's center</summary>
			///<param name='z'>[in]A value representing the center-points z-coordinate</param>
			virtual void SetZ(float z);

			virtual ShapeComplexity GetComplexity();

			///<summary>Checks this sphere against a plane to see if they intersect</summary>
			///<param name='bp'>[in]A pointer to the boundingplane to check against</param>
			///<returns>How the sphere intersects with the plane</returns>
			virtual Intersection IntersectsPlane(BoundingPlane* bp);
			///<summary>Checks this sphere against two planes to see if the sphere is between them</summary>
			///<param name='p1'>[in]A pointer to the first plane to check against</param>
			///<param name='p2'>[in]A pointer to the second plane to check against</param>
			///<returns>How the sphere intersects with the two planes</returns>
			virtual Intersection IntersectsBetweenPlanes(BoundingPlane* p1, BoundingPlane* p2);
			///<summary>Checks this object against another object to see if they intersect</summary>
			///<param name='bounding'>[in]A pointer to the object to check against</param>
			///<param name='shape'>A shape that determines how the boundingobject is constructed</param>
			///<returns>How the two objects intersect, Intersection_THROUGH means they overlap</returns>
			virtual Intersection IntersectsBounding(IBounding* bounding,Shape shape);
			///<summary>Transforms the box according to a matrix</summary>
			///<param name='mat'>The matrix to transform the sphere by</param>
			virtual void Transform(CelestialMath::Matrix mat);
		
			///<summary>Gets a copy of this sphere</summary>
			///<returns>A pointer to a copy of this sphere</returns>
			virtual IBounding* GetCopy() const;

	};
}
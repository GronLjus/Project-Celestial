#pragma once
#include "BoundingPlane.h"
#include "CelestialMatrix.h"
#include "CelestialVector.h"

namespace CrossHandlers
{

	enum Intersection{Intersection_FRONT,Intersection_BACK,Intersection_ON,Intersection_NA,Intersection_THROUGH};
	enum Shape{Shape_BOX,Shape_SPHERE,Shape_COUNT};
	enum ShapeComplexity{ ShapeComplexity_SPHERE, ShapeComplexity_BOX, ShapeComplexity_SIZE};

	///<summary>This interface serves to abstract the usage of boundingvolumes</summary>
	class IBounding
	{

		public:
		
			///<summary>Gets the x-coordinate of the volume's center</summary>
			///<returns>A value representing the x-coordinate of the center</returns>
			virtual float GetX() const = 0;
			///<summary>Gets the y-coordinate of the volume's center</summary>
			///<returns>A value representing the y-coordinate of the center</returns>
			virtual float GetY() const = 0;
			///<summary>Gets the z-coordinate of the volume's center</summary>
			///<returns>A value representing the z-coordinate of the center</returns>
			virtual float GetZ() const = 0;
			///<summary>Sets the x-coordinate of the volume's center</summary>
			///<param name='x'>[in]A value representing the center-points x-coordinate</param>
			virtual void SetX(float x) = 0;
			///<summary>Sets the y-coordinate of the volume's center</summary>
			///<param name='y'>[in]A value representing the center-points y-coordinate</param>
			virtual void SetY(float y) = 0;
			///<summary>Sets the x-coordinate of the volume's center</summary>
			///<param name='z'>[in]A value representing the center-points z-coordinate</param>
			virtual void SetZ(float z) = 0;

			///<summary>Gets the shapes complexity</summary>
			///<returns>How complex the shape is</returns>
			virtual ShapeComplexity GetComplexity() = 0;

			virtual Intersection IntersectsLine(CelestialMath::Vector3 origin, CelestialMath::Vector3 direction, float& smallestDistanceSquare) = 0;
			///<summary>Checks this volume against a plane to see if they intersect</summary>
			///<param name='bp'>[in]A pointer to the boundingplane to check against</param>
			///<returns>How the box intersects with the plane</returns>
			virtual Intersection IntersectsPlane(BoundingPlane* bp) = 0;
			///<summary>Checks this volume against two planes to see if the volume is between them</summary>
			///<param name='p1'>[in]A pointer to the first plane to check against</param>
			///<param name='p2'>[in]A pointer to the second plane to check against</param>
			///<returns>How the box intersects with the two planes</returns>
			virtual Intersection IntersectsBetweenPlanes(BoundingPlane* p1, BoundingPlane* p2) = 0;
			///<summary>Checks this volume against another volume to see if they intersect</summary>
			///<param name='bounding'>[in]A pointer to the object to check against</param>
			///<param name='shape'>A shape that determines how the boundingobject is constructed</param>
			///<returns>How the two objects intersect, Intersection_THROUGH means they overlap</returns>
			virtual Intersection IntersectsBounding(IBounding* bounding,Shape shape) = 0;

			virtual Intersection ContainsPoint(CelestialMath::Vector3 point) = 0;
			///<summary>Transforms the volume according to a matrix</summary>
			///<param name='mat'>The matrix to transform the box by</param>
			virtual void Transform(CelestialMath::Matrix mat) = 0;
		
			///<summary>Gets a copy of this volume</summary>
			///<returns>A pointer to a copy of this volume</returns>
			virtual IBounding* GetCopy() const = 0;

			virtual ~IBounding(){}
	};
}
#pragma once
#include "BoundingSphere.h"

namespace CrossHandlers
{

	enum Dimension{Dimension_X,Dimension_Y,Dimension_Z,Dimension_N};
	enum Plane{Plane_RIGHT,Plane_LEFT,Plane_UP,Plane_DOWN,Plane_FAR,Plane_NEAR};
	///<summary>This class encapsulates all data and methods required to simulate a frustum</summary>
	class Frustum
	{

		private:
			///<summary>A doublearray containing the indices of the points that make up a plane</summary>
			char** planePoints;
			///<summary>The planes that constitue this frustum</summary>
			BoundingPlane** planes;
			///<summary>The sphere that encomposses the frustum</summary>
			BoundingSphere* sphere;
			///<summary>Gets the point where three relativly orthogonal planes intersect</summary>
			///<param name='plane1'>[in]The first plane</param>
			///<param name='plane2'>[in]The second plane</param>
			///<param name='plane3'>[in]The third plane</param>
			CelestialMath::Vector3 getCommonPoint(BoundingPlane* plane1,BoundingPlane* plane2,BoundingPlane* plane3);

			///<summary>The points making up this frustum when view from the top</summary>
			CelestialMath::Vector2* topPoints;
			///<summary>The amount of toppoints</summary>
			int cornerSize;

			///<summary>Which point is neighbour to who</summary>
			char** neighBourTo;
			///<summary>An triple array containing the sides of which the neighbours are neighbours</summary>
			char*** neighSides;

			///<summary>Initilizes the frustum</summary>
			void init();

		public:
			///<param name='viewProjection'>[in]The matrix to construct a frustum from</param>
			///<param name='invViewProjection'>[in]The inverse of the matrix to construct from</param>
			Frustum(CelestialMath::Matrix viewProjection, CelestialMath::Matrix invViewProjection);
			///<param name='f'>A pointer to the frustum copy data from</param>
			Frustum(Frustum* f);
			///<summary>(re)Construct the frustum</summary>
			///<param name='viewProjection'>[in]The matrix to construct from</param>
			///<param name='invViewProjection'>[in]The inverse of the matrix to construct from</param>
			///<param name='changeShape'>[in]Wether or not the shape of frustum will change(i.e if the projection has changes)</param>
			void Construct(CelestialMath::Matrix viewProjection, CelestialMath::Matrix invViewProjection, bool changeShape);
			///<summary>Check a object against the frustum</summary>
			///<param name='boundingObject'>[in]The object to check</param>
			///<param name='ignoreThrough'>[in]In which dimension the through-intersection should be ignored</param>
			///<returns>How the object intersects with the frustum</returns>
			Intersection Check(IBounding* boundingObject,Dimension ignoreThrough);
			///<summary>Gets a plane from the frustum</summary>
			///<param name='code'>[in]Which plane to get</param>
			///<returns>A pointer to the plane</returns>
			BoundingPlane* GetPlane(Plane code);
			///<summary>Gets the sphere that encomposses the frustum</summary>
			///<returns>A pointer to the sphere</returns>
			BoundingSphere* GetSphere();

			///<summary>Gets the points making up the frustum when viewed from the top</summary>
			///<param name='size'>[out]The size of the retruned array</param>
			///<returns>An array of points that make up the frustum when viewed from the top</returns>
			CelestialMath::Vector2* GetUpperCornerPoints(int &size);
			~Frustum();

	};

}
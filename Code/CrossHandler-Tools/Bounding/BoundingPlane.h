#pragma once
#include "../../CelestialMath/CelestialMath.h"
namespace CrossHandlers
{

	///<summary>This class contains the data and methods requiered to represent a plane in 3D-space, since it doesn't have a volume it doesn't not inherit from IBounding</summary> 
	class BoundingPlane
	{

		private:
			///<summary>An array containing the normals for this plane</summary>
			CelestialMath::Vector3 normals;
			///<summary>The d-component of the plane-equation</summary>
			float dComponent;
			///<summary>The normalized D-component of the plane</summary>
			float pConstant;
			///<summary>The length of the normals</summary>
			float normalLength;

		public:
			BoundingPlane();
			///<param name='a'>[in]The x-normal of the plane-equation</param>
			///<param name='b'>[in]The y-normal of the plane-equation</param>
			///<param name='c'>[in]The z-normal of the plane-equation</param>
			///<param name='d'>[in]The d-component of the plane-equation</param>
			BoundingPlane(float a, float b, float c, float d);
			BoundingPlane(CelestialMath::Vector3 abc, float d);
			BoundingPlane(CelestialMath::Vector3 abc, float d, float length);
			///<summary>Gets the normals of this plane</summary>
			///<returns>The array containing the normals, this array is always of size 3</returns>
			CelestialMath::Vector3 GetNormal() const;
			///<summary>Gets the normalized normals of this plane</summary>
			///<returns>The array containing the normalized normals, this array is always of size 3</returns>
			CelestialMath::Vector3 GetUnitNormal() const;
			///<summary>Gets this planes D-component</summary>
			///<returns>A value containing the d-component of this planes equation</returns>
			float GetD() const;
			///<summary>Gets this planes P-constant of this planes normalized equation</summary>
			///<returns>A value containing the p-constant of this plane</returns>
			float GetP() const;
			///<summary>Gets the length of the normals in this planes</summary>
			float GetNormalLength() const;
			///<summary>An array containing the normalized normals for this plane</summary>
			CelestialMath::Vector3 unitNormals;
			~BoundingPlane();
		
	};

}
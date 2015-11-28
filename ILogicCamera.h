#pragma once
#include "CelestialVector.h"
#include "CelestialMatrix.h"
#include "IThreadSafeObject.h"

namespace Logic
{

	///<summary>This interface abstracts the usage of cameras for objects dealing with logic instead of graphics</summary>
	class ILogicCamera : public CrossHandlers::IThreadSafeObject
	{
	
		public:
			///<summary>Rotates the camera around it's viewpoint</summary>
			///<param name='angleX'>The angle to rotate around the x-axis</param>
			///<param name='angleY'>The angle to rotate around the y-axis</param>
			///<param name='angleZ'>The angle to rotate around the z-axis</param>
			virtual void RotateAroundViewPoint(float angleX,float angleY,float angleZ)=0;
		
			///<summary>Sets the cameras' position</summary>
			///<param name='vals'>The vector containing the new position</param>
			virtual void SetPos(CelestialMath::Vector3 vals)= 0;
			///<summary>Moves the camera</summary>
			///<param name='vals'>The vector containing the movement of the camera</param>
			virtual void Move(CelestialMath::Vector3 vals) = 0;
			///<summary>Rotates the camera</summary>
			///<param name='rotationAxis'>The matrix to rotate the view by</param>
			///<param name='sideAxis'>The matrix to rotate the side by</param>
			virtual void Rotate(CelestialMath::Matrix rotationAxis, CelestialMath::Matrix sideAxis) = 0;
			///<summary>Sets where the camera looks at</summary>
			///<param name='vals'>The vector containing where the write-camera should look</param>
			virtual void LookAt(CelestialMath::Vector3 vals) = 0;

			///<summary>Gets the cameras' inverted projection matrix</summary>
			///<returns>The inverted projection-matrix for the read-camera</returns>
			virtual CelestialMath::Matrix GetInvProjectionMatrix() = 0;
			///<summary>Gets the readcameras inverted view matrix</summary>
			///<returns>The inverted view-matrix for the read-camera</returns>
			virtual CelestialMath::Matrix GetInvViewMatrix() = 0;
			///<summary>Gets the readcameras inverted combined view projection-matrix</summary>
			///<returns>The inverted combined view projection-matrix for the read-camera</returns>
			virtual CelestialMath::Matrix GetInvViewProjectionMatrix() = 0;
			///<summary>Returns the vector describing what is up for the camera</summary>
			///<returns>Returns the cameras' up-vector</returns>
			virtual CelestialMath::Vector3 GetUp() = 0;
			///<summary>Gets the direction of this camera</summary>
			///<returns>The cameras normalized viewing direction</returns>
			virtual CelestialMath::Vector3 GetDirection() = 0;
			///<summary>Gets the side-direction of this camera</summary>
			///<returns>The cameras normalized side direction</returns>
			virtual CelestialMath::Vector3 GetSide() = 0;
			///<summary>Gets the position of this camera</summary>
			///<returns>The cameras position</returns>
			virtual CelestialMath::Vector3 GetPosition() = 0;

			virtual ~ILogicCamera(){}
	};

}
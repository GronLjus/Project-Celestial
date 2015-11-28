#pragma once
#include "IThreadSafeObject.h"
#include "CameraFrame.h"

namespace Graphics
{

	///<summary>This interface abstracts the usage of a camera for graphical objects</summary>
	class IGraphicCamera:public CrossHandlers::IThreadSafeObject
	{

		public:
			///<summary>Checks a bounding-object against the read-cameras frustum</summary>
			///<param name='bound'>The pointer to a bounding-object too be checked against the read-camera</param>
			///<returns>true if the object is contained within the read-cameras frustum, else false</returns>
			virtual bool CheckFrustum(CrossHandlers::IBounding* bound) = 0;
			///<summary>Returns the position of the camera</summary>
			///<returns>The vector containing the read-cameras position</returns>
			virtual CelestialMath::Vector3 GetPos() = 0;

			///<summary>Gets the frustum associated with this camera</summary>
			///<returns>A pointer to the frustum used by this camera</returns>
			virtual CrossHandlers::Frustum* GetFrustum() = 0;
		
			///<summary>Gets the cameras projection matrix</summary>
			///<returns>The projection-matrix for the read-camera</returns>
			virtual CelestialMath::Matrix GetProjectionMatrix() = 0;
			///<summary>Gets the cameras view matrix</summary>
			///<returns>The view-matrix for the read-camera</returns>
			virtual CelestialMath::Matrix GetViewMatrix() = 0;
			///<summary>Gets the cameras combined view projection matrix</summary>
			///<returns>The combined view projection-matrix for the read-camera</returns>
			virtual CelestialMath::Matrix GetViewProjectionMatrix() = 0;
		
			///<summary>Gets the cameras' inverted projection matrix</summary>
			///<returns>The inverted projection-matrix for the read-camera</returns>
			virtual CelestialMath::Matrix GetInvProjectionMatrix() = 0;
			///<summary>Gets the readcameras inverted view matrix</summary>
			///<returns>The inverted view-matrix for the read-camera</returns>
			virtual CelestialMath::Matrix GetInvViewMatrix() = 0;
			///<summary>Gets the readcameras inverted combined view projection-matrix</summary>
			///<returns>The inverted combined view projection-matrix for the read-camera</returns>
			virtual CelestialMath::Matrix GetInvViewProjectionMatrix() = 0;
			///<summary>Gets the FOV of the camera</summary>
			///<returns>The FOV in the y-axis for this camera</returns>
			virtual float GetFOV() = 0;

			///<summary>Locks the camera so only it can only be read, MUST BE CALLED before any functions are called to read otherwise it's not threadsafe</summary>
			virtual bool Lock() = 0;
			///<summary>Unlocks the camera so it can be written to, MUST BE CALLED eventually otherwise no changes will be registered</summary>
			virtual void Unlock() = 0;
		
			///<summary>Gets the viewport associated with this camera</summary>
			///<returns>The viewport from the read-camera</returns>
			virtual D3D10_VIEWPORT GetViewPort() = 0;
			///<summary>Gets the camera in the current frame</summary>
			///<returns>A pointer to the frame of the camera</returns>
			virtual CrossHandlers::CameraFrame* GetFrame() = 0;
			virtual ~IGraphicCamera(){}
	};

}
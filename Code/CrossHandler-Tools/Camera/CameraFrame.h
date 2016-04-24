#pragma once
#include "../../Graphics/GraphicAPI.h"
#include "../../CrossHandler-Tools/Frustum.h"

namespace CrossHandlers
{

	///<summary>This class contains the data representing a camera at acertain point</summary>
	class CameraFrame
	{
	
		public:

			///<param name='frust'>[in]The frustum that builds the camera</param>
			///<param name='view'>[in]The cameras' view-matrix</param>
			///<param name='projection'>[in]The cameras' projection-matrix</param>
			///<param name='viewProjection'>[in]The cameras' combined viewProjection-matrix</param>
			///<param name='invView'>[in]The cameras' inverted view-matrix</param>
			///<param name='invProjection'>[in]The cameras' inverted projection-matrix</param>
			///<param name='invViewProjection'>[in]The cameras' inverted combined viewProjection-matrix</param>
			///<param name='pos'>[in]The cameras' position</param>
			///<param name='viewPort'>[in]The viewport of the camera</param>
			///<param name='zN'>[in]The depth of the nearplane</param>
			///<param val='FOV'>[in]The cameras FOV</param>
			CameraFrame(
			Frustum* frust,
			CelestialMath::Matrix view,
			CelestialMath::Matrix projection,
			CelestialMath::Matrix viewProjection,
			CelestialMath::Matrix invView,
			CelestialMath::Matrix invProjection,
			CelestialMath::Matrix invViewProjection,
			CelestialMath::Vector3 pos,
			D3D10_VIEWPORT viewPort,
			float zN,
			float FOV);

			///<summary>Checks a bounding-object against the read-cameras frustum</summary>
			///<param name='bound'>The pointer to a bounding-object too be checked against the read-camera</param>
			///<returns>true if the object is contained within the read-cameras frustum, else false</returns>
			bool CheckFrustum(CrossHandlers::IBounding* bound);
			///<summary>Gets the frustum associated with this camera</summary>
			///<returns>A pointer to the frustum used by this camera</returns>
			Frustum* GetFrustum();
			///<summary>Returns the position of the camera</summary>
			///<returns>The vector containing the cameras position</returns>
			CelestialMath::Vector3 GetPos() const;
			///<summary>Gets the cameras projection matrix</summary>
			///<returns>The projection-matrix for the camera</returns>
			CelestialMath::Matrix GetProjectionMatrix() const;
			///<summary>Gets the cameras view matrix</summary>
			///<returns>The view-matrix for the camera</returns>
			CelestialMath::Matrix GetViewMatrix() const;
			///<summary>Gets the cameras combined view projection matrix</summary>
			///<returns>The combined view projection-matrix for the camera</returns>
			CelestialMath::Matrix GetViewProjectionMatrix() const;
		
			///<summary>Gets the cameras' inverted projection matrix</summary>
			///<returns>The inverted projection-matrix for the camera</returns>
			CelestialMath::Matrix GetInvProjectionMatrix() const;
			///<summary>Gets the readcameras inverted view matrix</summary>
			///<returns>The inverted view-matrix for the camera</returns>
			CelestialMath::Matrix GetInvViewMatrix() const;
			///<summary>Gets the readcameras inverted combined view projection-matrix</summary>
			///<returns>The inverted combined view projection-matrix for the camera</returns>
			CelestialMath::Matrix GetInvViewProjectionMatrix() const;
			///<summary>Gets the viewport associated with this camera</summary>
			///<returns>The viewport from the camera</returns>
			D3D10_VIEWPORT GetViewPort() const;
			///<summary>Gets the depth of the nearplane</summary>
			///<returns>The depth of the nearplane</returns>
			float GetZN() const;
			///<summary>Gets the cameras FOV</summary>
			///<returns>The FOV of the camera</returns>
			float GetFOV();
			~CameraFrame();


		private:
			///<summary>The frustum that builds the camera</summary>
			Frustum* frust;
			///<summary>The cameras' view-matrix</summary>
			CelestialMath::Matrix view;
			///<summary>The cameras' projection-matrix</summary>
			CelestialMath::Matrix projection;
			///<summary>The cameras' combined viewProjection-matrix</summary>
			CelestialMath::Matrix viewProjection;
			///<summary>The cameras' inverted view-matrix</summary>
			CelestialMath::Matrix invView;
			///<summary>The cameras' inverted projection-matrix</summary>
			CelestialMath::Matrix invProjection;
			///<summary>The cameras' inverted combined viewProjection-matrix</summary>
			CelestialMath::Matrix invViewProjection;
			///<summary>The cameras' position</summary>
			CelestialMath::Vector3 pos;
			///<summary>The viewport of the camera</summary>
			D3D10_VIEWPORT viewPort;
			///<summary>The depth of the nearplane</summary>
			float zN;
			///<summary>The cameras FOV</summary>
			float FOV;
	};
}
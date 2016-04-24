#pragma once
#include "../../Graphics/GraphicAPI.h"
#include "../../CrossHandler-Tools/Frustum.h"

namespace CrossHandlers
{

	///<summary>This class containes the real data that corresponds to a virtual camera, it's used by the camera-class to enable a certain thread-safety</summary>
	class UnderCamera
	{


		public:
			///<param name='position'>[in]The position of the camera</param>
			///<param name='lookAt'>[in]The point the camera is looking at</param>
			///<param name='up'>[in]The vector that describes the cameras up</param>
			///<param name='fov'>[in]The angle that defines the FOV for the y-coordinate</param>
			///<param name='width'>[in]The width of the cameras viewport</param>
			///<param name='height'>[in]The height of the cameras viweport</param>
			///<param name='depth'>[in]How far the camera can see</param>
			UnderCamera(CelestialMath::Vector3 position, CelestialMath::Vector3 lookAt, CelestialMath::Vector3 up, float fov, long width, long height, float depth);
			///<param name='cam'>[in]The undercamera to copy data from</param>
			///<param name='popFrustum'>[in]If the frustum should be copied or popped from the camera</param>
			UnderCamera(UnderCamera* cam,bool popFrustum);

			///<summary>Checks a bounding-object against the cameras frustum</summary>
			///<param name='bound'>The pointer to a bounding-object too be checked against the camera</param>
			///<returns>true if the object is contained within the cameras frustum, else false</returns>
			bool CheckFrustum(IBounding* bound);
			///<summary>Returns the position of the camera</summary>
			///<returns>The vector containing the cameras position</returns>
			CelestialMath::Vector3 GetPos() const;
			///<summary>Returns where the camera is looking</summary>
			///<returns>The vector containing the point where the camera is pointed</returns>
			CelestialMath::Vector3 GetLookAt() const;
			///<summary>Returns the vector describing what is up for the camera</summary>
			///<returns>Returns the cameras' up-vector</returns>
			CelestialMath::Vector3 GetUp() const;
			///<summary>Gets the direction of this camera</summary>
			///<returns>The cameras normalized viewing direction</returns>
			CelestialMath::Vector3 GetDirection() const;
			///<summary>Gets the side-direction of this camera</summary>
			///<returns>The cameras normalized side direction</returns>
			CelestialMath::Vector3 GetSide() const;

			///<summary>Gets the pointer frustum that defines the camera and removes it from this object</summary>
			///<returns>A pointer to the frustum</returns>
			Frustum* PopFrustum();
			///<summary>Gets the frustum that defines the camera</summary>
			///<returns>The pointer of a frustum that defines the camera</returns>
			Frustum* GetFrustum();
			///<summary>Gets the cameras' FOV in the y-axis</summary>
			///<returns>The FOV in the y-axis for this camera</returns>
			float GetFov() const;
			///<summary>Gets the cameras' width for its' viewport</summary>
			///<returns>The viewPorts width</returns>
			float GetWidth() const;
			///<summary>Gets the cameras' height for its' viewport</summary>
			///<returns>The viewPorts height</returns>
			float GetHeight() const;
			///<summary>Gets the cameras' depth for its' viewport</summary>
			///<returns>The viewPorts depth</returns>
			float GetDepth() const;
		
			///<summary>Sets the cameras position</summary>
			///<param name='vals'>The vector containing the new position</param>
			void SetPos(CelestialMath::Vector3 vals);
			///<summary>Moves the camera</summary>
			///<param name='vals'>The vector containing the movement of the camera</param>
			virtual void Move(CelestialMath::Vector3 vals);
			///<summary>Rotates the camera</summary>
			///<param name='rotationAxis'>The matrix to rotate the view by</param>
			///<param name='sideAxis'>The matrix to rotate the side by</param>
			virtual void Rotate(CelestialMath::Matrix rotationAxis, CelestialMath::Matrix sideAxis);
			///<summary>Sets where the camera looks at</summary>
			///<param name='vals'>The vector containing where the camera should look</param>
			void LookAt(CelestialMath::Vector3 vals);
		
			///<summary>Gets the cameras projection matrix</summary>
			///<returns>The matrix for the camera</returns>
			CelestialMath::Matrix GetProjectionMatrix() const;
			///<summary>Gets the cameras view matrix</summary>
			///<returns>The matrix for the camera</returns>
			CelestialMath::Matrix GetViewMatrix() const;
			///<summary>Gets the cameras combined view projection matrix</summary>
			///<returns>The combined view projection-matrix for the camera</returns>
			CelestialMath::Matrix GetViewProjectionMatrix() const;
		
			///<summary>Gets the readcameras inverted projection matrix</summary>
			///<returns>The inverted projection-matrix for the read-camera</returns>
			CelestialMath::Matrix GetInvProjectionMatrix() const;
			///<summary>Gets the cameras inverted view matrix</summary>
			///<returns>The inverted view-matrix for the camera</returns>
			CelestialMath::Matrix GetInvViewMatrix() const;
			///<summary>Gets the inverted combined view projection-matrix for the camera</summary>
			///<returns>The inverted combined view projection-matrix for the camera</returns>
			CelestialMath::Matrix GetInvViewProjectionMatrix() const;
		
			///<summary>Rotates the camera around it's viewpoint</summary>
			///<param name='angleX'>The angle to rotate around the x-axis</param>
			///<param name='angleY'>The angle to rotate around the y-axis</param>
			///<param name='angleZ'>The angle to rotate around the z-axis</param>
			void RotateAroundViewPoint(float angleX,float angleY,float angleZ);
			///<summary>Gets the viewport associated with this camera</summary>
			///<returns>The viewport from the camera</returns>
			D3D10_VIEWPORT GetViewPort() const;
		
			~UnderCamera();

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
			///<summary>Where the camera looks</summary>
			CelestialMath::Vector3 lookAt;
			///<summary>The cameras' normalized up</summary>
			CelestialMath::Vector3 up;
			///<summary>The cameras' normalized direction</summary>
			CelestialMath::Vector3 direction;
			///<summary>The cameras' normalized side</summary>
			CelestialMath::Vector3 side;
		
			///<summary>The cameras' FOV in the y-axis</summary>
			float fov;
			///<summary>The width of the cameras' viewport</summary>
			float width;
			///<summary>The height of the cameras' viewport</summary>
			float height;
			///<summary>The depth of the cameras' viewport</summary>
			float depth;
			///<summary>The cameras' viewport</summary>
			D3D10_VIEWPORT vP;
		
			///<summary>The cameras' rotation around the x-axis in radians</summary>
			float angX;
			///<summary>The cameras' rotation around the y-axis in radians</summary>
			float angY;
			///<summary>The cameras' rotation around the z-axis in radians</summary>
			float angZ;
			///<summary>Wether or not the projection of the camera should change and be recalculated</summary>
			bool projChanged;

			///<summary>Builds the frustum, view and maybe the projection-matrix from the data</summary>
			void construct();

			///<summary>Rotates the camera</summary>
			///<param name='rotation'>The axis-matrix to rotate by</param>
			///<param name='rotation2'>The secondary axis to rotate by</param>
			void rotate(CelestialMath::Matrix rotation, CelestialMath::Matrix rotation2);
	};
}
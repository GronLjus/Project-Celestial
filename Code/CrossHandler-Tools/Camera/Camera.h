#pragma once
#include "../../Graphics/IGraphicCamera.h"
#include "../../Logic/ILogicCamera.h"
#include "UnderCamera.h"

namespace CrossHandlers
{

	///<summary>This class contains all the data neccesary to convey a virtual camera, to make it threadsafe(relativly) it contains two seperate undercamerasm one the read from and one to read to, when locked all changes are made to the writecamera and the readcamera is frozen and when unlocked both cameras merge</summary>
	class Camera : public Graphics::IGraphicCamera,public Logic::ILogicCamera
	{

		public:
			///<param name='position'>[in]The position of the camera</param>
			///<param name='lookAt'>[in]The point the camera is looking at</param>
			///<param name='up'>[in]The vector that describes the cameras up</param>
			///<param name='fov'>[in]The angle that defines the FOV for the y-coordinate</param>
			///<param name='width'>[in]The width of the cameras viewport</param>
			///<param name='height'>[in]The height of the cameras viweport</param>
			///<param name='depth'>[in]How far the camera can see</param>
			Camera(CelestialMath::Vector3 position, CelestialMath::Vector3 lookAt, CelestialMath::Vector3 up, float fov, long width, long height, float depth);

			///<summary>Checks a bounding-object against the read-cameras frustum</summary>
			///<param name='bound'>The pointer to a bounding-object too be checked against the read-camera</param>
			///<returns>true if the object is contained within the read-cameras frustum, else false</returns>
			virtual bool CheckFrustum(IBounding* bound);
			///<summary>Gets the frustum associated with this camera</summary>
			///<returns>A pointer to the frustum used by this camera</returns>
			virtual Frustum* GetFrustum();
			///<summary>Returns the position of the camera</summary>
			///<returns>The vector containing the read-cameras position</returns>
			virtual CelestialMath::Vector3 GetPos();
			///<summary>Sets the cameras' position</summary>
			///<param name='vals'>The vector containing the new position</param>
			virtual void SetPos(CelestialMath::Vector3 vals);
			///<summary>Moves the camera</summary>
			///<param name='vals'>The vector containing the movement of the camera</param>
			virtual void Move(CelestialMath::Vector3 vals);
			///<summary>Rotates the camera</summary>
			///<param name='rotationAxis'>The matrix to rotate the view by</param>
			///<param name='sideAxis'>The matrix to rotate the side by</param>
			virtual void Rotate(CelestialMath::Matrix rotationAxis, CelestialMath::Matrix sideAxis);

			///<summary>Sets where the camera looks at</summary>
			///<param name='vals'>The vector containing where the write-camera should look</param>
			virtual void LookAt(CelestialMath::Vector3 vals);
		
			///<summary>Gets the cameras projection matrix</summary>
			///<returns>The projection-matrix for the read-camera</returns>
			virtual CelestialMath::Matrix GetProjectionMatrix();
			///<summary>Gets the cameras view matrix</summary>
			///<returns>The view-matrix for the read-camera</returns>
			virtual CelestialMath::Matrix GetViewMatrix();
			///<summary>Gets the cameras combined view projection matrix</summary>
			///<returns>The combined view projection-matrix for the read-camera</returns>
			virtual CelestialMath::Matrix GetViewProjectionMatrix();
		
			///<summary>Gets the cameras' inverted projection matrix</summary>
			///<returns>The inverted projection-matrix for the read-camera</returns>
			virtual CelestialMath::Matrix GetInvProjectionMatrix();
			///<summary>Gets the readcameras inverted view matrix</summary>
			///<returns>The inverted view-matrix for the read-camera</returns>
			virtual CelestialMath::Matrix GetInvViewMatrix();
			///<summary>Gets the readcameras inverted combined view projection-matrix</summary>
			///<returns>The inverted combined view projection-matrix for the read-camera</returns>
			virtual CelestialMath::Matrix GetInvViewProjectionMatrix();
			///<summary>Returns the vector describing what is up for the camera</summary>
			///<returns>Returns the cameras' up-vector</returns>
			virtual CelestialMath::Vector3 GetUp();
			///<summary>Gets the direction of this camera</summary>
			///<returns>The cameras normalized viewing direction</returns>
			virtual CelestialMath::Vector3 GetDirection();
			///<summary>Gets the side-direction of this camera</summary>
			///<returns>The cameras normalized side direction</returns>
			virtual CelestialMath::Vector3 GetSide();
			///<summary>Gets the position of this camera</summary>
			///<returns>The cameras position</returns>
			virtual CelestialMath::Vector3 GetPosition(); 
			virtual float GetFOV();

			///<summary>Rotates the camera around it's viewpoint</summary>
			///<param name='angleX'>The angle to rotate around the x-axis</param>
			///<param name='angleY'>The angle to rotate around the y-axis</param>
			///<param name='angleZ'>The angle to rotate around the z-axis</param>
			virtual void RotateAroundViewPoint(float angleX,float angleY,float angleZ);
		
			///<summary>Locks the camera so only it can only be read, MUST BE CALLED before any functions are called to read otherwise it's not threadsafe</summary>
			virtual bool Lock();
			///<summary>Unlocks the camera so it can be written to, MUST BE CALLED eventually otherwise no changes will be registered</summary>
			virtual void Unlock();
		
			///<summary>Gets the viewport associated with this camera</summary>
			///<returns>The viewport from the read-camera</returns>
			virtual D3D10_VIEWPORT GetViewPort();
			virtual CameraFrame* GetFrame();

			~Camera();

		private:

			///<summary>Stops the thread until it's its turn to write to the camera, may not work as it has never been properly tested</summary>
			///<param name='id'>[in]The id to wait for</param>
			void waitForWrite(int id);

			///<summary>Increases an int</summary>
			///<param name='i'>A reference to the int to increase</param>
			void increment(int &i);
			///<summary>Decreases an int</summary>
			///<param name='i'>A reference to the int to decrease</param>
			void decrement(int &i);

			///<summary>Determines the id of the thread calling it</summary>
			///<returns>The id of the calling thread to be used to seperate it from other threads</returns>
			int getId();

			///<summary>The id of the next thread</summary>
			int id;
			///<summary>How many thread that are currently writing or going to write</summary>
			int currentlyWriting;
			///<summary>If the read-camera is locked</summary>
			bool isLocked;
			///<summary>How many locks that are currently active</summary>
			int locks;

			///<summary>A handle to the semaphore object used to sync threads</summary>
			HANDLE semaphore;
			///<summary>A handle to the semaphore object used to sync write</summary>
			HANDLE writeSemaphore;
			///<summary>A handle to the semaphore object used to sync reads</summary>
			HANDLE readSemaphore;

			///<summary>The undercamera that is always read from</summary>
			UnderCamera* underCam;

	};

}
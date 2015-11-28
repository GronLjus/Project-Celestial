#include "StdAfx.h"
#include "Camera.h"

using namespace CrossHandlers;
using namespace CelestialMath;

CrossHandlers::Camera::Camera(Vector3 position,Vector3 look,Vector3 u, float fv,long w,long h,float d)
{

	underCam = new UnderCamera(position,look,u,fv,w,h,d);
	id = 0;
	currentlyWriting = 1;
	locks = 0;
	isLocked = false;
	semaphore = CreateSemaphore(nullptr,1,1,nullptr);
	readSemaphore = CreateSemaphore(nullptr,1,1,nullptr);
	writeSemaphore = CreateSemaphore(nullptr,1,1,nullptr);

}

void CrossHandlers::Camera::RotateAroundViewPoint(float angleX,float angleY,float angleZ)
{
	
	int id = getId();
	waitForWrite(id);
	WaitForSingleObject(writeSemaphore,INFINITE);
	underCam->RotateAroundViewPoint(angleX,angleY,angleZ);
	currentlyWriting++;
	ReleaseSemaphore(writeSemaphore,1,nullptr);

}

void CrossHandlers::Camera::SetPos(Vector3 vals)
{
	
	int id = getId();
	waitForWrite(id);
	WaitForSingleObject(writeSemaphore,INFINITE);
	underCam->SetPos(vals);
	currentlyWriting++;
	ReleaseSemaphore(writeSemaphore,1,nullptr);

}

void CrossHandlers::Camera::Move(Vector3 vals)
{
	
	int id = getId();
	waitForWrite(id);
	WaitForSingleObject(writeSemaphore,INFINITE);
	underCam->Move(vals);
	currentlyWriting++;
	ReleaseSemaphore(writeSemaphore,1,nullptr);

}

void CrossHandlers::Camera::Rotate(Matrix rotation1,Matrix rotation2)
{

	int id = getId();
	waitForWrite(id);
	WaitForSingleObject(writeSemaphore,INFINITE);
	underCam->Rotate(rotation1,rotation2);
	currentlyWriting++;
	ReleaseSemaphore(writeSemaphore,1,nullptr);

}

void CrossHandlers::Camera::LookAt(Vector3 vals)
{
	
	int id = getId();
	waitForWrite(id);
	WaitForSingleObject(writeSemaphore,INFINITE);
	underCam->LookAt(vals);
	currentlyWriting++;
	ReleaseSemaphore(writeSemaphore,1,nullptr);

}

bool CrossHandlers::Camera::CheckFrustum(IBounding* bound)
{
	
	WaitForSingleObject(readSemaphore,INFINITE);
	bool result = underCam->CheckFrustum(bound);
	ReleaseSemaphore(readSemaphore,1,nullptr);
	return result;
}
		
CrossHandlers::Frustum* CrossHandlers::Camera::GetFrustum()
{
	
	WaitForSingleObject(readSemaphore,INFINITE);
	Frustum* result = underCam->GetFrustum();
	ReleaseSemaphore(readSemaphore,1,nullptr);
	return result;

}

Vector3 CrossHandlers::Camera::GetPos()
{
	
	WaitForSingleObject(readSemaphore,INFINITE);
	Vector3 result = underCam->GetPos();
	ReleaseSemaphore(readSemaphore,1,nullptr);
	return result;

}

Matrix CrossHandlers::Camera::GetProjectionMatrix()
{
	
	WaitForSingleObject(readSemaphore,INFINITE);
	Matrix result = underCam->GetProjectionMatrix();
	ReleaseSemaphore(readSemaphore,1,nullptr);
	return result;

}

Matrix CrossHandlers::Camera::GetViewMatrix()
{
	
	WaitForSingleObject(readSemaphore,INFINITE);
	Matrix result = underCam->GetViewMatrix();
	ReleaseSemaphore(readSemaphore,1,nullptr);
	return result;

}

Matrix CrossHandlers::Camera::GetViewProjectionMatrix()
{
	
	WaitForSingleObject(readSemaphore,INFINITE);
	Matrix result = underCam->GetViewProjectionMatrix();
	ReleaseSemaphore(readSemaphore,1,nullptr);
	return result;

}
		
Matrix CrossHandlers::Camera::GetInvProjectionMatrix()
{
	
	WaitForSingleObject(readSemaphore,INFINITE);
	Matrix result = underCam->GetInvProjectionMatrix();
	ReleaseSemaphore(readSemaphore,1,nullptr);
	return result;

}

D3D10_VIEWPORT CrossHandlers::Camera::GetViewPort()
{
	
	WaitForSingleObject(readSemaphore,INFINITE);
	D3D10_VIEWPORT result = underCam->GetViewPort();
	ReleaseSemaphore(readSemaphore,1,nullptr);
	return result;

}

Matrix CrossHandlers::Camera::GetInvViewMatrix()
{
	
	WaitForSingleObject(readSemaphore,INFINITE);
	Matrix result = underCam->GetInvViewMatrix();
	ReleaseSemaphore(readSemaphore,1,nullptr);
	return result;

}

Matrix CrossHandlers::Camera::GetInvViewProjectionMatrix()
{
	
	WaitForSingleObject(readSemaphore,INFINITE);
	Matrix result = underCam->GetInvViewProjectionMatrix();
	ReleaseSemaphore(readSemaphore,1,nullptr);
	return result;

}

Vector3 CrossHandlers::Camera::GetUp()
{
	
	WaitForSingleObject(readSemaphore,INFINITE);
	Vector3 result = underCam->GetUp();
	ReleaseSemaphore(readSemaphore,1,nullptr);
	return result;

}

Vector3 CrossHandlers::Camera::GetDirection()
{
	
	WaitForSingleObject(readSemaphore,INFINITE);
	Vector3 result = underCam->GetDirection();
	ReleaseSemaphore(readSemaphore,1,nullptr);
	return result;

}

Vector3 CrossHandlers::Camera::GetSide()
{
	
	WaitForSingleObject(readSemaphore,INFINITE);
	Vector3 result = underCam->GetSide();
	ReleaseSemaphore(readSemaphore,1,nullptr);
	return result;

}

Vector3 CrossHandlers::Camera::GetPosition()
{
	
	WaitForSingleObject(readSemaphore,INFINITE);
	Vector3 result = underCam->GetPos();
	ReleaseSemaphore(readSemaphore,1,nullptr);
	return result;

}

float CrossHandlers::Camera::GetFOV()
{

	WaitForSingleObject(readSemaphore, INFINITE);
	float result = underCam->GetFov();
	ReleaseSemaphore(readSemaphore, 1, nullptr);
	return result;

}

CameraFrame* Camera::GetFrame()
{
	
	WaitForSingleObject(readSemaphore,INFINITE);
	CameraFrame* result = new CameraFrame(new Frustum(underCam->GetFrustum()),
		underCam->GetViewMatrix(),
		underCam->GetProjectionMatrix(),
		underCam->GetViewProjectionMatrix(),
		underCam->GetInvViewMatrix(),
		underCam->GetInvProjectionMatrix(),
		underCam->GetInvViewProjectionMatrix(),
		underCam->GetPos(),
		underCam->GetViewPort(),
		0.1f,
		underCam->GetFov());
	ReleaseSemaphore(readSemaphore,1,nullptr);
	return result;

}

bool CrossHandlers::Camera::Lock()
{
	
	increment(locks);

	if(locks == 1)
	{
		
		WaitForSingleObject(writeSemaphore,INFINITE);
		isLocked = true;

	}

	return true;

}

void CrossHandlers::Camera::Unlock()
{

	decrement(locks);

	if(isLocked && locks == 0)//Make sure we can only unlock when all locks have been released
	{
		
		isLocked=false;
		ReleaseSemaphore(writeSemaphore,1,nullptr);

	}
}

void CrossHandlers::Camera::increment(int &i)
{

	WaitForSingleObject(semaphore,INFINITE);
	i++;
	ReleaseSemaphore(semaphore,1,nullptr);

}

void CrossHandlers::Camera::decrement(int &i)
{

	WaitForSingleObject(semaphore,INFINITE);
	i--;
	ReleaseSemaphore(semaphore,1,nullptr);

}

void CrossHandlers::Camera::waitForWrite(int id)
{

	while(id != currentlyWriting){Sleep(0);}

}

int CrossHandlers::Camera::getId()
{
	increment(id);
	return id;

}

CrossHandlers::Camera::~Camera()
{
	
	WaitForSingleObject(writeSemaphore,INFINITE);
	WaitForSingleObject(readSemaphore,INFINITE);
	WaitForSingleObject(semaphore,INFINITE);

	delete underCam;

	ReleaseSemaphore(semaphore,1,nullptr);
	ReleaseSemaphore(readSemaphore,1,nullptr);
	ReleaseSemaphore(writeSemaphore,1,nullptr);
	CloseHandle(semaphore);
	CloseHandle(writeSemaphore);
	CloseHandle(readSemaphore);

}
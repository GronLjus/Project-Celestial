#include "StdAfx.h"
#include "UnderCamera.h"
#include "CelestialMath.h"

using namespace CrossHandlers;
using namespace CelestialMath;

UnderCamera::UnderCamera(Vector3 position,Vector3 look,Vector3 u, float fv,long w,long h,float d)
{

	frust = nullptr;
	pos = position;
	lookAt = look;
	up = u;
	fov = fv;
	width = (float)w;
	height = (float)h;
	depth = d;
	construct();
	angX =0;
	angY = 0;
	angZ = 0;
	
	vP.Width = (UINT)width;
	vP.Height = (UINT)height;
	vP.MinDepth = 0.0f;
	vP.MaxDepth = 1.0f;
	vP.TopLeftX = 0;
	vP.TopLeftY = 0;

	side = Vector3(1,0,0);
	up = Vector3(0,1,0);
	direction = Vector3(0,0,1);

}

UnderCamera::UnderCamera(UnderCamera* cam,bool popFrustum)
{

	frust = new Frustum(cam->GetFrustum());
	pos = cam->GetPos();
	lookAt = cam->GetLookAt();
	up = cam->GetUp();
	fov = cam->GetFov();
	width = cam->GetWidth();
	height = cam->GetHeight();
	depth = cam->GetDepth();
	angX = cam->angX;
	angY = cam->angY;
	angZ = cam->angZ;
	view = cam->GetViewMatrix();
	projection = cam->GetProjectionMatrix();
	viewProjection = cam->GetViewProjectionMatrix();
	invView = cam->GetInvViewMatrix();
	invProjection = cam->GetInvProjectionMatrix();
	invViewProjection = cam->GetInvViewProjectionMatrix();
	vP = cam->GetViewPort();
	projChanged = false;

	side = cam->GetSide();
	up = cam->GetUp();
	direction = cam->GetDirection();

}

Frustum* UnderCamera::GetFrustum()
{

	return frust;

}

Frustum* UnderCamera::PopFrustum()
{

	Frustum* retVal = frust;
	frust = nullptr;
	return frust;

}

void UnderCamera::construct()
{
	
	view = MatrixLookAtLH(pos, lookAt, Vector3(0, 1, 0));
	invView = MatrixInverse(view);

	projection = MatrixPerspectiveFovLH(fov, width / height, 0.1f, depth);
	invProjection = MatrixInverse(projection);
	
	viewProjection = MatrixMultiply(view, projection);
	invViewProjection = MatrixMultiply(invProjection, invView);

	if(frust != nullptr)
	{

		frust->Construct(viewProjection,invViewProjection,projChanged);

	}
	else
	{ 
		 
	
		frust = new Frustum(viewProjection,invViewProjection);

	}

	projChanged = false;

}

void UnderCamera::RotateAroundViewPoint(float angleX,float angleY,float angleZ)
{

	Vector3 relPos = Vector3(pos.x-lookAt.x,pos.y-lookAt.y,pos.z-lookAt.z);//Find where the camera is relative to the looking point
	
	Matrix rotMatX = MatrixRotationX(angleX);
	Matrix rotMatY = MatrixRotationY(angleY);
	Matrix rotMatZ = MatrixRotationZ(angleZ);
	Matrix rotMat = MatrixMultiply(MatrixMultiply(rotMatX, rotMatY), rotMatZ);
	rotate(rotMat,rotMatY);
	Vector4 out = VectorTransform(Vector4(relPos,1.0f), rotMat);
	angX += angleX;
	angY += angleY;
	angZ += angleZ;
	
	pos = Vector3(lookAt.x+out.x,lookAt.y+out.y,lookAt.z+out.z);
	construct();

}

bool UnderCamera::CheckFrustum(IBounding* bound)
{

	Intersection result = frust->Check(bound,Dimension_N);
	return  result != Intersection_BACK && result != Intersection_NA;

}
		
Vector3 UnderCamera::GetPos() const
{

	return pos;

}

Vector3 UnderCamera::GetLookAt() const
{

	return lookAt;

}

Vector3 UnderCamera::GetUp() const
{

	return up;

}

Vector3 UnderCamera::GetDirection() const
{

	return direction;

}

Vector3 UnderCamera::GetSide() const
{

	return side;

}

float UnderCamera::GetFov() const
{

	return fov;

}

float UnderCamera::GetWidth() const
{

	return width;

}
		
float UnderCamera::GetHeight() const
{

	return height;

}

float UnderCamera::GetDepth()  const
{

	return depth;

}

void UnderCamera::SetPos(Vector3 vals)
{

	pos = vals;

}

void UnderCamera::Move(Vector3 vals)
{

	pos += vals;
	lookAt += vals;
	construct();

}

void UnderCamera::Rotate(Matrix rotation1,Matrix rotation2)
{

	Vector4 relPos = Vector4(lookAt.x-pos.x,lookAt.y-pos.y,lookAt.z-pos.z,1.0f);//Find where the looking point is relative to the camera
	Vector4 out = VectorTransform(VectorTransform(relPos, rotation2), rotation1);
	lookAt = pos+Vector3(out.x,out.y,out.z);
	rotate(rotation1,rotation2);
	construct();

}

void UnderCamera::LookAt(Vector3 vals)
{

	lookAt = vals;
	construct();

}

Matrix UnderCamera::GetProjectionMatrix() const
{

	return projection;

}

Matrix UnderCamera::GetViewMatrix() const
{

	return view;

}

Matrix UnderCamera::GetViewProjectionMatrix() const
{

	return viewProjection;

}
		
Matrix UnderCamera::GetInvProjectionMatrix() const
{

	return invProjection;

}

Matrix UnderCamera::GetInvViewMatrix() const
{

	return invView;

}

Matrix UnderCamera::GetInvViewProjectionMatrix() const
{

	return invViewProjection;

}

D3D10_VIEWPORT UnderCamera::GetViewPort() const
{

	return vP;

}

void UnderCamera::rotate(Matrix rot,Matrix rot2)
{

	Vector4 temp = VectorTransform(VectorTransform(Vector4(direction, 1), rot2), rot);
	direction.x = temp.x;
	direction.y = temp.y;
	direction.z = temp.z;

	temp = VectorTransform(Vector4(side, 1), rot2);
	side.x = temp.x;
	side.y = temp.y;
	side.z = temp.z;

	temp = VectorTransform(VectorTransform(Vector4(up, 1), rot2), rot);
	up.x = temp.x;
	up.y = temp.y;
	up.z = temp.z;

}

UnderCamera::~UnderCamera()
{

	if(frust != nullptr)
	{
	
		delete frust;

	}
}
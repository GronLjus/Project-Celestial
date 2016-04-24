#include "../../../stdafx.h"
#include "CameraFrame.h"

using namespace CrossHandlers;
using namespace CelestialMath;

CameraFrame::CameraFrame(Frustum* frust,Matrix view,Matrix projection,Matrix viewProjection,Matrix invView,Matrix invProjection,Matrix invViewProjection,Vector3 pos,D3D10_VIEWPORT viewPort,float zN,float FOV)
{

	this->frust = frust;
	this->view = view;
	this->projection = projection;
	this->viewProjection = viewProjection;
	this->invView = invView;
	this->invProjection = invProjection;
	this->invViewProjection = invViewProjection;
	this->pos = pos;
	this->viewPort = viewPort;
	this->zN = zN;
	this->FOV = FOV;
}

bool CameraFrame::CheckFrustum(IBounding* bound)
{

	Intersection result = frust->Check(bound,Dimension_N);
	return  result != Intersection_BACK && result != Intersection_NA;

}

Frustum* CameraFrame::GetFrustum()
{

	return frust;

}

Vector3 CameraFrame::GetPos() const
{

	return pos;

}

Matrix CameraFrame::GetProjectionMatrix() const	
{

	return projection;

}

Matrix CameraFrame::GetViewMatrix() const
{

	return view;

}

Matrix CameraFrame::GetViewProjectionMatrix() const
{

	return viewProjection;

}

Matrix CameraFrame::GetInvProjectionMatrix() const
{

	return invProjection;

}

Matrix CameraFrame::GetInvViewMatrix() const
{

	return invView;

}

Matrix CameraFrame::GetInvViewProjectionMatrix() const
{

	return invViewProjection;

}

D3D10_VIEWPORT CameraFrame::GetViewPort() const
{

	return viewPort;

}

float CameraFrame::GetZN() const
{

	return zN;

}

float CameraFrame::GetFOV()
{

	return FOV;

}

CameraFrame::~CameraFrame()
{

	delete frust;

}
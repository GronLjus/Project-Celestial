#include "StdAfx.h"
#include "PointLight.h"
#include "CelestialMath.h"
#include <time.h>

using namespace Resources;
using namespace CrossHandlers;
using namespace CelestialMath;

PointLight::PointLight(BaseLightDesc base)
{

	forwards = Vector3(0,0,1);
	sidewards = Vector3(1,0,0);
	pos = Vector3(0, 0, 0);
	
	this->diffCol = base.diff;
	this->specCol = base.spec;
	bound = nullptr;
	this->size = base.size;
	SetPosIntensity(pos, base.intensity);
	id = 0;

}

Vector4 PointLight::GetSpec() const
{

	return specCol;

}

Vector4 PointLight::GetDiff() const
{

	return diffCol;

}

Vector3 PointLight::GetPos() const
{

	return pos;

}

Vector3 PointLight::GetDirection() const
{

	return forwards;

}

float PointLight::GetSize() const
{

	return size;

}

Vector3 PointLight::GetSide() const
{

	return sidewards;
}

LightType PointLight::GetType() const
{

	return LightType_POINT;

}
		
float PointLight::GetAngle() const
{

	return CELESTIAL_PI*0.5;//A pointlight always has an angle of 90 degrees

}

float PointLight::GetIntensity() const
{

	return baseIntensity;

}

int PointLight::GetId() const
{

	return id;

}

IBounding* PointLight::GetBoundingVolume()
{

	return bound;

}

void PointLight::SetId(int id)
{

	this->id = id;

}

void PointLight::SetSpec(Vector4 col)
{

	specCol = col;

}

void PointLight::SetDiff(Vector4 col)
{

	diffCol = col;

}

void PointLight::SetPosIntensity(Vector3 &pos,float intens) 
{
	
	bool changeProj = baseIntensity == intens;
	baseIntensity = intens;
	intensity = intens*intens;
	this->pos = pos;

	if(bound == nullptr)
	{

		bound = new BoundingSphere(pos.x,pos.y,pos.z,intensity);

	}
	else
	{

		bound->SetX(pos.x);
		bound->SetY(pos.y);
		bound->SetZ(pos.z);
		bound->SetRadi(intensity);

	}
}

void PointLight::SetPos(Vector3 &newPos)
{

	SetPosIntensity(newPos,baseIntensity);

}

void PointLight::Point(Vector3 forward,float roll)
{

	this->forwards = forward;
	this->sidewards = Vector3(cos(roll)-forwards.x,-sin(roll),1-forwards.z);//Is perpendicular to the forward vector

}

PointLight::~PointLight()
{

	if(bound != nullptr)
	{

		delete bound;

	}
}
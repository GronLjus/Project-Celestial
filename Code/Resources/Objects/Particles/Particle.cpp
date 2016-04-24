#include "../../../../stdafx.h"
#include "Particle.h"

using namespace Resources;
using namespace CelestialMath;

Particle::Particle(Vector3 pos,Vector3 vel,Vector3 size,float ft,float lt,float ttl,int st)
{

	position = pos;
	velocity = vel;
	this->size = size;
	fadeTime = ft;
	loopTime = lt;
	timeToLive = ttl;
	startTime = st;

}

Vector3 Particle::GetPosition() const
{

	return position;

}

Vector3 Particle::GetVelocity() const
{

	return velocity;

}

Vector3 Particle::GetSize() const
{

	return size;

}

float Particle::GetFadeTime() const
{

	return fadeTime;

}

float Particle::GetLoopTime() const
{

	return loopTime;

}

float Particle::GetTimeToLive() const
{

	return timeToLive;

}

int Particle::GetStartTime() const
{

	return startTime;

}
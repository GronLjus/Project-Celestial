#include "stdafx.h"
#include "ParticleEmitterInstance.h"
#include "CelestialMath.h"

using namespace Resources;
using namespace CelestialMath;

ParticleEmitterInstance::ParticleEmitterInstance(Vector3 &position,Vector3 &rotation,Vector3 &scale,float sizefactor,float velocityFactor,IParticleEmitter* emitter,float startTime,float endTime,Vector4 colour,float glow,float fade)
{
	
	this->scale = scale;
	this->rotation = rotation;
	this->position = position;
	this->sizefactor = sizefactor;
	this->velocityFactor = velocityFactor;
	this->emitter = emitter;
	this->startTime = startTime;
	this->endTime = endTime;
	this->colour = colour;
	this->glowFactor = glow;
	this->fade = fade;

}

void ParticleEmitterInstance::SetPosition(Vector3 &position)
{

	this->position = position;

}

void ParticleEmitterInstance::SetScale(CelestialMath::Vector3 &scale)
{

	this->scale = scale;

}

void ParticleEmitterInstance::SetRotation(CelestialMath::Vector3 &rotation)
{

	this->rotation = rotation;

}

Matrix ParticleEmitterInstance::GetTransformation() const
{

	Matrix s = MatrixScaling(scale.x, scale.y, scale.z);
	Matrix t = MatrixTranslation(position.x, position.y, position.z);
	Matrix r = MatrixRotationYawPitchRoll(rotation.y, rotation.x, rotation.z);
	return MatrixMultiply(MatrixMultiply(r, s), t);

}

float ParticleEmitterInstance::GetVelocityFactor() const
{

	return velocityFactor;

}

float ParticleEmitterInstance::GetSizeFactor() const
{

	return sizefactor;

}

IParticleEmitter* ParticleEmitterInstance::GetEmitter() const
{

	return emitter;

}

float ParticleEmitterInstance::GetStartTime() const
{

	return startTime;

}

float ParticleEmitterInstance::GetEndTime() const
{

	return endTime;

}

Vector4 ParticleEmitterInstance::GetColour() const
{

	return colour;

}

float ParticleEmitterInstance::GetGlow() const
{

	return glowFactor;

}

float ParticleEmitterInstance::GetFade() const
{

	return fade;

}
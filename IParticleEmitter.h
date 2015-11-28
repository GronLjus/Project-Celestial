#pragma once
#include "IParticleEmitterBufferable.h"
#include "IContainable.h"

namespace Resources
{

	enum ParticleSystem{ ParticleSystem_Light, ParticleSystem_SkyBox, ParticleSystem_SIZE };

	struct BaseParticle
	{

		public:
			///<summary>How long the particles in the system will live for in milliseconds</summary>
			int timeToLive;
			///<summary>The number of particles in the system</summary>
			int size;
			///<summary>The speed per millisecond to use as base for each particle</summary>
			float baseVelocity;
			///<summary>The max +-% in the velocity of each particle</summary>
			int velFactor;
			///<summary>The size to use as base for each particle</summary>
			CelestialMath::Vector3 baseSize;
			///<summary>The max +-% in the size of each particle</summary>
			int sizeFactor;

	};

	///<summary>This interface abstracts the usage of particle-emitters</summary>
	class IParticleEmitter : public IParticleEmitterBufferable, public IContainable
	{

		public:

			///<summary>Gets the stride of a vertex this system uses</summary>
			///<returns>The size of a vertex used by this system</returns>
			virtual UINT GetVertexStride() const = 0;
			///<summary>Gets the stride of the instances this system uses</summary>
			///<returns>The size of a instance used by this system</returns>
			virtual UINT GetInstanceStride() const = 0;
			///<summary>Gets the type of particlesystem</summary>
			///<returns>The type of system</returns>
			virtual ParticleSystem GetType() const = 0;

			virtual ~IParticleEmitter(){}
	};
}
#pragma once
#include "IParticleEmitter.h"


namespace Resources
{

	///<summary>This class contains the common functions of particleemitters</summary>
	class CommonParticle : public IParticleEmitter
	{
		public:

			///<param name='base'>[in]The basic description of the emitter</param>
			///<param val='inputTexture'>[in]A pointer to the texture that should be used</param>
			CommonParticle(BaseParticle base, CrossHandlers::TextureResourceObject* inputTexture);
			virtual void InitiateBuffers(int flips);

			virtual int GetNrOfParticles() const;

			virtual UINT GetVertexStride() const;
			virtual UINT GetInstanceStride() const;

			virtual void SetVertexStride(UINT stride);
			virtual void SetInstanceStride(UINT stride);

			virtual int GetId() const;
			virtual void SetId(int id);

			virtual void SetBuffer(CrossHandlers::BufferObject* buffer, CrossHandlers::BufferTypes type, int flip);
			virtual CrossHandlers::BufferObject* GetBuffer(CrossHandlers::BufferTypes type, int flip);
			virtual CrossHandlers::TextureResourceObject* GetInputTexture() const;

			virtual ParticleSystem GetType() const;
			virtual ~CommonParticle();

		protected:
			///<summary>How long a particle lives</summary>
			int particleLife;
			///<summary>The number of particles</summary>
			int size;
			///<summary>The velocity to use as base for each particle</summary>
			float baseVelocity;
			///<summary>The max +-% in the velocity of each particle</summary>
			int velFactor;
			///<summary>The size to use as base for each particle</summary>
			CelestialMath::Vector3 baseSize;
			///<summary>The max +-% in the size of each particle</summary>
			int sizeFactor;

			///<summary>The id of the system</summary>
			int id;

			///<summary>How many of each buffers there are</summary>
			int flips;
			///<summary>The types of buffer used by the emitter</summary>
			int buffertypes;
			///<summary>An array of pointer to the buffers used for instances</summary>
			CrossHandlers::BufferObject** iBuffers;
			///<summary>A pointer to the buffer used for vertices</summary>
			CrossHandlers::BufferObject* vBuffer;
			///<summary>A pointer to the texture used for this particle</summary>
			CrossHandlers::TextureResourceObject* texture;

			///<summary>The size of each vertex</summary>
			UINT vertexStride;
			///<summary>The size of each instance</summary>
			UINT instanceStride;

			///<summary>The type of particlesystem this is</summary>
			ParticleSystem type;
	};
}
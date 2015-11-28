#pragma once
#include "BufferObject.h"
#include "Particle.h"
#include "TextureResourceObject.h"

namespace Resources
{

	///<summary>This interface encapsulates the methods required to manage buffers within a particlesystem</summary>
	class IParticleEmitterBufferable
	{

		public:

			///<summary>Initiates the buffers</summary>
			///<param val='flips'>[in]The amount of flips to use</param>
			virtual void InitiateBuffers(int flips) = 0;
			///<summary>Gets the total number of particles in this emitter</summary>
			///<returns>The number of particles this emitter emits</returns>
			virtual int GetNrOfParticles() const = 0 ;
			///<summary>Gets the nth particle of this emitter</summary>
			///<param name='n'>[in]The particle to get</param>
			///<returns>The nth particle from this emitter</returns>
			virtual Particle GetParticle(int n) = 0;

			///<summary>Sets the buffer used by this particle, also removes the ones in place</summary>
			///<param name='buffer'>[in]A pointer to the buffer to use</param>
			///<param name='type'>[in]Which buffer to use</param>
			///<param name='flip'>[in]The flip to set</param>
			virtual void SetBuffer(CrossHandlers::BufferObject* buffer, CrossHandlers::BufferTypes type,int flip) = 0;

			///<summary>Gets the buffer used by this particle</summary>
			///<param name='type'>[in]The type of buffer to get</param>
			///<param name='flip'>[in]From which flip to get the buffer from</param>
			///<returns>A pointer to the bufferobject used by the emitter or nullptr if empty</returns>
			virtual CrossHandlers::BufferObject* GetBuffer(CrossHandlers::BufferTypes type,int flip) = 0;
			///<summary>Gets the texture used for particlegeneration</summary>
			///<returns>A pointer to the texture</returns>
			virtual CrossHandlers::TextureResourceObject* GetInputTexture() const = 0;

			///<summary>Set the stride used by the vertices</summary>
			///<param name='stride'>[in]The size of the vertex this system uses</param>
			virtual void SetVertexStride(UINT stride) = 0;
			///<summary>Set the stride used by the instances</summary>
			///<param name='stride'>[in]The size of the instance this system uses</param>
			virtual void SetInstanceStride(UINT stride) = 0;
			virtual ~IParticleEmitterBufferable(){}
	};
}
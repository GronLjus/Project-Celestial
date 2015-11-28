#pragma once
#include "IBufferHandler.h"
#include "ResourceObject.h"

namespace Graphics
{

	class Intermediator
	{
	
		private:
			///<summary>A pointer to the bufferhandler for this object</summary>
			IBufferHandler* bH;
			///<summary>If the intermediator should add particles to the buffers</summary>
			bool particles;
			///<summary>If the intermediator should add lights to the buffers</summary>
			bool lhgts;

		public:
			///<param name='bH'>A pointer to the bufferhandler for this object</param>
			///<param name='particles'>If the intermediator should add particles to the buffers</param>
			///<param name='lhgts'>If the intermediator should add lights to the buffers</param>
			Intermediator(IBufferHandler* bH,bool particles,bool lhgts);
			///<summary>Initilalizes a object for drawing , must be called before any attempts are made to draw the mesh</summary>
			///<param name='mesh'>[in]A pointer to the object to initiate</param>
			///<param val='flips'>[in]The number of flips</param>
			void InitMesh(Resources::MeshObject* object,int flips);
			///<summary>Initilalizes a particlesyntem, must be called before any attempts are made to draw the system</summary>
			///<param name='mesh'>[in]A pointer to the emitter to initiate</param>
			///<param val='flips'>[in]The number of flips</param>
			void InitParticleSystem(Resources::IParticleEmitter* object, int flips);
			///<summary>Adds an instance</summary>
			///<param name='object'>[in]The object to add</param>
			///<param val='resource'>[in]The resource to add</param>
			///<param name='flip'>[in]Which flip to use write to</param>
			void AddInstance(Resources::ResourceObject* object, Resources::ResourceCode resource, int flip);

			///<summary>Toggle if the inter should enable particles</summary>
			///<param name='enabled'>[in]The value to set</param>
			void ToggleParticles(bool enabled);
			///<summary>Toggle if the inter should enable lights</summary>
			///<param name='enabled'>[in]The value to set</param>
			void ToggleLights(bool enabled);

	};
}
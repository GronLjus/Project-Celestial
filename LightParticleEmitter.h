#pragma once
#include "CommonParticle.h"

namespace Resources
{

	///<summary>This class generates a number of particles in a spiral fashion out from the six extremes of a sphere</summary>
	class LightParticleEmitter : public CommonParticle
	{
		public:
			///<param name='base'>[in]The basic description of the emitter</param>
			///<param name='allAtOnce'>[in]If all particles should start at the same time</param>
			///<param val='inputTexture'>[in]A pointer to the texture that should be used</param>
			LightParticleEmitter(BaseParticle base, bool allAtOnce, CrossHandlers::TextureResourceObject* inputTexture);

			virtual int GetNrOfParticles() const;
			virtual Particle GetParticle(int n);

			virtual ~LightParticleEmitter();

		private:

			///<summary>The size to choose from in the array</summary>
			int selectableSize;
			///<summary>An array of numbers to select startTimes</summary>
			int* begins;
			
			///<summary>The total amount of particles</summary>
			int totalSize;
			///<summary>The rotation angle to use</summary>
			float angle;

	};
}
#include "CommonParticle.h"

namespace Resources
{

	class SkyboxParticle : public CommonParticle
	{

		public:
			///<param name='base'>[in]The basic description of the emitter</param>
			///<param val='inputTexture'>[in]A pointer to the texture that should be used</param>
			SkyboxParticle(BaseParticle base, CrossHandlers::TextureResourceObject* inputTexture);
			virtual Particle GetParticle(int n);
			virtual ~SkyboxParticle();

	};
}
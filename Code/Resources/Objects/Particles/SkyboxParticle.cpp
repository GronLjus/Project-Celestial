#include "../../../../stdafx.h"
#include "SkyboxParticle.h"

using namespace Resources;
using namespace CelestialMath;

SkyboxParticle::SkyboxParticle(BaseParticle base, CrossHandlers::TextureResourceObject* inputTexture) : CommonParticle(base,inputTexture)
{

	type = ParticleSystem_SkyBox;

}

Particle SkyboxParticle::GetParticle(int n)
{

	Particle part = Particle(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), -1, 0, -1, 0);
	return part;

}

SkyboxParticle::~SkyboxParticle()
{

}
#include "../../../../stdafx.h"
#include "CommonParticle.h"

using namespace Resources;
using namespace CrossHandlers;
using namespace CelestialMath;

CommonParticle::CommonParticle(BaseParticle base, TextureResourceObject* inputTexture)
{

	this->texture = inputTexture;
	this->particleLife = base.timeToLive;
	this->size = base.size;
	this->baseVelocity = base.baseVelocity;
	this->velFactor = base.velFactor;
	this->baseSize = base.baseSize;
	this->sizeFactor = base.sizeFactor;

	buffertypes = 2;
	flips = 0;
	iBuffers = nullptr;
	vBuffer = nullptr;
	type = ParticleSystem_SIZE;

}

void CommonParticle::InitiateBuffers(int flips)
{

	if (this->flips != flips)
	{

		iBuffers = new BufferObject*[flips];

		for (int i = 0; i < flips; i++)
		{

			iBuffers[i] = nullptr;

		}
	}

	this->flips = flips;
}

void CommonParticle::SetBuffer(BufferObject* buffer, BufferTypes type, int flip)
{

	if (type == BufferTypes_VERTEX)
	{

		if (vBuffer != nullptr)
		{

			delete vBuffer;

		}

		vBuffer = buffer;

	}
	else if (type == BufferTypes_INSTANCE)
	{

		if (iBuffers[flip] != nullptr)
		{

			delete iBuffers[flip];

		}

		iBuffers[flip] = buffer;

	}
}

BufferObject* CommonParticle::GetBuffer(BufferTypes type, int flip)
{

	if (type == BufferTypes_VERTEX)
	{

		return vBuffer;

	}
	else if (type == BufferTypes_INSTANCE)
	{

		return iBuffers[flip];

	}

	return nullptr;

}

TextureResourceObject* CommonParticle::GetInputTexture() const
{

	return texture;

}

int CommonParticle::GetNrOfParticles() const
{

	return size;

}

UINT CommonParticle::GetVertexStride() const
{

	return vertexStride;

}

UINT CommonParticle::GetInstanceStride() const
{

	return instanceStride;

}

void CommonParticle::SetVertexStride(UINT stride)
{

	vertexStride = stride;

}

void CommonParticle::SetInstanceStride(UINT stride)
{

	instanceStride = stride;

}

int CommonParticle::GetId() const
{

	return id;

}

void CommonParticle::SetId(int id)
{

	this->id = id;

}

ParticleSystem CommonParticle::GetType() const
{

	return type;

}

CommonParticle::~CommonParticle()
{

	if (texture != nullptr)
	{

		delete texture;

	}

	if (iBuffers != nullptr)
	{

		for (int i = 0; i < flips; i++)
		{

			if (iBuffers[i])
			{

				delete iBuffers[i];

			}

		}

		delete[] iBuffers;
	}

	if (vBuffer != nullptr)
	{

		delete vBuffer;

	}
}
#include "stdafx.h"
#include "Intermediator.h"

using namespace Graphics;
using namespace Resources;
using namespace CrossHandlers;

Intermediator::Intermediator(IBufferHandler* bH,bool particles,bool lhgts)
{

	this->bH = bH;
	this->particles = particles;
	this->lhgts = lhgts;

}

void Intermediator::InitMesh(MeshObject* mesh, int flips)
{

	mesh->SetInstances(flips);
	bH->InitMesh(mesh);

}

void Intermediator::InitParticleSystem(IParticleEmitter* object, int flips)
{

	object->InitiateBuffers(flips);
	bH->InitParticleEmitter(object);

}

void Intermediator::AddInstance(ResourceObject* object, ResourceCode resource, int flip)
{

	if (resource == ResourceCode_MESH)
	{

		MeshObject* mesh = object->GetMesh();
		BufferObject* buffer = mesh->GetBuffer(BufferTypes_INSTANCE, flip);

		if (buffer == nullptr)
		{

			buffer = new BufferObject();
			mesh->SetBuffer(BufferTypes_INSTANCE, buffer, flip);

		}

		bH->AddInstance(buffer, object);

	}
	else if (resource == ResourceCode_PARTICLES)
	{

		if (object->GetParticleEmitter() != nullptr)
		{

			bH->AddInstance(object->GetParticleEmitter()->GetEmitter(), object->GetParticleEmitter(), flip);

		}
	}
}

void Intermediator::ToggleParticles(bool enabled)
{

	particles = enabled;

}

void Intermediator::ToggleLights(bool enabled)
{

	lhgts = enabled;

}
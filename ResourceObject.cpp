#include "stdafx.h"
#include "ResourceObject.h"
#include "CelestialMath.h"

using namespace Resources;
using namespace CrossHandlers;
using namespace CelestialMath;

ResourceObject::ResourceObject()
{
	
	id = -1;
	position = Vector3(0, 0, 0);
	scale = Vector3(1, 1, 1);
	rotations = Vector3(0,0,0);
	meshPointer = nullptr;
	particleSystem = nullptr;
	lightPointer = nullptr;
	transformation = Matrix();
	boundingChain = new CelestialList<IBounding*>*[ResourceCode_SIZE];

	for (int i = 0; i < ResourceCode_SIZE; i++)
	{

		boundingChain[i] = new CelestialList<IBounding*>();

	}
}

int ResourceObject::GetId() const
{

	return id;

}

void ResourceObject::SetId(int id)
{

	this->id = id;

}

void ResourceObject::transform()
{

	Matrix s = MatrixScaling(scale.x, scale.y, scale.z);
	Matrix t = MatrixTranslation(position.x, position.y, position.z);
	Matrix r = MatrixRotationYawPitchRoll(rotations.y, rotations.x, rotations.z);

	transformation = MatrixMultiply(MatrixMultiply(r, t), s);
	invTransformation = MatrixInverse(transformation);

	for (int i = 0; i< ResourceCode_SIZE; i++)
	{

		CelestialListNode<IBounding*>*  node = boundingChain[ResourceCode(i)]->GetFirstNode();

		while (node != nullptr)
		{
		
			node->GetNodeObject()->Transform(transformation);
			node = node->GetNext();

		}
	}
}

Matrix ResourceObject::GetTransformation() const
{

	return transformation;

}

Matrix ResourceObject::GetInverseTransformation() const
{

	return invTransformation;

}

Matrix ResourceObject::PopTransformation()
{

	lastTransformation = transformation;
	return transformation;

}

Matrix ResourceObject::GetLastTransformation() const
{

	return lastTransformation;

}

CelestialList<IBounding*>* ResourceObject::GetBoundingChain(ResourceCode code)
{

	return boundingChain[code];

}

MeshObject* ResourceObject::GetMesh()
{

	return meshPointer;

}

void ResourceObject::SetMesh(MeshObject* obj)
{ 

	meshPointer = obj;

	boundingChain[ResourceCode_MESH]->Reset();

	for (int i = 0; i < ShapeComplexity_SIZE; i++)
	{
		for (int k = 0; k < Shape_COUNT; k++)
		{

			if (obj->GetBoundingObject(Shape(k))->GetComplexity() == i)
			{

				boundingChain[ResourceCode_MESH]->AddElement(obj->GetBoundingObjectCopy(Shape(k)));

			}
		}
	}
}

ParticleEmitterInstance* ResourceObject::GetParticleEmitter()
{

	return particleSystem;

}

void ResourceObject::SetParticleEmitter(IParticleEmitter* particleSystem)
{

	Vector4 color = Vector4(1, 1, 1, 1);
	float fac = 1;
	float sizeFactor = 1;

	if (lightPointer != nullptr)
	{

		color = lightPointer->GetDiff();
		fac = lightPointer->GetSize() / (particleSystem->GetParticle(0).GetLoopTime());
		sizeFactor = lightPointer->GetIntensity() / 1500;

	}

	this->particleSystem = new ParticleEmitterInstance(position, rotations, scale, sizeFactor, fac, particleSystem, 0, -1, color, 0.75f, 0.1f);

}

ILight* ResourceObject::GetLightEmitter()
{

	return lightPointer;

}

void ResourceObject::SetLightEmitter(ILight* lightPointer)
{

	this->lightPointer = lightPointer;
	lightPointer->SetPos(position);
	boundingChain[ResourceCode_LIGHT]->Reset();
	boundingChain[ResourceCode_LIGHT]->AddElement(lightPointer->GetBoundingVolume()->GetCopy());

}

Vector3 ResourceObject::GetPosition() const
{

	return position;

}

void ResourceObject::SetPosition(Vector3 &pos)
{

	this->position = pos;

	if (particleSystem != nullptr)
	{

		particleSystem->SetPosition(pos);

	}

	if (lightPointer != nullptr)
	{

		lightPointer->SetPos(pos);

	}

	transform();

}

void ResourceObject::SetRotation(Vector3 &rotations)
{


	this->rotations = rotations;

	if (particleSystem != nullptr)
	{

		particleSystem->SetPosition(rotations);

	}

	transform();

}

Vector3 ResourceObject::GetRotation() const
{

	return rotations;

}

void ResourceObject::SetScale(Vector3 &scale)
{

	this->scale = scale;

	if (particleSystem != nullptr)
	{

		particleSystem->SetPosition(scale);

	}

	transform();

}

Vector3 ResourceObject::GetScales() const
{

	return scale;

}

ResourceObject::~ResourceObject()
{

	for (int i = 0; i < ResourceCode_SIZE; i++)
	{

		boundingChain[i]->KillList();
		delete boundingChain[i];

	}

	delete particleSystem;
	delete[] boundingChain;

}
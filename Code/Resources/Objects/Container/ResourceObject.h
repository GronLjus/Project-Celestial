#pragma once
#include <string>
#include "IBounding.h"
#include "MeshObject.h"
#include "CelestialList.h"
#include "ParticleEmitterInstance.h"
#include "ILight.h"

namespace Resources
{

	enum ResourceCode{ ResourceCode_MESH, ResourceCode_LIGHT, ResourceCode_PARTICLES, ResourceCode_SIZE };
	///<summary>This class contains all data with an object required to handle it in a logic</summary>
	class ResourceObject : public IContainable
	{
		public:

			ResourceObject();
			virtual int GetId() const;
			virtual void SetId(int id);

			///<summary>Gets the mesh that belongs to this object</summary>
			///<returns>A pointer to the mesh this object belongs to</returns>
			MeshObject* GetMesh();
			///<summary>Sets the mesh of this object</summary>
			///<param name='mesh'>[in]A pointer to the mesh</param>
			void SetMesh(MeshObject* mesh);
			///<summary>Gets the particlesystem that belongs to this object</summary>
			///<returns>A pointer to the emitter used by the system</returns>
			ParticleEmitterInstance* GetParticleEmitter();
			///<summary>Sets the particlesystem of this object</summary>
			///<param name='mesh'>[in]A pointer to the emitter</param>
			void SetParticleEmitter(IParticleEmitter* particleSystem);
			///<summary>Gets the light that belongs to this object</summary>
			///<returns>A pointer to the lightemitter used by the system</returns>
			ILight* GetLightEmitter();
			///<summary>Sets the light of this object</summary>
			///<param name='mesh'>[in]A pointer to the light</param>
			void SetLightEmitter(ILight* lightPointer);

			///<summary>Sets the position of the object</summary>
			///<param name='pos'>[in]The new position</param>
			void SetPosition(CelestialMath::Vector3 &position);
			///<summary>Gets the position of the object</summary>
			///<returns>The position of the object</returns>
			CelestialMath::Vector3 GetPosition() const;
			///<summary>Sets the rotation around the object's axles</summary>
			///<param name='rotations'>[in]The rotations in radians</param>
			void SetRotation(CelestialMath::Vector3 &rotations);
			///<summary>Gets the rotation of the object around it's axles</summary>
			///<returns>The rotaions around the axles in radians</returns>
			CelestialMath::Vector3 GetRotation() const;
			///<summary>Sets the scale of the object</summary>
			///<param name='scale'>[in]The scales</param>
			void SetScale(CelestialMath::Vector3 &scale);
			///<summary>Gets how the object is scaled</summary>
			///<returns>The scales of the objects</returns>
			CelestialMath::Vector3 GetScales() const;

			///<summary>Gets the transformation matrix of the object</summary>
			///<returns>A matrix that describes the objects transformation</returns>
			CelestialMath::Matrix GetTransformation() const;

			///<summary>Gets the inverse transformation matrix of the object</summary>
			///<returns>A matrix that describes the inverse of the objects transformation</returns>
			CelestialMath::Matrix GetInverseTransformation() const;
			///<summary>Gets the transformation matrix of the object and stores it internally as the last transformation used</summary>
			///<returns>A matrix that describes the objects transformation</returns>
			CelestialMath::Matrix PopTransformation();
			///<summary>Gets the last transformation matrix used before it was popped</summary>
			///<returns>A matrix that describes the objects old transformation</returns>
			CelestialMath::Matrix GetLastTransformation() const;

			///<summary>Gets the bouningobject associated with this object</summary>
			///<param val='code'>[in]The resource to get volume for</param>
			///<returns>A pointer to the chain of boundingobjects that surround the object</returns>
			CelestialList<IBounding*>* GetBoundingChain(ResourceCode code);
			 ~ResourceObject();

		private:
			///<summary>The objects id</summary>
			int id;
			///<summary>Creates a tranformation matrix for this object</summary>
			void transform();

			///<summary>A pointer to the meshobject of this object</summary>
			MeshObject* meshPointer;
			///<summary>A pointer to the particlesystem of this object</summary>
			ParticleEmitterInstance* particleSystem;
			///<summary>A pointer to the light this object emits</summary>
			ILight* lightPointer;

			///<summary>The objects position</summary>
			CelestialMath::Vector3 position;
			///<summary>The objects rotation around its axles</summary>
			CelestialMath::Vector3 rotations;
			///<summary>The scale of the object</summary>
			CelestialMath::Vector3 scale;
			///<summary>The transformation matrix of the object</summary>
			CelestialMath::Matrix transformation;
			///<summary>The transformation matrix of the object</summary>
			CelestialMath::Matrix invTransformation;
			///<summary>The transformation matrix of the object</summary>
			CelestialMath::Matrix lastTransformation;
			///<summary>The volume the object occupies</summary>
			CelestialList<IBounding*>** boundingChain;

	};
}
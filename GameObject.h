#pragma once
#include "PositionableObject.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"

namespace Resources
{
	class GameObject : public PositionableObject
	{
		public:
			GameObject(CrossHandlers::BoundingBox* baseBox, CrossHandlers::BoundingSphere* baseSphere, unsigned int meshId);
			virtual void Update(CrossHandlers::Message* mess);
			unsigned int GetMeshId() const;
			unsigned int GetScriptId() const;
			bool IsFlipBuffered(unsigned char flip);
			void SetParent(BaseObject* parent);

			///<summary>Gets the transformation matrix of the object and stores it internally as the last transformation used</summary>
			///<returns>A matrix that describes the objects transformation</returns>
			CelestialMath::Matrix PopTransformation();
			///<summary>Gets the last transformation matrix used before it was popped</summary>
			///<returns>A matrix that describes the objects old transformation</returns>
			CelestialMath::Matrix GetLastTransformation() const;

			CrossHandlers::BoundingBox* GetBox();
			CrossHandlers::BoundingSphere* GetSphere();
			virtual ~GameObject();

		private:
			CrossHandlers::BoundingBox* box;
			CrossHandlers::BoundingSphere* sphere;
			unsigned int mesh;
			unsigned int script;
			unsigned char flipBuffered;
			bool flipInit;
			CelestialMath::Matrix lastMatrix;
			BaseObject* parent;

	};
}
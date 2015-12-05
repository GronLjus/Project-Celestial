#pragma once
#include "BaseObject.h"
#include "CelestialMath.h"
#include "Message.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"

namespace Resources
{
	class GameObject : public BaseObject
	{
		public:
			GameObject(CrossHandlers::BoundingBox* baseBox, CrossHandlers::BoundingSphere* baseSphere);
			void Update(CrossHandlers::Message* mess);
			unsigned int GetMeshId() const;
			unsigned int GetScriptId() const;
			CrossHandlers::BoundingBox* GetBox() const;
			CrossHandlers::BoundingSphere* GetSphere() const;
			CelestialMath::Vector3 GetPosition() const;
			CelestialMath::Vector3 GetRotation() const;
			CelestialMath::Vector3 GetScale() const;
			CelestialMath::Matrix GetMatrix() const;
			virtual ~GameObject();

		private:
			CelestialMath::Vector3 position;
			CelestialMath::Vector3 rotation;
			CelestialMath::Vector3 scale;
			CelestialMath::Matrix transformMatrix;
			CrossHandlers::BoundingBox* box;
			CrossHandlers::BoundingSphere* sphere;
			unsigned int mesh;
			unsigned int script;

	};
}
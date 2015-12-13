#pragma once
#include "BaseObject.h"
#include "CelestialMath.h"
#include "Message.h"

namespace Resources
{

	class PositionableObject : public BaseObject
	{

		public:
			PositionableObject();
			virtual void Update(CrossHandlers::Message* mess);
			CelestialMath::Vector3 GetPosition() const;
			CelestialMath::Vector3 GetRotation() const;
			CelestialMath::Vector3 GetScale() const;
			CelestialMath::Matrix GetMatrix() const;

		private:
			CelestialMath::Vector3 position;
			CelestialMath::Vector3 rotation;
			CelestialMath::Vector3 scale;
			CelestialMath::Matrix transformMatrix;
	};
}
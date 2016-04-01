#pragma once
#include "ScriptableObject.h"
#include "CelestialMath.h"

namespace Resources
{

	class PositionableObject : public ScriptableObject
	{

		public:
			PositionableObject();
			PositionableObject(CelestialMath::Vector3 position, CelestialMath::Vector3 scale);
			virtual void Update(CrossHandlers::Message* mess);
			CelestialMath::Vector3 GetPosition() const;
			CelestialMath::Vector3 GetRotation() const;
			CelestialMath::Vector3 GetScale() const;
			CelestialMath::Matrix GetMatrix() const;
			unsigned char GetLayer() const;

		protected:
			void refresh(CelestialMath::Vector3 position, CelestialMath::Vector3 scale);
			void setLayer(unsigned char layer);

		private:
			CelestialMath::Vector3 position;
			CelestialMath::Vector3 rotation;
			CelestialMath::Vector3 scale;
			CelestialMath::Matrix transformMatrix;
			unsigned char layer;
	};
}
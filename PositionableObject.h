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
			CelestialMath::Vector3 GetDirection() const;
			CelestialMath::Matrix GetMatrix() const;
			CelestialMath::Matrix GetInvTrnMatrix() const;
			unsigned char GetLayer() const;

		protected:
			void refresh(CelestialMath::Vector3 position, CelestialMath::Vector3 scale);
			void setLayer(unsigned char layer);

		private:
			CelestialMath::Vector3 position;
			CelestialMath::Vector3 rotation;
			CelestialMath::Vector3 scale;
			CelestialMath::Vector3 direction;
			CelestialMath::Matrix transformMatrix;
			CelestialMath::Matrix transformInvTrMatrix;
			void createMatrix();
			void rotateObjectToPoint(CelestialMath::Vector3 point);
			void orbitAroundPoint(CelestialMath::Vector3 point, float arc);
			unsigned char layer;
	};
}
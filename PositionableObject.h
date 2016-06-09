#pragma once
#include "ScriptableObject.h"
#include "CelestialMath.h"
#include "CelestialSlicedList.h"

namespace Resources
{

	class PositionableObject : public ScriptableObject
	{

		public:
			PositionableObject();
			PositionableObject(CelestialMath::Vector3 position, CelestialMath::Vector3 scale);
			virtual void Update(CrossHandlers::Message* mess);
			void SetPosition(CelestialMath::Vector3 pos);
			void SetScale(CelestialMath::Vector3 scsal);
			void SetRotation(CelestialMath::Vector3 rotation);
			void AddSubObject(PositionableObject* object, CelestialMath::Vector3 relativePosition);
			void SetObjectParent(PositionableObject* parent, unsigned int childId, CelestialMath::Vector3 relativePosition);
			CelestialMath::Vector3 GetObjectCenterLine(CelestialMath::Vector3 startPoint);
			CelestialMath::Vector3 GetPosition() const;
			CelestialMath::Vector3 GetRotation() const;
			CelestialMath::Vector3 GetScale() const;
			CelestialMath::Vector3 GetDirection() const;
			CelestialMath::Matrix GetMatrix() const;
			CelestialMath::Matrix GetInvTrnMatrix() const;
			void UpdateMatrix();
			unsigned char GetLayer() const;
			virtual ~PositionableObject();

		protected:
			void refresh(CelestialMath::Vector3 position, CelestialMath::Vector3 scale);
			void setLayer(unsigned char layer);
			bool hasMatrixChanged();

		private:
			CelestialMath::Vector3 relativePosition;
			CelestialMath::Vector3 position;
			CelestialMath::Vector3 rotation;
			CelestialMath::Vector3 scale;
			CelestialMath::Vector3 direction;
			CelestialMath::Matrix transformMatrix;
			CelestialMath::Matrix transformInvTrMatrix;

			void createMatrix();
			void rotateObjectToPoint(CelestialMath::Vector3 point);
			void orbitAroundPoint(CelestialMath::Vector3 point, float arc);

			CrossHandlers::CelestialSlicedList<PositionableObject*>* subObjects;
			unsigned int subObjectAmount;

			unsigned char layer;
			PositionableObject* parent;
			unsigned int childId;
			bool matrixChanged;
	};
}
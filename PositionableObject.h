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
			void Move(float distance);

			CelestialMath::Vector3 GetObjectCenterLine(CelestialMath::Vector3 startPoint, CelestialMath::Vector3 direction);
			CelestialMath::Vector3 GetObjectCenterLine(CelestialMath::Vector3 startPoint);

			CelestialMath::Vector3 GetPosition() const;
			CelestialMath::Vector3 GetRelativePosition() const;
			CelestialMath::Vector3 GetRotation() const;
			CelestialMath::Vector3 GetScale() const;
			CelestialMath::Vector3 GetDirection() const;
			CelestialMath::Matrix GetMatrix() const;
			CelestialMath::Matrix GetInvTrnMatrix() const;
			virtual unsigned int GetTargetId() const;
			
			unsigned int GetSubobjects() const;
			PositionableObject* GetSubobject(unsigned int object) const;

			void Point(CelestialMath::Vector3 point);

			void UpdateMatrix();
			unsigned char GetLayer() const;
			virtual ~PositionableObject();

		protected:
			void refresh(CelestialMath::Vector3 position, CelestialMath::Vector3 scale);
			void setLayer(unsigned char layer);
			bool hasMatrixChanged();
			CelestialMath::Matrix GetBoundingMatrix() const;

		private:
			CelestialMath::Vector3 relativePosition;
			CelestialMath::Vector3 position;
			CelestialMath::Vector3 rotation;
			CelestialMath::Vector3 scale;
			CelestialMath::Vector3 absScale;
			CelestialMath::Vector3 absOffset;
			CelestialMath::Vector3 direction;
			CelestialMath::Matrix boundingMatrix;
			CelestialMath::Matrix transformMatrix;
			CelestialMath::Matrix transformInvTrMatrix;

			CelestialMath::Vector3 getPlane(CelestialMath::Vector3 point);
			CelestialMath::Vector3 capLine(CelestialMath::Vector3 point);

			void createMatrix();
			void rotateObjectToPoint(CelestialMath::Vector3 point);
			void orbitAroundPoint(CelestialMath::Vector3 point, float arc);
			void figureOutAbsoluteSize();

			CrossHandlers::CelestialSlicedList<PositionableObject*>* subObjects;
			unsigned int subObjectAmount;

			unsigned char layer;
			PositionableObject* parent;
			unsigned int childId;
			bool matrixChanged;
	};
}
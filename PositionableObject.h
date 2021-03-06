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
			virtual unsigned char* Update(CrossHandlers::Message* mess);
			
			void SetPosition(CelestialMath::Vector3 pos);
			void SetScale(CelestialMath::Vector3 scsal);
			void SetRotation(CelestialMath::Vector3 rotation);
			void SetRotationLimit(CelestialMath::Vector3 upperLimit, CelestialMath::Vector3 lowerLimit, bool greaterCircle);

			void AddSubObject(PositionableObject* object, CelestialMath::Vector3 relativePosition);
			void SetObjectParent(PositionableObject* parent, unsigned int childId, CelestialMath::Vector3 relativePosition);
			void Move(float distance);

			CelestialMath::Vector3 GetObjectCenterLine(CelestialMath::Vector3 startPoint, CelestialMath::Vector3 endPoint, float offSetFromEdge);
			CelestialMath::Vector3 GetObjectCenterLine(CelestialMath::Vector3 startPoint, CelestialMath::Vector3 direction);
			CelestialMath::Vector3 GetObjectCenterLine(CelestialMath::Vector3 startPoint);
			CelestialMath::Vector3 GetObjectCenterLineNoCap(CelestialMath::Vector3 startPoint);

			CelestialMath::Vector3 GetRightDistance(CelestialMath::Vector3 startPoint);

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
			bool IsChild() const;


			void Point(CelestialMath::Vector3 point);

			void UpdateMatrix();
			unsigned char GetLayer() const;

			//Serializable Interface
			virtual char* Serialize(unsigned int &size);
			virtual char* Unserialize(char* data);

			virtual ~PositionableObject();

		protected:
			void refresh(CelestialMath::Vector3 position, CelestialMath::Vector3 scale);
			void setLayer(unsigned char layer);
			bool hasMatrixChanged();
			CelestialMath::Matrix GetBoundingMatrix() const;

		private:
			unsigned int serial;

			CelestialMath::Vector3 upperRotLimit;
			CelestialMath::Vector3 lowerRotLimit;
			bool greaterCircle;

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

			void dealWithRotation(CelestialMath::Vector3 newRot);
			float rotateInAxis(unsigned int axis, float angle);

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

			unsigned char* returnBlock;

	};
}
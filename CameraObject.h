#pragma once
#include "PositionableObject.h"
#include "ViewObject.h"

namespace Resources
{

	class CameraObject : public PositionableObject
	{
		public:
			CameraObject(unsigned int width, unsigned int height, float depth,unsigned int flips);
			virtual void Update(CrossHandlers::Message* mess);
			void IncrementFlipBuff();
			Entities::ViewObject* GetView() const;
			CelestialMath::Matrix GetViewProjection(unsigned int flipBuffer) const;
			~CameraObject();

		private:
			Entities::ViewObject* theView;

			CelestialMath::Matrix* viewProjections;
			CelestialMath::Matrix* invViewProjections;

			CelestialMath::Vector3 sidePoint;
			CelestialMath::Vector3 lookAtPoint;
			CelestialMath::Vector3 up;

			unsigned int width;
			unsigned int height;
			unsigned int flip; 
			unsigned int flips;
			float fov;

	};
}

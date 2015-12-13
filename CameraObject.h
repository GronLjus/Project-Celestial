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
			Entities::ViewObject* GetView() const;
			CelestialMath::Matrix GetViewProjection() const;
			~CameraObject();

		private:
			Entities::ViewObject* theView;

			CelestialMath::Matrix viewProjection;
			CelestialMath::Matrix invViewProjection;

			CelestialMath::Vector3 sidePoint;
			CelestialMath::Vector3 lookAtPoint;
			CelestialMath::Vector3 up;

			unsigned int width;
			unsigned int height;
			float fov;

	};
}

#pragma once
#include "PositionableObject.h"
#include "../../Entities/Objects/ViewObject.h"

namespace Resources
{

	class CameraObject : public PositionableObject
	{
		public:
			CameraObject(unsigned int width, unsigned int height, float depth,unsigned int flips);
			virtual void Update(CrossHandlers::Message* mess);
			void IncrementFlipBuff();
			unsigned int PeekNextFlip() const;
			unsigned int GetFlip() const;
			Entities::ViewObject* GetView() const;
			~CameraObject();

		private:
			Entities::ViewObject* theView;
			float fov;

	};
}

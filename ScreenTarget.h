#pragma once
#include "CelestialMath.h"
#include "PositionableObject.h"

namespace Resources
{

	class ScreenTarget : public PositionableObject
	{
		public:
			ScreenTarget(CelestialMath::Vector4 target);
			virtual void Update(CrossHandlers::Message* mess);
			unsigned int GetEnterScript() const;
			unsigned int GetExitScript() const;
			unsigned int GetHoverScript() const;
			bool IsHovering() const;
			void SetHovering(bool val);
		private:
			unsigned int hoverScript;
			unsigned int enterScript;
			unsigned int exitScript;
			bool isHovering;
	};
}
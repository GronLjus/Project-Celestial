#pragma once
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
			virtual unsigned int GetTargetId() const;
			void SetTargetId(unsigned int id);
			bool IsHovering() const;
			void SetHovering(bool val);
			void Refresh(CelestialMath::Vector4 target);
			bool IsVisible() const;
			void SetVisible(bool vis);
			bool ShouldRemove() const;
			void Lock();
			void Unlock();
			void Remove();
			void SetLayer(unsigned char layer);

		private:
			unsigned int hoverScript;
			unsigned int enterScript;
			unsigned int exitScript;
			bool isHovering;
			bool isVisible;
			bool remove;
			bool locked;
			unsigned int targetId;
	};
}
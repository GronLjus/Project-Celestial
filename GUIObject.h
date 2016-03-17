#pragma once
#include "CelestialVector.h"
#include "PositionableObject.h" 
#include "Triggers.h"
#include "CelestialSlicedList.h"

namespace Resources
{

	enum GUISnap { GUISnap_TOP, GUISnap_MIDDLE, GUISnap_BOTTOM, GUISnap_RIGHT, GUISnap_LEFT };
	enum GUIObjects { GUIObjects_TEXTBOX, GUIObjects_LAYOUT, GUIObjects_IMAGE, GUIObjects_NA };

	class GUIObject : public PositionableObject
	{

		public:
			GUIObject();

			virtual void Update(CrossHandlers::Message* mess);
			///<summary>Gets how the object should snap horizontally</summary>
			///<returns>How the object should snap</returns>
			GUISnap GetHorizontalSnap() const;
			///<summary>Gets how the object should snap vertically</summary>
			///<returns>How the object should snap</returns>
			GUISnap GetVerticalSnap() const;

			///<summary>Checks what type of GUIobject this is</summary>
			///<returns>The type of GUIObject</returns>
			GUIObjects GetType() const;
			unsigned int GetParentID() const;

			void SetParent(GUIObject* parent, unsigned int childId);
			void ToggleVisibility(bool isOnScreen);
			bool IsVisible();
			bool IsEnabled();

			unsigned int GetContentBrush() const;
			void SetContentBrush(unsigned int brush);
			unsigned int GetBorderBrush() const;
			void SetBorderBrush(unsigned int brush);

			virtual void Enable();
			virtual void Disable();

			void ShouldPause();

			virtual ~GUIObject();

		protected:
			void pauseRendering();
			void resumeRendering();
			bool isVisible;
			///<summary>If the object is enabled</summary>
			bool enabled;
			///<summary>The horizontal snapping</summary>
			GUISnap hSnap;
			///<summary>The vertical snapping</summary>
			GUISnap vSnap;

			GUIObject* parentGUI;
			unsigned int childId;
			unsigned int contentBrush;
			unsigned int borderBrush;
			GUIObjects type;

			bool pause;
			bool paused;
			
	};
}

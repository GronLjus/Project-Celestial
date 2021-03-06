#pragma once
#include "CelestialVector.h"
#include "PositionableObject.h" 
#include "Triggers.h"
#include "CelestialSlicedList.h"
#include "ScreenTarget.h"
#include "SaveObject.h"
#include "GUIEnums.h"

namespace Resources
{

	class GUIObject : public PositionableObject, public SaveObject
	{

		public:
			GUIObject();
			void UpdateScreenTarget();
			virtual void SetId(unsigned int id);
			virtual unsigned char* Update(CrossHandlers::Message* mess);
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

			virtual unsigned int GetParentId() const;
			virtual void SetParent(GUIObject* parent, unsigned int childId);
			void ToggleVisibility(bool isOnScreen);
			bool IsVisible();
			bool IsEnabled();
			void SetFocus(bool focus);
			bool GetFocus() const;

			unsigned int GetContentBrush() const;
			void SetContentBrush(unsigned int brush);
			unsigned int GetBorderBrush() const;
			void SetBorderBrush(unsigned int brush);
			ScreenTarget* GetScreenTarget() const;
			CelestialMath::Vector2 GetTopLeft() const;
			void SetLayer(unsigned char layer);

			virtual void Enable();
			virtual void Disable();

			void ShouldPause();

			//Serializable Interface
			virtual char* Serialize(unsigned int &size);
			virtual char* Unserialize(char* data);

			virtual ~GUIObject();

		protected:
			void pauseRendering();
			void resumeRendering();
			void toggleScreenTarget(bool enabled);
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

		private:
			bool focused;
			ScreenTarget* target;
			
	};
}

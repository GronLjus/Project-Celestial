#pragma once
#include "CelestialVector.h"
#include "BaseObject.h"
#include "Triggers.h"
#include "CelestialSlicedList.h"

namespace Resources
{

	enum GUISnap { GUISnap_TOP, GUISnap_MIDDLE, GUISnap_BOTTOM, GUISnap_RIGHT, GUISnap_LEFT };
	enum GUIObjects { GUIObjects_TEXTBOX, GUIObjects_LAYOUT, GUIObjects_NA };

	class GUIObject : public BaseObject
	{

		public:
			GUIObject();
			virtual void Update(CrossHandlers::Message* mess){}
			///<summary>Gets how the object should snap horizontally</summary>
			///<returns>How the object should snap</returns>
			GUISnap GetHorizontalSnap();
			///<summary>Gets how the object should snap vertically</summary>
			///<returns>How the object should snap</returns>
			GUISnap GetVerticalSnap();
			///<summary>Sets how the object should snap horizontally</summary>
			///<param val='snap'>How the object should snap</param>
			void SetHorizontalSnap(GUISnap snap);
			///<summary>Sets how the object should snap vertically</summary>
			///<param val='snap'>How the object should snap</param>
			void SetVerticalSnap(GUISnap snap);

			///<summary>Gets the position of the object</summary>
			///<returns>A vector containing the object position on the screen</returns>
			CelestialMath::Vector2 GetPosition();
			///<summary>Sets the objects position</summary>
			///<param val='pos'>[in]The new position</param>
			void SetPosition(CelestialMath::Vector2 pos);

			///<summary>Gets the size of the object</summary>
			///<returns>A vector containing the object size on the screen</returns>
			CelestialMath::Vector2 GetSize();
			///<summary>Sets the objects Size</summary>
			///<param val='pos'>[in]The new size</param>
			void SetSize(CelestialMath::Vector2 size);

			///<summary>Enables/disables tho object</summary>
			///<param val='val'>Wether or not the object should be enabled</param>
			void Toggle(bool val);
			///<summary>Check wether or not the object is enabled</summary>
			///<returns>The status of the object</returns>
			bool IsEnabled();

			///<summary>Checks what type of GUIobject this is</summary>
			///<returns>The type of GUIObject</returns>
			GUIObjects GetType();

			unsigned int GetTrigger(TriggerType type);
			void SetTrigger(TriggerType type, unsigned int trigger);

			unsigned int GetParentID();
			void SetParentID(unsigned int parent, unsigned int childId);
			unsigned int GetChildren();
			GUIObject* GetChild(unsigned int child);
			void AddChild(GUIObject* gui);
			void ToggleOnScreen(bool isOnScreen);
			bool IsOnScreen();

			int GetId() const;
			void SetId(int id);
			virtual ~GUIObject();

		protected:
			bool isOnScreen;
			///<summary>If the object is enabled</summary>
			bool enabled;
			///<summary>The position of the object</summary>
			CelestialMath::Vector2 position;
			///<summary>The size of the object</summary>
			CelestialMath::Vector2 size;
			///<summary>The horizontal snapping</summary>
			GUISnap hSnap;
			///<summary>The vertical snapping</summary>
			GUISnap vSnap;

			unsigned int* triggers;
			unsigned int parentGUI;

			unsigned int childAmount;
			CelestialSlicedList<GUIObject*>* children;
			unsigned int childId;

			GUIObjects type;
			///<summary>The id of the object</summary>
			int id;
			
	};
}

#pragma once
#include "GUITextBox.h"

namespace Resources
{
	///<summary>This class contains data for GUI objects</summary>
	class GUILayout : public GUIObject
	{
		public:
			GUILayout(GUISnap h, GUISnap v);
			void AddChild(GUIObject* gui);
			virtual void Update(CrossHandlers::Message* mess);
			GUIObject* GetChild(unsigned int child) const;
			unsigned int GetChildren() const;

			virtual void Enable();
			virtual void Disable();

			virtual void SetParent(GUIObject* parent, unsigned int childId);

			~GUILayout();

		private:
			CelestialSlicedList<GUIObject*>* children;
	};
}
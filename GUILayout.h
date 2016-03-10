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
			GUIObject* GetChild(unsigned int child) const;
			unsigned int GetChildren() const;
			~GUILayout();

		private:
			unsigned int childAmount;
			CelestialSlicedList<GUIObject*>* children;
	};
}
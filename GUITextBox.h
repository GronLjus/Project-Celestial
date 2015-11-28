#pragma once
#include "GUIObject.h"
#include "TextContainer.h"

namespace Resources
{

	class GUITextBox : public GUIObject , public CrossHandlers::TextContainer
	{

		public:
			///<param val='multi'>[in]If the box should have multiple lines</param>
			///<param val='h'>[in]The horizontal snapping</param>
			///<param val='v'>[in]The vertical stapping</param>
			GUITextBox(bool multi, GUISnap h, GUISnap v);
			virtual bool IsMultiLine();
			virtual int GetTextLines();
			virtual float GetAngle();
			virtual void SetText(int line, std::string text);
			~GUITextBox();

		private:
			///<summary>If the box should have multiple lines</summary>
			bool isMulti;
	};
}
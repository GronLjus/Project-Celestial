#pragma once
#include "GUIObject.h"
#include "TextContainer.h"

namespace Resources
{

	class GUITextBox : public GUIObject 
	{

		public:
			///<param val='h'>[in]The horizontal snapping</param>
			///<param val='v'>[in]The vertical stapping</param>
			GUITextBox(GUISnap h, GUISnap v);
			virtual void Update(CrossHandlers::Message* mess);
			virtual float GetAngle();
			CrossHandlers::TextContainer* GetText() const;
			void SetText(CrossHandlers::TextContainer* textCont);
			unsigned int GetCaretPosition() const;
			~GUITextBox();

		private:
			CrossHandlers::TextContainer* textContainer;
			unsigned int caretRow;
			unsigned int caretCol;
			unsigned int lastTime;
	};
}
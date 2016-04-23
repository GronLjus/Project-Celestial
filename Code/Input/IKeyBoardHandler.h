#pragma once

namespace Input
{

	enum Key{Key_FORWARD,Key_BACKWARD,Key_LEFT,Key_RIGHT,
		Key_NOUGHT,Key_ONE,Key_TWO,Key_THREE,Key_FOUR,Key_FIVE,Key_SIX,Key_SEVEN,Key_EIGHT,Key_NINE,
		Key_A,Key_B,Key_C,Key_D,Key_E,Key_F,Key_G,Key_H,Key_I,Key_J,Key_K,Key_L,Key_M,Key_N,Key_O,Key_P,Key_Q,Key_R,Key_S,Key_T,Key_U,Key_V,Key_W,Key_X,Key_Y,Key_Z,
		Key_APOSTROPHE,Key_TILDE,Key_DOT,
		Key_PLUS,Key_DASH,
		Key_F7,Key_F8,Key_F9,Key_F11,Key_F12,
		Key_SHIFT, Key_ENTER, Key_BACKSPACE, Key_SPACE, Key_TAB,
		Key_COUNT};//The keys we use

	class IKeyBoardHandler
	{
		public:

			///<summary>See if a numbered key is down</summary>
			///<returns>Which numberkey is down</returns>
			virtual int GetNumberKeyDown() = 0;
			///<summary>Check if a key is down</summary>
			///<param name='key'>[in]The key to see if pressed</param>
			///<returns>If the key is pressed</returns>
			virtual bool IsKeyDown(Key key) = 0;

	};
}
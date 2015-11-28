#pragma once
#include "IHandleMessages.h"
#include "IKeyBoardHandler.h"
#include "CelestialList.h"

namespace Input
{

	///<summary>This class handles all matters regarding input</summary>
	class InputHandler : public IKeyBoardHandler, public CrossHandlers::IHandleMessages
	{

		public:
			InputHandler();
			///<summary>Initializes the inpuhandler</summary>
			///<param name='hWnd'>[in]The handle to the window we're using</param>
			void Init(HWND hWnd);
			///<summary>Updates the handler</summary>
			void Update(unsigned int time);
			///<summary>Gets an array of movement-factors</summary>
			///<returns>An array of values ranging from [-1,1] in three dimensions</returns>
			int* GetMovement();

			virtual int GetNumberKeyDown();
			virtual bool IsKeyDown(Key key);

			///<summary>Gets the current position of the mouse</summary>
			///<returns>An array containing the positions of the mouse in the window</returns>
			float* GetMousePos();
			///<summary>Gets the last position of the mouse</summary>
			///<returns>An array containing the last positions of the mouse in the window</returns>
			float* GetLastMousePos();
			///<summary>Gets whether or not the mouse has moved</summary>
			///<returns>A value indicating if the mouse has moved</returns>
			bool HasMouseMoved();
			///<summary>Toggles whether the handler should contain the mouse in the center of the window</summary>
			void ToggleContainMouse();

			~InputHandler();

		private:

			///<summary>The position of the mouse</summary>
			float* mousePos;
			///<summary>The last position of the mouse</summary>
			float* lastMousePos;
			///<summary>If a mouse has moved</summary>
			bool mouseMoved;
			///<summary>The handle to the window we're using</summary>
			HWND hWnd;
			///<summary>If mouse should be contained within one place</summary>
			bool contain;
			///<summary>Centers the mouse in the center of the window</summary>
			void centerMouse();

			bool keyFit(unsigned char celKey,unsigned int sourceKey);

			struct keyTrigger
			{

				unsigned char scriptAmount;
				unsigned char maxScripts;
				unsigned int* scripts;
				unsigned short filter;
				unsigned int obj;
				bool charTrigg;

			};

			unsigned int mouseTriggerId;
			CrossHandlers::CelestialList<keyTrigger>* triggers;
	};
}
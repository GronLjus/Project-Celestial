#pragma once
#include "IHandleMessages.h"
#include "CelestialSlicedList.h"
#include "ScreenTarget.h"
#include "CelestialMath.h"
#include "KeyCodes.h"

namespace Input
{

	///<summary>This class handles all matters regarding input</summary>
	class InputHandler : public CrossHandlers::IHandleMessages
	{

		public:
			InputHandler();
			///<summary>Initializes the inpuhandler</summary>
			///<param name='hWnd'>[in]The handle to the window we're using</param>
			void Init(CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* gameObjects);
			///<summary>Updates the handler</summary>
			void Update(unsigned int time);

			~InputHandler();

			enum keyCode{
				keyCodeMouseL,
				keyCodeMouseM,
				keyCodeMouseR,
				keyCodeMouseW,
				keyCodeNA
			};

		private:

			struct keyState{

				keyState() : lastState(true), state(true), lastTime(0), thisTime(0){}
				bool lastState;
				bool state;
				unsigned int lastTime;
				unsigned int thisTime;

			};

			struct key{

				key(){}
				key(CelestialKeyCategories cat, unsigned char key) : cat(cat), keyVal(key){}
				CelestialKeyCategories cat;
				unsigned char keyVal;

			};

			bool** keyStatus;
			CrossHandlers::CelestialStack<key>* pressedKeys;
			keyState* keyStates;
			CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* gameObjects;

			CelestialMath::vectorUI2 mouse;
			void handleMouse(unsigned int time);
			void handleKeys(unsigned int time);

			bool checkMouseClick(unsigned int time, keyCode ks) const;
			bool checkDrag(unsigned int time);
			void checkMouseWheel(unsigned int time);

			void sendMessage(unsigned int mess, unsigned int time, short delta);
			void sendMessage(unsigned int mess, unsigned int time,keyCode key);
			void sendMessage(unsigned int mess, unsigned int time);
			keyCode dragging;
			unsigned short clickTime;

			unsigned int draggedTarget;
			unsigned int draggedScript;

			short mouseWheel;
	};
}
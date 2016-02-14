#pragma once
#include "IHandleMessages.h"
#include "CelestialSlicedList.h"
#include "ScreenTarget.h"
#include "CelestialMath.h"

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
				keyCodeMouseR,
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

			void triggerScript(unsigned int script, unsigned int time);
			keyState* keyStates;
			CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* gameObjects;
			CrossHandlers::CelestialSlicedList<Resources::ScreenTarget*>* screenTargets;
			unsigned int maxScreenTargets;
			CelestialMath::vectorUI2 mouse;
			void handleMouse(unsigned int time);
	};
}
#pragma once
#include "../../Resources/Objects/GUIObjects/GUITextBox.h"
#include "RunTime/CelScriptRunTimeHandler.h"
#include "KeyWords/Keyword.h"
#include "../../CrossHandler-Tools/Messaging/IHandleMessages.h"
#include "../../CrossHandler-Tools/Container/CelestialStack.h"

namespace Logic
{

	///<summary>This class Handles all execution of celscript</summary>
	class CelscriptHandler : public CrossHandlers::IHandleMessages
	{

		public:
			CelscriptHandler();
			///<summary>Initializes the handler</summary>
			void Init(CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* objectContainer, unsigned int crossScriptObject);
			///<summary>Updates the handler</summary>
			void Update(unsigned int time);
			void Kill();
			bool AllStopped() const;
			~CelscriptHandler();

		private:

			enum stackStatus { stackStatus_PREPPED, stackStatus_RUNNING, stackStatus_STOPPED,stackStatus_CHRASHED, stackStatus_PAUSED
			};
			struct ScriptStack
			{
				CelestialStack<unsigned int>* stack = nullptr;
				unsigned int sleep;
				stackStatus status;
				~ScriptStack()
				{
					delete stack;
				}
			};

			CelestialSlicedList<Resources::BaseObject*>* objectContainer;
			CelestialSlicedList<ScriptStack*>* scriptStacks;
			CelestialSlicedList<unsigned int>* reverseStacks;
			CelestialSlicedList<bool>* takenStacks;

			RunTimeError handleRTErr(RunTimeError err, unsigned int scriptId);
			///<summary>A pointer to the runtime of the scripts</summary>
			CelScriptRuntimeHandler* runTime;

	};
}
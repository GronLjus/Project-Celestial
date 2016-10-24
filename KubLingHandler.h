#pragma once
#include "GUITextBox.h"
#include "KubLingMachineHandler.h"
#include "IHandleMessages.h"
#include "KubLingLabel.h"

namespace Logic
{

	///<summary>This class Handles all execution of celscript</summary>
	class KubLingHandler : public CrossHandlers::IHandleMessages
	{

		public:
			KubLingHandler();
			///<summary>Initializes the handler</summary>
			void Init(CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* objectContainer);
			///<summary>Updates the handler</summary>
			void Update(unsigned int time);
			void Kill();
			bool AllStopped() const;
			~KubLingHandler();

		private:

			CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* objectContainer;
			CelestialSlicedList<unsigned int>* translatedLabels;

			void initScript(Resources::KubLingLabel* label);
			unsigned int getLatestTranslation(unsigned int label);

			///<summary>A pointer to the runtime of the scripts</summary>
			KubLingMachineHandler* runTime;

	};
}
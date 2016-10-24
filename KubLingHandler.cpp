#include "stdafx.h"
#include "KubLingHandler.h"
#include "KubLingRaw.h"
#include "HeapMemContainer.h"

using namespace Logic;
using namespace Resources;
using namespace CrossHandlers;
using namespace std;

KubLingHandler::KubLingHandler() : IHandleMessages(200,MessageSource_CELSCRIPT)
{

	filter = CrossHandlers::MessageType_SCRIPT;

	translatedLabels = new CelestialSlicedList<unsigned int>(32);
	translatedLabels->Add(9999999);
	this->objectContainer = nullptr;
	runTime = nullptr;

}

void KubLingHandler::Init(CelestialSlicedList<BaseObject*>* objectContainer)
{

	this->objectContainer = objectContainer;
	runTime = new KubLingMachineHandler(outQueue, messageBuffer, outMessages, currentMessage, objectContainer);

}

bool KubLingHandler::AllStopped() const
{

	return runTime->Stopped();

}

void KubLingHandler::initScript(KubLingLabel* label)
{

	if (label->GetTranslation() == 0)
	{

		label->SetTranslation(translatedLabels->Add(0));

	}
}

unsigned int KubLingHandler::getLatestTranslation(unsigned int label)
{

	KubLingLabel* lab = (KubLingLabel*)objectContainer->GetValue(label);
	initScript(lab);

	if (translatedLabels->GetValue(lab->GetTranslation()) == 0)
	{

		translatedLabels->Add(runTime->PrimeLabel(lab->GetStart(), lab->GetMemoffset()), lab->GetTranslation());

	}

	return translatedLabels->GetValue(lab->GetTranslation());

}

void KubLingHandler::Update(unsigned int time)
{

	Message* currentMessage = inQueue->PopMessage();

	while (currentMessage->type != MessageType_NA)
	{

		unsigned int param1 = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);

		if (currentMessage->mess == ScriptMess_RUN)
		{

			unsigned int transl = getLatestTranslation(param1);

			runTime->RunCode(transl);
			KubLingLabel* lab = (KubLingLabel*)objectContainer->GetValue(param1);
			lab->Reset();
			translatedLabels->Add(0, lab->GetTranslation());

		}
		else if (currentMessage->mess == ScriptMess_RESUME)
		{

			unsigned int param2 = currentMessage->params[4] | ((int)currentMessage->params[5] << 8) | ((int)currentMessage->params[6] << 16) | ((int)currentMessage->params[7] << 24);
			unsigned int param3 = currentMessage->params[8] | ((int)currentMessage->params[9] << 8) | ((int)currentMessage->params[10] << 16) | ((int)currentMessage->params[11] << 24);
			unsigned int param4 = currentMessage->params[12] | ((int)currentMessage->params[13] << 8) | ((int)currentMessage->params[14] << 16) | ((int)currentMessage->params[15] << 24);

			unsigned char val = 255;

			runTime->SetStackVar(param1, param2, &currentMessage->params[8], 4);
			runTime->SetStackVar(param1, param4, &val, 1);
			//runTime->RunCode(param1);

		}
		else if (currentMessage->mess == ScriptMess_ADDPARASTR)
		{

			unsigned int transl = getLatestTranslation(param1);
			KubLingLabel* lab = (KubLingLabel*)objectContainer->GetValue(param1);

			if (lab->hasParam('s'))
			{

				unsigned int varAdr = lab->GetParam('s') + lab->GetMemoffset();

				if (varAdr - lab->GetMemoffset() != 0)
				{

					string parString((char*)(&currentMessage->params[4]));
					unsigned int size = parString.length();
					memcpy(&currentMessage->params[0], &size, 4);
					runTime->SetStackVar(transl, varAdr, &(currentMessage->params[0]), size + 4);

				}
			}
		}
		else if (currentMessage->mess == ScriptMess_ADDPARNUM || currentMessage->mess == ScriptMess_ADDPARFLOAT)
		{

			unsigned int transl = getLatestTranslation(param1);
			KubLingLabel* lab = (KubLingLabel*)objectContainer->GetValue(param1);
			char val = currentMessage->mess == ScriptMess_ADDPARNUM ? 'n' : 'f';

			if (lab->hasParam(val))
			{

				unsigned int varAdr = lab->GetParam(val) + lab->GetMemoffset();

				if (varAdr - lab->GetMemoffset() != 0)
				{

					runTime->SetStackVar(transl, varAdr, &(currentMessage->params[4]), 4);

				}
			}
		}
		else if (currentMessage->mess == ScriptMess_SETCODE)
		{

			unsigned int param2 = currentMessage->params[4] | ((int)currentMessage->params[5] << 8) | ((int)currentMessage->params[6] << 16) | ((int)currentMessage->params[7] << 24);
			KubLingRaw* code = (KubLingRaw*)objectContainer->GetValue(param1);
			HeapMemContainer* heap = (HeapMemContainer*)objectContainer->GetValue(param2);
			runTime->SetCode(code->GetCode(), code->GetLength(), heap->GetHeap());
			

		}

		currentMessage->read = true; 
		
		if (currentMessage->killWhenDone)
		{

			delete currentMessage;

		}

		currentMessage = inQueue->PopMessage();

	}

	runTime->Update(time);

}

void KubLingHandler::Kill()
{
	
	runTime->KillMachines();

}

KubLingHandler::~KubLingHandler()
{

	delete translatedLabels;

	if (runTime != nullptr)
	{

		delete runTime;

	}
}
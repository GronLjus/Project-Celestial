#include "stdafx.h"
#include "CelscriptHandler.h"
#include <sstream>

using namespace Logic;
using namespace Resources;
using namespace CrossHandlers;
using namespace std;

CelscriptHandler::CelscriptHandler() : IHandleMessages(20,MessageSource_CELSCRIPT)
{

	filter = CrossHandlers::MessageType_SCRIPT;
	scriptStacks = new CelestialSlicedList<ScriptStack*>(127, nullptr);
	takenStacks = new CelestialSlicedList<bool>(127, nullptr);
	reverseStacks = new CelestialSlicedList<unsigned int>(127, 0);
	this->objectContainer = nullptr;
	runTime = nullptr;

}

void CelscriptHandler::Init(CelestialSlicedList<BaseObject*>* objectContainer, unsigned int crossScriptObject)
{

	this->objectContainer = objectContainer;
	runTime = new CelScriptRuntimeHandler(outQueue, objectContainer, crossScriptObject);

}

bool CelscriptHandler::AllStopped() const
{

	for (unsigned int i = 0; i < scriptStacks->GetFirstEmpty(); i++)
	{

		ScriptStack* stack = scriptStacks->GetValue(i);

		if (stack->sleep == 0 && stack->status != stackStatus_STOPPED)
		{

			return false;

		}
	}

	return true;

}

void CelscriptHandler::Update(unsigned int time)
{

	Message* currentMessage = inQueue->PopMessage();

	while (currentMessage->type != MessageType_NA)
	{

		unsigned int param1 = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);

		if (currentMessage->mess == ScriptMess_RUN)
		{
			CelScriptCompiled* script = (CelScriptCompiled*)objectContainer->GetValue(param1);

			if (script != nullptr)
			{

				unsigned int chosenStack = takenStacks->Add(true);

				if (scriptStacks->GetValue(chosenStack) == nullptr)
				{
					
					ScriptStack* newStack = new ScriptStack();
					newStack->stack = new CelestialStack<unsigned int>(false);
					scriptStacks->Add(newStack);

				}

				reverseStacks->Add(chosenStack, param1);
				ScriptStack* stack = scriptStacks->GetValue(chosenStack);
				stack->sleep = 0;
				stack->stack->PushElement(param1);
				stack->status = stackStatus_PREPPED;

			}


		}
		else if (currentMessage->mess == ScriptMess_RUNFROM)
		{

			unsigned int param2 = currentMessage->params[4] | ((int)currentMessage->params[5] << 8) | ((int)currentMessage->params[6] << 16) | ((int)currentMessage->params[7] << 24);
			reverseStacks->Add(param2, param1);
			scriptStacks->GetValue(param2)->stack->PushElement(param1);
			scriptStacks->GetValue(param2)->status = stackStatus_PREPPED;

		}
		else if (currentMessage->mess == ScriptMess_RESUME)
		{

			unsigned int param2 = currentMessage->params[4] | ((int)currentMessage->params[5] << 8) | ((int)currentMessage->params[6] << 16) | ((int)currentMessage->params[7] << 24);
			unsigned int param3 = currentMessage->params[8] | ((int)currentMessage->params[9] << 8) | ((int)currentMessage->params[10] << 16) | ((int)currentMessage->params[11] << 24);
			runTime->SetWaitingScriptVar(param1, param2, param3);
			unsigned int val1 = reverseStacks->GetValue(param1);
			scriptStacks->GetValue(val1)->status = stackStatus_PREPPED;

		}
		else if (currentMessage->mess == ScriptMess_ADDPARASTR)
		{

			string parString((char*)(&currentMessage->params[4]));
			runTime->AddScriptStrParam(param1, parString);

		}
		else if (currentMessage->mess == ScriptMess_ADDPARNUM)
		{
			
			runTime->AddScriptNumParam(param1, &(currentMessage->params[4]));

		}
		else if (currentMessage->mess == ScriptMess_ADDPARFLOAT)
		{

			runTime->AddScriptFloatParam(param1, &(currentMessage->params[4]));

		}

		currentMessage->read = true;
		currentMessage = inQueue->PopMessage();

	}

	for (unsigned int i = 0; i < scriptStacks->GetFirstEmpty(); i++)
	{

		ScriptStack* stack = scriptStacks->GetValue(i);

		while (stack->sleep == 0 && stack->status == stackStatus_PREPPED)
		{

			stack->status = stackStatus_RUNNING;
			RunTimeError rt = runTime->RunScript(stack->stack->PeekElement(), i, time);
			stack->status = rt == RunTimeError_OK || rt == RunTimeError_ABORT ? stackStatus_STOPPED : 
				rt == RunTimeError_MSGFULL || rt == RunTimeError_WAITINGFORWAR || rt == RunTimeError_HALT ? stackStatus_PAUSED :
				stackStatus_CHRASHED;
			stack->sleep += rt == RunTimeError_MSGFULL ? 5 : 0;

			if (stack->status == stackStatus_STOPPED)
			{
				
				unsigned int lastScript = stack->stack->PopElement();

				if (stack->stack->GetCount() == 0)
				{

					takenStacks->Remove(i);

				}
				else
				{

					stack->status = stackStatus_PREPPED;

				}
			}
		}

		if(stack->sleep > 0)
		{

			stack->sleep = stack->sleep > time ? stack->sleep - time : 0;

		}
	}
}


void CelscriptHandler::Kill()
{
	
	runTime->KillExecutions();

}

CelscriptHandler::~CelscriptHandler()
{

	scriptStacks->KillList();
	delete scriptStacks;
	delete reverseStacks;
	delete takenStacks;

	if (runTime != nullptr)
	{

		delete runTime;

	}
}
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

void CelscriptHandler::Init(CelestialSlicedList<BaseObject*>* objectContainer)
{

	this->objectContainer = objectContainer;
	runTime = new CelScriptRuntimeHandler(outQueue, objectContainer);

}

void CelscriptHandler::Update(unsigned int time)
{

	Message* currentMessage = inQueue->PopMessage();

	while (currentMessage->type != MessageType_NA)
	{

		if (currentMessage->mess == ScriptMess_RUN)
		{
			CelScriptCompiled* script = (CelScriptCompiled*)objectContainer->GetValue(currentMessage->param1);

			if (script != nullptr)
			{

				unsigned int chosenStack = takenStacks->Add(true);

				if (scriptStacks->GetValue(chosenStack) == nullptr)
				{

					ScriptStack* newStack = new ScriptStack();
					newStack->stack = new CelestialStack<unsigned int>(false);
					scriptStacks->Add(newStack);

				}

				reverseStacks->Add(chosenStack, currentMessage->param1);
				ScriptStack* stack = scriptStacks->GetValue(chosenStack);
				stack->sleep = 0;
				stack->stack->PushElement(currentMessage->param1);
				stack->status = stackStatus_PREPPED;

			}
		}
		else if (currentMessage->mess == ScriptMess_RUNFROM)
		{

			reverseStacks->Add(currentMessage->param2, currentMessage->param1);
			scriptStacks->GetValue(currentMessage->param2)->stack->PushElement(currentMessage->param1);
			scriptStacks->GetValue(currentMessage->param2)->status = stackStatus_PREPPED;

		}
		else if (currentMessage->mess == ScriptMess_RESUME)
		{

			runTime->SetWaitingScriptVar(currentMessage->param1, currentMessage->param2, currentMessage->param3);
			unsigned int val1 = reverseStacks->GetValue(currentMessage->param1);
			scriptStacks->GetValue(val1)->status = stackStatus_PREPPED;

		}
		else if (currentMessage->mess == ScriptMess_ADDPARASTR)
		{

			runTime->AddScriptParam(currentMessage->param1, currentMessage->stringParam);

		}
		else if (currentMessage->mess == ScriptMess_ADDPARNUM)
		{

			runTime->AddScriptParam(currentMessage->param1, currentMessage->param2);

		}

		currentMessage->read = true;
		currentMessage = inQueue->PopMessage();

	}

	for (unsigned int i = 0; i < scriptStacks->GetFirstEmpty(); i++)
	{

		ScriptStack* stack = scriptStacks->GetValue(i);

		if (scriptStacks->GetValue(i)->sleep == 0 && stack->status == stackStatus_PREPPED)
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
		else
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
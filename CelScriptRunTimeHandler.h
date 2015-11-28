#pragma once
#include "CelScriptCompiled.h"
#include "CelScriptRuntime.h"

namespace Logic
{

	class CelScriptRuntimeHandler
	{ 

	private:
		///<summary>The Base method for operators</summary>
		///<param val='returnVar'>[in]The variable to put the returnvalue in</param>
		///<param val='params'>[in]The array of chars making up the parameters</param>
		///<param val='paramSize'>[in]The amount of bytes in params</param>
		///<param val='mId'>[in]The memorypool to use in the operator</param>
		///<param val='rtc'>[in]The common values for all operators</param>
		///<returns>Any runtimeerror</returns>
		typedef RunTimeError(*ExecuteFunction) (unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc);
		///<summary>A pointer to the list of scripts</summary>
		CrossHandlers::CelestialSlicedList<Resources::CelScriptCompiled*>* scripts;
		///<summary>Where the scripts start</summary>
		CrossHandlers::CelestialSlicedList<unsigned int>* scriptStarts;
		CrossHandlers::CelestialSlicedList<unsigned int>* scriptNumParams;
		CrossHandlers::CelestialSlicedList<unsigned int>* scriptStrParams;
		CrossHandlers::CelestialSlicedList<unsigned int>* scriptParents;
		CrossHandlers::CelestialSlicedList<bool>* scriptInited;
		///<summary>The common values for all operators</summary>
		CrossHandlers::CelestialSlicedList<RunTimeCommons*>* rtc;
		///<summary>An array of methods to use as operators</summary>
		ExecuteFunction* operators;
		bool abort;
		RunTimeError commonScripts(unsigned int end, RunTimeCommons* rtc, unsigned int id);
		CrossHandlers::MessageQueue* mQueue;
		unsigned int maxOutMessages;

	public:

		///<param val='output'>[in]A pointer to the textbox used as output</param>
		CelScriptRuntimeHandler(CrossHandlers::MessageQueue* mQueue);
		///<summary>Add a script to the runtime</summary>
		///<param val='script'>[in]A pointer to the compiled script to add</param>
		///<returns>The index of the added scripts</returns>
		RunTimeError AddScript(Resources::CelScriptCompiled* script, int &id);
		///<summary>Run a script</summary>
		///<param val='id'>[in]The id of the script to run</param>
		///<returns>Any runtime errors</returns>
		RunTimeError RunScript(int id, unsigned int stackId, unsigned int eTime);
		///<summary>Resumes a script</summary>
		///<param val='id'>[in]The id of the script to run</param>
		///<param val='inVal'>[in]The value to use as parameter</param>
		///<returns>Any runtime errors</returns>
		RunTimeError AddScriptParam(int scriptId, int value);
		RunTimeError AddScriptParam(int scriptId, std::string value);

		void SetWaitingScriptVar(unsigned int scriptId, unsigned int scriptVar, int value);
		///<summary>Remove a script from the runtime</summary>
		///<param val='id'>[in]The id of the script to delete</param>
		void DeleteScript(int id);

		void KillExecutions();
		~CelScriptRuntimeHandler();

	};
}
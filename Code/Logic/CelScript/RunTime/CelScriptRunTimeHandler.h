#pragma once
#include "../../../Resources/Objects/CelScript/CelScriptCompiled.h"
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
		CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* gameObjects;
		///<summary>Where the scripts start</summary>
		CrossHandlers::CelestialSlicedList<unsigned int>* scriptStarts;

		CrossHandlers::CelestialSlicedList<unsigned int>* scriptNumParams;
		CrossHandlers::CelestialSlicedList<unsigned int>* scriptStrParams;
		CrossHandlers::CelestialSlicedList<unsigned int>* scriptFloatParams;

		CrossHandlers::CelestialSlicedList<unsigned int>* scriptParents;
		CrossHandlers::CelestialSlicedList<bool>* scriptInited;
		///<summary>The common values for all operators</summary>
		CrossHandlers::CelestialSlicedList<RunTimeCommons*>* rtc;
		///<summary>An array of methods to use as operators</summary>
		ExecuteFunction* operators;
		bool abort;
		RunTimeError commonScripts(unsigned int end, RunTimeCommons* rtc, Resources::CelScriptCompiled* script);
		RunTimeError initScript(Resources::CelScriptCompiled* script, unsigned int id);
		CrossHandlers::MessageQueue* mQueue;
		unsigned int maxOutMessages;
		unsigned int scriptIds;
		unsigned int crossScriptObject;

	public:

		CelScriptRuntimeHandler(CrossHandlers::MessageQueue* mQueue, CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* gameObjects, unsigned int crossScriptObject);
		///<summary>Run a script</summary>
		///<param val='id'>[in]The id of the script to run</param>
		///<returns>Any runtime errors</returns>
		RunTimeError RunScript(int id, unsigned int stackId, unsigned int eTime);

		RunTimeError AddScriptFloatParam(int scriptId, unsigned char* value);
		RunTimeError AddScriptNumParam(int scriptId, unsigned char* value);
		RunTimeError AddScriptStrParam(int scriptId, std::string value);

		void SetWaitingScriptVar(unsigned int scriptId, unsigned int scriptVar, int value);

		void KillExecutions();
		~CelScriptRuntimeHandler();

	};
}
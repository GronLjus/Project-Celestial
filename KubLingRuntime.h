#pragma once
#include "RunTimeOperations.h"
#include "MemoryPool.h"
#include "MessageQueue.h"
#include "BaseObject.h"

namespace Logic
{

	enum scriptStatus{
		scriptStatus_HALTED, scriptStatus_PAUSED,scriptStatus_RUNNING ,scriptStatus_PRIMED,
		scriptStatus_RUNOTHERSCRIPT, scriptStatus_SETSCRIPTNUMPARAMETER, scriptStatus_SETSCRIPTSTRPARAMETER, scriptStatus_NA
	};

	///<summary>A struct containing the common resources for all operators</summary>
	struct RunTimeCommons{
		///<summary>A pointer to the scripts memorypool</summary>
		MemoryPool* memory = nullptr;
		///<summary>The active status of the script</summary>
		scriptStatus status = scriptStatus_PRIMED;
		///<summary>A pointer to the message for communicating out</summary>
		CrossHandlers::Message* outMessageBuffer;
		unsigned int outCurrentMessage = 0;
		unsigned int outMessages;
		unsigned int outParam;
		unsigned int stack;
		unsigned int boundObject; 
		CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* gameObjects;
		///<summary>The array of bytes to use for loading ints</summary>
		unsigned char* intLoader = new unsigned char[4];
		///<summary>The maximum size of the stringloader</summary>
		unsigned int stringLoadSize = 1000;
		unsigned int currentStringSize = 0;
		///<summary>The array of bytes to use for loading strings</summary>
		unsigned char* stringLoader = new unsigned char[stringLoadSize];
		///<summary>Where the script is in the execution</summary>
		unsigned int counter = 0;
		unsigned int crossScriptObject = 0;
		CrossHandlers::MessageQueue* mQueue = nullptr;
		CrossHandlers::CelestialSlicedList<bool>* varWaiting = nullptr;
		bool pause = false;
		~RunTimeCommons()
		{
			delete varWaiting;
			delete[] intLoader;
			delete[] stringLoader;
			delete[] outMessageBuffer;
			delete memory;
		}
	};

}
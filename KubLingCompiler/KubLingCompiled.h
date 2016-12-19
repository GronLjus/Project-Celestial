#pragma once
#include "RunTimeOperations.h"
#include "CelestialStack.h"
#include <string>

namespace Resources
{

	class KubLingCompiled
	{
	public:

		struct Param
		{

			unsigned char* byteVal;
			unsigned char byteSize;
			unsigned int adr;

		};

		KubLingCompiled(unsigned int maxParams, unsigned int maxStringParams, unsigned int maxFloatParams, std::string name);
		///<summary>Add a compiled command to the script</summary>
		///<param val='code'>[in]An array of bytes</param>
		///<param val='codeSize'>[in]The amount of bytes in the array</param>
		void AddCommand(unsigned char* code, int codeSize);

		void AddParam(unsigned char byteSize, unsigned char* byteVal, unsigned int adr);
		void FinishParams();
		CrossHandlers::CelestialStack<Param>* GetParamList() const;
		CrossHandlers::CelestialStack<unsigned int>* GetParams() const;

		void AddParamAdr(unsigned int param, unsigned int adr, char type);
		void AddSystemParamAdr(Logic::RunTimeParams rtp, unsigned int adr);
		void UpParam(char type);
		void ResetParam(char type);

		std::string GetName() const;

		void AddStackPlaceHolder(unsigned int line);
		void AddMemOffsetPlaceHolder(unsigned int line);
		void SetMaxVar(unsigned int var);
		void SetMemOffset(unsigned int offset);
		unsigned int GetStackPHLine() const;
		unsigned int GetMemOffsetPHLine() const;

		unsigned char GetCurrentParam(char type) const;
		unsigned int GetMaxParams(char type) const;
		unsigned int GetAdr(unsigned int param, char type) const;
		unsigned int GetAdr(Logic::RunTimeParams rtp) const;
		unsigned int GetMaxVar() const;
		unsigned int GetMemOffset() const;

		///<summary>Sets the offset of the script</summary>
		///<param val='off'>[in]The offset</param>
		void SetOffset(int off);
		///<summary>Gets the amount of commands</summary>
		///<returns>The number off commands in the script</returns>
		int GetCodeSize();
		///<summary>Get a command from the script</summary>
		///<param val='size'>[out]The number of bytes in the command</param>
		///<param val='place'>[in]The command to load</param>
		///<returns>An array of bytes </returns>
		unsigned char* GetCode(int &size, int place);
		///<summary>Gets the scripts offset</summary>
		///<returns>The scripts offset</returns>
		int GetOffset();

		unsigned int GetScriptId() const;
		void SetScriptId(unsigned int scriptId);
		virtual ~KubLingCompiled();

		private:

			///<summary>A list of commands in the script</summary>
			unsigned char** code;
			///<summary>Haw many bytes are in each commands</summary>
			int* cSize;
			///<summary>The amount of commands</summary>
			int codeSize;
			///<summary>The maximum amount of commands</summary>
			int maxCodeSize;
			///<summary>The scripts offset</summary>
			int offset;
			unsigned int* systemParams;
			unsigned int* paramAdrDic;
			unsigned int* stringParamAdrDic;
			unsigned int* floatParamAdrDic;
			unsigned int maxParams;
			unsigned int maxStringParams;
			unsigned int maxFloatParams;
			unsigned int scriptId;
			CrossHandlers::CelestialStack<Param>* paramList;
			CrossHandlers::CelestialStack<unsigned int>* params;

			CrossHandlers::CelestialStack<unsigned int>* placeHolders;
			CrossHandlers::CelestialStack<unsigned int>* memOffsetplaceHolders;
			unsigned int currentParams;
			unsigned int maxVar;
			unsigned int memOffset;

			unsigned char cIP;
			unsigned char cFP;
			unsigned char cSP;
		
			std::string name;

	};
}
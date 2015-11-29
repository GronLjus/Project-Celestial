#pragma once
#include "BaseObject.h"
#include "RunTimeOperations.h"

namespace Resources
{

	///<summary>The class contains the bytecode of the script</summary>
	class CelScriptCompiled : public BaseObject
	{

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
			unsigned int maxParams;
			unsigned int maxStringParams;
			unsigned int scriptId;

		public:
			CelScriptCompiled(unsigned int maxParams, unsigned int maxStringParams);
			///<summary>Add a compiled command to the script</summary>
			///<param val='code'>[in]An array of bytes</param>
			///<param val='codeSize'>[in]The amount of bytes in the array</param>
			void AddCommand(unsigned char* code, int codeSize);

			void AddParamAdr(unsigned int param, unsigned int adr,bool string);
			void AddSystemParamAdr(Logic::RunTimeParams rtp, unsigned int adr);

			unsigned int GetMaxParams(bool string) const;
			unsigned int GetAdr(unsigned int param,bool string) const;
			unsigned int GetAdr(Logic::RunTimeParams rtp) const;

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
			virtual ~CelScriptCompiled();

	};
}
#include "../../../../stdafx.h"
#include "CelScriptCompiled.h"

using namespace Resources;

CelScriptCompiled::CelScriptCompiled(unsigned int maxParams, unsigned int maxStringParams, unsigned int maxFloatParams)
{

	maxCodeSize = 100;
	
	this->maxParams = maxParams;
	this->maxStringParams = maxStringParams;
	this->maxFloatParams = maxFloatParams;

	paramAdrDic = new unsigned int[maxParams];
	for (unsigned int i = 0; i < maxParams; i++) { paramAdrDic[i] = 0; }
	stringParamAdrDic = new unsigned int[maxStringParams];
	for (unsigned int i = 0; i < maxStringParams; i++) { stringParamAdrDic[i] = 0; }
	floatParamAdrDic = new unsigned int[maxFloatParams];
	for (unsigned int i = 0; i < maxFloatParams; i++) { floatParamAdrDic[i] = 0; }

	systemParams = new unsigned int[Logic::RunTimeParams_NA];

	code = new unsigned char*[maxCodeSize];
	cSize = new int[maxCodeSize];
	codeSize = 0;
	offset = 0;
	scriptId = 0;

}

void CelScriptCompiled::AddCommand(unsigned char* code, int codeSize)
{

	if (this->codeSize == maxCodeSize)
	{

		maxCodeSize += 200;
		unsigned char** tempCommands = new unsigned char*[maxCodeSize];
		int* tempCSize = new int[maxCodeSize];

		for (int i = 0; i < this->codeSize;i++)
		{

			tempCommands[i] = this->code[i];
			tempCSize[i] = cSize[i];

		}

		delete[] this->code;
		delete[] this->cSize;

		this->code = tempCommands;
		this->cSize = tempCSize;

	}

	this->code[this->codeSize] = code;
	this->cSize[this->codeSize] = codeSize;
	this->codeSize++;;

}

void CelScriptCompiled::AddParamAdr(unsigned int param, unsigned int adr, char type)
{

	if(type == 's' && param <maxStringParams)
	{

		stringParamAdrDic[param] = adr;

	}
	else if (type == 'f' && param <maxFloatParams)
	{

		floatParamAdrDic[param] = adr;

	}
	else
	{

		paramAdrDic[param] = adr;

	}
}

void CelScriptCompiled::AddSystemParamAdr(Logic::RunTimeParams rtp, unsigned int adr)
{

	if (rtp != Logic::RunTimeParams_NA)
	{

		systemParams[rtp] = adr;

	}
}

unsigned int CelScriptCompiled::GetMaxParams(bool string) const
{

	return string ? maxStringParams :maxParams;

}

unsigned int CelScriptCompiled::GetAdr(unsigned int param, char type) const
{

	switch (type)
	{

		case 'n':
			if (param < maxParams)
			{

				return paramAdrDic[param];

			}
			break;
		case 's':
			if (param < maxStringParams)
			{

				return stringParamAdrDic[param];

			}
			break;
		case 'f':
			if (param < maxFloatParams)
			{

				return floatParamAdrDic[param];

			}
			break;
	}

	return 0;

}

unsigned int CelScriptCompiled::GetAdr(Logic::RunTimeParams rtp) const
{
	
	if (rtp != Logic::RunTimeParams_NA)
	{

		return systemParams[rtp];

	}

	return 0;

}


void CelScriptCompiled::SetOffset(int off)
{

	offset = off;

}

int CelScriptCompiled::GetCodeSize()
{

	return codeSize;

}

unsigned char* CelScriptCompiled::GetCode(int &size, int place)
{

	if (place < maxCodeSize)
	{

		size = cSize[place];
		return code[place];

	}

	return nullptr;

}

int CelScriptCompiled::GetOffset()
{

	return offset;

}

unsigned int CelScriptCompiled::GetScriptId() const
{

	return scriptId;

}

void CelScriptCompiled::SetScriptId(unsigned int scriptId)
{

	this->scriptId = scriptId;

}

CelScriptCompiled::~CelScriptCompiled()
{

	for (int i = 0; i < codeSize; i++)
	{

		delete[] code[i];

	}

	delete[] code;
	delete[] cSize;

	delete[] paramAdrDic;
	delete[] stringParamAdrDic;
	delete[] floatParamAdrDic;

	delete[] systemParams;

}
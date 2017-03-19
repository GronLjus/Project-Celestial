#include "KubLingCompiled.h"

using namespace Resources;
using namespace CrossHandlers;

KubLingCompiled::KubLingCompiled(unsigned int maxParams, unsigned int maxStringParams, unsigned int maxFloatParams, std::string name)
{

	this->name = name;
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
	memSize = 0;

	paramList = new CelestialStack<Param>(false);
	params = new CelestialStack<unsigned int>(false);
	placeHolders = new CelestialStack<unsigned int>(false);
	memOffsetplaceHolders = new CelestialStack<unsigned int>(false);
	initSizePlaceHolders = new CelestialStack<unsigned int>(false);
	memSizePlaceHolders = new CelestialStack<unsigned int>(false);

	currentParams = 0;

	cIP = 0;
	cFP = 0;
	cSP = 0;

}

unsigned int KubLingCompiled::GetMaxVar() const
{

	return maxVar;

}

unsigned int KubLingCompiled::GetMemOffset() const
{

	return memOffset;

}

unsigned int KubLingCompiled::GetMemSize() const
{

	return memSize;

}

void KubLingCompiled::SetMaxVar(unsigned int var)
{

	maxVar = var;

}

void KubLingCompiled::SetMemOffset(unsigned int offset)
{

	memOffset = offset;

}

void KubLingCompiled::SetMemSize(unsigned int size)
{

	this->memSize = size;

}

std::string KubLingCompiled::GetName() const
{

	return name;

}

void KubLingCompiled::AddStackPlaceHolder(unsigned int line)
{

	placeHolders->PushElement(line);

}

void KubLingCompiled::AddMemOffsetPlaceHolder(unsigned int line)
{

	memOffsetplaceHolders->PushElement(line);

}

void KubLingCompiled::AddInitSizePlaceHolder(unsigned int line)
{

	initSizePlaceHolders->PushElement(line);

}

void KubLingCompiled::AddMemSizePlaceHolder(unsigned int line)
{

	memSizePlaceHolders->PushElement(line);

}

unsigned int KubLingCompiled::GetStackPHLine() const
{

	if (placeHolders->GetCount() > 0)
	{

		return placeHolders->PopElement();

	}

	return 0;

}

unsigned int KubLingCompiled::GetMemOffsetPHLine() const
{

	if (memOffsetplaceHolders->GetCount() > 0)
	{

		return memOffsetplaceHolders->PopElement();

	}

	return 0;

}

unsigned int KubLingCompiled::GetInitSizePHLine() const
{

	if (initSizePlaceHolders->GetCount() > 0)
	{

		return initSizePlaceHolders->PopElement();

	}

	return 0;

}

unsigned int KubLingCompiled::GetMemSizePHLine() const
{

	if (memSizePlaceHolders->GetCount() > 0)
	{

		return memSizePlaceHolders->PopElement();

	}

	return 0;

}

void KubLingCompiled::AddParam(unsigned char byteSize, unsigned char* byteVal, unsigned int adr)
{

	Param par;
	par.adr = adr;
	par.byteSize = byteSize;
	par.byteVal = byteVal;

	paramList->PushElement(par);
	currentParams++;

}

void KubLingCompiled::FinishParams()
{
	
	if (currentParams > 0)
	{

		params->PushElement(currentParams);
		currentParams = 0;

	}
}

CelestialStack<KubLingCompiled::Param>* KubLingCompiled::GetParamList() const
{

	return paramList;

}

CelestialStack<unsigned int>* KubLingCompiled::GetParams() const
{

	return params;

}

void KubLingCompiled::AddCommand(unsigned char* code, int codeSize)
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

void KubLingCompiled::AddParamAdr(unsigned int param, unsigned int adr, char type)
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

void KubLingCompiled::AddSystemParamAdr(Logic::RunTimeParams rtp, unsigned int adr)
{

	if (rtp != Logic::RunTimeParams_NA)
	{

		systemParams[rtp] = adr;

	}
}

void KubLingCompiled::UpParam(char type)
{

	switch (type)
	{

	case 'n':
		cIP++;
		break;
	case 's':
		cSP++;
		break;
	case 'f':
		cFP++;
		break;
	}
}

void KubLingCompiled::ResetParam(char type)
{

	switch (type)
	{

	case 'n':
		cIP = 0;
		break;
	case 's':
		cSP = 0;
		break;
	case 'f':
		cFP = 0;
		break;
	}

}

unsigned int KubLingCompiled::GetMaxParams(char type) const
{

	switch (type)
	{

	case 'n':
		return maxParams;
		break;
	case 's':
		return maxStringParams;
		break;
	case 'f':
		return maxFloatParams;
		break;
	}

	return 0;

}

unsigned char KubLingCompiled::GetCurrentParam(char type) const
{

	switch (type)
	{

	case 'n':
		return cIP;
		break;
	case 's':
		return cSP;
		break;
	case 'f':
		return cFP;
		break;
	}

	return 0;

}

unsigned int KubLingCompiled::GetAdr(unsigned int param, char type) const
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

unsigned int KubLingCompiled::GetAdr(Logic::RunTimeParams rtp) const
{
	
	if (rtp != Logic::RunTimeParams_NA)
	{

		return systemParams[rtp];

	}

	return 0;

}


void KubLingCompiled::SetOffset(int off)
{

	offset = off;

}

int KubLingCompiled::GetCodeSize()
{

	return codeSize;

}

unsigned char* KubLingCompiled::GetCode(int &size, int place)
{

	if (place < maxCodeSize)
	{

		size = cSize[place];
		return code[place];

	}

	return nullptr;

}

int KubLingCompiled::GetOffset()
{

	return offset;

}

unsigned int KubLingCompiled::GetScriptId() const
{

	return scriptId;

}

void KubLingCompiled::SetScriptId(unsigned int scriptId)
{

	this->scriptId = scriptId;

}

KubLingCompiled::~KubLingCompiled()
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

	delete paramList;
	delete params;
	delete placeHolders;
	delete memOffsetplaceHolders;
	delete initSizePlaceHolders;
	delete memSizePlaceHolders;

}
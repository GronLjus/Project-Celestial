#include "stdafx.h"
#include "KubLingLabel.h"

using namespace Resources;

KubLingLabel::KubLingLabel()
{

	start = 0;
	memOffset = 0;
	script = "";

	params[0] = nullptr;
	params[1] = nullptr;
	params[2] = nullptr;
	params[3] = nullptr;

	paramMax[0] = 0;
	paramMax[1] = 0;
	paramMax[2] = 0;
	paramMax[3] = 0;

	translation = 0;

	Reset();

}

KubLingLabel::KubLingLabel(std::string script, unsigned int start, unsigned int* params[4], unsigned int paramMax[4]) : KubLingLabel()
{

	this->memOffset = memOffset;
	this->start = start;
	this->script = script;

	this->params[0] = params[0];
	this->params[1] = params[1];
	this->params[2] = params[2];
	this->params[3] = params[3];

	this->paramMax[0] = paramMax[0];
	this->paramMax[1] = paramMax[1];
	this->paramMax[2] = paramMax[2];
	this->paramMax[3] = paramMax[3];

}

KubLingLabel::KubLingLabel(KubLingLabel &copy) : KubLingLabel()
{

	this->memOffset = copy.memOffset;
	this->start = copy.start;
	this->script = copy.script;

	this->params[0] = copy.params[0];
	this->params[1] = copy.params[1];
	this->params[2] = copy.params[2];
	this->params[3] = copy.params[3];

	this->paramMax[0] = copy.paramMax[0];
	this->paramMax[1] = copy.paramMax[1];
	this->paramMax[2] = copy.paramMax[2];
	this->paramMax[3] = copy.paramMax[3];

}

unsigned int KubLingLabel::GetMemoffset() const
{

	return memOffset;

}

void KubLingLabel::SetMemOffset(unsigned int memOffset)
{

	this->memOffset = memOffset;

}

void KubLingLabel::SetTranslation(unsigned int translation)
{

	this->translation = translation;

}

unsigned int KubLingLabel::GetTranslation() const
{

	return translation;

}

bool KubLingLabel::hasParam(char val) const
{

	char parVal = val == 'n' ? 0 : val == 'f' ? 1 : val == 's' ? 2 : 3;

	return params[parVal] != nullptr && paramSize[parVal] < paramMax[parVal];

}

unsigned int KubLingLabel::GetParam(char val)
{

	char parVal = val == 'n' ? 0 : val == 'f' ? 1 : val == 's' ? 2 : 3;
	unsigned int retVal = params[parVal][paramSize[parVal]];
	paramSize[parVal]++;
	return retVal;

}

std::string KubLingLabel::GetName() const
{

	return script;

}

unsigned int KubLingLabel::GetStart() const
{

	return start;

}

void KubLingLabel::Reset()
{

	paramSize[0] = 0;
	paramSize[1] = 0;
	paramSize[2] = 0;
	paramSize[3] = 0;

}

void KubLingLabel::Delete()
{

	if (params[0] != nullptr)
	{

		delete[] params[0];

	}

	if (params[1] != nullptr)
	{

		delete[] params[1];

	}

	if (params[2] != nullptr)
	{

		delete[] params[2];

	}

	if (params[3] != nullptr)
	{

		delete[] params[3];

	}
}
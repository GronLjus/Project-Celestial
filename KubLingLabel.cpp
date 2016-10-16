#include "stdafx.h"
#include "KubLingLabel.h"

using namespace Resources;

KubLingLabel::KubLingLabel()
{

	start = 0;
	script = "";

	params[0] = nullptr;
	params[1] = nullptr;
	params[2] = nullptr;
	params[3] = nullptr;

}

KubLingLabel::KubLingLabel(std::string script, unsigned int start, unsigned int* params[4])
{

	this->start = start;
	this->script = script;

	this->params[0] = params[0];
	this->params[1] = params[1];
	this->params[2] = params[2];
	this->params[3] = params[3];

}

KubLingLabel::KubLingLabel(KubLingLabel &copy)
{

	this->start = copy.start;
	this->script = copy.script;
	this->params[0] = copy.params[0];
	this->params[1] = copy.params[1];
	this->params[2] = copy.params[2];
	this->params[3] = copy.params[3];

}

unsigned int KubLingLabel::GetParam(char val)
{

	char parVal = val == 'n' ? 0 : val == 'f' ? 1 : val == 's' ? 2 : 3;
	unsigned int retVal = params[parVal][paramSize[parVal]];
	paramSize[parVal]++;
	return retVal;

}

std::string KubLingLabel::GetName()
{

	return script;

}

unsigned int KubLingLabel::GetStart()
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
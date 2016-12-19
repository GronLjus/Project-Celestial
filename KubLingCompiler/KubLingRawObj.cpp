#include "KubLingRawObj.h"

using namespace Resources;


KubLingRawObj::KubLingRawObj(unsigned long long* code, unsigned int codes,
	KubLingLabelObj* labels, unsigned int totalLabels,
	unsigned int heapVars, unsigned int stackSpace)
{

	this->code = code;
	this->codes = codes;
	this->labels = labels;
	this->totalLabels = totalLabels;

	this->heapVars = heapVars;
	this->stackSpace = stackSpace;

}

KubLingRawObj::KubLingRawObj(KubLingRawObj* copy)
{
	
	code = copy->code;
	codes = copy->codes;

	labels = copy->labels;
	totalLabels = copy->totalLabels;

	heapVars = copy->heapVars;
	stackSpace = copy->stackSpace;
	
	copy->code = nullptr;
	copy->labels = nullptr;

}

unsigned int KubLingRawObj::GetMemOffset() const
{

	return stackSpace;

}

KubLingLabelObj KubLingRawObj::GetLabelObj(std::string label) const
{

	bool found = false;
	unsigned int i;

	for (i = 0; i < totalLabels && !found; i++)
	{

		found = labels[i].GetName() == label;

	}

	if (found)
	{

		return labels[i - 1];

	}

	return KubLingLabelObj();

}

unsigned int KubLingRawObj::GetStart(std::string label) const
{

	return GetLabelObj(label).GetStart();

}

unsigned int KubLingRawObj::GetHeapVars() const
{

	return heapVars;

}

unsigned int KubLingRawObj::GetLength() const
{

	return codes;

}

unsigned long long* KubLingRawObj::GetCode() const
{

	return code;

}

KubLingRawObj::~KubLingRawObj()
{

	if (labels != nullptr)
	{

		for (unsigned int i = 0; i < totalLabels; i++)
		{

			labels[i].Delete();

		}

		delete[] labels;

	}

	if (code != nullptr)
	{

		delete[] code;

	}
}
#include "stdafx.h"
#include "KubLingRaw.h"

using namespace Resources;


KubLingRaw::KubLingRaw(unsigned long long* code, unsigned int codes,
	KubLingLabel* labels, unsigned int totalLabels,
	unsigned int heapVars, unsigned int stackSpace)
{

	this->code = code;
	this->codes = codes;
	this->labels = labels;
	this->totalLabels = totalLabels;

	this->heapVars = heapVars;
	this->stackSpace = stackSpace;

}

unsigned int KubLingRaw::GetMemOffset() const
{

	return stackSpace;

}

KubLingLabel KubLingRaw::GetLabel(std::string label) const
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

	return KubLingLabel();

}

unsigned int KubLingRaw::GetStart(std::string label) const
{

	return GetLabel(label).GetStart();

}

unsigned int KubLingRaw::GetHeapVars() const
{

	return heapVars;

}

unsigned int KubLingRaw::GetLength() const
{

	return codes;

}

unsigned long long* KubLingRaw::GetCode() const
{

	return code;

}

KubLingRaw::~KubLingRaw()
{

	for (unsigned int i = 0; i < totalLabels; i++)
	{

		labels[i].Delete();

	}

	delete[] code;
	delete[] labels;

}
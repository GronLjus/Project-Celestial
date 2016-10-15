#include "stdafx.h"
#include "KubLingRaw.h"

using namespace Resources;


KubLingRaw::KubLingRaw(unsigned long long* code, unsigned int codes,
	KubLingLabel* labels, unsigned int totalLabels)
{

	this->code = code;
	this->codes = codes;
	this->labels = labels;
	this->totalLabels = totalLabels;

}

unsigned int KubLingRaw::GetStart(std::string label) const
{

	bool found = false;
	unsigned int i;

	for (i = 0; i < totalLabels && found; i++)
	{

		found = labels[i].script == label;

	}

	if (found)
	{

		return labels[i - 1].start;

	}

	return 0;

}

unsigned long long* KubLingRaw::GetCode() const
{

	return code;

}
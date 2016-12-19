#include "stdafx.h"
#include "KubLingRaw.h"

using namespace Resources;


KubLingRaw::KubLingRaw(unsigned long long* code, unsigned int codes,
	KubLingLabel* labels, unsigned int totalLabels,
	unsigned int heapVars, unsigned int stackSpace) :
	KubLingRawObj(code, codes, labels, totalLabels,
		heapVars, stackSpace)
{

}
KubLingRaw::KubLingRaw(KubLingRawObj* raw) : KubLingRawObj(raw)
{

}

KubLingRaw::~KubLingRaw()
{


}
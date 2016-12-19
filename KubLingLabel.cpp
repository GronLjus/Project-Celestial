#include "stdafx.h"
#include "KubLingLabel.h"

using namespace Resources;

KubLingLabel::KubLingLabel() : KubLingLabelObj()
{

}

KubLingLabel::KubLingLabel(std::string script, unsigned int start, unsigned int* params[4], unsigned int paramMax[4]) : 
	KubLingLabelObj(script, start, params, paramMax)
{

}

KubLingLabel::KubLingLabel(KubLingLabelObj &copy) : KubLingLabelObj(copy)
{

}
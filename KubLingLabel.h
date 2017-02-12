#pragma once
#include "BaseObject.h"
#include "KubLingLabelObj.h"

namespace Resources
{

	class KubLingLabel : public BaseObject, public KubLingLabelObj
	{

		public:
			KubLingLabel();
			KubLingLabel(std::string script, unsigned int start, unsigned int* params[4], unsigned int paramMax[4]);
			KubLingLabel(KubLingLabelObj &copy);

			virtual unsigned char* Update(CrossHandlers::Message* mess) { return nullptr; }

			virtual ~KubLingLabel() {}
	};
}
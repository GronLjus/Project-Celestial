#pragma once
#include "Message.h"
#include "KubLingLabel.h"
#include "KubLingRawObj.h"

namespace Resources
{

	///<summary>The class contains the rawcode of the script</summary>
	class KubLingRaw : public BaseObject, public KubLingRawObj
	{

		public:
			KubLingRaw(unsigned long long* code, unsigned int codes,
				KubLingLabel* labels, unsigned int totalLabels,
				unsigned int heapVars, unsigned int stackSpace);
			KubLingRaw(KubLingRawObj* raw);

			virtual unsigned char* Update(CrossHandlers::Message* mess) { return nullptr; }
			virtual ~KubLingRaw();

	};
}
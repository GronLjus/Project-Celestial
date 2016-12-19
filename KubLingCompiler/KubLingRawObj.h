#pragma once
#include "KubLingLabelObj.h"

namespace Resources
{

	///<summary>The class contains the rawcode of the script</summary>
	class KubLingRawObj
	{

		private:
			unsigned long long* code;
			unsigned int codes;

			KubLingLabelObj* labels;
			unsigned int totalLabels;

			unsigned int heapVars;
			unsigned int stackSpace;

		public:
			KubLingRawObj(unsigned long long* code, unsigned int codes,
				KubLingLabelObj* labels, unsigned int totalLabels,
				unsigned int heapVars, unsigned int stackSpace);

			KubLingRawObj(KubLingRawObj* copy);

			unsigned int GetMemOffset() const;
			unsigned int GetStart(std::string label) const;
			unsigned int GetLength() const;
			unsigned int GetHeapVars() const;
			unsigned long long* GetCode() const;
			KubLingLabelObj GetLabelObj(std::string label) const;

			virtual ~KubLingRawObj();

	};
}
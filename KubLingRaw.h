#pragma once
#include "Message.h"
#include "KubLingLabel.h"

namespace Resources
{

	///<summary>The class contains the rawcode of the script</summary>
	class KubLingRaw : public BaseObject
	{

		private:
			unsigned long long* code;
			unsigned int codes;

			KubLingLabel* labels;
			unsigned int totalLabels;

			unsigned int heapVars;
			unsigned int stackSpace;

		public:
			KubLingRaw(unsigned long long* code, unsigned int codes,
				KubLingLabel* labels, unsigned int totalLabels,
				unsigned int heapVars, unsigned int stackSpace);

			unsigned int GetMemOffset() const;
			unsigned int GetStart(std::string label) const;
			unsigned int GetLength() const;
			unsigned int GetHeapVars() const;
			unsigned long long* GetCode() const;
			KubLingLabel GetLabel(std::string label) const;
			virtual void Update(CrossHandlers::Message* mess) {}

			virtual ~KubLingRaw();

	};
}
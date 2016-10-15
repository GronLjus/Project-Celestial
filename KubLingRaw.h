#pragma once
#include "BaseObject.h"
#include "RunTimeOperations.h"
#include "CelestialStack.h"

namespace Resources
{

	struct KubLingLabel
	{

		std::string script;
		unsigned int start;

	};

	///<summary>The class contains the rawcode of the script</summary>
	class KubLingRaw : public BaseObject
	{

		private:
			unsigned long long* code;
			unsigned int codes;

			KubLingLabel* labels;
			unsigned int totalLabels;

		public:
			KubLingRaw(unsigned long long* code, unsigned int codes,
				KubLingLabel* labels, unsigned int totalLabels);

			unsigned int GetStart(std::string label) const;
			unsigned long long* GetCode() const;

			virtual void Update(CrossHandlers::Message* mess) {}

	};
}
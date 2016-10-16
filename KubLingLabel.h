#pragma once
#include "BaseObject.h"

namespace Resources
{

	class KubLingLabel : public BaseObject
	{

		private:
			std::string script;
			unsigned int start;

			unsigned int paramSize[4];
			unsigned int* params[4];

		public:
			KubLingLabel();
			KubLingLabel(std::string script, unsigned int start, unsigned int* params[4]);
			KubLingLabel(KubLingLabel &copy);

			unsigned int GetParam(char val);
			std::string GetName();
			unsigned int GetStart();

			void Reset();

			void Delete();

			virtual void Update(CrossHandlers::Message* mess) {}
	};
}
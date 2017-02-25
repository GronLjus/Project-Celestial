#pragma once
#include <string>

namespace Resources
{

	class KubLingLabelObj
	{

		private:
			std::string script;
			unsigned int start;
			unsigned int memOffset;

			unsigned int paramSize[4];
			unsigned int paramMax[4];
			unsigned int* params[4];

			unsigned int translation;

			unsigned int initSize;

		public:
			KubLingLabelObj();
			KubLingLabelObj(std::string script, unsigned int start, unsigned int* params[4], unsigned int paramMax[4]);
			KubLingLabelObj(KubLingLabelObj &copy);

			void SetTranslation(unsigned int translation);
			void SetMemOffset(unsigned int memOffset);
			void SetInitSize(unsigned int initSize);

			unsigned int GetTranslation() const;
			bool hasParam(char val) const;
			unsigned int GetParam(char val);
			std::string GetName() const;
			unsigned int GetStart() const;
			unsigned int GetMemoffset() const;
			unsigned int GetInitSize() const;

			void Reset();

			void Delete();
	};
}
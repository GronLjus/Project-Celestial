#pragma once
#include "KubLingRawTranslator.h"

namespace Logic
{

	class KubLingRawGenerator
	{

		private:

			Resources::KubLingLabel* labels;
			unsigned int totalLabels;
			unsigned int maxLabels;

			unsigned int totalCode;
			unsigned int totalOffset; 
			unsigned int maxStack;

			void expandLabels();

			Resources::KubLingCompiled** byteCodes;
			unsigned int compiled;

			KubLingRawTranslator* translator;

			rawCode expandBlock(rawCode code, unsigned int newMax);

			rawCode translateBlock(Resources::KubLingCompiled* byteCode,
				CrossHandlers::CelestialSlicedList<heapVar>* heap,
				unsigned int start,
				unsigned int stop,
				unsigned int* translation);

			rawCode createInitHeader(heapVar initData);
			rawCode createInitBlock(Resources::KubLingCompiled* byteCode,
				CrossHandlers::CelestialSlicedList<heapVar>* heap,
				unsigned int initLength,
				unsigned int* translation);

			rawCode createFooterBlock();

			void addLabel(Resources::KubLingCompiled* byteCode);
			rawCode assemble(Resources::KubLingCompiled* byteCode,
				unsigned int current,
				CrossHandlers::CelestialSlicedList<heapVar>* heap);

		public:
			KubLingRawGenerator(unsigned int maxStack);

			Resources::KubLingRaw* Assemble(
				Resources::KubLingCompiled** byteCodes,
				unsigned int compiled);

			~KubLingRawGenerator();

	};
}
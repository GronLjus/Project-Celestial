#pragma once
#include "KubLingRawTranslator.h"

namespace Logic
{

	class KubLingRawGenerator
	{

		private:

			Resources::KubLingLabelObj* labels;
			unsigned int totalLabels;
			unsigned int maxLabels;

			unsigned int totalCode;
			unsigned int totalInitMemory;
			unsigned int totalMemory; 
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

			void addLabel(Resources::KubLingCompiled* byteCode, unsigned int start);
			rawCode assemble(Resources::KubLingCompiled* byteCode,
				unsigned int current,
				CrossHandlers::CelestialSlicedList<heapVar>* heap);

			unsigned int figureOutMaxSize(rawCode* codes, unsigned int code, unsigned int codeSize);

		public:
			KubLingRawGenerator(unsigned int maxStack);

			Resources::KubLingRawObj* Assemble(
				Resources::KubLingCompiled** byteCodes,
				unsigned int compiled);

			~KubLingRawGenerator();

	};
}
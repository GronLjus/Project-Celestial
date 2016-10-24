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

			void expandLabels();

			Resources::KubLingCompiled** byteCodes;
			unsigned int compiled;

			KubLingRawTranslator* translator;

			rawCode expandBlock(rawCode code, unsigned int newMax);

			rawCode translateBlock(Resources::KubLingCompiled* byteCode,
				CrossHandlers::CelestialSlicedList<heapVar>* heap,
				unsigned int start,
				unsigned stop,
				unsigned int* jmpPlaceHolders,
				unsigned int* translation,
				unsigned int current,
				MemoryPool* memPol);

			rawCode createInitHeader(heapVar initData);
			rawCode createInitBlock(Resources::KubLingCompiled* byteCode,
				unsigned int current,
				CrossHandlers::CelestialSlicedList<heapVar>* heap,
				unsigned int initLength,
				unsigned int* jmpPlaceHolders,
				unsigned int* translation,
				MemoryPool* memPol);

			rawCode createFooterBlock();

			void addLabel(Resources::KubLingCompiled* byteCode);
			rawCode assemble(Resources::KubLingCompiled* byteCode,
				unsigned int current,
				CrossHandlers::CelestialSlicedList<heapVar>* heap);

		public:
			KubLingRawGenerator();

			Resources::KubLingRaw* Assemble(
				Resources::KubLingCompiled** byteCodes,
				unsigned int compiled);

			~KubLingRawGenerator();

	};
}
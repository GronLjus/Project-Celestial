#pragma once
#include "KubLingCompiled.h"
#include "KubLingRawObj.h"
#include "CelestialStack.h"
#include "MemoryPool.h"

namespace Logic
{

	struct rawCode
	{

		struct line
		{

			unsigned long long code = opcode_PLACE;
			unsigned long long r1 = 0;
			unsigned long long r2 = 1;
			unsigned long long r3 = 2;
			unsigned long long type = 0;

			unsigned long long scale = 10;

			bytecode traceCode = bytecode_NA;

		};

		line* code;
		unsigned int codeSize;
		unsigned int start;

		unsigned int maxLines;
		unsigned int initSize;
		unsigned int memSize;

		CelestialStack<unsigned int>* subs = nullptr;
		unsigned int totalMemSize = 0;

	};

	struct heapVar
	{

		std::string name;
		unsigned int var;

	};

	struct runTimeVal
	{
		CrossHandlers::CelestialSlicedList<heapVar>* heap;
		unsigned int* jmpPlaceHolders;
		unsigned int* translation;
		Resources::KubLingCompiled** byteCodes;
		unsigned int current;
		unsigned int compiled;
		MemoryPool* memory;
		bool initBlock;
		CrossHandlers::CelestialStack<unsigned int>* subScripts;
		unsigned int offset;

	};

	class KubLingRawTranslator
	{

		private:

			runTimeVal rtv;

			typedef RunTimeError(*TranslateByteCode) (rawCode* raw,unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv);
			TranslateByteCode* translator;

		public:
			KubLingRawTranslator();

			void SetRTV(CrossHandlers::CelestialSlicedList<heapVar>* heap,
				unsigned int* jmpPlaceHolders,
				unsigned int* translation,
				Resources::KubLingCompiled** byteCodes,
				unsigned int compiled,
				unsigned int current,
				MemoryPool* memPool,
				bool initBlock,
				CrossHandlers::CelestialStack<unsigned int>* subScripts);

			rawCode Translate(unsigned char* bytes,
				unsigned int byteSize,
				unsigned int codeOffset,
				unsigned int line);

			~KubLingRawTranslator();

	};
}
#pragma once
#include "CelestialSlicedList.h"
#include "CelestialStack.h"
#include "KubLingCompiled.h"
#include <string>

namespace Logic
{

	///<summary>Handles dynamic memory allocation</summary>
	class MemoryPool
	{

		public:

			struct offset
			{
				unsigned int offsetVar;
				unsigned int size;

			};

			MemoryPool(unsigned int maxStack);

			void AddSystemMem(Resources::KubLingCompiled* compiled, bool initBlock);

			unsigned int AddVariable(unsigned int var, unsigned int valSize, bool initBlock);
			unsigned int AddVariable(unsigned int var, unsigned int adr, unsigned int valSize, bool initBlock);

			void AddOffset(unsigned int var, unsigned int offAdr, unsigned int offsize);

			offset GetOffset(unsigned int var);

			unsigned int GetVarLength(unsigned int var) const;
			unsigned int GetMaxVar() const;

			unsigned int GetStartingAdr() const;
			unsigned int GetMemorySize() const;

			unsigned int GetVarSize(unsigned int var) const;
			unsigned int GetVarAdr(unsigned int var) const;
			bool GetVarInitBlock(unsigned int var) const;

			unsigned int GetMaxStack() const;

			~MemoryPool();

		private:

			///<summary>A struct describing a memory block</summary>
			struct memBlock
			{
				memBlock() : length(0), place(0), initBlock(false){}
				///<summary>Where the block starts</summary>
				unsigned int place = 0;
				///<summary>How many bytes large it is</summary>
				unsigned int length = 0;
				bool initBlock = false;

			};
			
			struct offsetStack
			{

				offsetStack() { offsets = nullptr; }

				offset* offsets;
				unsigned int lastOffset;

			};

			///<summary>The last allocated address</summary>
			unsigned int adrLast;
			///<summary>A pointer to the list of all variables</summary>
			CrossHandlers::CelestialSlicedList<memBlock>* variables;
			CrossHandlers::CelestialSlicedList<offsetStack>* offsets;

			///<summary>The last hole</summary>
			unsigned char holeVal;
			///<summary>An array of unused holes in the memory, 255 is the maximum amount stored in a single byte excluding the 0-value</summary>
			memBlock holes[255];

			///<summary>Resourt the hole array from biggest to smallest</summary>
			///<param val='pivot'>[in]The hole to resort</param>
			void resortHoles(unsigned char pivot);;
			///<summary>Find where the variable should be saved in the memory</summary>
			///<param val='var'>[in]The variable to figure out</param>
			///<param val='valSize'>[in]The size of the variable in bytes</param>
			///<returns>The starting address to start writing</returns>
			unsigned int findAddress(unsigned int var, unsigned int valSize, bool inInit);
			unsigned int sysAdr;
			unsigned int maxVar;
			unsigned int maxStack;

			unsigned int maxOffsets;

	};
}
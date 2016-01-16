#pragma once
#include "CelestialSlicedList.h"
#include "CelestialStack.h"
#include <string>

namespace Logic
{

	enum MemErrorCode{ ErrorCode_OK = 0, ErrorCode_UNKNOWN = -1, ErrorCode_BADMEM = -2 };

	///<summary>Handles dynamic memory allocation</summary>
	class MemoryPool
	{

		private:

			///<summary>A struct describing a memory block</summary>
			struct memBlock
			{
				memBlock() : length(0), place(0){}
				///<summary>Where the block starts</summary>
				unsigned int place = 0;
				///<summary>How many bytes large it is</summary>
				unsigned int length = 0;

			};

			///<summary>A pointer to the primary pool to use for memory managment</summary>
			CrossHandlers::CelestialSlicedList<unsigned char>* memoryA;
			///<summary>The last allocated address</summary>
			unsigned int adrLast;
			///<summary>A pointer to the list of all variables</summary>
			CrossHandlers::CelestialSlicedList<memBlock>* variables;

			///<summary>The last hole</summary>
			unsigned char holeVal;
			///<summary>An array of unused holes in the memory, 255 is the maximum amount stored in a single byte excluding the 0-value</summary>
			memBlock holes[255];

			///<summary>Resourt the hole array from biggest to smallest</summary>
			///<param val='pivot'>[in]The hole to resort</param>
			void resortHoles(unsigned char pivot);
			///<summary>Write data to a variable by page</summary>
			///<param val='var'>[in]The variable to write to</param>
			///<param val='bytes'>[in]The amount of bytes to write</param>
			///<param val='val'>[in]An array of bytes to write</param>
			void writeData(unsigned int var, unsigned int bytes, unsigned char* val);

			///<summary>Reads the memory of a variable</summary>
			///<param val='var'>[in]The variable to read from</param>
			///<param val='offset'>[in]Where to start</param>
			///<param val='bytes'>[in]The amount of bytes to read</param>
			///<param val='val'>[out]An array of bytes to write the results to</param>
			///<returns>Any errors</returns>
			MemErrorCode readData(unsigned int var, unsigned int offset, unsigned int bytes, unsigned char* &out);
			///<summary>Find where the variable should be saved in the memory</summary>
			///<param val='var'>[in]The variable to figure out</param>
			///<param val='valSize'>[in]The size of the variable in bytes</param>
			///<returns>The starting address to start writing</returns>
			unsigned int findAddress(unsigned int var, unsigned int valSize);

		public:
			///<param val='pageSize'>[in]How big a page of memory should be</param>
			MemoryPool(unsigned int pageSize);
			///<summary>Adds/updates a variable in the memory</summary>
			///<param val='var'>[in]The variable to add/update</param>
			///<param val='val'>[in]An array of bytes to write</param>
			///<param val='valSize'>[in]How many bytes to add</param>
			///<returns>Any errors</returns>
			MemErrorCode AddVariable(unsigned int var, unsigned char* val, unsigned int valSize);
			///<summary>Reads the whole variable from memory</summary>
			///<param val='var'>[in]The variable to read</param>
			///<param val='out'>[out]An array of bytes to write the results in</param>
			///<param val='valSize'>[out]How many bytes were read</param>
			///<returns>Any errors</returns>
			MemErrorCode ReadVariable(unsigned int var, unsigned char* &out, unsigned int &valSize);
			///<summary>Reads a variable from memory</summary>
			///<param val='var'>[in]The variable to read</param>
			///<param val='offset'>[in]How many bytes in to start reading</param>
			///<param val='out'>[out]An array of bytes to write the results in</param>
			///<param val='valSize'>[in/out]How many bytes to read and were read</param>
			///<returns>Any errors</returns>
			MemErrorCode ReadVariable(unsigned int var, unsigned int offset, unsigned int &bytes, unsigned char* &out);
			///<summary>Copy a variable to another variable</summary>
			///<param val='dst'>[in]The destination variable</param>
			///<param val='dst'>[in]The source variable</param>
			///<returns>Any errors</returns>
			MemErrorCode CopyVariable(unsigned int dst, unsigned int src);
			~MemoryPool();

	};
}
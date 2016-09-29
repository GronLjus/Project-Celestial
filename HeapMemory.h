#pragma once
#include <vector>

namespace Logic
{

	class HeapMemory
	{

		private:
			struct block
			{

				unsigned int adr;
				unsigned int size;

				friend bool operator<=(const block& block1, const block& block2)
				{
					return block1.size <= block2.size;

				}

				friend bool operator<(const block& block1, const block& block2)
				{
					return block1.size < block2.size;

				}

				friend bool operator==(const block& block1, const block& block2)
				{
					return block1.adr == block2.adr;

				}
			};

			class prio_queue
			{

			public:

				prio_queue() :nSize(0)
				{

					oVect.resize(0);

				}

				void push(block value)
				{

					//Push the cell into the back of the heap
					oVect.push_back(value);
					unsigned int pivot = nSize++;

					//Bubble the last value up
					while (pivot > 0)
					{

						unsigned int leftRight = pivot % 2 == 0 ? 2 : 1;
						unsigned int newPivot = (pivot - leftRight) / 2;

						if (oVect[pivot] <= oVect[newPivot])
						{

							block tempVal = oVect[pivot];
							oVect[pivot] = oVect[newPivot];
							oVect[newPivot] = tempVal;
							pivot = newPivot;

						}
						else
						{

							return;

						}
					}
				}

				const block top() const
				{

					return oVect[0];

				}

				void pop()
				{

					//Replace the root node with the last in the heap
					oVect[0] = oVect.back();
					oVect.pop_back();
					nSize--;

					unsigned int pivot = 0;

					//Bubble the root node down
					while (pivot < nSize)
					{

						unsigned int left = 2 * pivot + 1;
						unsigned int right = 2 * pivot + 2;

						if (left >= nSize)//At the bottom of the heap
						{

							return;

						}
						else
						{

							unsigned int smallest = left;

							if (right < nSize && oVect[right] < oVect[left])
							{

								smallest = right;

							}

							if (oVect[smallest] <= oVect[pivot])//The child is smaller, flip them around
							{

								block tempVal = oVect[pivot];
								oVect[pivot] = oVect[smallest];
								oVect[smallest] = tempVal;
								pivot = smallest;

							}
							else
							{

								return;

							}
						}
					}
				}

				bool empty()
				{

					return nSize == 0;

				}

				unsigned int nSize;
				std::vector<block> oVect;
			};

			prio_queue holes;

			char* memoryBlock;
			unsigned int blockSize;
			unsigned int incrSize;

			void expandMemory();

		public:
			HeapMemory(unsigned int size);

			unsigned int Allocate(unsigned int blockSize);
			void DeAllocate(unsigned int address, unsigned int size);

			char* GetMemory(unsigned int adress);

			~HeapMemory();

	};
}
#pragma once
#include <vector>

namespace CrossHandlers
{

	template <class T>
	class CelestialPrioQueue
	{

	public:

		CelestialPrioQueue() :nSize(0)
		{

			oVect.resize(0);

		}

		void Push(T value)
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

					T tempVal = oVect[pivot];
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

		const T Top() const
		{

			return oVect[0];

		}

		void Pop()
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

						T tempVal = oVect[pivot];
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

		bool Empty()
		{

			return nSize == 0;

		}

		void Clear()
		{

			nSize = 0;
			oVect.clear();

		}

		unsigned int nSize;
		std::vector<T> oVect;
	};

}
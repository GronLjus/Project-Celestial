#pragma once
#include "CelestialStack.h"

namespace CrossHandlers
{

	template <class T>
	///<summary>A sliced list container</summary>
	class CelestialSlicedList
	{
	
		public:
			///<param val='sliceSize'>[in]The size of each slice</param>
			CelestialSlicedList(unsigned int sliceSize);
			///<param val='sliceSize'>[in]The size of each slice</param>
			///<param val='baseVal'>[in]The value to use as a template in the list</param>
			CelestialSlicedList(unsigned int sliceSize, T baseVal);
			///<summary>Gets the size of each slice</summary>
			///<returns>The slicesize</returns>
			unsigned int GetSliceSize();
			///<summary>Gets the slice of the id</summary>
			///<param val='id'>[in]The global id of the object</param>
			///<returns>An array with the element in it</returns>
			T* GetSlice(unsigned int id);
			///<summary>Gets the value in the list</summary>
			///<param val='id'>[in]The global id of the value to get</param>
			///<returns>The value of the id</returns>
			T GetValue(unsigned int id);
			///<summary>Converts the global id to the sliceid</summary>
			///<param val='id'>[in]The global id to translate</param>
			///<returns>The local id</returns>
			unsigned int TransformId(unsigned int id);
			///<summary>Adds an element to the list</summary>
			///<param val='element'>[in]The element to add</param>
			///<returns>The global id of the added element</returns>
			unsigned int Add(T element);
			///<summary>Adds an element to the list</summary>
			///<param val='element'>[in]The element to add</param>
			///<param val='element'>[in]The global id of the added element</param>
			///<returns>Sucess</returns>
			bool Add(T element, unsigned int id);
			///<summary>Removes en element from the list, but does not delete it</summary>
			///<param val='id'>[in]The global id of the element to remove</param>
			void Remove(unsigned int id);
			///<summary>Deletes an element from the list</summary>
			///<param val='id'>[in]The global id of the element to delete</param>
			void Kill(unsigned int id);
			///<summary>Resets the list without deleting the elements and makes it empty</summary>
			void Reset();
			///<summary>Resets the list and deletes all the elements</summary>
			void KillList();
			unsigned int GetFirstEmpty();
			unsigned int GetHighest() const;
			~CelestialSlicedList();

		private:
			///<summary>Expand the array of lists</summary>
			void expand();
			///<summary>The current amount of elements in the slices</summary>
			unsigned int* sliceSize;
			///<summary>The maximum amount of elements in each slice</summary>
			unsigned int maxSliceSize;
			///<summary>The current slice</summary>
			unsigned int slice;
			///<summary>The maximum number of slices</summary>
			unsigned int maxSlices;

			unsigned int highestValue;

			///<summary>The list</summary>
			T** list;

			T* base;
			///<summary>Where the holes are</summary>
			CelestialStack<unsigned int>* holes;
	};
}

using namespace CrossHandlers;

template <class T>
CelestialSlicedList<T>::CelestialSlicedList(unsigned int sliceSize)
{

	highestValue = 0;
	holes = new CelestialStack<unsigned int>(false);
	maxSliceSize = sliceSize;
	slice = 0;
	maxSlices = 10;

	list = new T*[maxSlices]; 
	this->sliceSize = new unsigned int[maxSlices];
	base = nullptr;

	for (unsigned int i = 0; i < maxSlices; i++)
	{

		list[i] = nullptr;
		this->sliceSize[i] = 0;

	}
}

template <class T>
CelestialSlicedList<T>::CelestialSlicedList(unsigned int sliceSize, T baseVal) : CelestialSlicedList(sliceSize)
{

	base = new T[sliceSize];

	for (unsigned int i = 0; i < sliceSize; i++)
	{

		base[i] = baseVal;

	}
}

template <class T>
unsigned int CelestialSlicedList<T>::GetSliceSize()
{

	return maxSliceSize;

}

template <class T>
void CelestialSlicedList<T>::expand()
{

	maxSlices += 10;

	T** temp = new T*[maxSlices];
	unsigned int* tempSize = new unsigned int[maxSlices];

	for (unsigned int i = 0; i < maxSlices; i++)
	{

		if (i < slice)
		{

			temp[i] = list[i];
			tempSize[i] = sliceSize[i];

		}
		else
		{

			temp[i] = nullptr;
			tempSize[i] = 0;

		}
	}

	delete[] list;
	list = temp;
	sliceSize = tempSize;

}

template <class T>
T* CelestialSlicedList<T>::GetSlice(unsigned int id)
{

	unsigned int slice = id / maxSliceSize;

	if (slice >= maxSlices)
	{

		expand();

	}

	if (list[slice] == nullptr)
	{

		list[slice] = new T[maxSliceSize];

		if (base != nullptr)
		{

			memcpy(list[slice], base, sizeof(T)*maxSliceSize);

		}
	}

	return list[slice];

}

template <class T>
T CelestialSlicedList<T>::GetValue(unsigned int id)
{

	T* arr = GetSlice(id);
	return arr[TransformId(id)];

}

template <class T>
unsigned int CelestialSlicedList<T>::TransformId(unsigned int id)
{

	return id % maxSliceSize;
}

template <class T>
unsigned int CelestialSlicedList<T>::Add(T element)
{

	unsigned int id = 0;

	if (holes->GetCount() > 0)
	{

		id = holes->PopElement();

	}
	else
	{

		bool breakFor = false;

		for (unsigned int i = 0; i < maxSlices && !breakFor; i++)
		{
			if (sliceSize[i] < maxSliceSize)
			{

				id = i*maxSliceSize + sliceSize[i];
				breakFor = true;
				sliceSize[i]++;

			}
		}
	}

	Add(element, id);
	return id;

}

template <class T>
unsigned int CelestialSlicedList<T>::GetFirstEmpty()
{

	unsigned int id = 0;
	bool breakFor = false;

	for (unsigned int i = 0; i < maxSlices && !breakFor; i++)
	{
		if (sliceSize[i] < maxSliceSize)
		{

			id = i*maxSliceSize + sliceSize[i];
			breakFor = true;

		}
	}

	return id;

}

template <class T>
bool CelestialSlicedList<T>::Add(T element, unsigned int id)
{

	unsigned int slice = id / maxSliceSize;

	if (slice >= maxSlices)
	{
		
		expand();

	}

	if (list[slice] == nullptr)
	{

		list[slice] = new T[maxSliceSize];

		if (base != nullptr)
		{

			memcpy(list[slice], base, sizeof(T)*maxSliceSize);

		}
	}

	list[slice][id%maxSliceSize] = element;
	//sliceSize[slice]++;

	if (id > highestValue)
	{

		highestValue = id;

	}

	return true;

}

template <class T>
void CelestialSlicedList<T>::Remove(unsigned int id)
{

	unsigned int slice = id / maxSliceSize;
	list[slice][TransformId(id)] = nullptr;

	if (id == GetFirstEmpty())
	{

		sliceSize[slice]--;

	}
	else
	{

		holes->PushElement(id);

	}
}

template <class T>
void CelestialSlicedList<T>::Kill(unsigned int id)
{

	unsigned int slice = id / maxSliceSize;
	unsigned int localId = TransformId(id);

	if (list[slice][localId] != nullptr)
	{

		delete list[slice][localId];
		list[slice][localId] = nullptr;

		if (id == GetFirstEmpty())
		{

			sliceSize[slice]--;

		}
		else
		{

			holes->PushElement(id);

		}

	}
}

template <class T>
void CelestialSlicedList<T>::Reset()
{

	for (unsigned int i = 0; i < maxSlices; i++)
	{

		if (base != nullptr && 
			list[i] != nullptr)
		{

			memcpy(list[i], base, sizeof(T)*maxSliceSize);

		}

		sliceSize[i] = 0;

	}

	holes->Reset();
	slice = 0;
	highestValue = 0;

}

template <class T>
void CelestialSlicedList<T>::KillList()
{

	for (unsigned int i = 0; i < maxSlices; i++)
	{

		if (list[i] != nullptr)
		{

			for (unsigned int k = 0; k < maxSliceSize; k++)
			{

				if (list[i][k] != nullptr)
				{

					delete list[i][k];

				}
			}

			delete[] list[i];
			list[i] = nullptr;

		}
	}
}

template <class T>
unsigned int CelestialSlicedList<T>::GetHighest() const
{

	return highestValue;

}

template <class T>
CelestialSlicedList<T>::~CelestialSlicedList()
{

	Reset();
	
	for (unsigned int i = 0; i < maxSlices; i++)
	{

		if (list[i] != nullptr)
		{

			delete[] list[i];

		}
	}
	
	delete[] sliceSize;
	delete[] list;

	if (base != nullptr)
	{

		delete[] base;

	}

	delete holes;

}
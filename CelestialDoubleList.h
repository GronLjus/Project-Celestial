#pragma once
#include "CelestialDoubleListNode.h"

namespace CrossHandlers
{

	template <class T>

	class CelestialDoubleList
	{
	public:
		CelestialDoubleList();
		CelestialDoubleList(unsigned int deleteOver);

		///<summary>Adds a element last in the list</summary>
		///<param val='element'>[in]The element to add</param>
		void AddElement(T element);
		///<summary>Appends a copy last in the list</summary>
		///<param val='element'>[in]A pointer to the list to copy
		void AddElement(CelestialDoubleList<T>* element);
		///<summary>Gets the last node in the list</summary>
		///<returns>A pointer the last node</returns>
		CelestialDoubleListNode<T>* GetLastNode() const;
		///<summary>Gets the first node in the list</summary>
		///<returns>A pointer the first node</returns>
		CelestialDoubleListNode<T>* GetFirstNode() const;
		///<summary>Decreases the amount of elemnts in the list</summary>
		void DecreaseCount();
		///<summary>Resets the list without deleting the elements and makes it empty</summary>
		void Reset();
		///<summary>Resets the list and deletes all the elements</summary>
		void KillList();
		///<summary>Gets the number of elements in the list</summary>
		///<returns>The total amount of elemonts in the rist</returns>
		unsigned int GetCount();
		///<summary>Puts an element first in the list</summary>
		///<param val='elemnt'>[i]The element to add</param>
		void PushElement(T element);
		///<summary>Extract the first element in the list</summary>
		///<returns>The first object in the list</returns>
		T PopElement();
		~CelestialDoubleList();

	private:
		///<summary>The first node in the list</summary>
		CelestialDoubleListNode<T>* first;
		///<summary>The last node in the list</summary>
		CelestialDoubleListNode<T>* last;
		unsigned int count;
		unsigned int deleteOver;
	};
}

using namespace CrossHandlers;

template <class T>
CelestialDoubleList<T>::CelestialDoubleList()
{

	first = nullptr;
	last = nullptr;
	count = 0;
	deleteOver = 0;

}

template <class T>
CelestialDoubleList<T>::CelestialDoubleList(unsigned int deleteOver) : CelestialDoubleList<T>()
{

	this->deleteOver = deleteOver;

}

template <class T>
T CelestialDoubleList<T>::PopElement()
{

	if (first == nullptr)
	{

		return T();

	}
	else
	{

		T retVal = first->GetNodeObject();
		CelestialDoubleListNode<T>* newFirst = first->GetNext();

		if (deleteOver <= count)
		{

			CelestialDoubleListNode<T>* newPrev = first->GetPrev();
			delete first;
			newFirst->SetPrev(newPrev);

		}

		first = newFirst;
		count--;
		return retVal;

	}
}

template <class T>
void CelestialDoubleList<T>::PushElement(T element)
{

	if (first == nullptr)
	{

		first = new CelestialDoubleListNode<T>(element);
		last = first;

	}
	else if (first->GetPrev() == nullptr)
	{

		CelestialDoubleListNode<T>* newFirst = new CelestialDoubleListNode<T>(element);
		newFirst->SetNext(first);
		first = newFirst;

	}
	else
	{

		first->GetPrev()->SetNodeObject(element);
		first = first->GetPrev();
	}

	count++;

}

template <class T>
void CelestialDoubleList<T>::AddElement(T element)
{

	if (first == nullptr)
	{

		first = new CelestialDoubleListNode<T>(element);
		last = first;

	}
	else if (last->GetNext() == nullptr)
	{

		this->last->SetNext(new CelestialDoubleListNode<T>(element));
		this->last = last->GetNext();

	}
	else
	{
		last->GetNext()->SetNodeObject(element);
		last = last->GetNext();

	}

	count++;

}

template <class T>
void CelestialDoubleList<T>::AddElement(CelestialDoubleList<T>* element)
{

	CelestialDoubleListNode<T>* lN = element->GetFirstNode();

	while (lN != nullptr)
	{

		AddElement(lN->GetNodeObject());
		lN = lN->GetNext();

	}
}

template <class T>
CelestialDoubleListNode<T>* CelestialDoubleList<T>::GetLastNode() const
{

	return last;

}

template <class T>
CelestialDoubleListNode<T>* CelestialDoubleList<T>::GetFirstNode() const
{

	return first;

}

template <class T>
void CelestialDoubleList<T>::DecreaseCount()
{

	count--;

}
template <class T>
void CelestialDoubleList<T>::Reset()
{

	CelestialDoubleListNode<T>* next1 = first;

	while (next1 != nullptr)
	{

		CelestialDoubleListNode<T>* next2 = next1->GetNext();
		delete next1;
		next1 = next2;

	}

	first = nullptr;
	last = nullptr;
	count = 0;

}

template <class T>
void CelestialDoubleList<T>::KillList()
{

	CelestialDoubleListNode<T>* next1 = first;

	while (next1 != nullptr)
	{

		CelestialDoubleListNode<T>* next2 = next1->GetNext();
		delete next1->GetNodeObject();
		delete next1;
		next1 = next2;

	}

	first = nullptr;
	last = nullptr;
	count = 0;

}

template <class T>
unsigned int CelestialDoubleList<T>::GetCount()
{

	return count;

}

template <class T>
CelestialDoubleList<T>::~CelestialDoubleList()
{

	CelestialDoubleListNode<T>* next1 = first;

	while (next1 != nullptr)
	{

		CelestialDoubleListNode<T>* next2 = next1->GetNext();
		delete next1;
		next1 = next2;

	}
}
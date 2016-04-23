#pragma once
#include "CelestialListNode.h"
#include "CelestialDoubleList.h"

namespace CrossHandlers
{

	template <class T>

	class CelestialList
	{
		public:
			CelestialList(); 
			///<summary>Adds a element last in the list</summary>
			///<param val='element'>[in]The element to add</param>
			void AddElement(T element);
			///<summary>Adds a series of elements last in the list</summary>
			///<param val='elements'>[in]A pointer to the first of the elements to add</param>
			void AddElement(CelestialListNode<T>* element);
			///<summary>Appends a copy last in the list</summary>
			///<param val='element'>[in]A pointer to the list to copy
			void AddElement(CelestialList<T>* element);
			///<summary>Adds a series of elements last in the list</summary>
			///<param val='elements'>[in]A pointer to the first of the elements to add</param>
			void AddElement(CelestialDoubleListNode<T>* element);
			///<summary>Appends a copy last in the list</summary>
			///<param val='element'>[in]A pointer to the list to copy
			void AddElement(CelestialDoubleList<T>* element);
			///<summary>Gets the last node in the list</summary>
			///<returns>A pointer the last node</returns>
			CelestialListNode<T>* GetLastNode() const;
			///<summary>Gets the first node in the list</summary>
			///<returns>A pointer the first node</returns>
			CelestialListNode<T>* GetFirstNode() const;
			///<summary>Deletes the first element in the list</summary>
			void DeleteFirst();
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
			///<summary>Puts an element first in the list</summary>
			///<param val='elemnt'>[i]A pointer to the node to add</param>
			void PushElement(CelestialListNode<T>* element);
			///<summary>Extract the first element in the list</summary>
			///<returns>The first object in the list</returns>
			T PopElement();
			///<summary>Sets the last element in the list</summary>
			///<param val='element'>[in]A pointer to the element to be last</param>
			void SetLast(CelestialListNode<T>* element);
			~CelestialList();

		private:
			///<summary>The first node in the list</summary>
			CelestialListNode<T>* first;
			///<summary>The last node in the list</summary>
			CelestialListNode<T>* last;
			int count;
	};
}

using namespace CrossHandlers;

template <class T> 
CelestialList<T>::CelestialList()
{

	first = nullptr;
	last = nullptr;
	count = 0;

}

template <class T>
T CelestialList<T>::PopElement()
{

	if (first == nullptr)
	{

		return T();

	}
	else
	{

		T retVal = first->GetNodeObject();
		CelestialListNode<T>* newFirst = first->GetNext();
		delete first;
		first = newFirst;
		count--;
		return retVal;
		
	}
}

template <class T>
void CelestialList<T>::PushElement(T element)
{

	if (first == nullptr)
	{

		first = new CelestialListNode<T>(element);
		last = first;

	}
	else
	{

		CelestialListNode<T>* newFirst = new CelestialListNode<T>(element);
		newFirst->SetNext(first);
		first = newFirst;

	}

	count++;

}

template <class T>
void CelestialList<T>::PushElement(CelestialListNode<T>* element)
{

	if (first == nullptr)
	{

		first = element;
		last = first;

	}
	else
	{

		element->SetNext(first);
		first = element;

	}

	count++;

}

template <class T>
void CelestialList<T>::AddElement(T element)
{

	if (first == nullptr)
	{

		first = new CelestialListNode<T>(element);
		last = first;

	}
	else
	{

		this->last->SetNext(new CelestialListNode<T>(element));
		this->last = last->GetNext();

	}

	count++;

}

template <class T>
void CelestialList<T>::AddElement(CelestialListNode<T>* element)
{

	if (first == nullptr)
	{

		first = element;
		last = first;

	}
	else
	{

		this->last->SetNext(element);
		this->last = last->GetNext();

	}

	count++;

}

template <class T>
void CelestialList<T>::AddElement(CelestialDoubleListNode<T>* element)
{

	if (first == nullptr)
	{

		first = element;
		last = first;

	}
	else
	{

		this->last->SetNext(element);
		this->last = last->GetNext();

	}

	count++;

}

template <class T>
void CelestialList<T>::SetLast(CelestialListNode<T>* element)
{

	last = element;

}

template <class T>
void CelestialList<T>::AddElement(CelestialList<T>* element)
{

	CelestialListNode<T>* lN = element->GetFirstNode();

	while (lN != nullptr)
	{

		AddElement(lN->GetNodeObject());
		lN = lN->GetNext();

	}
}

template <class T>
void CelestialList<T>::AddElement(CelestialDoubleList<T>* element)
{

	CelestialDoubleListNode<T>* lN = element->GetFirstNode();

	while (lN != nullptr)
	{

		AddElement(lN->GetNodeObject());
		lN = lN->GetNext();

	}
}

template <class T>
CelestialListNode<T>* CelestialList<T>::GetLastNode() const
{

	return last;

}

template <class T>
CelestialListNode<T>* CelestialList<T>::GetFirstNode() const
{

	return first;

}

template <class T>
void CelestialList<T>::DeleteFirst()
{

	if (first != nullptr)
	{

		CelestialListNode<T>* oldFirst = first;
		first = first->GetNext();
		delete oldFirst;

	}

	count--;

}

template <class T>
void CelestialList<T>::DecreaseCount()
{

	count--;

}
template <class T>
void CelestialList<T>::Reset()
{

	CelestialListNode<T>* next1 = first;

	while (next1 != nullptr)
	{

		CelestialListNode<T>* next2 = next1->GetNext();
		delete next1;
		next1 = next2;

	}

	first = nullptr;
	last = nullptr;
	count = 0;

}

template <class T>
void CelestialList<T>::KillList()
{

	CelestialListNode<T>* next1 = first;

	while (next1 != nullptr)
	{

		CelestialListNode<T>* next2 = next1->GetNext();
		delete next1->GetNodeObject();
		delete next1;
		next1 = next2;

	}

	first = nullptr;
	last = nullptr;
	count = 0;

}

template <class T>
unsigned int CelestialList<T>::GetCount()
{

	return count;

}

template <class T>
CelestialList<T>::~CelestialList()
{

	CelestialListNode<T>* next1 = first;

	while (next1 != nullptr)
	{

		CelestialListNode<T>* next2 = next1->GetNext();
		delete next1;
		next1 = next2;

	}
}
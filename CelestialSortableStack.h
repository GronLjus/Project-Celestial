#pragma once
#include "CelestialDoubleListNode.h"

namespace CrossHandlers
{

	template <class T>

	class CelestialSortableStack
	{
	public:
		CelestialSortableStack(bool delOnPop);
		void PushElement(T element);
		T PopElement();
		T PeekElement();
		unsigned int GetCount();
		void Reset();
		void Rewind();
		~CelestialSortableStack();

	private:
		///<summary>The first node in the list</summary>
		CelestialDoubleListNode<T>* first;
		CelestialDoubleListNode<T>* root;
		int count;
		unsigned int maxCount;
		bool del;
	};
}

using namespace CrossHandlers;

template <class T>
CelestialSortableStack<T>::CelestialSortableStack(bool delOnPop)
{

	first = nullptr;
	root = nullptr;
	count = 0;
	maxCount = 0;
	del = delOnPop;

}

template <class T>
T CelestialSortableStack<T>::PeekElement()
{

	if (first == nullptr || count == 0)
	{

		return T();

	}
	else
	{

		return first->GetNodeObject();

	}
}

template <class T>
T CelestialSortableStack<T>::PopElement()
{

	if (first == nullptr || count == 0)
	{

		return T();

	}
	else
	{

		T retVal = first->GetNodeObject();
		CelestialDoubleListNode<T>* newFirst = first->GetNext();

		if (del)
		{

			delete first;

			if (newFirst != nullptr)
			{

				newFirst->SetPrev(nullptr);

			}
			else
			{

				root = nullptr;

			}

			maxCount--;

		}

		first = newFirst;
		count--;
		return retVal;

	}
}

template <class T>
unsigned int CelestialSortableStack<T>::GetCount()
{

	return count;

}

template <class T>
void CelestialSortableStack<T>::PushElement(T element)
{

	if (first == nullptr)
	{

		if (root == nullptr)
		{

			root = new CelestialDoubleListNode<T>(element);

		}
		else
		{

			root->SetNodeObject(element);

		}

		first = root;

	}
	else
	{
		CelestialDoubleListNode<T>* newFirst = first->GetPrev();

		if (newFirst == nullptr)
		{

			newFirst = new CelestialDoubleListNode<T>(element);
			newFirst->SetNext(first);
			first->SetPrev(newFirst);

		}
		else
		{

			newFirst->SetNodeObject(element);

		}

		first = newFirst;

		while (newFirst->GetNext() != nullptr &&
			newFirst->GetNodeObject() < newFirst->GetNext()->GetNodeObject())
		{

			CelestialDoubleListNode<T>* temp = newFirst->GetNext();
			temp->SetPrev(newFirst->GetPrev());
			newFirst->SetPrev(temp);
			newFirst->SetNext(temp->GetNext());
			temp->SetNext(newFirst);

		}
	}

	count++;

	if (count > maxCount)
	{

		maxCount = count;

	}

}

template <class T>
void CelestialSortableStack<T>::Reset()
{

	if (del)
	{

		if (root != nullptr)
		{

			CelestialDoubleListNode<T>* next = root->GetNext();
			CelestialDoubleListNode<T>* prev = root->GetPrev();
			delete root;

			while (next != nullptr)
			{

				CelestialDoubleListNode<T>* nextNext = next->GetNext();
				delete next;
				next = nextNext;

			}

			while (prev != nullptr)
			{

				CelestialDoubleListNode<T>* prevPrev = prev->GetPrev();
				delete prev;
				prev = prevPrev;

			}
		}
	}
	else
	{

		first = nullptr;

	}

	count = 0;
	maxCount = 0;

}

template <class T>
void CelestialSortableStack<T>::Rewind()
{

	if (!del)
	{

		count = maxCount;
		first = root;

	}
}

template <class T>
CelestialSortableStack<T>::~CelestialSortableStack()
{

	if (root != nullptr)
	{

		CelestialDoubleListNode<T>* next = root->GetNext();
		CelestialDoubleListNode<T>* prev = root->GetPrev();
		delete root;

		while (next != nullptr)
		{

			CelestialDoubleListNode<T>* nextNext = next->GetNext();
			delete next;
			next = nextNext;

		}

		while (prev != nullptr)
		{

			CelestialDoubleListNode<T>* prevPrev = prev->GetPrev();
			delete prev;
			prev = prevPrev;

		}
	}
}
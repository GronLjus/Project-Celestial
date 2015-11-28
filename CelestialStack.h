#pragma once
#include "CelestialDoubleListNode.h"

namespace CrossHandlers
{

	template <class T>

	class CelestialStack
	{
	public:
		CelestialStack(bool delOnPop);
		void PushElement(T element);
		T PopElement();
		T PeekElement();
		unsigned int GetCount();
		~CelestialStack();

	private:
		///<summary>The first node in the list</summary>
		CelestialDoubleListNode<T>* first;
		int count;
		bool del;
	};
}

using namespace CrossHandlers;

template <class T>
CelestialStack<T>::CelestialStack(bool delOnPop)
{

	first = nullptr;
	count = 0;
	del = delOnPop;

}

template <class T>
T CelestialStack<T>::PeekElement()
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
T CelestialStack<T>::PopElement()
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
		}

		first = newFirst;
		count--;
		return retVal;

	}
}

template <class T>
unsigned int CelestialStack<T>::GetCount()
{

	return count;
}

template <class T>
void CelestialStack<T>::PushElement(T element)
{

	if (first == nullptr)
	{

		first = new CelestialDoubleListNode<T>(element);

	}
	else
	{
		CelestialDoubleListNode<T>* newFirst = first->GetPrev();

		if (newFirst == nullptr)
		{

			newFirst = new CelestialDoubleListNode<T>(element);
			newFirst->SetNext(first);
			first->SetPrev(newFirst);
			first = newFirst;

		}
		else
		{

			newFirst->SetNodeObject(element);

		}
	}

	count++;

}

template <class T>
CelestialStack<T>::~CelestialStack()
{

	if (first != nullptr)
	{

		CelestialDoubleListNode<T>* next = first->GetNext();
		CelestialDoubleListNode<T>* prev = first->GetPrev();
		delete first;

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
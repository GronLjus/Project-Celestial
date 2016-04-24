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
		void Remove();
		unsigned int GetCount();
		void Reset();
		void Rewind();
		~CelestialStack();

	private:
		///<summary>The first node in the list</summary>
		CelestialDoubleListNode<T>* first;
		CelestialDoubleListNode<T>* root;
		CelestialDoubleListNode<T>* leftOvers;
		CelestialDoubleListNode<T>* last;
		unsigned int count;
		unsigned int maxCount;
		bool del;
	};
}

using namespace CrossHandlers;

template <class T>
CelestialStack<T>::CelestialStack(bool delOnPop)
{

	first = nullptr;
	leftOvers = nullptr;
	root = nullptr;
	last = nullptr;
	count = 0;
	maxCount = 0;
	del = delOnPop;

}

template <class T>
void CelestialStack<T>::Remove()
{

	if (first != nullptr || count == 0)
	{

		if (first->GetNext() != nullptr)
		{

			first->GetNext()->SetPrev(first->GetPrev());

		}

		if (first->GetPrev() != nullptr)
		{

			first->GetPrev()->SetNext(first->GetNext());

		}

		CelestialDoubleListNode<T>* removed = first;
		first = first->GetNext();

		if (removed == root)
		{

			root = first;

		}

		if (removed == last)
		{

			last = nullptr;

		}


		removed->SetNext(nullptr);
		removed->SetPrev(nullptr);

		if (leftOvers != nullptr)
		{

			leftOvers->SetNext(removed);
			removed->SetPrev(leftOvers);


		}

		leftOvers = removed;
		count--;
		maxCount--;
		
	}
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

			root = newFirst;
			maxCount--;

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

		if (last == nullptr)
		{

			if (leftOvers == nullptr)
			{

				first = new CelestialDoubleListNode<T>(element);

			}
			else
			{

				first = leftOvers;
				leftOvers = leftOvers->GetPrev();

				if (leftOvers != nullptr)
				{

					leftOvers->SetNext(nullptr);

				}

				first->SetPrev(nullptr);
				first->SetNext(nullptr);

			}

			last = first;
		}

		first = last;
		first->SetNodeObject(element);

	}
	else
	{
		CelestialDoubleListNode<T>* newFirst = first->GetPrev();

		if (newFirst == nullptr)
		{

			if (leftOvers == nullptr)
			{

				newFirst = new CelestialDoubleListNode<T>(element);

			}
			else
			{

				newFirst = leftOvers;
				leftOvers = leftOvers->GetPrev();

				if (leftOvers != nullptr)
				{

					leftOvers->SetNext(nullptr);

				}

				newFirst->SetPrev(nullptr);
				newFirst->SetNodeObject(element);

			}

			newFirst->SetNext(first);
			first->SetPrev(newFirst);

		}
		else
		{

			newFirst->SetNodeObject(element);

		}

		first = newFirst;

	}

	count++;
	root = first;

	if (count > maxCount)
	{

		maxCount = count;

	}

}

template <class T>
void CelestialStack<T>::Reset()
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
void CelestialStack<T>::Rewind()
{

	if (!del)
	{

		count = maxCount;
		first = root;

	}
}

template <class T>
CelestialStack<T>::~CelestialStack()
{

	if (last != nullptr)
	{

		CelestialDoubleListNode<T>* next = last->GetNext();
		CelestialDoubleListNode<T>* prev = last->GetPrev();
		delete last;

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

	while (leftOvers != nullptr)
	{

		CelestialDoubleListNode<T>* next = leftOvers->GetPrev();
		delete leftOvers;
		leftOvers = next;

	}
}
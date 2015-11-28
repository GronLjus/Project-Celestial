#include "stdafx.h"
#include "CelestialList.h"

using namespace CrossHandlers;

template <class T>
CelestialList<T>::CelestialList()
{

	first = NULL;
	last = NULL;

}

template <class T>
void CelestialList<T>::AddElement(T element)
{

	this->AddNode(new CelestialListNode<T>(T));

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

	if (first != NULL)
	{

		CelestialListNode<T>* oldFirst = first;
		first = first->GetNext();
		delete oldFirst;

	}
}

template <class T>
void CelestialList<T>::Reset()
{

	delete first;
	first = NULL;
	last = NULL;

}

template <class T>
CelestialList<T>::~CelestialList()
{

	CelestialListNode<T>* next1 = first;
	CelestialListNode<T>* next2;

	while (next1 != NULL)
	{

		next2 = next1->GetNext();
		delete next1;
		next1 = next2;

	}
}
#include "stdafx.h"
#include "CelestialListNode.h"

using namespace CrossHandlers;
template <class T>

CelestialListNode<T>::CelestialListNode(T object)
{

	this->object = mesh;
	this->next = NULL;

}

template <class T>
T CelestialListNode<T>::GetNodeObject() const
{

	return object;

}

template <class T>
CelestialListNode<T>* CelestialListNode<T>::GetNext() const
{

	return next;

}

template <class T>
void CelestialListNode<T>::SetNext(CelestialListNode<T>* next)
{

	this->next = next;

}

template <class T>
CelestialListNode<T>::~CelestialListNode()
{

}
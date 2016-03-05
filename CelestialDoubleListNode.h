#pragma once

namespace CrossHandlers
{

	template <class T>

	class CelestialDoubleListNode
	{

	public:
		///<param val='mesh'>[in]A pointer to the object in the node</param>
		CelestialDoubleListNode(T object);
		///<summary>Gets the object contained by the node</summary>
		///<returns>A pointer to the mesh</returns>
		T GetNodeObject() const;
		///<summary>Gets the next node</summary>
		///<returns>A pointer to the next node</returns>
		CelestialDoubleListNode<T>* GetNext() const;
		///<summary>Sets the next node</summary>
		///<param val='next'>[in]A pointer to the next node/</param>
		void SetNext(CelestialDoubleListNode<T>* next);
		///<summary>Gets the previous node</summary>
		///<returns>A pointer to the previous node</returns>
		CelestialDoubleListNode<T>* GetPrev() const;
		///<summary>Sets the previous node</summary>
		///<param val='next'>[in]A pointer to the previous node/</param>
		void SetPrev(CelestialDoubleListNode<T>* prev);
		///<summary>Replaces the object</summary>
		///<param val='object'>[in]The object to set)
		void SetNodeObject(T object);
		~CelestialDoubleListNode();
	private:
		///<summary>The object</summary>
		T object;
		///<summary>The next node</summary>
		CelestialDoubleListNode<T>* next;
		///<summary>The previous node</summary>
		CelestialDoubleListNode<T>* prev;
	};
}

using namespace CrossHandlers;
template <class T>

CelestialDoubleListNode<T>::CelestialDoubleListNode(T object)
{

	this->object = object;
	this->next = nullptr;
	this->prev = nullptr;

}

template <class T>
T CelestialDoubleListNode<T>::GetNodeObject() const
{

	return object;

}

template <class T>
CelestialDoubleListNode<T>* CelestialDoubleListNode<T>::GetNext() const
{

	return next;

}

template <class T>
void CelestialDoubleListNode<T>::SetNext(CelestialDoubleListNode<T>* next)
{

	this->next = next;

}

template <class T>
CelestialDoubleListNode<T>* CelestialDoubleListNode<T>::GetPrev() const
{

	return prev;

}

template <class T>
void CelestialDoubleListNode<T>::SetPrev(CelestialDoubleListNode<T>* prev)
{

	this->prev = prev;

}

template <class T>
void CelestialDoubleListNode<T>::SetNodeObject(T object)
{

	this->object = object;

}

template <class T>
CelestialDoubleListNode<T>::~CelestialDoubleListNode()
{

}
#pragma once

namespace CrossHandlers
{

	template <class T>

	class CelestialListNode
	{
	
		public:
			///<param val='mesh'>[in]A pointer to the object in the node</param>
			CelestialListNode(T object);
			///<summary>Gets the object contained by the node</summary>
			///<returns>A pointer to the mesh</returns>
			T GetNodeObject() const;
			///<summary>Gets the next node</summary>
			///<returns>A pointer to the next node</returns>
			CelestialListNode<T>* GetNext() const;
			///<summary>Sets the next node</summary>
			///<param val='next'>[in]A pointer to the next node/</param>
			void SetNext(CelestialListNode<T>* next);
			///<summary>Replaces the object</summary>
			///<param val='object'>[in]The object to set)
			void SetNodeObject(T object);
			~CelestialListNode();
		private:
			///<summary>The object</summary>
			T object;
			///<summary>The next node</summary>
			CelestialListNode<T>* next;
	};
}

using namespace CrossHandlers;
template <class T>

CelestialListNode<T>::CelestialListNode(T object)
{

	this->object = object;
	this->next = nullptr;

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
void CelestialListNode<T>::SetNodeObject(T object)
{

	this->object = object;

}

template <class T>
CelestialListNode<T>::~CelestialListNode()
{

}
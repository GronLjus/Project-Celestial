#pragma once
#include "CelestialList.h"

namespace CrossHandlers
{

	template <class T>

	///<summary>The node in a tree</summary>
	class CelestialTreeNode
	{
		private:
			///<summary>The internal object</summary>
			T object;
			///<summary>The pointer to the list of leafpointers</summary>
			CelestialList<CelestialTreeNode<T>*>* leafs;
			///<summary>A pointer to the parent of the node</summary>
			CelestialTreeNode<T>* parent;

		public:
			///<parm val='object'>[in]The object to contain</param>
			///<param val='parent'>[in]A pointer to the parental node</param>
			CelestialTreeNode(T object, CelestialTreeNode<T>* parent);
			///<summary>Adds a new leaf to the node</summary>
			///<param val='object'>[in]The object to add</param>
			void AddLeaf(T object);
			///<summary>Adds a new leaf to the node</summary>
			///<param val='leaf'>[in]A pointer to the leaf to add</param>
			void AddLeaf(CelestialTreeNode<T>* leaf);
			///<summary>Gets the list of leafs of the node</summary>
			///<returns>A pointer to the list containing the leafs of the node</returns>
			CelestialList<CelestialTreeNode<T>*>* GetLeafs();
			///<summary>Sets the nodes parent</summary>
			///<param val='parent'>[in]A pointer to the parent node</param>
			void SetParent(CelestialTreeNode<T>* parent);
			///<summary>Gets the parentnode of the node</summary>
			///<returns>A pointer to the parental node</returns>
			CelestialTreeNode<T>* GetParent();
			///<summary>Updates the object in the node</summary>
			///<param val='object'>[in]The new object to set in the node</param>
			void SetNodeObject(T object);
			///<summary>Gets the internal object of the node</summary>
			///<returns>The internal object</returns>
			T GetNodeObject();
			///<summary>Deletes the internal node</summary>
			void KillNode();
			///<summary>Removes the nodes leafs</summary>
			void RemoveLeafs();
			~CelestialTreeNode();

	};
}

using namespace CrossHandlers;

template <class T>
CelestialTreeNode<T>::CelestialTreeNode(T object, CelestialTreeNode<T>* parent)
{

	this->object = object;
	this->parent = parent;
	leafs = new CelestialList<CelestialTreeNode<T>*>();

}

template <class T>
void CelestialTreeNode<T>::AddLeaf(T object)
{

	if (leafs == nullptr)
	{

		leafs = new CelestialList<CelestialTreeNode<T>*>();

	}

	leafs->AddElement(new CelestialTreeNode<T>(object,this));

}

template <class T>
void CelestialTreeNode<T>::AddLeaf(CelestialTreeNode<T>* leaf)
{

	if (leafs == nullptr)
	{

		leafs = new CelestialList<CelestialTreeNode<T>*>();

	}

	leafs->AddElement(leaf);

}


template <class T>
CelestialList<CelestialTreeNode<T>*>* CelestialTreeNode<T>::GetLeafs()
{

	return leafs;

}

template <class T>
void CelestialTreeNode<T>::SetParent(CelestialTreeNode<T>* parent)
{

	this->parent = parent;

}

template <class T>
CelestialTreeNode<T>* CelestialTreeNode<T>::GetParent()
{

	return parent;

}

template <class T>
void CelestialTreeNode<T>::SetNodeObject(T object)
{

	this->object = object;

}

template <class T>
T CelestialTreeNode<T>::GetNodeObject()
{

	return object;

}

template <class T>
void CelestialTreeNode<T>::RemoveLeafs()
{

	if (leafs != nullptr)
	{

		leafs->Reset();
		delete leafs;
		leafs = nullptr;

	}
}

template <class T>
void CelestialTreeNode<T>::KillNode()
{

	if (leafs != nullptr)
	{

		CelestialListNode<T>* node = leafs->GetFirstNode();

		while (node != nullptr)
		{

			node->GetNodeObject()->KillNode();
			node = node->GetNext();

		}

		leafs->KillList();

	}
}

template <class T>
CelestialTreeNode<T>::~CelestialTreeNode()
{

	if (leafs != nullptr)
	{

		leafs->KillList();
		delete leafs;

	}
}
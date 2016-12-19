#pragma once
#include "CelestialTreeNode.h"

namespace CrossHandlers
{

	template <class T>

	///<summary>A n-leaf tree container</summary>
	class CelestialTree
	{

		private:
			///<summary>The root node</summary>
			CelestialTreeNode<T>* root;
		public:
			///<param val='rootObj'>[i]A pointer to the object to use as a root</param>
			CelestialTree(CelestialTreeNode<T>* rootObj);
			///<summary>Gets the root of the tree</summary>
			///<returns>A pointer to the rootnode of the tree</returns>
			CelestialTreeNode<T>* GetRoot();

			void ResetRootNode();
			///<summary>Deletes all branches in the tree while leaving the objects alone</summary>
			void ResetTree();
			///<summary>Deletes all branches in the tree and  all the objects within</summary>
			void KillTree();
			~CelestialTree();

	};
}

using namespace CrossHandlers;

template <class T>
CelestialTree<T>::CelestialTree(CelestialTreeNode<T>* rootObj)
{

	root = rootObj;

}

template <class T>
CelestialTreeNode<T>* CelestialTree<T>::GetRoot()
{

	return root;

}

template <class T>
void CelestialTree<T>::ResetTree()
{

	delete root;

}

template <class T>
void CelestialTree<T>::KillTree()
{

	root->KillNode();
	delete root;

}

template <class T>
void CelestialTree<T>::ResetRootNode()
{

	root = nullptr;

}

template <class T>
CelestialTree<T>::~CelestialTree()
{

	if (root != nullptr)
	{

		delete root;

	}
}
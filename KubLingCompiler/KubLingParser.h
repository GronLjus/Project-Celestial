#pragma once
#include "Keyword.h"

namespace Logic
{

	///<summary>The module that parses a list of tokens into a tree</summary>
	class KubLingParser
	{
		
		public:
			///<param val='keywords'>[in]A list of keywords the language uses</param>
			///<param val='keyWordsSize'>[in]The amount of keywords</param>
			///<param val='flowOps'>[in]A list of flow operators the language uses</param>
			///<param val='flowSize'>[in]The amount of flowcontrollers</param>
			///<param val='operators'>[in]A list of operators the language uses</param>
			///<param val='operatorSize'>[in]The amount of operators</param>
			KubLingParser(Keyword* keywords, int keyWordsSize, FlowController* flowOps, int flowSize);
			///<summary>Parse a list of tokens and create a tree</summary>
			///<param val='tokens'>[in]A pointer to the list of tokens to parse</param>
			///<param val='err'>[out]Any errors that happened</param>
			///<returns>A pointer to the parsetree</returns>
			CrossHandlers::CelestialList<CrossHandlers::CelestialTree<Token>*>* ParseTokens(CrossHandlers::CelestialList<Token>* tokens, CompileError &err);
			~KubLingParser();

		private:
			///<summary>Analyze a node and create a tree</summary>
			///<param val='tokenNode'>[tokenNode]A pointer to the node to analyze</param>
			///<param val='err'>[out]Any errors that happened</param>
			///<param val='parent'>A pointer to the potential parentnode</param>
			int addChildrenTooTree(CelestialListNode<Token>* &tokenNode, CompileError &err, CelestialTreeNode<Token>* parent);
		
			///<summary>Reorders a tree accoring to the priority-values of interior keywords</summary>
			///<param val='treeRoot'>[in]A pointer to the root of the tree</param>
			///<param val='err'>[out]Any errors that happened</param>
			///<returns>A reorded tree</returns>
			CrossHandlers::CelestialTreeNode<Token>* orderTree(CrossHandlers::CelestialTreeNode<Token>* treeRoot, CompileError &err);
			
			CrossHandlers::CelestialTreeNode<Token>* handleFlowNode(CelestialListNode<Token>* &tokenNode, CompileError &err, CelestialTreeNode<Token>* parent);
			///<summary>Remove brackets from a tree</summary>
			///<param val='node'>[in]A pointer to the treeroot to prune</param>
			void pruneBrackets(CrossHandlers::CelestialTreeNode<Token>* node);
			///<summary>The list of keywords to check for</summary>
			Keyword* keywords;
			///<summary>The amount of keywords</summary>
			int keyWordsSize;
			///<summary>A list of flow operators the language uses</summary>
			FlowController* flowOps;
			///<summary>The amount of flowcontrollers</summary>
			int flowSize;

	};
}
#pragma once
#include "Keyword.h"

namespace Logic
{

	///<summary>The module that analyses and typechecks parse trees, compiles it into abstract syntax trees and creates a symboltable</summary>
	class KubLingAnalyzer
	{

		public:
			///<param val='keywords'>[in]A list of keywords the language uses</param>
			///<param val='keyWordsSize'>[in]The amount of keywords</param>
			///<param val='operators'>[in]A list of operators the language uses</param>
			///<param val='operatorSize'>[in]The amount of operators</param>
			///<param val='flowOps'>[in]A list of flow operators the language uses</param>
			///<param val='flowSize'>[in]The amount of flowcontrollers</param>
			KubLingAnalyzer(Keyword* keywords, int keyWordsSize, Operator* operators, int operatorSize, FlowController* flowOps, int flowSize, RuntimeParameter* rParams, int rPSize);
			///<summary>Anylyses a list of parsetree, outputs a list of abstract syntax tree and creates the symboltable</summary>
			///<param val='parseTrees'>[in]A pointer to a list of parsetrees to analyse</param>
			///<param val='err'>[out]Any errors that happened</param>
			///<returns>The new abstract syntax tree</returns>
			AST AnalyzeTrees(CrossHandlers::CelestialList<CrossHandlers::CelestialTree<Token>*>* parseTrees, CompileError &err);
			~KubLingAnalyzer();

		private:
			///<summary>The list of keywords to check for</summary>
			Keyword* keywords;
			///<summary>The amount of keywords</summary>
			int keyWordsSize;
			///<summary>A list of operators the language uses</summary>
			Operator* operators;
			///<summary>The amount of operators</summary>
			int operatorSize;
			///<summary>A list of flow operators the language uses</summary>
			FlowController* flowOps;
			///<summary>The amount of flowcontrollers</summary>
			int flowSize;
			RuntimeParameter* rParams;
			int rPSize;

			///<summary>The amount of symbols in the symboltable</summary>
			int symbols;
			///<summary>If we're in the initialization block</summary>
			bool inInit;
			///<summary>If the analyzedtree is empty</summary>
			bool empty;
			///<summary>A pointer to the list of symbols</summary>
			CelestialSlicedList<symbol>* symbolTable;

			struct operatorParams
			{

				int param;
				int opVar;
				bool hasEnum;

				VarType* sigType;
				ParamType* sigSource;
				int* sigAddrr;

			};

			CelestialTreeNode<syntax>* analyzeSyntaxNode(CompileError &err, CelestialTreeNode<syntax>* synTree);
			operatorParams analyzeOperatorParams(CompileError &err, OperatorTypes op, CelestialList<CelestialTreeNode<Token>*>* leafs, CelestialTreeNode<syntax>* newtree);
			operatorParams getOperatorVariation(CompileError &err, unsigned int line, OperatorTypes op, operatorParams ops);
			operatorParams expandTree(CompileError &err, unsigned int line, OperatorTypes op, operatorParams ops, CelestialTreeNode<syntax>* newtreeNode);
			operatorParams getTypeCheck(CompileError &err, unsigned int line, OperatorTypes op, operatorParams ops);
			operatorParams makeTreeConst(CompileError &err, OperatorTypes op, operatorParams ops, CelestialTreeNode<syntax>* newtree);
			operatorParams checkVarInit(CompileError &err, unsigned int line, OperatorTypes op, operatorParams ops);

			void finalizeNode(CompileError &err, unsigned int line, OperatorTypes op, operatorParams ops, CelestialTreeNode<syntax>* node, VarType &returnType, ParamType& returnSource);

			///<summary>Analyse a operatornode, typecheck and validate a node while also creating a symboltable</summary>
			///<param val='node'>[in]A pointer to the parsenode to analyze</param>
			///<param val='parent'>[in]A node to the last analyzed node</param>
			///<param val='err'>[out]Any errors that happened</param>
			///<param val='returnType'>[out]The type of value the analyzed node returns</param>
			CrossHandlers::CelestialTreeNode<syntax>* analyzeOperator(CrossHandlers::CelestialTreeNode<Token>* node, CrossHandlers::CelestialTreeNode<syntax>* parent, CompileError &err, VarType &returnType, ParamType& returnSource);

			///<summary>Analyse a flownode, typecheck and validate a node while also creating a symboltable</summary>
			///<param val='node'>[in]A pointer to the parsenode to analyze</param>
			///<param val='parent'>[in]A node to the last analyzed node</param>
			///<param val='err'>[out]Any errors that happened</param>
			CrossHandlers::CelestialTreeNode<syntax>* analyzeFlow(CrossHandlers::CelestialTreeNode<Token>* node, CrossHandlers::CelestialTreeNode<syntax>* parent, CompileError &err);

	};
}
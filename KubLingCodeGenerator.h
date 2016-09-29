#pragma once
#include "Keyword.h"

namespace Logic
{
	///<summary>The module that taken an abstract syntax tree and compiles it into code with the help of a symboltable</summary>
	class KubLingCodeGenerator
	{

		private:

			///<summary>The list of keywords to check for</summary>
			Keyword* keywords;
			///<summary>The amount of keywords</summary>
			int keyWordsSize;
			///<summary>The amount of symbols in the symboltable</summary>
			int symbols;

			///<summary>Generate code from a node in the tree</summary>
			///<param val='ast'>[in]A pointer to the treenode</param>
			///<param val='symbolTable'>[in]A pointer to a list of symbols</param>
			///<param val='codeCount'>[in/out]The total number of commands</param>
			CrossHandlers::CelestialList<CrossHandlers::CelestialList<unsigned char>*>* generateCode(CrossHandlers::CelestialTreeNode<syntax>* ast, CelestialSlicedList<symbol>* symbolTable, unsigned int &codeCount);
			int KubLingCodeGenerator::handleVariable(CrossHandlers::CelestialSlicedList<symbol>* symbolTable, int varAdr, CrossHandlers::CelestialList<int>* varClear, CrossHandlers::CelestialList<CrossHandlers::CelestialList<unsigned char>*>* commands, unsigned int &codeCount);
			
			///<summary>The last variable</summary>
			int var;
			///<summary>A list of unused variables</summary>
			CelestialList<int>* holes;
			///<summary>The variable to place as the returnvalue</summary>
			int opRetVar;
			///<summary>The return string from an operator</summary>
			std::string opRetConst;

		public:
			///<param val='keywords'>[in]A list of keywords the language uses</param>
			///<param val='keyWordsSize'>[in]The amount of keywords</param>
			KubLingCodeGenerator(Keyword* keywords, int keyWordsSize);
			///<summary>Outputs bytecode from an abstract syntax tree and a symbol table</summary>
			///<param val='ast'>[in]The abstract syntax tree and symboltable</param>
			///<returns>The bytecode generated</returns>
			CrossHandlers::CelestialList<CrossHandlers::CelestialList<unsigned char>*>* GenerateCode(AST ast);
			~KubLingCodeGenerator();

	};
}
#pragma once
#include "KubLingCompiled.h"
#include "KubLingSource.h"
#include "KubLingLexer.h"
#include "KubLingParser.h"
#include "KubLingAnalyzer.h"
#include "KubLingCodeGenerator.h"
#include "TextContainer.h"

namespace Logic
{

	///<summary>This class compiles a source file into bytecode</summary>
	class KubLingCompiler
	{

	private:
			///<summary>The textbox used for printing info</summary>
			CrossHandlers::TextContainer* outText;

			///<summary>A pointer to the parser module</summary>
			KubLingParser* parser;
			///<summary>A pointer to the lexermodule</summary>
			KubLingLexer* lexer;
			///<summary>A pointer to the analyzer module</summary>
			KubLingAnalyzer* analyzer;
			///<summary>A pointer to the codegenerator</summary>
			KubLingCodeGenerator* generator;

			///<summary>The list of keywords in the language</summary>
			Keyword* keywords;
			///<summary>The total amount of keywords</summary>
			int keyWordsSize; 
			///<summary>The list of operators in the language</summary>
			Operator* operators;
			///<summary>A list of flow operators the language uses</summary>
			FlowController* flowOps;

			RuntimeParameter* rParams;
			
			unsigned int getParam(std::string param) const;
			void setupArithmeticOps();
		public:
			KubLingCompiler();
			void Init(CrossHandlers::TextContainer* outText);
			///<summary>Compiles source code inte bytecode</summary>
			///<param val='source'>[in]A pointer to source to compile from</param>
			///<param val='error'>[out]Any errors in the compilation</param>
			///<returns>A pointer to the compiled bytecode</returns>
			Resources::KubLingCompiled* CompileSource(Resources::KubLingSource* source, CompileError &error);
			~KubLingCompiler();

	};
}
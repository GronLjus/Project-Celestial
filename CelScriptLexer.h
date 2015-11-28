#pragma once
#include "Keyword.h"

namespace Logic
{
	///<summary>The modules that lexes a line into tokens</summary>
	class CelScriptLexer
	{

	public:

			///<param val='keywords'>[in]A list of keywords the language uses</param>
			///<param val='keyWordsSize'>[in]The amount of keywords</param>
			///<param val='operators'>[in]A list of operators the language uses</param>
			///<param val='operatorSize'>[in]The amount of operators</param>
			CelScriptLexer(Keyword* keywords, int keyWordsSize, Operator* operators, int operatorSize);
			///<summary>Create a list of tokens from a text line</summary>
			///<param val='line'>[in]The line to lex</param>
			///<param val='lNumber'>[in]The number of the line</param>
			///<param val='err'>[out]Any errors that happened</param>
			///<returns>A list of tokens</returns>
			CrossHandlers::CelestialDoubleList<Token>* TokenizeLine(std::string line, int lNumber, CompileError &err);
			~CelScriptLexer();

		private:
			///<summary>The list of keywords to check for</summary>
			Keyword* keywords;
			///<summary>The amount of keywords</summary>
			int keyWordsSize;
			///<summary>A list of operators the language uses</summary>
			Operator* operators;
			///<summary>The amount of operators</summary>
			int operatorSize;
			///<summary>The array of acceptable characters that making op a number</summary>
			char* numbers;

	};
}
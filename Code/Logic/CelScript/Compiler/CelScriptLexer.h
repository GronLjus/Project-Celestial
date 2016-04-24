#pragma once
#include "../KeyWords/Keyword.h"

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
			///<summary>The atomic token</summary>
			struct shortHand
			{

				CelestialDoubleListNode<Token>* node;
				unsigned char bracketLevel;

			};

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

			CrossHandlers::CelestialList<std::string>* extractWords(std::string line);
			Token translateWord(std::string word, CompileError &err, int lNumber);
			CrossHandlers::CelestialDoubleList<Token>* tokenizeWord(std::string word, int lNumber, CompileError &err);
			CrossHandlers::CelestialDoubleList<Token>* tokenizeWords(CrossHandlers::CelestialList<std::string>* words);

			void moveParamsOfOp(CrossHandlers::CelestialDoubleListNode<Token>* shortHandNode, unsigned char totLeftParams);
			void flipParamsAroundOp(CrossHandlers::CelestialDoubleListNode<Token>* shortHandNode, unsigned char totLeftParams, unsigned char totRightParams);
			unsigned int getShortHand(std::string word);
			unsigned int getBracketLevels(CrossHandlers::CelestialDoubleList<Token>* tokens);
			CrossHandlers::CelestialDoubleListNode<Token>* extractRights(CrossHandlers::CelestialDoubleListNode<Token>* token, CrossHandlers::CelestialDoubleList<Token>* tokens, unsigned int shorthand, int lNumber, CompileError &err);
			CrossHandlers::CelestialDoubleListNode<Token>* breakOutShortHand(CrossHandlers::CelestialDoubleListNode<Token>* token, CrossHandlers::CelestialDoubleList<Token>* tokens, unsigned int shorthand, int lNumber, CompileError &err);

			void breakOutBrackets(CrossHandlers::CelestialDoubleListNode<Token>* token, int lNumber);

			void extractOps(CrossHandlers::CelestialDoubleList<Token>* tokens, int lNumber, CompileError &err);

	};
}
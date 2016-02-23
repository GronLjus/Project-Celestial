#include "stdafx.h"
#include "CelScriptLexer.h"
#include "CelestialDoubleList.h"

using namespace Logic;
using namespace CrossHandlers;
using namespace std;

CelScriptLexer::CelScriptLexer(Keyword* keywords, int keyWordsSize, Operator* operators, int operatorSize)
{

	this->keywords = keywords;
	this->keyWordsSize = keyWordsSize;
	this->operators = operators;
	this->operatorSize = operatorSize;
	numbers = new char[12];
	numbers[0] = '0';
	numbers[1] = '1';
	numbers[2] = '2';
	numbers[3] = '3';
	numbers[4] = '4';
	numbers[5] = '5';
	numbers[6] = '6';
	numbers[7] = '7';
	numbers[8] = '8';
	numbers[9] = '9';
	numbers[10] = '-';
	numbers[11] = '.';

}

CelestialDoubleList<Token>* CelScriptLexer::TokenizeLine(string line, int lNumber, CompileError &err)
{
	CelestialDoubleList<Token>* returnVal = nullptr;

	if (line != "")
	{

		returnVal = new CelestialDoubleList<Token>();
		string tempWord = "";
		bool isOpen = false;
		bool isFlow = false;

		CelestialList<CelestialDoubleListNode<Token>*>* replace = new CelestialList<CelestialDoubleListNode<Token>*>();
		CelestialList<Operator>* replaceOp = new CelestialList<Operator>();
		bool breakSearch = false;
		char bracketLevel = 0;

		for (int i = 0; i < line.size() && !breakSearch; i++)
		{

			Token toAdd;
			toAdd.line = lNumber;
			toAdd.val = "";

			if (line[i] != '\'' && isOpen && i == line.size() - 1)
			{

				err.errorType = ScriptError_UNCLOSED;
				err.line = lNumber;
				breakSearch = true;

			}

			breakSearch = line[i] == '\\';
			bool isOp = false;
			bool isSep = line[i] == ';' || line[i] == ',';
			bool isBracket = line[i] == '(' || line[i] == ')';
			bool isBod = line[i] == '{' || line[i] == '}';
			bool isQuotes = false;

			if (!breakSearch)
			{

				if (line[i] == '\'')
				{

					isQuotes = isOpen;
					isOpen = !isOpen;

				}
				else if (isSep)
				{

					if (returnVal->GetLastNode()->GetNodeObject().type != TokenType_SEPERATOR)
					{

						toAdd.type = TokenType_SEPERATOR;
						toAdd.val = line[i];


					}
				}
				else if (isBracket)
				{

					toAdd.type = TokenType_BRACKET;
					toAdd.val = line[i];

					if (line[i] == '(')
					{

						toAdd.val += to_string(bracketLevel);
						bracketLevel++;

					}
					else
					{

						bracketLevel--;
						breakSearch = bracketLevel < 0;
						toAdd.val += to_string(bracketLevel);

					}
				}
				else if (isBod)
				{

					toAdd.type = TokenType_BODY;
					toAdd.val = line[i];

				}
				else
				{

					for (int k = 0; k < keyWordsSize && !isOp && !isOpen; k++)
					{

						isOp = keywords[k].keyword.length() == 1 && keywords[k].keyword[0] == line[i] && keywords[k].type == KeyWordType_OPERATOR;

						if (isOp)
						{

							toAdd.type = TokenType_KEYWORD;
							toAdd.val = keywords[k].keyword;

						}
					}

					if ((line[i] != ' ' || isOpen) && !isOp)
					{

						tempWord += line[i];

					}
				}
			}

			if ((line[i] == ' ' || i == line.size() - 1 || breakSearch || isBod || isOp || isSep || isBracket || isQuotes) 
				&& !isOpen && tempWord.length() > 0)
			{

				bool found = false;
				string val = tempWord;
				TokenType type = TokenType_NA;

				for (int k = 0; k < keyWordsSize && !found; k++)
				{

					found = keywords[k].keyword == tempWord;

					if (found)
					{

						val = keywords[k].keyword;
						type = TokenType_KEYWORD;
						isOp = keywords[k].type == KeyWordType_OPERATOR;

						if (keywords[k].type == KeyWordType_FLOW)
						{

							type = TokenType_FLOW;

							if (isFlow)
							{

								err.errorType = ScriptError_UNCLOSED;
								err.line = lNumber;

							}

							isFlow = true;

						}
					}
				}

				if (!found)
				{

					bool isNumber = true;
					bool isParam = tempWord[0] == '?';
					bool isFloat = false;

					for (int k = isParam ? 1 : 0; k < tempWord.length() && isNumber; k++)
					{

						isNumber = false;
						isFloat = tempWord[k] == numbers[11] || isFloat;

						if (tempWord[k] == '?')
						{

							err.errorType = ScriptError_PARAMWRONGPLACE;
							err.line = lNumber;

						}

						for (int j = 0; j < (isParam ? 10 : 12) && !isNumber && !isParam; j++)
						{

							isNumber = tempWord[k] == numbers[j];

						}
					}

					if (isQuotes || isNumber)
					{

						type = TokenType_WORD;

						if (isNumber)
						{

							val = isFloat ? 
								'f' + val :
								'n' + val;

						}
						else
						{

							val = 's' + val;

						}
					}
					else
					{

						type = isParam ? TokenType_PARAM : TokenType_VAR;

					}
				}

				tempWord = "";
				Token newToken;
				newToken.type = type;
				newToken.val = val;
				newToken.line = lNumber;
				returnVal->AddElement(newToken);

				if (type == TokenType_FLOW)
				{

					Token condToken;
					condToken.type = TokenType_COND;
					condToken.val = "conditions:";
					condToken.line = lNumber;
					returnVal->AddElement(condToken);

				}
			}

			if ((isOp && toAdd.val.length() > 0) || isSep || isBracket || isBod)
			{

				returnVal->AddElement(toAdd);

			}

			if (isOp)
			{

				int sh = -1;

				for (int k = 0; k < operatorSize && sh == -1; k++)
				{

					for (int j = 0; j < operators[k].shortHandsAmounts && sh == -1; j++)
					{

						sh = strcmp(operators[k].shortHands[j].c_str(), returnVal->GetLastNode()->GetNodeObject().val.c_str()) == 0 ? j : -1;

						if (sh != -1)
						{

							replaceOp->AddElement(operators[k]);
							replace->AddElement(returnVal->GetLastNode());

						}
					}
				}
			}
		}

		if (isOpen || bracketLevel != 0)
		{

			err.errorType = ScriptError_UNCLOSED;
			err.line = lNumber;

		}
		else
		{

			CelestialListNode<CelestialDoubleListNode<Token>*>* repl = replace->GetFirstNode();
			CelestialListNode<Operator>* repOp = replaceOp->GetFirstNode();

			while (repl != nullptr)
			{

				CelestialDoubleListNode<Token>* toRep = repl->GetNodeObject();
				Operator op = repOp->GetNodeObject();
				int sh = -1;

				for (int i = 0; i < repOp->GetNodeObject().shortHandsAmounts && sh == -1; i++)
				{

					sh = strcmp(op.shortHands[i].c_str(), repl->GetNodeObject()->GetNodeObject().val.c_str()) == 0 ? i : -1;

				}

				Token tok = toRep->GetNodeObject();
				tok.val = op.keyword;
				toRep->SetNodeObject(tok);
				CelestialDoubleListNode<Token>* leftBrws = toRep->GetPrev();
				CelestialDoubleListNode<Token>* rightStart = toRep->GetNext();
				CelestialDoubleListNode<Token>* rightEnd = toRep->GetNext();

				if (op.shortFlipParams[sh])
				{

					for (int i = 0; i < op.rightParams[sh]; i++)
					{

						if (rightEnd->GetNodeObject().val[0] == '(')
						{

							int brack = -1;
							int target = stoi(string(&(rightEnd->GetNodeObject().val.c_str()[1])));

							while (rightEnd != nullptr && brack != target)
							{

								rightEnd = rightEnd->GetNext();

								if (rightEnd != nullptr && rightEnd->GetNodeObject().val[0] == ')')
								{

									brack = stoi(string(&(rightEnd->GetNodeObject().val.c_str()[1])));

								}
							}
						}

						if (rightEnd != nullptr)
						{

							rightEnd = rightEnd->GetNext();

						}
					}
				}

				for (int i = 0; i < op.leftParams[sh]; i++)
				{

					if (leftBrws->GetNodeObject().val[0] == ')')
					{

						int brack = -1;
						CelestialDoubleListNode<Token>* brwsr = toRep;
						int target = stoi(string(&(leftBrws->GetNodeObject().val.c_str()[1])));

						while (brwsr->GetPrev() != nullptr && brack != target)
						{

							Token tokenHere = brwsr->GetNodeObject();
							brwsr->SetNodeObject(brwsr->GetPrev()->GetNodeObject());
							brwsr->GetPrev()->SetNodeObject(tokenHere);
							brwsr = brwsr->GetPrev();

							if (brwsr != nullptr && brwsr->GetNodeObject().val[0] == '(')
							{

								brack = stoi(string(&(brwsr->GetNodeObject().val.c_str()[1])));

							}
						}

						leftBrws = brwsr;

					}
					else if (leftBrws != nullptr)
					{

						Token tokenHere = toRep->GetNodeObject();
						toRep->SetNodeObject(leftBrws->GetNodeObject());
						leftBrws->SetNodeObject(tokenHere);

					}

					if (leftBrws != nullptr)
					{

						toRep = leftBrws;
						leftBrws = toRep->GetPrev();

					}
				}

				if (op.shortFlipParams[sh])
				{

					CelestialDoubleListNode<Token>* endRepl = toRep;
					CelestialDoubleListNode<Token>* brwsr = rightStart;

					while (brwsr != rightEnd)
					{

						CelestialDoubleListNode<Token>* brwsr2 = brwsr;

						while (brwsr2->GetPrev() != endRepl)
						{

							Token tokenHere = brwsr2->GetNodeObject();
							brwsr2->SetNodeObject(brwsr2->GetPrev()->GetNodeObject());
							brwsr2->GetPrev()->SetNodeObject(tokenHere);
							brwsr2 = brwsr2->GetPrev();

						}

						endRepl = endRepl->GetNext();
						brwsr = brwsr->GetNext();

					}
				}
				repl = repl->GetNext();
				repOp = repOp->GetNext();

			}
		}

		delete replace;
		delete replaceOp;

		if (returnVal->GetCount() > 0 && returnVal->GetLastNode()->GetNodeObject().type != TokenType_SEPERATOR && returnVal->GetLastNode()->GetNodeObject().type != TokenType_BODY && returnVal->GetLastNode()->GetNodeObject().type != TokenType_COND)
		{

			Token newToken;
			newToken.type = TokenType_SEPERATOR;
			newToken.val = ';';
			newToken.line = lNumber;
			returnVal->AddElement(newToken);

		}

		if (isFlow)
		{

			Token condToken;
			condToken.type = TokenType_COND;
			condToken.val = ":conditions";
			condToken.line = lNumber;
			returnVal->AddElement(condToken);

		}
	}

	return returnVal;

}

CelScriptLexer::~CelScriptLexer()
{

	delete[] numbers;

}
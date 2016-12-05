#include "stdafx.h"
#include "KubLingLexer.h"
#include "CelestialDoubleList.h"

using namespace Logic;
using namespace CrossHandlers;
using namespace std;

bool hasOffset;

KubLingLexer::KubLingLexer(Keyword* keywords, int keyWordsSize, Operator* operators, int operatorSize)
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

CelestialList<string>* KubLingLexer::extractWords(string line)
{

	CelestialList<std::string>* stack = new CelestialList<std::string>();
	string tempWord = "";
	bool isQoutes = false;
	unsigned char bracketLevel = 0;

	for (unsigned int i = 0; i < line.length(); i++)
	{

		isQoutes = line[i] == '\'' ? !isQoutes : isQoutes;
		
		if (line[i] == '(' || line[i] == '[')
		{

			bracketLevel++;

		}
		else if (line[i] == ')' || line[i] == ']')
		{

			if (bracketLevel != 0)
			{

				bracketLevel--;

			}
		}

		if (!isQoutes && line[i] == ' ' && bracketLevel == 0)
		{

			stack->AddElement(tempWord);
			tempWord = "";

		}
		else
		{

			tempWord += line[i];

		}
	}

	if (tempWord != "")
	{

		stack->AddElement(tempWord);

	}

	return stack;

}

Token KubLingLexer::translateWord(string word, CompileError &err, int lNumber)
{

	bool found = false;
	TokenType type = TokenType_NA;
	string val = word;

	for (int k = 0; k < keyWordsSize && !found; k++)
	{

		found = keywords[k].keyword == word;

		if (found)
		{

			if (keywords[k].type == KeyWordType_FLOW)
			{

				type = TokenType_FLOW;

			}
			else
			{

				type = TokenType_KEYWORD;

			}
		}
	}

	if (!found)
	{

		bool isParam = word[0] == '?';
		bool isFloat = false;

		bool isOffset = false;

		//Check if bracket
		bool hasBracket = false;
		bool isBracket = word[0] == '(';
		unsigned char brLvl = 0;

		for (int k = 0; k < word.length() && isBracket; k++)
		{

			hasBracket = true;

			if (word[k] == ')')
			{

				brLvl--;

			}
			else if (word[k] == '(')
			{

				brLvl++;

			}
			else if (brLvl == 0)
			{

				isBracket = false;

			}
		}

		//Check if offset

		if (!isBracket)
		{

			isOffset = !getShortHand(word[0]);

			if (isOffset)
			{

				unsigned int firstPlace = word.find('[');
				isOffset = firstPlace != std::string::npos;
				unsigned char offLvl = 0;

				for (unsigned int k = firstPlace; k < word.length(); k++)
				{

					if (word[k] == '[')
					{

						hasBracket = true;
						offLvl++;

					}
					else if (word[k] == ']')
					{

						offLvl--;

					}
					else if (offLvl == 0)
					{

						isOffset = false;
					}

				}
			}
		}


		bool isNumber = !hasBracket;

		for (int k = isParam ? 1 : 0; k < word.length() && isNumber; k++)
		{

			isNumber = false;
			isFloat = word[k] == numbers[11] || isFloat;

			if (word[k] == '?')
			{

				err.errorType = ScriptError_PARAMWRONGPLACE;

			}

			for (int j = 0; j < (isParam ? 10 : 12) && !isNumber && !isParam; j++)
			{

				isNumber = word[k] == numbers[j];

			}
		}

		unsigned char bracketLevel = 0;

		if (word[0] == '\'' || isNumber)
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

				val = val.substr(1, val.length() - 2);
				val = 's' + val;

			}
		}
		else if (isBracket)
		{

			type = TokenType_BRACKET;

		}
		else if (isOffset)
		{

			type = TokenType_OFFSET;

		}
		else if (hasBracket)
		{

			type = TokenType_SEQ;

		}
		else
		{

			type = isParam ? TokenType_PARAM : TokenType_VAR;

		}
	}

	Token newToken;
	newToken.type = type;
	newToken.val = val;
	newToken.line = lNumber;
	return newToken;

}

CelestialDoubleList<Token>* KubLingLexer::tokenizeWord(string word, int lNumber, CompileError &err)
{

	bool breakSearch = false;
	bool isQoutes = false;
	CelestialDoubleList<Token>* returnVal = new CelestialDoubleList<Token>();

	CelestialStack<char>* lastBracket = new CelestialStack<char>(false);
	lastBracket->PushElement(' ');

	string temWord = "";

	for (unsigned int i = 0; i < word.length() && !breakSearch; i++)
	{

		isQoutes = word[i] == '\'' ? !isQoutes : isQoutes;

		if (!isQoutes)
		{

			breakSearch = err.errorType != ScriptError_OK || i > 0 && word[i] == '\\' && word[i - 1] == '\\';

			if (breakSearch)
			{
			
				return returnVal;

			}

			bool isOp = false;
			bool isSep = word[i] == ';' || word[i] == ',';
			bool isBracket = word[i] == '(' || word[i] == ')';
			bool isBod = word[i] == '{' || word[i] == '}';
			bool isOffset = word[i] == '[' || word[i] == ']';

			if (isSep)
			{

				if (returnVal->GetLastNode() == nullptr || returnVal->GetLastNode()->GetNodeObject().type != TokenType_SEPERATOR)
				{

					Token lexT = Token();
					lexT.line = lNumber;
					lexT.type = TokenType_SEPERATOR;
					lexT.val = word[i];
					returnVal->PushElement(lexT);


				}
			}
			else if (isBracket || isOffset)
			{

				if (word[i] == '(' || word[i] == '[')
				{

					lastBracket->PushElement(word[i]);

				}
				else
				{

					char lastBr = lastBracket->PeekElement() != ' ' ? lastBracket->PopElement() : ' ';
					bool lastMatch = lastBr != ' ' && (
						(lastBr == '(' && word[i] == ')') || 
						(lastBr == '[' && word[i] == ']'));

					if (!lastMatch)
					{

						err.errorType = ScriptError_UNCLOSED;
						err.line = lNumber;

					}
				}

				temWord += word[i];

			}
			else if (isBod)
			{

				Token lexT = Token();
				lexT.line = lNumber;
				lexT.type = TokenType_BODY;
				lexT.val = word[i];
				returnVal->PushElement(lexT);

			}
			else
			{

				temWord += word[i];

			}
		}
		else
		{

			temWord += word[i];

		}
	}

	if (isQoutes || lastBracket->PopElement() != ' ')
	{

		err.errorType = ScriptError_UNCLOSED;
		err.line = lNumber;
		
	}
	else if (temWord != "")
	{

		Token lt = translateWord(temWord, err, lNumber);

		if (err.errorType != ScriptError_OK)
		{

			return returnVal;

		}

		if (lt.type == TokenType_WORD)
		{

			char first = lt.val[0];
			lt.val = lt.val.substr(1);

			if (first == 's')
			{

				lt.val = ('\'' + lt.val + '\'');

			}
		}

		lt.type = lt.type == TokenType_PARAM ||
			lt.type == TokenType_VAR ||
			lt.type == TokenType_WORD ||
			lt.type == TokenType_SEQ ? TokenType_WORD : lt.type;

		lt.line = lNumber;
		returnVal->PushElement(lt); 

	}

	delete lastBracket;
	return returnVal;

}

void KubLingLexer::moveParamsOfOp(CelestialDoubleListNode<Token>* shortHandNode, unsigned char totLeftParams)
{

	unsigned char leftParams = 0;
	CelestialDoubleListNode<Token>* leftEnd = shortHandNode->GetPrev();
	CelestialDoubleListNode<Token>* leftStart = leftEnd;
	bool leftStop = leftEnd == nullptr;

	while ((leftParams < totLeftParams) && (!leftStop))
	{

		CelestialDoubleListNode<Token>* right = shortHandNode->GetNext();
		CelestialDoubleListNode<Token>* left = leftStart->GetPrev();

		shortHandNode->SetNext(leftStart);
		leftStart->SetPrev(shortHandNode);

		leftEnd->SetNext(right);
		shortHandNode->SetPrev(left);

		if (right != nullptr)
		{

			right->SetPrev(leftEnd);

		}

		if (left != nullptr)
		{

			left->SetNext(shortHandNode);

		}

		leftParams++;
		leftEnd = shortHandNode->GetPrev();
		leftStart = leftEnd;

		leftStop = leftEnd == nullptr || !(leftEnd->GetPrev() != nullptr && (
			leftEnd->GetPrev()->GetNodeObject().type == TokenType_PARAM ||
			leftEnd->GetPrev()->GetNodeObject().type == TokenType_VAR ||
			leftEnd->GetPrev()->GetNodeObject().type == TokenType_WORD ||
			leftEnd->GetPrev()->GetNodeObject().type == TokenType_UNARY ||
			leftEnd->GetPrev()->GetNodeObject().type == TokenType_KEYWORD));


	}
}

void KubLingLexer::flipParamsAroundOp(CelestialDoubleListNode<Token>* shortHandNode, unsigned char totLeftParams, unsigned char totRightParams)
{

	unsigned char leftParams = 0;
	unsigned char rightParams = 0;
	CelestialDoubleListNode<Token>* leftStart = shortHandNode;
	CelestialDoubleListNode<Token>* leftEnd = shortHandNode;
	CelestialDoubleListNode<Token>* rightStart = shortHandNode;
	CelestialDoubleListNode<Token>* rightEnd = shortHandNode;
	bool leftStop = leftStart->GetPrev() == nullptr;
	bool rightStop = rightEnd->GetNext() == nullptr;

	while ((leftParams < totLeftParams || rightParams < totRightParams) && (!leftStop || !rightStop))
	{

		leftStart = leftEnd->GetPrev();
		rightEnd = rightStart->GetNext();

		//Sort out
		if (leftStop && rightParams < totRightParams)
		{

			CelestialDoubleListNode<Token>* tempRight = rightEnd->GetNext();
			rightEnd->SetNext(leftEnd);
			leftEnd->SetPrev(rightEnd);

			rightStart->GetNext()->SetPrev(leftStart);

			if (leftStart != nullptr)
			{

				leftStart->SetNext(rightStart->GetNext());

			}

			leftEnd = rightStart->GetNext();
			rightStart->SetNext(tempRight);

			if (tempRight != nullptr)
			{

				tempRight->SetPrev(rightStart);
				rightStart = tempRight;

			}

			rightParams++;

		}

		if (rightStart->GetNext() == nullptr && leftParams < totLeftParams)
		{

			CelestialDoubleListNode<Token>* tempLeft = leftStart->GetPrev();
			leftStart->SetPrev(rightStart);
			rightStart->SetNext(leftStart);

			leftEnd->GetPrev()->SetNext(rightEnd);

			if (rightEnd != nullptr)
			{

				rightEnd->SetPrev(leftEnd->GetPrev());

			}

			rightStart = leftEnd->GetPrev();
			leftEnd->SetPrev(tempLeft);

			if (tempLeft != nullptr)
			{

				tempLeft->SetNext(leftEnd);
				leftEnd = tempLeft;

			}

			leftParams++;


		}

		if (leftEnd->GetPrev() != nullptr && rightStart->GetNext() != nullptr && leftParams < totLeftParams && rightParams < totRightParams)//Flip the two params
		{

			CelestialDoubleListNode<Token>* tempFarLeft = leftStart->GetPrev();

			rightStart->GetNext()->SetPrev(tempFarLeft);
			leftStart->SetPrev(rightStart);

			if (tempFarLeft != nullptr)
			{

				tempFarLeft->SetNext(rightStart->GetNext());

			}

			CelestialDoubleListNode<Token>* tempFarRight = rightEnd->GetNext();

			leftEnd->GetPrev()->SetNext(tempFarRight);
			rightEnd->SetNext(leftEnd);

			if (tempFarRight != nullptr)
			{

				tempFarRight->SetPrev(leftEnd->GetPrev());

			}

			leftEnd->SetPrev(rightEnd);
			rightStart->SetNext(leftStart);

			rightStart = tempFarRight == nullptr ? nullptr : tempFarRight->GetPrev();
			leftEnd = tempFarLeft == nullptr ? nullptr : tempFarLeft->GetNext();

			leftParams++;
			rightParams++;

		}

		leftStart = leftEnd;
		rightEnd = rightStart;

		leftStop = leftEnd == nullptr || !(leftEnd->GetPrev() != nullptr && (
			leftEnd->GetPrev()->GetNodeObject().type == TokenType_WORD ||
			leftEnd->GetPrev()->GetNodeObject().type == TokenType_UNARY ||
			leftEnd->GetPrev()->GetNodeObject().type == TokenType_KEYWORD));

		rightStop = rightStart == nullptr || !(rightStart->GetNext() != nullptr && (
			rightStart->GetNext()->GetNodeObject().type == TokenType_WORD ||
			rightStart->GetNext()->GetNodeObject().type == TokenType_UNARY ||
			leftEnd->GetPrev()->GetNodeObject().type == TokenType_KEYWORD));

	}
}

unsigned int KubLingLexer::breakOutBrackets(CelestialDoubleListNode<Token>* tok, int lNumber, CompileError &err)
{

	unsigned int increased = 0;
	Token tokn = tok->GetNodeObject();
	char startBrack = '(';
	char endBrack = ')';
	TokenType type = TokenType_BRACKET;

	if (tokn.type == TokenType_BRACKET)
	{

		if (tokn.val[0] == '(' && tokn.val[tokn.val.length()-1] == ')')
		{

			tokn.val = tokn.val.substr(1, tokn.val.length() - 2);
			tokn.type = TokenType_WORD;

		}
	}
	else if (tokn.type == TokenType_OFFSET)
	{

		hasOffset = true;
		if (tokn.val[tokn.val.length() - 1] == ']')
		{

			unsigned int start = 0;

			for (unsigned int i = 0; i < tokn.val.length(); i++)
			{

				if (tokn.val[i] == '[')
				{

					start = i;

				}
			}

			if (start != 0)
			{

				increased++;
				string var = tokn.val.substr(0,start);
				Token varTok = translateWord(var, err, lNumber);
				CelestialDoubleListNode<Token>* varPointer = new CelestialDoubleListNode<Token>(varTok);

				if (tok->GetPrev() != nullptr)
				{

					tok->GetPrev()->SetNext(varPointer);
					varPointer->SetPrev(tok->GetPrev());

				}

				varPointer->SetNext(tok);
				tok->SetPrev(varPointer);

				startBrack = '[';
				endBrack = ']';
				type = TokenType_OFFSET;

				tokn.val = tokn.val.substr(start + 1, tokn.val.length() - 1 - (start + 1));
				tokn.type = TokenType_WORD;

			}
		}
	}

	tok->SetNodeObject(tokn);

	Token startBracket;
	startBracket.line = lNumber;
	startBracket.type = type;
	startBracket.val = startBrack;
	CelestialDoubleListNode<Token>* startNode = new CelestialDoubleListNode<Token>(startBracket);

	if (tok->GetPrev() != nullptr)
	{

		tok->GetPrev()->SetNext(startNode);
		startNode->SetPrev(tok->GetPrev());

	}

	startNode->SetNext(tok);
	tok->SetPrev(startNode);

	Token endBracket;
	endBracket.line = lNumber;
	endBracket.type = type;
	endBracket.val = endBrack;
	CelestialDoubleListNode<Token>* endNode = new CelestialDoubleListNode<Token>(endBracket);

	if (tok->GetNext() != nullptr)
	{

		tok->GetNext()->SetPrev(endNode);
		endNode->SetNext(tok->GetNext());

	}

	endNode->SetPrev(tok);
	tok->SetNext(endNode);

	increased += 2;

	CelestialList<string>* words = extractWords(tokn.val);
	CelestialListNode<string>* word = words->GetFirstNode();
	CelestialDoubleList<Token>* returnVal = new CelestialDoubleList<Token>();

	while (word != nullptr && err.errorType == ScriptError_OK)
	{

		CelestialDoubleList<Token>* tokens = tokenizeWord(word->GetNodeObject(), lNumber, err);
		returnVal->AddElement(tokens);
		delete tokens;
		word = word->GetNext();

	}

	delete words;

	if (err.errorType == ScriptError_OK)
	{

		extractOps(returnVal, lNumber, err);
		tok->SetNodeObject(returnVal->GetFirstNode()->GetNodeObject());

		CelestialDoubleListNode<Token>* nextNode = returnVal->GetFirstNode()->GetNext();
		CelestialDoubleListNode<Token>* lastNode = returnVal->GetLastNode();

		if (nextNode != nullptr)
		{

			lastNode->SetNext(tok->GetNext());

			if (tok->GetNext() != nullptr)
			{

				tok->GetNext()->SetPrev(lastNode);

			}

			tok->SetNext(nextNode);
			nextNode->SetPrev(tok);
			returnVal->GetFirstNode()->SetNext(nullptr);

		}
	}

	returnVal->Reset();
	delete returnVal;

	return increased;

}

bool KubLingLexer::getShortHand(char val)
{

	for (int k = 0; k < operatorSize; k++)
	{

		for (int j = 0; j < operators[k].shortHandsAmounts; j++)
		{

			if (val == operators[k].shortHands[j][0])
			{


				return true;

			}
		}
	}

	return false;

}

unsigned int KubLingLexer::getShortHand(string word)
{

	unsigned int shortHand = 0;

	string tempWord = "";
	bool isQoutes = false;
	unsigned char bracketLev = 0;

	for (unsigned int i = 0; i < word.length() && shortHand == 0; i++)
	{

		isQoutes = word[i] == '\'' ? !isQoutes : isQoutes;

		if (word[i] == '(' || word[i] == '[')
		{

			bracketLev++;

		}
		else if (word[i] == ')' || word[i] == ']')
		{

			if (bracketLev != 0)
			{

				bracketLev--;

			}
		}

		if (bracketLev == 0 && !isQoutes)
		{

			shortHand = getShortHand(word[i]) ? i +1 : 0;

		}
	}

	return shortHand;

}

unsigned int KubLingLexer::getBracketLevels(CelestialDoubleList<Token>* tokens)
{

	unsigned char bracketLevels = 0;
	unsigned char maxBracketLevels = 0;
	CelestialDoubleListNode<Token>* tok = tokens->GetFirstNode();
	bool isQoutes = false;

	while (tok != nullptr)
	{

		Token token = tok->GetNodeObject();

		for (unsigned int i = 0; i < token.val.length(); i++)
		{

			isQoutes = token.val[i] == '\'' ? !isQoutes : isQoutes;

			if ((token.val[i] == '(' || token.val[i] == '[') && !isQoutes)
			{

				bracketLevels++;

				if (bracketLevels > maxBracketLevels)
				{

					maxBracketLevels = bracketLevels;

				}
			}
			else if ((token.val[i] == ')' || token.val[i] == ']') && !isQoutes)
			{

				if (bracketLevels != 0)
				{

					bracketLevels--;

				}
			}
		}

		tok = tok->GetNext();

	}

	return maxBracketLevels;

}

CelestialDoubleListNode<Token>* KubLingLexer::extractRights(CelestialDoubleListNode<Token>* token, CelestialDoubleList<Token>* tokens, unsigned int shorthand, int lNumber, CompileError &err)
{
	string rightOfShort = "";

	for (unsigned int i = shorthand + 1; i < token->GetNodeObject().val.length(); i++)
	{

		rightOfShort += token->GetNodeObject().val[i];

	}

	if (rightOfShort != "")
	{

		CelestialDoubleList<Token>* newTokens = tokenizeWord(rightOfShort, lNumber, err);
		newTokens->GetLastNode()->SetNext(token->GetNext());

		if (token->GetNext() != nullptr)
		{

			token->GetNext()->SetPrev(newTokens->GetLastNode());

		}

		token->SetNext(newTokens->GetFirstNode());
		newTokens->GetFirstNode()->SetPrev(token);
		tokens->IncrementCountBy(newTokens->GetCount());
		newTokens->SetFirst(nullptr);
		delete newTokens;

	}

	Token tok = token->GetNodeObject();
	tok.val = tok.val[shorthand];
	tok.type = TokenType_KEYWORD;
	token->SetNodeObject(tok);
	return token;

}

CelestialDoubleListNode<Token>* KubLingLexer::breakOutShortHand(CelestialDoubleListNode<Token>* token, CelestialDoubleList<Token>* tokens, unsigned int shorthand, int lNumber, CompileError &err)
{

	string leftOfShort = "";

	for (unsigned int i = 0; i < shorthand; i++)
	{

		leftOfShort += token->GetNodeObject().val[i];

	}

	if (leftOfShort != "")
	{

		CelestialDoubleList<Token>* newTokens = tokenizeWord(leftOfShort, lNumber, err);
		newTokens->GetLastNode()->SetNext(token);
		newTokens->GetFirstNode()->SetPrev(token->GetPrev());

		if (token->GetPrev() != nullptr)
		{

			token->GetPrev()->SetNext(newTokens->GetFirstNode());

		}

		token->SetPrev(newTokens->GetLastNode());
		tokens->IncrementCountBy(newTokens->GetCount());
		newTokens->SetFirst(nullptr);
		delete newTokens;

	}

	CelestialDoubleListNode<Token>* prevNode = token->GetPrev();

	if (prevNode == nullptr ||
		(leftOfShort == "" && token->GetNodeObject().val.length() > 1)||
		(prevNode->GetNodeObject().type != TokenType_WORD &&
			prevNode->GetNodeObject().type != TokenType_UNARY &&
			prevNode->GetNodeObject().type != TokenType_BRACKET &&
			prevNode->GetNodeObject().type != TokenType_OFFSET))//Is unary operator
	{

		if (token->GetNodeObject().val.length() == 1)
		{
			err.errorType = ScriptError_TOFEWPARAM;
			err.line = lNumber;

		}

		Token tok = token->GetNodeObject();
		tok.type = TokenType_UNARY;
		token->SetNodeObject(tok);
		return token;

	}

	return extractRights(token, tokens, shorthand, lNumber, err);

}

void KubLingLexer::extractOps(CelestialDoubleList<Token>* tokens, int lNumber, CompileError &err)
{

	if (err.errorType != ScriptError_OK)
	{

		return;

	}

	CelestialStack<CelestialDoubleListNode<Token>*>* unaries = new CelestialStack<CelestialDoubleListNode<Token>*>(false);

	CelestialStack<CelestialDoubleListNode<Token>*>* nonUnaries = new CelestialStack<CelestialDoubleListNode<Token>*>(false);

	CelestialStack<CelestialDoubleListNode<Token>*>* bracketOffsets = new CelestialStack<CelestialDoubleListNode<Token>*>(false);

	CelestialDoubleListNode<Token>* tok = tokens->GetFirstNode();
	unsigned int startVal = 0;

	while (tok != nullptr)
	{

		if (tok->GetNodeObject().type == TokenType_WORD || tok->GetNodeObject().type == TokenType_UNARY || tok->GetNodeObject().type == TokenType_KEYWORD)
		{

			unsigned int shortHand = getShortHand(tok->GetNodeObject().val.substr(startVal));

			if (shortHand > 0)
			{

				CelestialDoubleListNode<Token>* node = breakOutShortHand(tok, tokens, shortHand-1+startVal, lNumber, err);

				if (node->GetNodeObject().type == TokenType_UNARY)
				{

					startVal++;

				}
				else
				{

					nonUnaries->PushElement(node);
					tok = tok->GetNext();
					startVal = 0;

				}
			}
			else
			{

				tok = tok->GetNext();
				startVal = 0;

			}
		}
		else
		{

			tok = tok->GetNext();
			startVal = 0;

		}
	}

	tok = tokens->GetFirstNode();

	while (tok != nullptr)
	{

		if (tok->GetNodeObject().type == TokenType_WORD || tok->GetNodeObject().type == TokenType_UNARY)
		{

			unsigned int shortHand = getShortHand(tok->GetNodeObject().val);

			if (shortHand > 0)
			{

				CelestialDoubleListNode<Token>* node = breakOutShortHand(tok, tokens, shortHand - 1 + startVal, lNumber, err);

				if (node->GetNodeObject().type == TokenType_UNARY)
				{

					unaries->PushElement(node);

				}
			}
		}
		else if (tok->GetNodeObject().type == TokenType_BRACKET || tok->GetNodeObject().type == TokenType_OFFSET)
		{

			bracketOffsets->PushElement(tok);

		}
			
		tok = tok->GetNext();

	}

	while (nonUnaries->GetCount() > 0)
	{

		CelestialDoubleListNode<Token>* node = nonUnaries->PopElement();
		Token opTok = node->GetNodeObject();
		int sh = -1;

		for (int k = 0; k < operatorSize && sh == -1; k++)
		{

			for (int j = 0; j < operators[k].shortHandsAmounts && sh == -1; j++)
			{

				if (opTok.val == operators[k].shortHands[j])
				{

					sh = j;
					opTok.val = operators[k].keyword;

					if (operators[k].shortFlipParams[j])
					{

						flipParamsAroundOp(node, operators[k].leftParams[j], operators[k].rightParams[j]);

					}

					CelestialDoubleListNode<Token>* rightTok = node;

					for (unsigned char h = 0; h < operators[k].rightParams[j]; h++)
					{

						rightTok = rightTok->GetNext();

					}

					CelestialDoubleListNode<Token>* sep = rightTok;

					if (sep->GetNodeObject().type == TokenType_KEYWORD)
					{

						while (sep->GetNext() != nullptr && 
							sep->GetNodeObject().type != TokenType_SEPERATOR)
						{

							sep = sep->GetNext();

						}
					}

					Token sepTok;
					sepTok.line = lNumber;
					sepTok.type = TokenType_SEPERATOR;
					sepTok.val = ',';
					CelestialDoubleListNode<Token>* tokNode = new CelestialDoubleListNode<Token>(sepTok);

					tokNode->SetNext(sep->GetNext());
					tokNode->SetPrev(sep);
					sep->SetNext(tokNode);

					if (tokNode->GetNext() != nullptr)
					{

						tokNode->GetNext()->SetPrev(tokNode);

					}

					moveParamsOfOp(node, operators[k].leftParams[j]);
					node->SetNodeObject(opTok);

					while (tokens->GetFirstNode()->GetPrev() != nullptr)
					{

						tokens->SetFirst(tokens->GetFirstNode()->GetPrev());

					}

					while (tokens->GetLastNode()->GetNext() != nullptr)
					{

						tokens->SetLast(tokens->GetLastNode()->GetNext());

					}
				}
			}
		}
	}

	while (unaries->GetCount() > 0)
	{

		CelestialDoubleListNode<Token>* node = extractRights(unaries->PopElement(), tokens,0,lNumber,err);

		if (node->GetNext()->GetNodeObject().type == TokenType_BRACKET ||
			node->GetNext()->GetNodeObject().type == TokenType_OFFSET)
		{

			bracketOffsets->PushElement(node->GetNext());

		}

		Token leftBracket;
		leftBracket.line = lNumber;
		leftBracket.type = TokenType_BRACKET;
		leftBracket.val = '(';

		CelestialDoubleListNode<Token>* leftNode = new CelestialDoubleListNode<Token>(leftBracket);
		leftNode->SetNext(node);
		leftNode->SetPrev(node->GetPrev());
		node->SetPrev(leftNode);

		if (leftNode->GetPrev() != nullptr)
		{

			leftNode->GetPrev()->SetNext(leftNode);

		}

		Token opTok = node->GetNodeObject();
		int sh = -1;

		for (int k = 0; k < operatorSize && sh == -1; k++)
		{

			for (int j = 0; j < operators[k].shortHandsAmounts && sh == -1; j++)
			{

				if (opTok.val == operators[k].shortHands[j])
				{

					opTok.val = operators[k].keyword;
					node->SetNodeObject(opTok);
					node = node->GetNext();
					unsigned char rights = 1;

					while (node != nullptr && (
						node->GetNodeObject().type == TokenType_WORD ||
						node->GetNodeObject().type == TokenType_KEYWORD ||
						node->GetNodeObject().type == TokenType_UNARY) &&
						rights < operators[k].rightParams[j])
					{

						node = node->GetNext();
						rights++;

					}

					Token rightBracket;
					rightBracket.line = lNumber;
					rightBracket.type = TokenType_BRACKET;
					rightBracket.val = ')';

					Token rightSep;
					rightSep.line = lNumber;
					rightSep.type = TokenType_SEPERATOR;
					rightSep.val = ',';

					CelestialDoubleListNode<Token>* rightNode = new CelestialDoubleListNode<Token>(rightSep);
					CelestialDoubleListNode<Token>* rightNode2 = new CelestialDoubleListNode<Token>(rightBracket);
					rightNode2->SetPrev(rightNode);
					rightNode->SetNext(rightNode2);

					rightNode->SetPrev(node);
					rightNode2->SetNext(node->GetNext());
					node->SetNext(rightNode);

					if (rightNode2->GetNext() != nullptr)
					{

						rightNode2->GetNext()->SetPrev(rightNode2);

					}

					while (tokens->GetLastNode()->GetNext() != nullptr)
					{

						tokens->SetLast(tokens->GetLastNode()->GetNext());

					}

					sh = j;

				}
			}
		}

		tokens->IncrementCount();
		tokens->IncrementCount();
		tokens->IncrementCount();

	}

	while (bracketOffsets->GetCount() > 0 && err.errorType == ScriptError_OK)
	{

		CelestialDoubleListNode<Token>* tokn = bracketOffsets->PopElement();

		unsigned int breakOuts = breakOutBrackets(tokn, lNumber, err);
		tokens->IncrementCountBy(breakOuts);

		while (tokens->GetLastNode()->GetNext() != nullptr)
		{

			tokens->SetLast(tokens->GetLastNode()->GetNext());

		}

	}

	delete unaries;
	delete nonUnaries;
	delete bracketOffsets;

}

CelestialDoubleList<Token>* KubLingLexer::TokenizeLine(string line, int lNumber, CompileError &err)
{

	hasOffset = false;
	CelestialDoubleList<Token>* returnVal = new CelestialDoubleList<Token>();
	bool isFlow = false;

	if (line != "")
	{

		string newLine ="";
		bool str = false;

		//Add spaces
		/*for (unsigned int i = 0; i < line.length(); i++)
		{

			str = line[i] == '\'' && !str;

			if (!str && (getShortHand(line[i]) || line[i] == ',' || line[i] == ';'))
			{

				if (i > 0 && line[i - 1] != ' ')
				{

					newLine = newLine.append(" ");

				}

				newLine = newLine.append(&line[i], 1);

				if (i + 1 == line.length() || line[i + 1] != ' ')
				{

					newLine = newLine.append(" ");

				}
			}
			else
			{

				newLine = newLine.append(&line[i], 1);

			}
		}

		line = newLine;*/

		CelestialList<string>* words = extractWords(line);
		CelestialListNode<string>* word = words->GetFirstNode();

		while (word != nullptr && err.errorType == ScriptError_OK)
		{

			CelestialDoubleList<Token>* tokens = tokenizeWord(word->GetNodeObject(), lNumber, err);
			returnVal->AddElement(tokens);
			delete tokens;
			word = word->GetNext();

		}

		delete words;

		if (err.errorType == ScriptError_OK)
		{

			extractOps(returnVal, lNumber, err);
			CelestialDoubleListNode<Token>* tok = returnVal->GetFirstNode();

			while (tok != nullptr && err.errorType == ScriptError_OK)
			{

				if (tok->GetNodeObject().type == TokenType_WORD)
				{

					Token token = translateWord(tok->GetNodeObject().val, err, lNumber);
					token.line = lNumber;

					tok->SetNodeObject(token);

				}
				else if (tok->GetNodeObject().type == TokenType_FLOW)
				{

					if (isFlow)
					{

						err.errorType = ScriptError_UNCLOSED;
						err.line = lNumber;
						return returnVal;

					}

					isFlow = true;
					Token condToken;
					condToken.type = TokenType_COND;
					condToken.val = "conditions:";
					condToken.line = lNumber;

					CelestialDoubleListNode<Token>* cond = new CelestialDoubleListNode<Token>(condToken);
					cond->SetPrev(tok);
					cond->SetNext(tok->GetNext());
					tok->SetNext(cond);

					if (cond->GetNext() != nullptr)
					{

						cond->GetNext()->SetPrev(cond);

					}

					returnVal->IncrementCount();

					while (returnVal->GetLastNode()->GetNext() != nullptr)
					{

						returnVal->SetLast(returnVal->GetLastNode()->GetNext());

					}
				}

				tok = tok->GetNext();

			}

			if (returnVal->GetLastNode()->GetNodeObject().type == TokenType_SEPERATOR)
			{

				Token newToken = returnVal->GetLastNode()->GetNodeObject();
				newToken.val = ';';
				returnVal->GetLastNode()->SetNodeObject(newToken);

			}

			if (err.errorType == ScriptError_OK && 
				returnVal->GetCount() > 0 && 
				returnVal->GetLastNode()->GetNodeObject().type != TokenType_SEPERATOR && 
				returnVal->GetLastNode()->GetNodeObject().type != TokenType_BODY && 
				returnVal->GetLastNode()->GetNodeObject().type != TokenType_COND)
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
	}

	return returnVal;

}

KubLingLexer::~KubLingLexer()
{

	delete[] numbers;

}
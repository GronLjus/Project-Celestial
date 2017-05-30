#pragma once
#include <string>
#include "RunTimeOperations.h"
#include "CelestialTree.h"
#include "CelestialSlicedList.h"

namespace Logic
{

	///<summary>The possible error codes</summary>
	enum ScriptError{ ScriptError_OK, 
		ScriptError_ENUMMISPLACE, ScriptError_INCOMPATIBLEENUM,
		ScriptError_VARUNIN, 
		ScriptError_TOFEWPARAM, ScriptError_TOMANYPARAM, ScriptError_BADSIG,
		ScriptError_INCOMPATIBLE,  ScriptError_INCOMPATIBLERETURN, 
		ScriptError_UNCLOSED, ScriptError_UNCLOSEDBODY, 
		ScriptError_PARAMWRONGPLACE,ScriptError_UNKNOWNSYSTEMPARAM,
		ScriptError_UNKNOWNKEYWORD, ScriptError_HUH,
		ScriptError_CONDITION,ScriptError_NOBODY,
		ScriptError_UNEXPECTEDKEYWORD, ScriptError_NESTEDFLOW,
		ScriptError_MISSINGFLOW, ScriptError_ALREADYINIT,
		ScriptError_NA
	};
	
	///<summary>The different types of tokens</summary>
	enum TokenType{ TokenType_KEYWORD, TokenType_SEPERATOR, TokenType_OFFSET, TokenType_BRACKET, TokenType_WORD, TokenType_SEQ, TokenType_PARAM, TokenType_VAR, TokenType_FLOW, TokenType_COND, TokenType_BODY, TokenType_UNARY, TokenType_NA };
	///<summary>The different types of syntaxes</summary>
	enum SyntaxType{ SyntaxType_OPERATORCONST, SyntaxType_OPERATORCONSTPASS, SyntaxType_OPERATORVAR, SyntaxType_OPERATORNA,
		SyntaxType_VAR, SyntaxType_CONST, SyntaxType_CONSTPASS,
		SyntaxType_FLOW, SyntaxType_NA };
	///<summary>The different types of variables</summary>
	enum VarType{ VarType_NUMBER, VarType_FLOAT, VarType_STRING, VarType_ENUM, VarType_NA };
	///<summary>Where the parameters come from</summary>
	enum ParamType{ ParamType_VAR, ParamType_CONST, ParamType_NA };
	///<summary>The categorys of keywords</summary>
	enum KeyWordType{ KeyWordType_OPERATOR, KeyWordType_ENUM, KeyWordType_FLOW, KeyWordType_NA };
	///<summary>The different categories of branches</summary>
	enum FlowType{ FlowType_LOOP, FlowType_BRANCH, FlowType_BODY, FlowType_COND, FlowType_INIT,FlowType_NA };
	///<summary>The type of flowoperators</summary>
	enum FlowOperator{ FlowOperator_IF, FlowOperator_ELSE, FlowOperator_LOOP, FlowOperator_INIT, FlowOperator_NA };
	///<summary>The different types of operators in the script</summary>
	enum OperatorTypes{
		OperatorTypes_ADD, OperatorTypes_REMOVE, OperatorTypes_DELETE,
		OperatorTypes_LOAD, OperatorTypes_UNLOAD, OperatorTypes_SAVE,
		OperatorTypes_ADDTASKPARAM,
		OperatorTypes_SET, OperatorTypes_POST,
		OperatorTypes_SUM, OperatorTypes_SUB, OperatorTypes_MUL, OperatorTypes_DIV, 
		OperatorTypes_MOD,
		OperatorTypes_CAST,
		OperatorTypes_EQUAL, OperatorTypes_GREATER, 
		OperatorTypes_PARENT, OperatorTypes_SNAP,
		OperatorTypes_ROTATE, OperatorTypes_ORBIT,
		OperatorTypes_POS, OperatorTypes_MOVE, OperatorTypes_PROPEL,
		OperatorTypes_SIZE, OperatorTypes_SCALE,
		OperatorTypes_SCOPE,
		OperatorTypes_RUN, OperatorTypes_SETPARAM,
		OperatorTypes_IMPORT, OperatorTypes_EXPORT,
		OperatorTypes_FOCUS, OperatorTypes_HIDE, OperatorTypes_SHOW,
		OperatorTypes_INCREMENT, OperatorTypes_DECREMENT, 
		OperatorTypes_IGNORE, OperatorTypes_ACCEPT,
		OperatorTypes_TRANSLATE,
		OperatorTypes_TRACK, OperatorTypes_CLEAR,
		OperatorTypes_HOOK, OperatorTypes_GLUE,
		OperatorTypes_ROUTE,
		OperatorTypes_LINK,
		OperatorTypes_SPAWN, OperatorTypes_TRAVEL,
		OperatorTypes_SPLIT,
		OperatorTypes_PAUSE, OperatorTypes_RESUME,
		OperatorTypes_ARRAY, OperatorTypes_OFFSET,
		OperatorTypes_AND, OperatorTypes_OR,
		OperatorTypes_FILL,
		OperatorTypes_LOCK,
		OperatorTypes_NA
	};


	///<summary>A struct containing the errors caused by compilation</summary>
	struct CompileError
	{
		///<summary>the type of error</summary>
		ScriptError errorType;
		///<summary>Where in the source the error comes from</summary>
		int line;

	};

	///<summary>The atomic token</summary>
	struct Token
	{
		///<summary>The type of token</summary>
		TokenType type;
		///<summary>The value of the token</summary>
		std::string val;
		///<summary>Where in the script the token came from</summary>
		int line;

	};

	///<summary>A struct of a keyword</summary>
	struct Keyword
	{

		///<summary>The keywords text value</summary>
		std::string keyword;
		///<summary>The type of keyword</summary>
		KeyWordType type;
		///<summary>The priority of the keyword to sort by</summary>
		unsigned char priority;

	};

	///<summary>The struct of a syntax</summary>
	struct syntax
	{

		///<summary>The type of syntax</summary>
		SyntaxType type;
		///<summary>The textvalue of the syntax</summary>
		std::string val;

	};

	///<summary>The struct of a symbol</summary>
	struct symbol
	{

		///<summary>The type of variable the symbol hos</summary>
		VarType type;
		///<summary>The symbols name</summary>
		std::string name;
		///<summary>The variable adress of the symbol</summary>
		unsigned int address;
		///<summary>How many times the symbol is hit by tha analyser</summary>
		unsigned int symCount;
		///<summary>How many times the symbol is hit by the codegenerator</summary>
		unsigned int codeCount;
		///<summary>If the symbol should be garbagecollected</summary>
		bool gC;
		bool waitFlag;

	};

	///<summary>A list of abstract syntax trees</summary>
	struct AST
	{
		///<summary>A pointer to the list of syntax trees</summary>
		CrossHandlers::CelestialList<CrossHandlers::CelestialTree<syntax>*>* trees;
		///<summary>A pointer to a list of symbols</summary>
		CrossHandlers::CelestialSlicedList<symbol>* symbolTable = nullptr;
		///<summary>The amount of symbols</summary>
		int symbols;

	};

	///<summary>The struct of controlflow</summary>
	struct FlowController
	{

		///<summary>The sort of operator of the flow</summary>
		FlowOperator op;
		///<summary>The textvalue of the flow operator</summary>
		std::string keyword;
		///<summary>The maximum amount of parameters</summary>
		int params;
		///<summary>The optional parameters</summary>
		bool* optionalParams;
		///<summary>The types of parameters</summary>
		FlowType* parameters;
		///<summary>The minimum amount of arguments in each parameter</summary>
		int* minParamArgs;
		///<summary>The maximum amount of arguments in each parameter</summary>
		int* maxParamArgs;
		///<summary>The type of flow the command represents</summary>
		FlowType type;

	};

	struct RuntimeParameter
	{
		std::string name;
		VarType type;

	};

	///<summary>The struct of an operator</summary>
	struct Operator
	{
		///<summary>the amount of shorthands of the operator</summary>
		unsigned char shortHandsAmounts;
		///<summary>The shorthands of the operator</summary>
		std::string* shortHands;
		///<summary>The amount of parameters to the left of each short hand</summary>
		unsigned char* leftParams;
		///<summary>The amount of parameters to the right of each short hand</summary>
		unsigned char* rightParams;
		///<summary>If the parameters should flip with the shorthand</summary>
		bool* shortFlipParams;

		///<summary>The textvalue of the operator</summary>
		std::string keyword;
		///<summary>The amount of variatons of the operator</summary>
		unsigned char enumAmount;
		///<summary>The array of enumtextvalues</summary>
		std::string* enums;
		///<summary>An array each variaton containing an array of variable types for each parameter</summary> 
		VarType** paramsyntax;
		///<summary>An array each variaton containing an array of variable sources for each parameter</summary> 
		ParamType** paramTypes;
		///<summary>An array containing the amount of parameters for each variaton</summary>
		unsigned char* params;
		///<summary>An array containing which parameter is the target of the readparam each variaton</summary>
		unsigned char* writeParam;
		///<summary>An array containing which parameter is copied to the writeparam in each variaton</summary>
		unsigned char* readParam;
		///<summary>An array containing the bytecode of each variaton</summary>
		unsigned char* byteCodes;
		///<summary>An array containing what each variaton returns</summary>
		VarType* returns;
		///<summary>An array containing the type each variaton returns</summary>
		ParamType* returnType;

		unsigned char* amountParOperators;
		OperatorTypes** parOperators;
		unsigned char** parOperator;
		bool** parOperatorAppend;
		bool** optionalPar;

		unsigned char* minParams;

		unsigned char** parRepeatsMin;
		unsigned char** parRepeatsMax;

		///<summary>The priority to sort by</summary>
		unsigned char priority;

	};
}
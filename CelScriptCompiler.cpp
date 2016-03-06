#include "stdafx.h"
#include "CelScriptCompiler.h"

using namespace Logic;
using namespace Resources;


CelScriptCompiler::CelScriptCompiler()
{

	this->outText = nullptr;
	operators = new Operator[OperatorTypes_NA];

	operators[OperatorTypes_SET].keyword = "set";
	operators[OperatorTypes_SET].enumAmount = 5;
	operators[OperatorTypes_SET].enums = new std::string[operators[OperatorTypes_SET].enumAmount]; 
	operators[OperatorTypes_SET].enums[0] = ""; 
	operators[OperatorTypes_SET].enums[1] = ""; 
	operators[OperatorTypes_SET].enums[2] = "gameboard";
	operators[OperatorTypes_SET].enums[3] = "camera";
	operators[OperatorTypes_SET].enums[4] = "leftclick";
	operators[OperatorTypes_SET].byteCodes = new unsigned char[operators[OperatorTypes_SET].enumAmount]{opcode_SETCONST, opcode_SETVAR, opcode_SETGMEBRD, opcode_SETCMRA, opcode_SETLCLK};
	operators[OperatorTypes_SET].params = new unsigned char[operators[OperatorTypes_SET].enumAmount]{2, 2, 1, 1, 2};
	operators[OperatorTypes_SET].minParams = new unsigned char[operators[OperatorTypes_SET].enumAmount]{ 2, 2, 1, 1, 2};
	operators[OperatorTypes_SET].paramsyntax = new VarType*[operators[OperatorTypes_SET].enumAmount]{
		new VarType[operators[OperatorTypes_SET].params[0]]{VarType_NA, VarType_NA}, 
			new VarType[operators[OperatorTypes_SET].params[1]]{VarType_NA, VarType_NA},
			new VarType[operators[OperatorTypes_SET].params[2]]{VarType_NUMBER},
			new VarType[operators[OperatorTypes_SET].params[3]]{ VarType_NUMBER },
			new VarType[operators[OperatorTypes_SET].params[4]]{ VarType_NUMBER, VarType_NUMBER} };
	operators[OperatorTypes_SET].paramTypes = new ParamType*[operators[OperatorTypes_SET].enumAmount]{
		new ParamType[operators[OperatorTypes_SET].params[0]]{ParamType_VAR, ParamType_CONST}, 
			new ParamType[operators[OperatorTypes_SET].params[1]]{ParamType_VAR, ParamType_VAR},
			new ParamType[operators[OperatorTypes_SET].params[2]]{ParamType_VAR},
			new ParamType[operators[OperatorTypes_SET].params[3]]{ ParamType_VAR },
			new ParamType[operators[OperatorTypes_SET].params[4]]{ ParamType_VAR, ParamType_VAR} };
	operators[OperatorTypes_SET].optionalPar = new bool*[operators[OperatorTypes_SET].enumAmount]{
	new bool[operators[OperatorTypes_SET].params[0]]{ false, false },
	new bool[operators[OperatorTypes_SET].params[1]]{ false, false },
	new bool[operators[OperatorTypes_SET].params[2]]{ false },
	new bool[operators[OperatorTypes_SET].params[3]]{ false },
	new bool[operators[OperatorTypes_SET].params[4]]{ false, false}
	};
	operators[OperatorTypes_SET].readParam = new unsigned char[operators[OperatorTypes_SET].enumAmount]{2,2,0,0,0};
	operators[OperatorTypes_SET].returns = new VarType[operators[OperatorTypes_SET].enumAmount]{VarType_NA, VarType_NA, VarType_NA, VarType_NA, VarType_NA};
	operators[OperatorTypes_SET].returnType = new ParamType[operators[OperatorTypes_SET].enumAmount]{ParamType_NA, ParamType_NA, ParamType_NA, ParamType_NA, ParamType_NA};
	operators[OperatorTypes_SET].writeParam = new unsigned char[operators[OperatorTypes_SET].enumAmount]{1,1,0,0,0};
	operators[OperatorTypes_SET].priority = 0;
	operators[OperatorTypes_SET].shortHandsAmounts = 1;
	operators[OperatorTypes_SET].shortHands = new std::string[operators[OperatorTypes_SET].shortHandsAmounts]; operators[OperatorTypes_SET].shortHands[0] = "=" ;
	operators[OperatorTypes_SET].rightParams = new unsigned char[operators[OperatorTypes_SET].shortHandsAmounts]{1};
	operators[OperatorTypes_SET].leftParams = new unsigned char[operators[OperatorTypes_SET].shortHandsAmounts]{1};
	operators[OperatorTypes_SET].shortFlipParams = new bool[operators[OperatorTypes_SET].shortHandsAmounts]{false};
	operators[OperatorTypes_SET].amountParOperators = new unsigned char[operators[OperatorTypes_SET].enumAmount]{0, 0,0,0,0};
	operators[OperatorTypes_SET].parRepeatsMin = new unsigned char*[operators[OperatorTypes_SET].enumAmount]{
		new unsigned char[operators[OperatorTypes_SET].params[0]]{0, 0}, 
			new unsigned char[operators[OperatorTypes_SET].params[1]]{0, 0},
			new unsigned char[operators[OperatorTypes_SET].params[2]]{0},
			new unsigned char[operators[OperatorTypes_SET].params[3]]{ 0 },
			new unsigned char[operators[OperatorTypes_SET].params[4]]{ 0, 0}
	};
	operators[OperatorTypes_SET].parRepeatsMax = new unsigned char*[operators[OperatorTypes_SET].enumAmount]{
		new unsigned char[operators[OperatorTypes_SET].params[0]]{0, 0}, 
			new unsigned char[operators[OperatorTypes_SET].params[1]]{0, 0},
			new unsigned char[operators[OperatorTypes_SET].params[2]]{0},
			new unsigned char[operators[OperatorTypes_SET].params[3]]{ 0 },
			new unsigned char[operators[OperatorTypes_SET].params[4]]{ 0, 1}
	};
	operators[OperatorTypes_SET].parOperatorAppend = new bool*[operators[OperatorTypes_SET].enumAmount]{
		new bool[operators[OperatorTypes_SET].params[0]]{false, false},
			new bool[operators[OperatorTypes_SET].params[1]]{false, false},
			new bool[operators[OperatorTypes_SET].params[2]]{false},
			new bool[operators[OperatorTypes_SET].params[3]]{ false },
			new bool[operators[OperatorTypes_SET].params[4]]{ false, false}
	};

	operators[OperatorTypes_LOAD].keyword = "load";
	operators[OperatorTypes_LOAD].enumAmount = 10;
	operators[OperatorTypes_LOAD].enums = new std::string[operators[OperatorTypes_LOAD].enumAmount]; 
	operators[OperatorTypes_LOAD].enums[0] = "light"; 
	operators[OperatorTypes_LOAD].enums[1] = "mesh";
	operators[OperatorTypes_LOAD].enums[2] = "script";
	operators[OperatorTypes_LOAD].enums[3] = "textbox";
	operators[OperatorTypes_LOAD].enums[4] = "panel";
	operators[OperatorTypes_LOAD].enums[5] = "keytrgr";
	operators[OperatorTypes_LOAD].enums[6] = "keychartrgr";
	operators[OperatorTypes_LOAD].enums[7] = "gameboard";
	operators[OperatorTypes_LOAD].enums[8] = "camera";
	operators[OperatorTypes_LOAD].enums[9] = "object";
	operators[OperatorTypes_LOAD].byteCodes = new unsigned char[operators[OperatorTypes_LOAD].enumAmount]{opcode_LOADLIGHT, opcode_LOADMESH,opcode_LOADSCRIPT,opcode_LOADTXTBX,opcode_LOADPANEL, opcode_LOADKYTRGR,opcode_LOADCHRKYTRGR,opcode_LOADGMBRD, opcode_LOADCAM,opcode_LOADOBJCT};
	operators[OperatorTypes_LOAD].params = new unsigned char[operators[OperatorTypes_LOAD].enumAmount]{3, 1,1,0,0,2,2,2,0, 1};
	operators[OperatorTypes_LOAD].minParams = new unsigned char[operators[OperatorTypes_LOAD].enumAmount]{ 3, 1,1,0,0,2,2,1, 0, 1};
	operators[OperatorTypes_LOAD].paramsyntax = new VarType*[operators[OperatorTypes_LOAD].enumAmount]{
		new VarType[operators[OperatorTypes_LOAD].params[0]]{VarType_NUMBER, VarType_NUMBER, VarType_NUMBER}, 
			new VarType[operators[OperatorTypes_LOAD].params[1]]{VarType_STRING},
			new VarType[operators[OperatorTypes_LOAD].params[2]]{VarType_STRING},
			nullptr,nullptr,
			new VarType[operators[OperatorTypes_LOAD].params[5]]{VarType_NUMBER, VarType_STRING},
			new VarType[operators[OperatorTypes_LOAD].params[6]]{VarType_NUMBER, VarType_NUMBER},
			new VarType[operators[OperatorTypes_LOAD].params[7]]{VarType_NUMBER, VarType_NUMBER},
			nullptr,
			new VarType[operators[OperatorTypes_LOAD].params[9]]{VarType_NUMBER}};
	operators[OperatorTypes_LOAD].paramTypes = new ParamType*[operators[OperatorTypes_LOAD].enumAmount]{
		new ParamType[operators[OperatorTypes_LOAD].params[0]]{ParamType_NA, ParamType_NA, ParamType_NA}, 
			new ParamType[operators[OperatorTypes_LOAD].params[1]]{ParamType_NA},
			new ParamType[operators[OperatorTypes_LOAD].params[2]]{ParamType_NA},
			nullptr,
			nullptr,
			new ParamType[operators[OperatorTypes_LOAD].params[5]]{ParamType_NA, ParamType_NA},
			new ParamType[operators[OperatorTypes_LOAD].params[6]]{ParamType_NA, ParamType_NA},
			new ParamType[operators[OperatorTypes_LOAD].params[7]]{ParamType_NA, ParamType_NA},
			nullptr,
			new ParamType[operators[OperatorTypes_LOAD].params[9]]{ParamType_NA}};
	operators[OperatorTypes_LOAD].optionalPar = new bool*[operators[OperatorTypes_LOAD].enumAmount]{
		new bool[operators[OperatorTypes_LOAD].params[0]]{false, false, false },
		new bool[operators[OperatorTypes_LOAD].params[1]]{ false },
		new bool[operators[OperatorTypes_LOAD].params[2]]{ false},
		nullptr,
		nullptr,
		new bool[operators[OperatorTypes_LOAD].params[5]]{ false, false },
		new bool[operators[OperatorTypes_LOAD].params[6]]{ false, false },
		new bool[operators[OperatorTypes_LOAD].params[7]]{ false, true },
		nullptr,
		new bool[operators[OperatorTypes_LOAD].params[9]]{ false }
	};
	operators[OperatorTypes_LOAD].readParam = new unsigned char[operators[OperatorTypes_LOAD].enumAmount]{0,0,0,0,0,0,0,0,0,0};
	operators[OperatorTypes_LOAD].priority = 0;
	operators[OperatorTypes_LOAD].returns = new VarType[operators[OperatorTypes_LOAD].enumAmount]{VarType_NUMBER, VarType_NUMBER, VarType_NUMBER, VarType_NUMBER, VarType_NUMBER, VarType_NUMBER, VarType_NUMBER, VarType_NUMBER, VarType_NUMBER, VarType_NUMBER };
	operators[OperatorTypes_LOAD].returnType = new ParamType[operators[OperatorTypes_LOAD].enumAmount]{ParamType_VAR, ParamType_VAR, ParamType_VAR, ParamType_VAR, ParamType_VAR, ParamType_VAR, ParamType_VAR, ParamType_VAR, ParamType_VAR, ParamType_VAR };
	operators[OperatorTypes_LOAD].writeParam = new unsigned char[operators[OperatorTypes_LOAD].enumAmount]{0, 0,0,0,0,0,0,0, 0,0};
	operators[OperatorTypes_LOAD].shortHandsAmounts = 0;
	operators[OperatorTypes_LOAD].amountParOperators = new unsigned char[operators[OperatorTypes_LOAD].enumAmount]{0, 0, 0, 0, 0, 0,0,0, 0,0};
	operators[OperatorTypes_LOAD].parRepeatsMin = new unsigned char*[operators[OperatorTypes_LOAD].enumAmount]{
		new unsigned char[operators[OperatorTypes_LOAD].params[0]]{0, 0, 0},
			new unsigned char[operators[OperatorTypes_LOAD].params[1]]{0},
			new unsigned char[operators[OperatorTypes_LOAD].params[2]]{0},
			nullptr,
			nullptr,
			new unsigned char[operators[OperatorTypes_LOAD].params[5]]{0, 0},
			new unsigned char[operators[OperatorTypes_LOAD].params[6]]{0, 0},
			new unsigned char[operators[OperatorTypes_LOAD].params[7]]{0,0},
			nullptr,
			new unsigned char[operators[OperatorTypes_LOAD].params[9]]{0}
	};
	operators[OperatorTypes_LOAD].parRepeatsMax = new unsigned char*[operators[OperatorTypes_LOAD].enumAmount]{
		new unsigned char[operators[OperatorTypes_LOAD].params[0]]{0, 0, 0},
			new unsigned char[operators[OperatorTypes_LOAD].params[1]]{0},
			new unsigned char[operators[OperatorTypes_LOAD].params[2]]{0},
			nullptr,
			nullptr,
			new unsigned char[operators[OperatorTypes_LOAD].params[5]]{0, 0},
			new unsigned char[operators[OperatorTypes_LOAD].params[6]]{0, 0},
			new unsigned char[operators[OperatorTypes_LOAD].params[7]]{0,0},
			nullptr,
			new unsigned char[operators[OperatorTypes_LOAD].params[9]]{0}
	};
	operators[OperatorTypes_LOAD].parOperatorAppend = new bool*[operators[OperatorTypes_LOAD].enumAmount]{
		new bool[operators[OperatorTypes_LOAD].params[0]]{false, false, false},
			new bool[operators[OperatorTypes_LOAD].params[1]]{false},
			new bool[operators[OperatorTypes_LOAD].params[2]]{false},
			nullptr,
			nullptr,
			new bool[operators[OperatorTypes_LOAD].params[5]]{false, false},
			new bool[operators[OperatorTypes_LOAD].params[6]]{false, false},
			new bool[operators[OperatorTypes_LOAD].params[7]]{false, 0},
			nullptr,
			new bool[operators[OperatorTypes_LOAD].params[9]]{false}
	};

	operators[OperatorTypes_ADD].keyword = "add";
	operators[OperatorTypes_ADD].enumAmount = 4;
	operators[OperatorTypes_ADD].enums = new std::string[operators[OperatorTypes_ADD].enumAmount]; 
	operators[OperatorTypes_ADD].enums[0] = "object"; 
	operators[OperatorTypes_ADD].enums[1] = "2dto"; 
	operators[OperatorTypes_ADD].enums[2] = "keytrgr";
	operators[OperatorTypes_ADD].enums[3] = "mesh";
	operators[OperatorTypes_ADD].byteCodes = new unsigned char[operators[OperatorTypes_ADD].enumAmount]{opcode_ADDOBJECT,opcode_2DADDASNODE,opcode_2DADDKYTRGR,opcode_ADDMESH};
	operators[OperatorTypes_ADD].params = new unsigned char[operators[OperatorTypes_ADD].enumAmount]{1,2,2,1};
	operators[OperatorTypes_ADD].minParams = new unsigned char[operators[OperatorTypes_ADD].enumAmount]{ 1,2,2,1 };
	operators[OperatorTypes_ADD].paramsyntax = new VarType*[operators[OperatorTypes_ADD].enumAmount]{
		new VarType[operators[OperatorTypes_ADD].params[0]]{VarType_NUMBER}, 
			new VarType[operators[OperatorTypes_ADD].params[1]]{VarType_NUMBER, VarType_NUMBER}, 
			new VarType[operators[OperatorTypes_ADD].params[2]]{VarType_NUMBER, VarType_NUMBER},
			new VarType[operators[OperatorTypes_ADD].params[3]]{VarType_NUMBER}};
	operators[OperatorTypes_ADD].paramTypes = new ParamType*[operators[OperatorTypes_ADD].enumAmount]{
		new ParamType[operators[OperatorTypes_ADD].params[0]]{ParamType_NA}, 
			new ParamType[operators[OperatorTypes_ADD].params[1]]{ParamType_NA, ParamType_NA}, 
			new ParamType[operators[OperatorTypes_ADD].params[2]]{ParamType_NA, ParamType_NA},
			new ParamType[operators[OperatorTypes_ADD].params[3]]{ParamType_NA}};
	operators[OperatorTypes_ADD].optionalPar = new bool*[operators[OperatorTypes_ADD].enumAmount]{
		new bool[operators[OperatorTypes_ADD].params[0]]{ false,},
		new bool[operators[OperatorTypes_ADD].params[1]]{ false, false },
		new bool[operators[OperatorTypes_ADD].params[2]]{ false, false },
		new bool[operators[OperatorTypes_ADD].params[3]]{ false}
	};
	operators[OperatorTypes_ADD].readParam = new unsigned char[operators[OperatorTypes_ADD].enumAmount]{0,0,0,0};
	operators[OperatorTypes_ADD].priority = 0;
	operators[OperatorTypes_ADD].returns = new VarType[operators[OperatorTypes_ADD].enumAmount]{VarType_NA, VarType_NUMBER, VarType_NUMBER, VarType_NA};
	operators[OperatorTypes_ADD].returnType = new ParamType[operators[OperatorTypes_ADD].enumAmount]{ParamType_NA, ParamType_VAR, ParamType_VAR, ParamType_NA};
	operators[OperatorTypes_ADD].writeParam = new unsigned char[operators[OperatorTypes_ADD].enumAmount]{0,0,0,0};
	operators[OperatorTypes_ADD].shortHandsAmounts = 0;
	operators[OperatorTypes_ADD].amountParOperators = new unsigned char[operators[OperatorTypes_ADD].enumAmount]{0, 0, 0,0};
	operators[OperatorTypes_ADD].parRepeatsMin = new unsigned char*[operators[OperatorTypes_ADD].enumAmount]{
		new unsigned char[operators[OperatorTypes_ADD].params[0]]{0},
			new unsigned char[operators[OperatorTypes_ADD].params[1]]{0, 0},
			new unsigned char[operators[OperatorTypes_ADD].params[2]]{0, 0},
			new unsigned char[operators[OperatorTypes_ADD].params[3]]{0}
	};
	operators[OperatorTypes_ADD].parRepeatsMax = new unsigned char*[operators[OperatorTypes_ADD].enumAmount]{
		new unsigned char[operators[OperatorTypes_ADD].params[0]]{0},
			new unsigned char[operators[OperatorTypes_ADD].params[1]]{0, 0},
			new unsigned char[operators[OperatorTypes_ADD].params[2]]{0, 0},
			new unsigned char[operators[OperatorTypes_ADD].params[3]]{0}
	};
	operators[OperatorTypes_ADD].parOperatorAppend = new bool*[operators[OperatorTypes_ADD].enumAmount]{
		new bool[operators[OperatorTypes_ADD].params[0]]{false},
			new bool[operators[OperatorTypes_ADD].params[1]]{false, false},
			new bool[operators[OperatorTypes_ADD].params[2]]{false, false},
			new bool[operators[OperatorTypes_ADD].params[3]]{false}
	};

	operators[OperatorTypes_POST].keyword = "post";
	operators[OperatorTypes_POST].enumAmount = 3;
	operators[OperatorTypes_POST].enums = new std::string[operators[OperatorTypes_POST].enumAmount]; operators[OperatorTypes_POST].enums[0] = ""; operators[OperatorTypes_POST].enums[1] = ""; operators[OperatorTypes_POST].enums[2] = "";
	operators[OperatorTypes_POST].byteCodes = new unsigned char[operators[OperatorTypes_POST].enumAmount]{opcode_POSTSTR, opcode_POSTFLOAT, opcode_POSTNMBR};
	operators[OperatorTypes_POST].params = new unsigned char[operators[OperatorTypes_POST].enumAmount]{1, 1,1};
	operators[OperatorTypes_POST].minParams = new unsigned char[operators[OperatorTypes_POST].enumAmount]{ 1, 1,1 };
	operators[OperatorTypes_POST].paramsyntax = new VarType*[operators[OperatorTypes_POST].enumAmount]{
		new VarType[operators[OperatorTypes_POST].params[0]]{VarType_STRING}, 
		new VarType[operators[OperatorTypes_POST].params[1]]{VarType_FLOAT}, 
		new VarType[operators[OperatorTypes_POST].params[2]]{VarType_NUMBER}};
	operators[OperatorTypes_POST].paramTypes = new ParamType*[operators[OperatorTypes_POST].enumAmount]{
		new ParamType[operators[OperatorTypes_POST].params[0]]{ParamType_NA}, 
		new ParamType[operators[OperatorTypes_POST].params[1]]{ParamType_NA},
		new ParamType[operators[OperatorTypes_POST].params[2]]{ParamType_NA}};
	operators[OperatorTypes_POST].optionalPar = new bool*[operators[OperatorTypes_POST].enumAmount]{
		new bool[operators[OperatorTypes_POST].params[0]]{false},
		new bool[operators[OperatorTypes_POST].params[1]]{false},
		new bool[operators[OperatorTypes_POST].params[2]]{false}
	};
	operators[OperatorTypes_POST].readParam = new unsigned char[operators[OperatorTypes_POST].enumAmount]{0, 0, 0};
	operators[OperatorTypes_POST].priority = 0;
	operators[OperatorTypes_POST].returns = new VarType[operators[OperatorTypes_POST].enumAmount]{VarType_NA, VarType_NA, VarType_NA};
	operators[OperatorTypes_POST].returnType = new ParamType[operators[OperatorTypes_POST].enumAmount]{ParamType_NA, ParamType_NA, ParamType_NA};
	operators[OperatorTypes_POST].writeParam = new unsigned char[operators[OperatorTypes_POST].enumAmount]{0,0,0};
	operators[OperatorTypes_POST].shortHandsAmounts = 0;
	operators[OperatorTypes_POST].amountParOperators = new unsigned char[operators[OperatorTypes_POST].enumAmount]{0, 0, 0};
	operators[OperatorTypes_POST].parRepeatsMin = new unsigned char*[operators[OperatorTypes_POST].enumAmount]{
		new unsigned char[operators[OperatorTypes_POST].params[0]]{0},
			new unsigned char[operators[OperatorTypes_POST].params[1]]{0},
			new unsigned char[operators[OperatorTypes_POST].params[2]]{0}
	};
	operators[OperatorTypes_POST].parRepeatsMax = new unsigned char*[operators[OperatorTypes_POST].enumAmount]{
		new unsigned char[operators[OperatorTypes_POST].params[0]]{0},
			new unsigned char[operators[OperatorTypes_POST].params[1]]{0},
			new unsigned char[operators[OperatorTypes_POST].params[2]]{0}
	};
	operators[OperatorTypes_POST].parOperatorAppend = new bool*[operators[OperatorTypes_POST].enumAmount]{
		new bool[operators[OperatorTypes_POST].params[0]]{false},
			new bool[operators[OperatorTypes_POST].params[1]]{false},
			new bool[operators[OperatorTypes_POST].params[2]]{false}
	};

	operators[OperatorTypes_CAST].keyword = "cast";
	operators[OperatorTypes_CAST].enumAmount = 1;
	operators[OperatorTypes_CAST].enums = new std::string[operators[OperatorTypes_CAST].enumAmount]; operators[OperatorTypes_CAST].enums[0] = "";
	operators[OperatorTypes_CAST].byteCodes = new unsigned char[operators[OperatorTypes_CAST].enumAmount]{opcode_CASTFLOAT};
	operators[OperatorTypes_CAST].params = new unsigned char[operators[OperatorTypes_CAST].enumAmount]{1};
	operators[OperatorTypes_CAST].minParams = new unsigned char[operators[OperatorTypes_CAST].enumAmount]{1};
	operators[OperatorTypes_CAST].paramsyntax = new VarType*[operators[OperatorTypes_CAST].enumAmount]{
		new VarType[operators[OperatorTypes_CAST].params[0]]{VarType_NUMBER}};
	operators[OperatorTypes_CAST].paramTypes = new ParamType*[operators[OperatorTypes_CAST].enumAmount]{
		new ParamType[operators[OperatorTypes_CAST].params[0]]{ParamType_NA}};
	operators[OperatorTypes_CAST].optionalPar = new bool*[operators[OperatorTypes_CAST].enumAmount]{
		new bool[operators[OperatorTypes_CAST].params[0]]{false}
	};
	operators[OperatorTypes_CAST].readParam = new unsigned char[operators[OperatorTypes_CAST].enumAmount]{0};
	operators[OperatorTypes_CAST].priority = 0;
	operators[OperatorTypes_CAST].returns = new VarType[operators[OperatorTypes_CAST].enumAmount]{VarType_FLOAT};
	operators[OperatorTypes_CAST].returnType = new ParamType[operators[OperatorTypes_CAST].enumAmount]{ParamType_VAR};
	operators[OperatorTypes_CAST].writeParam = new unsigned char[operators[OperatorTypes_CAST].enumAmount]{0};
	operators[OperatorTypes_CAST].shortHandsAmounts = 0;
	operators[OperatorTypes_CAST].amountParOperators = new unsigned char[operators[OperatorTypes_CAST].enumAmount]{0};
	operators[OperatorTypes_CAST].parRepeatsMin = new unsigned char*[operators[OperatorTypes_CAST].enumAmount]{
		new unsigned char[operators[OperatorTypes_CAST].params[0]]{0}
	};
	operators[OperatorTypes_CAST].parRepeatsMax = new unsigned char*[operators[OperatorTypes_CAST].enumAmount]{
		new unsigned char[operators[OperatorTypes_CAST].params[0]]{0}
	};
	operators[OperatorTypes_CAST].parOperatorAppend = new bool*[operators[OperatorTypes_CAST].enumAmount]{
		new bool[operators[OperatorTypes_CAST].params[0]]{false}
	};

	setupArithmeticOps();

	operators[OperatorTypes_EQUAL].keyword = "eql";
	operators[OperatorTypes_EQUAL].enumAmount = 8;
	operators[OperatorTypes_EQUAL].enums = new std::string[operators[OperatorTypes_EQUAL].enumAmount]; operators[OperatorTypes_EQUAL].enums[0] = ""; operators[OperatorTypes_EQUAL].enums[1] = ""; operators[OperatorTypes_EQUAL].enums[2] = "", operators[OperatorTypes_EQUAL].enums[3] = ""; operators[OperatorTypes_EQUAL].enums[4] = ""; operators[OperatorTypes_EQUAL].enums[5] = ""; operators[OperatorTypes_EQUAL].enums[6] = ""; operators[OperatorTypes_EQUAL].enums[7] = "";
	operators[OperatorTypes_EQUAL].byteCodes = new unsigned char[operators[OperatorTypes_EQUAL].enumAmount]{opcode_NUMEQUAL2CONST, opcode_NUMEQUAL2CONST, opcode_NUMEQUALVAR, opcode_NUMEQUALFLOAT, opcode_NUMEQUALFLOAT, opcode_NUMEQUALFLOAT, opcode_STREQUAL2CONST, opcode_STREQUALVAR};
	operators[OperatorTypes_EQUAL].params = new unsigned char[operators[OperatorTypes_EQUAL].enumAmount]{2, 2, 2, 2, 2, 2, 2, 2};
	operators[OperatorTypes_EQUAL].minParams = new unsigned char[operators[OperatorTypes_EQUAL].enumAmount]{ 2, 2, 2, 2, 2, 2, 2, 2 };
	operators[OperatorTypes_EQUAL].paramsyntax = new VarType*[operators[OperatorTypes_EQUAL].enumAmount]{
		new VarType[operators[OperatorTypes_EQUAL].params[0]]{VarType_NUMBER, VarType_NUMBER}, 
			new VarType[operators[OperatorTypes_EQUAL].params[1]]{VarType_FLOAT, VarType_FLOAT},
			new VarType[operators[OperatorTypes_EQUAL].params[2]]{VarType_NUMBER, VarType_NUMBER},
			new VarType[operators[OperatorTypes_EQUAL].params[3]]{VarType_FLOAT, VarType_FLOAT},
			new VarType[operators[OperatorTypes_EQUAL].params[4]]{VarType_NUMBER, VarType_FLOAT},
			new VarType[operators[OperatorTypes_EQUAL].params[5]]{VarType_FLOAT, VarType_NUMBER},

			new VarType[operators[OperatorTypes_EQUAL].params[6]]{VarType_STRING, VarType_STRING}, 
			new VarType[operators[OperatorTypes_EQUAL].params[7]]{VarType_STRING, VarType_STRING}};
	operators[OperatorTypes_EQUAL].paramTypes = new ParamType*[operators[OperatorTypes_EQUAL].enumAmount]{
		new ParamType[operators[OperatorTypes_EQUAL].params[0]]{ParamType_CONST, ParamType_CONST}, 
		new ParamType[operators[OperatorTypes_EQUAL].params[1]]{ParamType_CONST, ParamType_CONST},
		new ParamType[operators[OperatorTypes_EQUAL].params[2]]{ParamType_NA, ParamType_NA},
		new ParamType[operators[OperatorTypes_EQUAL].params[3]]{ParamType_NA, ParamType_NA},
		new ParamType[operators[OperatorTypes_EQUAL].params[4]]{ParamType_NA, ParamType_NA},
		new ParamType[operators[OperatorTypes_EQUAL].params[5]]{ParamType_NA, ParamType_NA},

		new ParamType[operators[OperatorTypes_EQUAL].params[6]]{ParamType_CONST, ParamType_CONST},
		new ParamType[operators[OperatorTypes_EQUAL].params[7]]{ParamType_NA, ParamType_NA}};
	operators[OperatorTypes_EQUAL].optionalPar = new bool*[operators[OperatorTypes_EQUAL].enumAmount]{
		new bool[operators[OperatorTypes_EQUAL].params[0]]{ false, false },
		new bool[operators[OperatorTypes_EQUAL].params[1]]{ false, false },
		new bool[operators[OperatorTypes_EQUAL].params[2]]{ false, false },
		new bool[operators[OperatorTypes_EQUAL].params[3]]{ false, false },
		new bool[operators[OperatorTypes_EQUAL].params[4]]{ false, false },
		new bool[operators[OperatorTypes_EQUAL].params[5]]{ false, false },
		new bool[operators[OperatorTypes_EQUAL].params[6]]{ false, false },
		new bool[operators[OperatorTypes_EQUAL].params[7]]{ false, false }
	};
	operators[OperatorTypes_EQUAL].readParam = new unsigned char[operators[OperatorTypes_EQUAL].enumAmount]{0, 0, 0, 0, 0, 0, 0, 0};
	operators[OperatorTypes_EQUAL].returns = new VarType[operators[OperatorTypes_EQUAL].enumAmount]{VarType_NUMBER, VarType_NUMBER, VarType_NUMBER, VarType_NUMBER, VarType_NUMBER, VarType_NUMBER, VarType_NUMBER, VarType_NUMBER};
	operators[OperatorTypes_EQUAL].returnType = new ParamType[operators[OperatorTypes_EQUAL].enumAmount]{ParamType_CONST, ParamType_CONST, ParamType_VAR, ParamType_VAR, ParamType_VAR, ParamType_VAR, ParamType_CONST, ParamType_VAR};
	operators[OperatorTypes_EQUAL].writeParam = new unsigned char[operators[OperatorTypes_EQUAL].enumAmount]{0, 0, 0, 0, 0, 0, 0, 0};
	operators[OperatorTypes_EQUAL].priority = 4;
	operators[OperatorTypes_EQUAL].shortHandsAmounts = 1;
	operators[OperatorTypes_EQUAL].shortHands = new std::string[operators[OperatorTypes_EQUAL].shortHandsAmounts]; operators[OperatorTypes_EQUAL].shortHands[0] = ":";
	operators[OperatorTypes_EQUAL].rightParams = new unsigned char[operators[OperatorTypes_EQUAL].shortHandsAmounts]{1};
	operators[OperatorTypes_EQUAL].leftParams = new unsigned char[operators[OperatorTypes_EQUAL].shortHandsAmounts]{1};
	operators[OperatorTypes_EQUAL].shortFlipParams = new bool[operators[OperatorTypes_EQUAL].shortHandsAmounts]{false};
	operators[OperatorTypes_EQUAL].amountParOperators = new unsigned char[operators[OperatorTypes_EQUAL].enumAmount]{0, 0, 0, 0, 1, 1, 0, 0};
	operators[OperatorTypes_EQUAL].parRepeatsMin = new unsigned char*[operators[OperatorTypes_EQUAL].enumAmount]{
		new unsigned char[operators[OperatorTypes_EQUAL].params[0]]{0, 0},
			new unsigned char[operators[OperatorTypes_EQUAL].params[1]]{0, 0},
			new unsigned char[operators[OperatorTypes_EQUAL].params[2]]{0, 0},
			new unsigned char[operators[OperatorTypes_EQUAL].params[3]]{0, 0},
			new unsigned char[operators[OperatorTypes_EQUAL].params[4]]{0, 0},
			new unsigned char[operators[OperatorTypes_EQUAL].params[5]]{0, 0},
			new unsigned char[operators[OperatorTypes_EQUAL].params[6]]{0, 0},
			new unsigned char[operators[OperatorTypes_EQUAL].params[7]]{0, 0}
	};
	operators[OperatorTypes_EQUAL].parRepeatsMax = new unsigned char*[operators[OperatorTypes_EQUAL].enumAmount]{
		new unsigned char[operators[OperatorTypes_EQUAL].params[0]]{0, 0},
			new unsigned char[operators[OperatorTypes_EQUAL].params[1]]{0, 0},
			new unsigned char[operators[OperatorTypes_EQUAL].params[2]]{0, 0},
			new unsigned char[operators[OperatorTypes_EQUAL].params[3]]{0, 0},
			new unsigned char[operators[OperatorTypes_EQUAL].params[4]]{0, 0},
			new unsigned char[operators[OperatorTypes_EQUAL].params[5]]{0, 0},
			new unsigned char[operators[OperatorTypes_EQUAL].params[6]]{0, 0},
			new unsigned char[operators[OperatorTypes_EQUAL].params[7]]{0, 0}
	};
	operators[OperatorTypes_EQUAL].parOperators = new OperatorTypes*[operators[OperatorTypes_EQUAL].enumAmount]{
		nullptr,
			nullptr,
			nullptr,
			nullptr,
			new OperatorTypes[operators[OperatorTypes_EQUAL].amountParOperators[4]]{OperatorTypes_CAST},
			new OperatorTypes[operators[OperatorTypes_EQUAL].amountParOperators[5]]{OperatorTypes_CAST},
			nullptr,
			nullptr
	};
	operators[OperatorTypes_EQUAL].parOperator = new unsigned char*[operators[OperatorTypes_EQUAL].enumAmount]{
		nullptr,
			nullptr,
			nullptr,
			nullptr,
			new unsigned char[operators[OperatorTypes_EQUAL].params[4]]{1, 0},
			new unsigned char[operators[OperatorTypes_EQUAL].params[5]]{0, 1},
			nullptr,
			nullptr
	};
	operators[OperatorTypes_EQUAL].parOperatorAppend = new bool*[operators[OperatorTypes_EQUAL].enumAmount]{
		new bool[operators[OperatorTypes_EQUAL].params[0]]{false, false},
			new bool[operators[OperatorTypes_EQUAL].params[1]]{false, false},
			new bool[operators[OperatorTypes_EQUAL].params[2]]{false, false},
			new bool[operators[OperatorTypes_EQUAL].params[3]]{false, false}, 
			new bool[operators[OperatorTypes_EQUAL].params[4]]{false, false},
			new bool[operators[OperatorTypes_EQUAL].params[5]]{false, false}, 
			new bool[operators[OperatorTypes_EQUAL].params[6]]{false, false},
			new bool[operators[OperatorTypes_EQUAL].params[7]]{false, false}
	};

	operators[OperatorTypes_GREATER].keyword = "grtr";
	operators[OperatorTypes_GREATER].enumAmount = 6;
	operators[OperatorTypes_GREATER].enums = new std::string[operators[OperatorTypes_GREATER].enumAmount]; operators[OperatorTypes_GREATER].enums[0] = ""; operators[OperatorTypes_GREATER].enums[1] = ""; operators[OperatorTypes_GREATER].enums[2] = ""; operators[OperatorTypes_GREATER].enums[3] = ""; operators[OperatorTypes_GREATER].enums[4] = ""; operators[OperatorTypes_GREATER].enums[5] = "";
	operators[OperatorTypes_GREATER].byteCodes = new unsigned char[operators[OperatorTypes_GREATER].enumAmount]{opcode_NUMGRT2CONST, opcode_NUMGRT2CONST, opcode_NUMGRTFLOAT, opcode_NUMGRTVAR, opcode_NUMGRTFLOAT, opcode_NUMGRTFLOAT};
	operators[OperatorTypes_GREATER].params = new unsigned char[operators[OperatorTypes_GREATER].enumAmount]{2, 2, 2, 2, 2, 2};
	operators[OperatorTypes_GREATER].minParams = new unsigned char[operators[OperatorTypes_GREATER].enumAmount]{ 2, 2, 2, 2, 2, 2};
	operators[OperatorTypes_GREATER].paramsyntax = new VarType*[operators[OperatorTypes_GREATER].enumAmount]{
		new VarType[operators[OperatorTypes_GREATER].params[0]]{VarType_NUMBER, VarType_NUMBER}, 
			new VarType[operators[OperatorTypes_GREATER].params[1]]{VarType_FLOAT, VarType_FLOAT},
			new VarType[operators[OperatorTypes_GREATER].params[2]]{VarType_FLOAT, VarType_FLOAT},
			new VarType[operators[OperatorTypes_GREATER].params[3]]{VarType_NUMBER, VarType_NUMBER}, 
			new VarType[operators[OperatorTypes_GREATER].params[4]]{VarType_NUMBER, VarType_FLOAT},
			new VarType[operators[OperatorTypes_GREATER].params[5]]{VarType_FLOAT, VarType_NUMBER}};
	operators[OperatorTypes_GREATER].paramTypes = new ParamType*[operators[OperatorTypes_GREATER].enumAmount]{
		new ParamType[operators[OperatorTypes_GREATER].params[0]]{ParamType_CONST, ParamType_CONST}, 
			new ParamType[operators[OperatorTypes_GREATER].params[1]]{ParamType_CONST, ParamType_CONST},
			new ParamType[operators[OperatorTypes_GREATER].params[2]]{ParamType_NA, ParamType_NA},
			new ParamType[operators[OperatorTypes_GREATER].params[3]]{ParamType_NA, ParamType_NA},
			new ParamType[operators[OperatorTypes_GREATER].params[4]]{ParamType_NA, ParamType_NA},
			new ParamType[operators[OperatorTypes_GREATER].params[5]]{ParamType_NA, ParamType_NA}};
	operators[OperatorTypes_GREATER].optionalPar = new bool*[operators[OperatorTypes_GREATER].enumAmount]{
		new bool[operators[OperatorTypes_GREATER].params[0]]{ false, false },
		new bool[operators[OperatorTypes_GREATER].params[1]]{ false, false },
		new bool[operators[OperatorTypes_GREATER].params[2]]{ false, false },
		new bool[operators[OperatorTypes_GREATER].params[3]]{ false, false },
		new bool[operators[OperatorTypes_GREATER].params[4]]{ false, false },
		new bool[operators[OperatorTypes_GREATER].params[5]]{ false, false }
	};
	operators[OperatorTypes_GREATER].readParam = new unsigned char[operators[OperatorTypes_GREATER].enumAmount]{0, 0, 0, 0, 0, 0};
	operators[OperatorTypes_GREATER].returns = new VarType[operators[OperatorTypes_GREATER].enumAmount]{VarType_NUMBER, VarType_NUMBER, VarType_NUMBER, VarType_NUMBER, VarType_NUMBER, VarType_NUMBER};
	operators[OperatorTypes_GREATER].returnType = new ParamType[operators[OperatorTypes_GREATER].enumAmount]{ParamType_CONST, ParamType_CONST, ParamType_VAR, ParamType_VAR, ParamType_VAR, ParamType_VAR};
	operators[OperatorTypes_GREATER].writeParam = new unsigned char[operators[OperatorTypes_GREATER].enumAmount]{0, 0, 0, 0, 0, 0};
	operators[OperatorTypes_GREATER].priority = 4;
	operators[OperatorTypes_GREATER].shortHandsAmounts = 2;
	operators[OperatorTypes_GREATER].shortHands = new std::string[operators[OperatorTypes_GREATER].shortHandsAmounts]; operators[OperatorTypes_GREATER].shortHands[0] = ">"; operators[OperatorTypes_GREATER].shortHands[1] = "<";
	operators[OperatorTypes_GREATER].rightParams = new unsigned char[operators[OperatorTypes_GREATER].shortHandsAmounts]{1,1};
	operators[OperatorTypes_GREATER].leftParams = new unsigned char[operators[OperatorTypes_GREATER].shortHandsAmounts]{1,1};
	operators[OperatorTypes_GREATER].shortFlipParams = new bool[operators[OperatorTypes_GREATER].shortHandsAmounts]{false, true};
	operators[OperatorTypes_GREATER].amountParOperators = new unsigned char[operators[OperatorTypes_GREATER].enumAmount]{0, 0, 0, 0, 1, 1};
	operators[OperatorTypes_GREATER].parOperators = new OperatorTypes*[operators[OperatorTypes_GREATER].enumAmount]{
		nullptr,
			nullptr,
			nullptr,
			nullptr,
			new OperatorTypes[operators[OperatorTypes_GREATER].amountParOperators[4]]{OperatorTypes_CAST},
			new OperatorTypes[operators[OperatorTypes_GREATER].amountParOperators[5]]{OperatorTypes_CAST}
	};
	operators[OperatorTypes_GREATER].parOperator = new unsigned char*[operators[OperatorTypes_GREATER].enumAmount]{
		nullptr,
			nullptr,
			nullptr,
			nullptr,
			new unsigned char[operators[OperatorTypes_GREATER].params[4]]{1, 0},
			new unsigned char[operators[OperatorTypes_GREATER].params[5]]{0, 1}
	};
	operators[OperatorTypes_GREATER].parRepeatsMin = new unsigned char*[operators[OperatorTypes_GREATER].enumAmount]{
		new unsigned char[operators[OperatorTypes_GREATER].params[0]]{0, 0},
			new unsigned char[operators[OperatorTypes_GREATER].params[1]]{0, 0},
			new unsigned char[operators[OperatorTypes_GREATER].params[2]]{0, 0},
			new unsigned char[operators[OperatorTypes_GREATER].params[3]]{0, 0},
			new unsigned char[operators[OperatorTypes_GREATER].params[4]]{0, 0},
			new unsigned char[operators[OperatorTypes_GREATER].params[5]]{0, 0}
	};
	operators[OperatorTypes_GREATER].parRepeatsMax = new unsigned char*[operators[OperatorTypes_GREATER].enumAmount]{
		new unsigned char[operators[OperatorTypes_GREATER].params[0]]{0, 0},
			new unsigned char[operators[OperatorTypes_GREATER].params[1]]{0, 0},
			new unsigned char[operators[OperatorTypes_GREATER].params[2]]{0, 0},
			new unsigned char[operators[OperatorTypes_GREATER].params[3]]{0, 0},
			new unsigned char[operators[OperatorTypes_GREATER].params[4]]{0, 0},
			new unsigned char[operators[OperatorTypes_GREATER].params[5]]{0, 0}
	};
	operators[OperatorTypes_GREATER].parOperatorAppend = new bool*[operators[OperatorTypes_GREATER].enumAmount]{
		new bool[operators[OperatorTypes_GREATER].params[0]]{false, false},
			new bool[operators[OperatorTypes_GREATER].params[1]]{false, false}, 
			new bool[operators[OperatorTypes_GREATER].params[2]]{false, false},
			new bool[operators[OperatorTypes_GREATER].params[3]]{false, false},
			new bool[operators[OperatorTypes_GREATER].params[4]]{false, false},
			new bool[operators[OperatorTypes_GREATER].params[5]]{false, false}
	};

	operators[OperatorTypes_GET].keyword = "get";
	operators[OperatorTypes_GET].enumAmount = 2;
	operators[OperatorTypes_GET].enums = new std::string[operators[OperatorTypes_GET].enumAmount]; operators[OperatorTypes_GET].enums[0] = "screenX"; operators[OperatorTypes_GET].enums[1] = "screenY";
	operators[OperatorTypes_GET].byteCodes = new unsigned char[operators[OperatorTypes_GET].enumAmount]{opcode_GETSCRNX, opcode_GETSCRNY};
	operators[OperatorTypes_GET].params = new unsigned char[operators[OperatorTypes_GET].enumAmount]{0,0};
	operators[OperatorTypes_GET].minParams = new unsigned char[operators[OperatorTypes_GET].enumAmount]{ 0,0 };
	operators[OperatorTypes_GET].paramsyntax = new VarType*[operators[OperatorTypes_GET].enumAmount]{};
	operators[OperatorTypes_GET].paramTypes = new ParamType*[operators[OperatorTypes_GET].enumAmount]{};
	operators[OperatorTypes_GET].optionalPar = new bool*[operators[OperatorTypes_GET].enumAmount]{};
	operators[OperatorTypes_GET].readParam = new unsigned char[operators[OperatorTypes_GET].enumAmount]{0, 0};
	operators[OperatorTypes_GET].returns = new VarType[operators[OperatorTypes_GET].enumAmount]{VarType_NUMBER, VarType_NUMBER};
	operators[OperatorTypes_GET].returnType = new ParamType[operators[OperatorTypes_GET].enumAmount]{ParamType_VAR, ParamType_VAR};
	operators[OperatorTypes_GET].writeParam = new unsigned char[operators[OperatorTypes_GET].enumAmount]{0, 0};
	operators[OperatorTypes_GET].priority = 0;
	operators[OperatorTypes_GET].shortHandsAmounts = 0;
	operators[OperatorTypes_GET].amountParOperators = new unsigned char[operators[OperatorTypes_GET].enumAmount]{0, 0};
	operators[OperatorTypes_GET].parRepeatsMin = new unsigned char*[operators[OperatorTypes_GET].enumAmount]{};
	operators[OperatorTypes_GET].parRepeatsMax = new unsigned char*[operators[OperatorTypes_GET].enumAmount]{};
	operators[OperatorTypes_GET].parOperatorAppend = new bool*[operators[OperatorTypes_GET].enumAmount]{};

	operators[OperatorTypes_SNAP].keyword = "snap";
	operators[OperatorTypes_SNAP].enumAmount = 1;
	operators[OperatorTypes_SNAP].enums = new std::string[operators[OperatorTypes_SNAP].enumAmount]; operators[OperatorTypes_SNAP].enums[0] = "";
	operators[OperatorTypes_SNAP].byteCodes = new unsigned char[operators[OperatorTypes_SNAP].enumAmount]{opcode_RESNAP};
	operators[OperatorTypes_SNAP].params = new unsigned char[operators[OperatorTypes_SNAP].enumAmount]{3};
	operators[OperatorTypes_SNAP].minParams = new unsigned char[operators[OperatorTypes_SNAP].enumAmount]{ 3 };
	operators[OperatorTypes_SNAP].paramsyntax = new VarType*[operators[OperatorTypes_SNAP].enumAmount]{
		new VarType[operators[OperatorTypes_SNAP].params[0]]{VarType_NUMBER, VarType_NUMBER, VarType_NUMBER}};
	operators[OperatorTypes_SNAP].paramTypes = new ParamType*[operators[OperatorTypes_SNAP].enumAmount]{
		new ParamType[operators[OperatorTypes_SNAP].params[0]]{ParamType_NA, ParamType_NA, ParamType_NA}};
	operators[OperatorTypes_SNAP].optionalPar = new bool*[operators[OperatorTypes_SNAP].enumAmount]{
		new bool[operators[OperatorTypes_SNAP].params[0]]{ false, false, false }
	};
	operators[OperatorTypes_SNAP].readParam = new unsigned char[operators[OperatorTypes_SNAP].enumAmount]{0};
	operators[OperatorTypes_SNAP].returns = new VarType[operators[OperatorTypes_SNAP].enumAmount]{VarType_NA};
	operators[OperatorTypes_SNAP].returnType = new ParamType[operators[OperatorTypes_SNAP].enumAmount]{ParamType_NA};
	operators[OperatorTypes_SNAP].writeParam = new unsigned char[operators[OperatorTypes_SNAP].enumAmount]{0};
	operators[OperatorTypes_SNAP].priority = 0;
	operators[OperatorTypes_SNAP].shortHandsAmounts = 0;
	operators[OperatorTypes_SNAP].amountParOperators = new unsigned char[operators[OperatorTypes_SNAP].enumAmount]{0};
	operators[OperatorTypes_SNAP].parRepeatsMin = new unsigned char*[operators[OperatorTypes_SNAP].enumAmount]{
		new unsigned char[operators[OperatorTypes_SNAP].params[0]]{0, 0, 0}
	};
	operators[OperatorTypes_SNAP].parRepeatsMax = new unsigned char*[operators[OperatorTypes_SNAP].enumAmount]{
		new unsigned char[operators[OperatorTypes_SNAP].params[0]]{0, 0, 0}
	};
	operators[OperatorTypes_SNAP].parOperatorAppend = new bool*[operators[OperatorTypes_SNAP].enumAmount]{
		new bool[operators[OperatorTypes_SNAP].params[0]]{false, false, false}
	};

	operators[OperatorTypes_POS].keyword = "pos";
	operators[OperatorTypes_POS].enumAmount = 1;
	operators[OperatorTypes_POS].enums = new std::string[operators[OperatorTypes_POS].enumAmount]; operators[OperatorTypes_POS].enums[0] = "2d";
	operators[OperatorTypes_POS].byteCodes = new unsigned char[operators[OperatorTypes_POS].enumAmount]{opcode_RESNAP};
	operators[OperatorTypes_POS].params = new unsigned char[operators[OperatorTypes_POS].enumAmount]{3};
	operators[OperatorTypes_POS].minParams = new unsigned char[operators[OperatorTypes_POS].enumAmount]{ 3 };
	operators[OperatorTypes_POS].paramsyntax = new VarType*[operators[OperatorTypes_POS].enumAmount]{
		new VarType[operators[OperatorTypes_POS].params[0]]{VarType_NUMBER, VarType_NUMBER, VarType_NUMBER}};
	operators[OperatorTypes_POS].paramTypes = new ParamType*[operators[OperatorTypes_POS].enumAmount]{
		new ParamType[operators[OperatorTypes_POS].params[0]]{ParamType_NA, ParamType_NA, ParamType_NA}};
	operators[OperatorTypes_POS].optionalPar = new bool*[operators[OperatorTypes_POS].enumAmount]{
		new bool[operators[OperatorTypes_POS].params[0]]{ false, false, false }
	};
	operators[OperatorTypes_POS].readParam = new unsigned char[operators[OperatorTypes_POS].enumAmount]{0};
	operators[OperatorTypes_POS].returns = new VarType[operators[OperatorTypes_POS].enumAmount]{VarType_NA};
	operators[OperatorTypes_POS].returnType = new ParamType[operators[OperatorTypes_POS].enumAmount]{ParamType_NA};
	operators[OperatorTypes_POS].writeParam = new unsigned char[operators[OperatorTypes_POS].enumAmount]{0};
	operators[OperatorTypes_POS].priority = 0;
	operators[OperatorTypes_POS].shortHandsAmounts = 0;
	operators[OperatorTypes_POS].amountParOperators = new unsigned char[operators[OperatorTypes_POS].enumAmount]{0};
	operators[OperatorTypes_POS].parRepeatsMin = new unsigned char*[operators[OperatorTypes_POS].enumAmount]{
		new unsigned char[operators[OperatorTypes_POS].params[0]]{0, 0, 0}
	};
	operators[OperatorTypes_POS].parRepeatsMax = new unsigned char*[operators[OperatorTypes_POS].enumAmount]{
		new unsigned char[operators[OperatorTypes_POS].params[0]]{0, 0, 0}
	};
	operators[OperatorTypes_POS].parOperatorAppend = new bool*[operators[OperatorTypes_POS].enumAmount]{
		new bool[operators[OperatorTypes_POS].params[0]]{false, false, false}
	};

	operators[OperatorTypes_SIZE].keyword = "size";
	operators[OperatorTypes_SIZE].enumAmount = 1;
	operators[OperatorTypes_SIZE].enums = new std::string[operators[OperatorTypes_SIZE].enumAmount]; operators[OperatorTypes_SIZE].enums[0] = "2d";
	operators[OperatorTypes_SIZE].byteCodes = new unsigned char[operators[OperatorTypes_SIZE].enumAmount]{opcode_RESNAP};
	operators[OperatorTypes_SIZE].params = new unsigned char[operators[OperatorTypes_SIZE].enumAmount]{3};
	operators[OperatorTypes_SIZE].minParams = new unsigned char[operators[OperatorTypes_SIZE].enumAmount]{ 3 };
	operators[OperatorTypes_SIZE].paramsyntax = new VarType*[operators[OperatorTypes_SIZE].enumAmount]{
		new VarType[operators[OperatorTypes_SIZE].params[0]]{VarType_NUMBER, VarType_NUMBER, VarType_NUMBER}};
	operators[OperatorTypes_SIZE].paramTypes = new ParamType*[operators[OperatorTypes_SIZE].enumAmount]{
		new ParamType[operators[OperatorTypes_SIZE].params[0]]{ParamType_NA, ParamType_NA, ParamType_NA}};
	operators[OperatorTypes_SIZE].optionalPar = new bool*[operators[OperatorTypes_SIZE].enumAmount]{
		new bool[operators[OperatorTypes_SIZE].params[0]]{ false, false, false }
	};
	operators[OperatorTypes_SIZE].readParam = new unsigned char[operators[OperatorTypes_SIZE].enumAmount]{0};
	operators[OperatorTypes_SIZE].returns = new VarType[operators[OperatorTypes_SIZE].enumAmount]{VarType_NA};
	operators[OperatorTypes_SIZE].returnType = new ParamType[operators[OperatorTypes_SIZE].enumAmount]{ParamType_NA};
	operators[OperatorTypes_SIZE].writeParam = new unsigned char[operators[OperatorTypes_SIZE].enumAmount]{0};
	operators[OperatorTypes_SIZE].priority = 0;
	operators[OperatorTypes_SIZE].shortHandsAmounts = 0;
	operators[OperatorTypes_SIZE].amountParOperators = new unsigned char[operators[OperatorTypes_SIZE].enumAmount]{0};
	operators[OperatorTypes_SIZE].parRepeatsMin = new unsigned char*[operators[OperatorTypes_SIZE].enumAmount]{
		new unsigned char[operators[OperatorTypes_SIZE].params[0]]{0, 0, 0}
	};
	operators[OperatorTypes_SIZE].parRepeatsMax = new unsigned char*[operators[OperatorTypes_SIZE].enumAmount]{
		new unsigned char[operators[OperatorTypes_SIZE].params[0]]{0, 0, 0}
	};
	operators[OperatorTypes_SIZE].parOperatorAppend = new bool*[operators[OperatorTypes_SIZE].enumAmount]{
		new bool[operators[OperatorTypes_SIZE].params[0]]{false, false, false}
	};

	operators[OperatorTypes_RUN].keyword = "run";
	operators[OperatorTypes_RUN].enumAmount = 1;
	operators[OperatorTypes_RUN].enums = new std::string[operators[OperatorTypes_RUN].enumAmount]; operators[OperatorTypes_RUN].enums[0] = "script";
	operators[OperatorTypes_RUN].byteCodes = new unsigned char[operators[OperatorTypes_RUN].enumAmount]{opcode_RNSCRPT};
	operators[OperatorTypes_RUN].params = new unsigned char[operators[OperatorTypes_RUN].enumAmount]{2};
	operators[OperatorTypes_RUN].minParams = new unsigned char[operators[OperatorTypes_RUN].enumAmount]{ 1 };
	operators[OperatorTypes_RUN].paramsyntax = new VarType*[operators[OperatorTypes_RUN].enumAmount]{
		new VarType[operators[OperatorTypes_RUN].params[0]]{VarType_NUMBER, VarType_NA}};
	operators[OperatorTypes_RUN].paramTypes = new ParamType*[operators[OperatorTypes_RUN].enumAmount]{
		new ParamType[operators[OperatorTypes_RUN].params[0]]{ParamType_NA, ParamType_NA}};
	operators[OperatorTypes_RUN].optionalPar = new bool*[operators[OperatorTypes_RUN].enumAmount]{
		new bool[operators[OperatorTypes_RUN].params[0]]{ false, true }
	};
	operators[OperatorTypes_RUN].readParam = new unsigned char[operators[OperatorTypes_RUN].enumAmount]{0};
	operators[OperatorTypes_RUN].returns = new VarType[operators[OperatorTypes_RUN].enumAmount]{VarType_NA};
	operators[OperatorTypes_RUN].returnType = new ParamType[operators[OperatorTypes_RUN].enumAmount]{ParamType_NA};
	operators[OperatorTypes_RUN].writeParam = new unsigned char[operators[OperatorTypes_RUN].enumAmount]{0};
	operators[OperatorTypes_RUN].priority = 0;
	operators[OperatorTypes_RUN].shortHandsAmounts = 0;
	operators[OperatorTypes_RUN].amountParOperators = new unsigned char[operators[OperatorTypes_RUN].enumAmount]{1};
	operators[OperatorTypes_RUN].parRepeatsMin = new unsigned char*[operators[OperatorTypes_RUN].enumAmount]{
		new unsigned char[operators[OperatorTypes_RUN].params[0]]{0, 0}
	};
	operators[OperatorTypes_RUN].parRepeatsMax = new unsigned char*[operators[OperatorTypes_RUN].enumAmount]{
		new unsigned char[operators[OperatorTypes_RUN].params[0]]{0, 255}
	};
	operators[OperatorTypes_RUN].parOperators = new OperatorTypes*[operators[OperatorTypes_RUN].enumAmount]{
		new OperatorTypes[operators[OperatorTypes_RUN].amountParOperators[0]]{OperatorTypes_SETPARAM}
	};
	operators[OperatorTypes_RUN].parOperator = new unsigned char*[operators[OperatorTypes_RUN].enumAmount]{
		new unsigned char[operators[OperatorTypes_RUN].params[0]]{1,1}
	};
	operators[OperatorTypes_RUN].parOperatorAppend = new bool*[operators[OperatorTypes_RUN].enumAmount]{
		new bool[operators[OperatorTypes_RUN].params[0]]{true, false}
	};

	operators[OperatorTypes_SETPARAM].keyword = "stprm";
	operators[OperatorTypes_SETPARAM].enumAmount = 2;
	operators[OperatorTypes_SETPARAM].enums = new std::string[operators[OperatorTypes_SETPARAM].enumAmount]; operators[OperatorTypes_SETPARAM].enums[0] = "num"; operators[OperatorTypes_SETPARAM].enums[1] = "str";
	operators[OperatorTypes_SETPARAM].byteCodes = new unsigned char[operators[OperatorTypes_SETPARAM].enumAmount]{opcode_STSCRPTPRMNMBR,opcode_STSCRPTPRMSTR};
	operators[OperatorTypes_SETPARAM].params = new unsigned char[operators[OperatorTypes_SETPARAM].enumAmount]{2,2};
	operators[OperatorTypes_SETPARAM].minParams = new unsigned char[operators[OperatorTypes_SETPARAM].enumAmount]{ 2,2 };
	operators[OperatorTypes_SETPARAM].paramsyntax = new VarType*[operators[OperatorTypes_SETPARAM].enumAmount]{
		new VarType[operators[OperatorTypes_SETPARAM].params[0]]{VarType_NUMBER, VarType_NUMBER},
			new VarType[operators[OperatorTypes_SETPARAM].params[1]]{VarType_STRING, VarType_NUMBER}
	};
	operators[OperatorTypes_SETPARAM].paramTypes = new ParamType*[operators[OperatorTypes_SETPARAM].enumAmount]{
		new ParamType[operators[OperatorTypes_SETPARAM].params[0]]{ParamType_NA, ParamType_NA},
			new ParamType[operators[OperatorTypes_SETPARAM].params[1]]{ParamType_NA, ParamType_NA}
	};
	operators[OperatorTypes_SETPARAM].optionalPar = new bool*[operators[OperatorTypes_SETPARAM].enumAmount]{
		new bool[operators[OperatorTypes_SETPARAM].params[0]]{ false, false },
		new bool[operators[OperatorTypes_SETPARAM].params[1]]{ false, false }
	};
	operators[OperatorTypes_SETPARAM].readParam = new unsigned char[operators[OperatorTypes_SETPARAM].enumAmount]{0,0};
	operators[OperatorTypes_SETPARAM].returns = new VarType[operators[OperatorTypes_SETPARAM].enumAmount]{VarType_NA, VarType_NA};
	operators[OperatorTypes_SETPARAM].returnType = new ParamType[operators[OperatorTypes_SETPARAM].enumAmount]{ParamType_NA, ParamType_NA};
	operators[OperatorTypes_SETPARAM].writeParam = new unsigned char[operators[OperatorTypes_SETPARAM].enumAmount]{0,0};
	operators[OperatorTypes_SETPARAM].priority = 0;
	operators[OperatorTypes_SETPARAM].shortHandsAmounts = 0;
	operators[OperatorTypes_SETPARAM].amountParOperators = new unsigned char[operators[OperatorTypes_SETPARAM].enumAmount]{0,0};
	operators[OperatorTypes_SETPARAM].parRepeatsMin = new unsigned char*[operators[OperatorTypes_SETPARAM].enumAmount]{
		new unsigned char[operators[OperatorTypes_SETPARAM].params[0]]{0, 0},
			new unsigned char[operators[OperatorTypes_SETPARAM].params[1]]{0, 0}
	};
	operators[OperatorTypes_SETPARAM].parRepeatsMax = new unsigned char*[operators[OperatorTypes_SETPARAM].enumAmount]{
		new unsigned char[operators[OperatorTypes_SETPARAM].params[0]]{0, 0},
			new unsigned char[operators[OperatorTypes_SETPARAM].params[1]]{0, 0}
	};
	operators[OperatorTypes_SETPARAM].parOperatorAppend = new bool*[operators[OperatorTypes_SETPARAM].enumAmount]{
		new bool[operators[OperatorTypes_SETPARAM].params[0]]{false, false},
			new bool[operators[OperatorTypes_SETPARAM].params[1]]{false, false}
	};


	operators[OperatorTypes_EXPORT].keyword = "export";
	operators[OperatorTypes_EXPORT].enumAmount = 2;
	operators[OperatorTypes_EXPORT].enums = new std::string[operators[OperatorTypes_EXPORT].enumAmount]; operators[OperatorTypes_EXPORT].enums[0] = ""; operators[OperatorTypes_EXPORT].enums[1] = "";
	operators[OperatorTypes_EXPORT].byteCodes = new unsigned char[operators[OperatorTypes_EXPORT].enumAmount]{opcode_EXPRTCNST, opcode_EXPRTVAR};
	operators[OperatorTypes_EXPORT].params = new unsigned char[operators[OperatorTypes_EXPORT].enumAmount]{2, 2};
	operators[OperatorTypes_EXPORT].minParams = new unsigned char[operators[OperatorTypes_EXPORT].enumAmount]{2, 2};
	operators[OperatorTypes_EXPORT].paramsyntax = new VarType*[operators[OperatorTypes_EXPORT].enumAmount]{
		new VarType[operators[OperatorTypes_EXPORT].params[0]]{VarType_STRING, VarType_NA},
		new VarType[operators[OperatorTypes_EXPORT].params[1]]{VarType_STRING, VarType_NA}
	};
	operators[OperatorTypes_EXPORT].paramTypes = new ParamType*[operators[OperatorTypes_EXPORT].enumAmount]{
		new ParamType[operators[OperatorTypes_EXPORT].params[0]]{ParamType_NA, ParamType_CONST},
		new ParamType[operators[OperatorTypes_EXPORT].params[0]]{ParamType_NA, ParamType_VAR}
	};
	operators[OperatorTypes_EXPORT].optionalPar = new bool*[operators[OperatorTypes_EXPORT].enumAmount]{
		new bool[operators[OperatorTypes_EXPORT].params[0]]{ false, false },
		new bool[operators[OperatorTypes_EXPORT].params[0]]{ false, false }
	};
	operators[OperatorTypes_EXPORT].readParam = new unsigned char[operators[OperatorTypes_EXPORT].enumAmount]{0,0};
	operators[OperatorTypes_EXPORT].returns = new VarType[operators[OperatorTypes_EXPORT].enumAmount]{VarType_NA, VarType_NA};
	operators[OperatorTypes_EXPORT].returnType = new ParamType[operators[OperatorTypes_EXPORT].enumAmount]{ParamType_NA, ParamType_NA};
	operators[OperatorTypes_EXPORT].writeParam = new unsigned char[operators[OperatorTypes_EXPORT].enumAmount]{0, 0};
	operators[OperatorTypes_EXPORT].priority = 0;
	operators[OperatorTypes_EXPORT].shortHandsAmounts = 0;
	operators[OperatorTypes_EXPORT].amountParOperators = new unsigned char[operators[OperatorTypes_EXPORT].enumAmount]{0, 0};
	operators[OperatorTypes_EXPORT].parRepeatsMin = new unsigned char*[operators[OperatorTypes_EXPORT].enumAmount]{
		new unsigned char[operators[OperatorTypes_EXPORT].params[0]]{0, 0},
		new unsigned char[operators[OperatorTypes_EXPORT].params[1]]{0, 0}
	};
	operators[OperatorTypes_EXPORT].parRepeatsMax = new unsigned char*[operators[OperatorTypes_EXPORT].enumAmount]{
		new unsigned char[operators[OperatorTypes_EXPORT].params[0]]{0, 0},
		new unsigned char[operators[OperatorTypes_EXPORT].params[1]]{0, 0}
	};
	operators[OperatorTypes_EXPORT].parOperatorAppend = new bool*[operators[OperatorTypes_EXPORT].enumAmount]{
		new bool[operators[OperatorTypes_EXPORT].params[0]]{false, false},
		new bool[operators[OperatorTypes_EXPORT].params[1]]{false, false}
	};

	operators[OperatorTypes_IMPORT].keyword = "import";
	operators[OperatorTypes_IMPORT].enumAmount = 3;
	operators[OperatorTypes_IMPORT].enums = new std::string[operators[OperatorTypes_IMPORT].enumAmount]; operators[OperatorTypes_IMPORT].enums[0] = "num"; operators[OperatorTypes_IMPORT].enums[1] = "float"; operators[OperatorTypes_IMPORT].enums[2] = "string";
	operators[OperatorTypes_IMPORT].byteCodes = new unsigned char[operators[OperatorTypes_IMPORT].enumAmount]{opcode_IMPRT, opcode_IMPRT, opcode_IMPRT};
	operators[OperatorTypes_IMPORT].params = new unsigned char[operators[OperatorTypes_IMPORT].enumAmount]{1, 1, 1};
	operators[OperatorTypes_IMPORT].minParams = new unsigned char[operators[OperatorTypes_IMPORT].enumAmount]{1, 1, 1};
	operators[OperatorTypes_IMPORT].paramsyntax = new VarType*[operators[OperatorTypes_IMPORT].enumAmount]{
		new VarType[operators[OperatorTypes_IMPORT].params[0]]{VarType_STRING},
		new VarType[operators[OperatorTypes_IMPORT].params[1]]{VarType_STRING},
		new VarType[operators[OperatorTypes_IMPORT].params[2]]{VarType_STRING}
	};
	operators[OperatorTypes_IMPORT].paramTypes = new ParamType*[operators[OperatorTypes_IMPORT].enumAmount]{
		new ParamType[operators[OperatorTypes_IMPORT].params[0]]{ParamType_NA},
		new ParamType[operators[OperatorTypes_IMPORT].params[1]]{ParamType_NA},
		new ParamType[operators[OperatorTypes_IMPORT].params[2]]{ParamType_NA}
	};
	operators[OperatorTypes_IMPORT].optionalPar = new bool*[operators[OperatorTypes_IMPORT].enumAmount]{
		new bool[operators[OperatorTypes_IMPORT].params[0]]{ false },
		new bool[operators[OperatorTypes_IMPORT].params[1]]{ false },
		new bool[operators[OperatorTypes_IMPORT].params[2]]{ false }
	};
	operators[OperatorTypes_IMPORT].readParam = new unsigned char[operators[OperatorTypes_IMPORT].enumAmount]{0,0,0};
	operators[OperatorTypes_IMPORT].returns = new VarType[operators[OperatorTypes_IMPORT].enumAmount]{VarType_NUMBER, VarType_FLOAT, VarType_STRING};
	operators[OperatorTypes_IMPORT].returnType = new ParamType[operators[OperatorTypes_IMPORT].enumAmount]{ParamType_VAR, ParamType_VAR, ParamType_VAR};
	operators[OperatorTypes_IMPORT].writeParam = new unsigned char[operators[OperatorTypes_IMPORT].enumAmount]{0,0,0};
	operators[OperatorTypes_IMPORT].priority = 0;
	operators[OperatorTypes_IMPORT].shortHandsAmounts = 0;
	operators[OperatorTypes_IMPORT].amountParOperators = new unsigned char[operators[OperatorTypes_IMPORT].enumAmount]{0,0,0};
	operators[OperatorTypes_IMPORT].parRepeatsMin = new unsigned char*[operators[OperatorTypes_IMPORT].enumAmount]{
		new unsigned char[operators[OperatorTypes_IMPORT].params[0]]{0},
		new unsigned char[operators[OperatorTypes_IMPORT].params[1]]{0},
		new unsigned char[operators[OperatorTypes_IMPORT].params[2]]{0}
	};
	operators[OperatorTypes_IMPORT].parRepeatsMax = new unsigned char*[operators[OperatorTypes_IMPORT].enumAmount]{
		new unsigned char[operators[OperatorTypes_IMPORT].params[0]]{0},
		new unsigned char[operators[OperatorTypes_IMPORT].params[1]]{0},
		new unsigned char[operators[OperatorTypes_IMPORT].params[2]]{0}
	};
	operators[OperatorTypes_IMPORT].parOperatorAppend = new bool*[operators[OperatorTypes_IMPORT].enumAmount]{
		new bool[operators[OperatorTypes_IMPORT].params[0]]{false},
		new bool[operators[OperatorTypes_IMPORT].params[1]]{false},
		new bool[operators[OperatorTypes_IMPORT].params[2]]{false}
	};


	flowOps = new FlowController[FlowOperator_NA];

	flowOps[FlowOperator_IF].keyword = "if";
	flowOps[FlowOperator_IF].op = FlowOperator_IF;
	flowOps[FlowOperator_IF].params = 3;
	flowOps[FlowOperator_IF].optionalParams = new bool[flowOps[FlowOperator_IF].params]{false, false, true};
	flowOps[FlowOperator_IF].maxParamArgs = new int[flowOps[FlowOperator_IF].params]{1, 0, 0};
	flowOps[FlowOperator_IF].minParamArgs = new int[flowOps[FlowOperator_IF].params]{1, 0, 0};
	flowOps[FlowOperator_IF].parameters = new FlowType[flowOps[FlowOperator_IF].params]{FlowType_COND, FlowType_BODY, FlowType_BRANCH};
	flowOps[FlowOperator_IF].type = FlowType_BRANCH;

	flowOps[FlowOperator_ELSE].keyword = "else";
	flowOps[FlowOperator_ELSE].op = FlowOperator_ELSE;
	flowOps[FlowOperator_ELSE].params = 3;
	flowOps[FlowOperator_ELSE].optionalParams = new bool[flowOps[FlowOperator_ELSE].params]{false, false, true};
	flowOps[FlowOperator_ELSE].maxParamArgs = new int[flowOps[FlowOperator_ELSE].params]{1, 0, 0};
	flowOps[FlowOperator_ELSE].minParamArgs = new int[flowOps[FlowOperator_ELSE].params]{0, 0, 0};
	flowOps[FlowOperator_ELSE].parameters = new FlowType[flowOps[FlowOperator_ELSE].params]{FlowType_COND, FlowType_BODY, FlowType_BRANCH};
	flowOps[FlowOperator_ELSE].type = FlowType_BRANCH;

	flowOps[FlowOperator_LOOP].keyword = "loop";
	flowOps[FlowOperator_LOOP].op = FlowOperator_LOOP;
	flowOps[FlowOperator_LOOP].params = 2;
	flowOps[FlowOperator_LOOP].optionalParams = new bool[flowOps[FlowOperator_LOOP].params]{false, false};
	flowOps[FlowOperator_LOOP].maxParamArgs = new int[flowOps[FlowOperator_LOOP].params]{3, 0};
	flowOps[FlowOperator_LOOP].minParamArgs = new int[flowOps[FlowOperator_LOOP].params]{1, 0};
	flowOps[FlowOperator_LOOP].parameters = new FlowType[flowOps[FlowOperator_LOOP].params]{FlowType_COND, FlowType_BODY};
	flowOps[FlowOperator_LOOP].type = FlowType_LOOP;

	flowOps[FlowOperator_INIT].keyword = "$";
	flowOps[FlowOperator_INIT].op = FlowOperator_INIT;
	flowOps[FlowOperator_INIT].params = 2;
	flowOps[FlowOperator_INIT].optionalParams = new bool[flowOps[FlowOperator_INIT].params]{false,false};
	flowOps[FlowOperator_INIT].maxParamArgs = new int[flowOps[FlowOperator_INIT].params]{0, 0};
	flowOps[FlowOperator_INIT].minParamArgs = new int[flowOps[FlowOperator_INIT].params]{0,0};
	flowOps[FlowOperator_INIT].parameters = new FlowType[flowOps[FlowOperator_INIT].params]{FlowType_COND, FlowType_BODY};
	flowOps[FlowOperator_INIT].type = FlowType_INIT;

	rParams = new RuntimeParameter[RunTimeParams_NA];

	rParams[RunTimeParams_ETIME].name = "?etime";
	rParams[RunTimeParams_ETIME].type = VarType_NUMBER;

	int enumKeyWords = 0;
	int shortHandAmounts = 0;

	for (int i = 0; i < OperatorTypes_NA; i++)
	{

		for (int k = 0; k < operators[i].enumAmount; k++)
		{

			enumKeyWords += operators[i].enums[k] != "" ? 1 : 0;

		}

		shortHandAmounts += operators[i].shortHandsAmounts;

	}

	keyWordsSize = OperatorTypes_NA + enumKeyWords + shortHandAmounts +  + FlowOperator_NA;
	keywords = new Keyword[keyWordsSize];
	int totalKeyWords = 0;

	for (int i = 0; i < OperatorTypes_NA; i++)
	{

		keywords[i].priority = operators[i].priority;
		keywords[i].keyword = operators[i].keyword;
		keywords[i].type = KeyWordType_OPERATOR;

		for (int k = 0; k < operators[i].enumAmount; k++)
		{

			if (operators[i].enums[k] != "")
			{

				keywords[OperatorTypes_NA + totalKeyWords].keyword = operators[i].enums[k];
				keywords[OperatorTypes_NA + totalKeyWords].type = KeyWordType_ENUM;
				keywords[OperatorTypes_NA + totalKeyWords].priority = 0;
				totalKeyWords++;

			}
		}
		
		for (int k = 0; k < operators[i].shortHandsAmounts; k++)
		{

			keywords[OperatorTypes_NA + totalKeyWords].keyword = operators[i].shortHands[k];
			keywords[OperatorTypes_NA + totalKeyWords].type = KeyWordType_OPERATOR;
			keywords[OperatorTypes_NA + totalKeyWords].priority = operators[i].priority;
			totalKeyWords++;

		}
	}

	totalKeyWords = OperatorTypes_NA + enumKeyWords + shortHandAmounts;

	for (int i = 0; i < FlowOperator_NA; i++)
	{

		keywords[totalKeyWords].priority = 0;
		keywords[totalKeyWords].keyword = flowOps[i].keyword;
		keywords[totalKeyWords].type = KeyWordType_FLOW;
		totalKeyWords++;

	}

	parser = new CelScriptParser(keywords, keyWordsSize, flowOps, FlowOperator_NA);
	lexer = new CelScriptLexer(keywords, keyWordsSize, operators, OperatorTypes_NA);
	analyzer = new CelScriptAnalyzer(keywords, keyWordsSize, operators, OperatorTypes_NA, flowOps, FlowOperator_NA,rParams,RunTimeParams_NA);
	generator = new CelScriptCodeGenerator(keywords, keyWordsSize);

}

void CelScriptCompiler::setupArithmeticOps()
{

	operators[OperatorTypes_SUM].keyword = "sum";
	operators[OperatorTypes_SUM].enumAmount = 6;
	operators[OperatorTypes_SUM].enums = new std::string[operators[OperatorTypes_SUM].enumAmount]; operators[OperatorTypes_SUM].enums[0] = ""; operators[OperatorTypes_SUM].enums[1] = ""; operators[OperatorTypes_SUM].enums[2] = ""; operators[OperatorTypes_SUM].enums[3] = ""; operators[OperatorTypes_SUM].enums[4] = ""; operators[OperatorTypes_SUM].enums[5] = "";
	operators[OperatorTypes_SUM].byteCodes = new unsigned char[operators[OperatorTypes_SUM].enumAmount]{opcode_SUM2CONST, opcode_SUM2CONST, opcode_SUMVAR, opcode_SUMFLOAT, opcode_SUMFLOAT, opcode_SUMFLOAT};
	operators[OperatorTypes_SUM].params = new unsigned char[operators[OperatorTypes_SUM].enumAmount]{2, 2, 2, 2, 2, 2};
	operators[OperatorTypes_SUM].minParams = new unsigned char[operators[OperatorTypes_SUM].enumAmount]{2, 2, 2, 2, 2, 2};
	operators[OperatorTypes_SUM].paramsyntax = new VarType*[operators[OperatorTypes_SUM].enumAmount]{
		new VarType[operators[OperatorTypes_SUM].params[0]]{VarType_NUMBER, VarType_NUMBER},
			new VarType[operators[OperatorTypes_SUM].params[1]]{VarType_FLOAT, VarType_FLOAT},
			new VarType[operators[OperatorTypes_SUM].params[2]]{VarType_NUMBER, VarType_NUMBER},
			new VarType[operators[OperatorTypes_SUM].params[3]]{VarType_FLOAT, VarType_FLOAT},
			new VarType[operators[OperatorTypes_SUM].params[4]]{VarType_NUMBER, VarType_FLOAT},
			new VarType[operators[OperatorTypes_SUM].params[5]]{VarType_FLOAT, VarType_NUMBER}};
	operators[OperatorTypes_SUM].paramTypes = new ParamType*[operators[OperatorTypes_SUM].enumAmount]{
		new ParamType[operators[OperatorTypes_SUM].params[0]]{ParamType_CONST, ParamType_CONST},
			new ParamType[operators[OperatorTypes_SUM].params[1]]{ParamType_CONST, ParamType_CONST},
			new ParamType[operators[OperatorTypes_SUM].params[2]]{ParamType_NA, ParamType_NA},
			new ParamType[operators[OperatorTypes_SUM].params[3]]{ParamType_NA, ParamType_NA},
			new ParamType[operators[OperatorTypes_SUM].params[4]]{ParamType_NA, ParamType_NA},
			new ParamType[operators[OperatorTypes_SUM].params[5]]{ParamType_NA, ParamType_NA}};
	operators[OperatorTypes_SUM].optionalPar = new bool*[operators[OperatorTypes_SUM].enumAmount]{
		new bool[operators[OperatorTypes_SUM].params[0]]{ false, false },
			new bool[operators[OperatorTypes_SUM].params[1]]{ false, false },
			new bool[operators[OperatorTypes_SUM].params[2]]{ false, false },
			new bool[operators[OperatorTypes_SUM].params[3]]{ false, false },
			new bool[operators[OperatorTypes_SUM].params[4]]{ false, false },
			new bool[operators[OperatorTypes_SUM].params[4]]{ false, false }
	};
	operators[OperatorTypes_SUM].readParam = new unsigned char[operators[OperatorTypes_SUM].enumAmount]{0, 0, 0, 0, 0, 0};
	operators[OperatorTypes_SUM].returns = new VarType[operators[OperatorTypes_SUM].enumAmount]{VarType_NUMBER, VarType_FLOAT, VarType_NUMBER, VarType_FLOAT, VarType_FLOAT, VarType_FLOAT};
	operators[OperatorTypes_SUM].returnType = new ParamType[operators[OperatorTypes_SUM].enumAmount]{ParamType_CONST, ParamType_CONST, ParamType_VAR, ParamType_VAR, ParamType_VAR, ParamType_VAR};
	operators[OperatorTypes_SUM].writeParam = new unsigned char[operators[OperatorTypes_SUM].enumAmount]{0, 0, 0, 0, 0, 0};
	operators[OperatorTypes_SUM].priority = 3;
	operators[OperatorTypes_SUM].shortHandsAmounts = 1;
	operators[OperatorTypes_SUM].shortHands = new std::string[operators[OperatorTypes_SUM].shortHandsAmounts]; operators[OperatorTypes_SUM].shortHands[0] = "+";
	operators[OperatorTypes_SUM].rightParams = new unsigned char[operators[OperatorTypes_SUM].shortHandsAmounts]{1};
	operators[OperatorTypes_SUM].leftParams = new unsigned char[operators[OperatorTypes_SUM].shortHandsAmounts]{1};
	operators[OperatorTypes_SUM].shortFlipParams = new bool[operators[OperatorTypes_SUM].shortHandsAmounts]{false};
	operators[OperatorTypes_SUM].amountParOperators = new unsigned char[operators[OperatorTypes_SUM].enumAmount]{0, 0, 0, 0, 1, 1};
	operators[OperatorTypes_SUM].parRepeatsMin = new unsigned char*[operators[OperatorTypes_SUM].enumAmount]{
		new unsigned char[operators[OperatorTypes_SUM].params[0]]{0, 0},
			new unsigned char[operators[OperatorTypes_SUM].params[1]]{0, 0},
			new unsigned char[operators[OperatorTypes_SUM].params[2]]{0, 0},
			new unsigned char[operators[OperatorTypes_SUM].params[3]]{0, 0},
			new unsigned char[operators[OperatorTypes_SUM].params[4]]{0, 0},
			new unsigned char[operators[OperatorTypes_SUM].params[5]]{0, 0}
	};
	operators[OperatorTypes_SUM].parRepeatsMax = new unsigned char*[operators[OperatorTypes_SUM].enumAmount]{
		new unsigned char[operators[OperatorTypes_SUM].params[0]]{0, 0},
			new unsigned char[operators[OperatorTypes_SUM].params[1]]{0, 0},
			new unsigned char[operators[OperatorTypes_SUM].params[2]]{0, 0},
			new unsigned char[operators[OperatorTypes_SUM].params[3]]{0, 0},
			new unsigned char[operators[OperatorTypes_SUM].params[4]]{0, 0},
			new unsigned char[operators[OperatorTypes_SUM].params[5]]{0, 0}
	};
	operators[OperatorTypes_SUM].parOperators = new OperatorTypes*[operators[OperatorTypes_SUM].enumAmount]{
		nullptr,
			nullptr,
			nullptr,
			nullptr,
			new OperatorTypes[operators[OperatorTypes_SUM].amountParOperators[4]]{OperatorTypes_CAST},
			new OperatorTypes[operators[OperatorTypes_SUM].amountParOperators[5]]{OperatorTypes_CAST}
	};
	operators[OperatorTypes_SUM].parOperator = new unsigned char*[operators[OperatorTypes_SUM].enumAmount]{
		nullptr,
			nullptr,
			nullptr,
			nullptr,
			new unsigned char[operators[OperatorTypes_SUM].params[4]]{1, 0},
			new unsigned char[operators[OperatorTypes_SUM].params[5]]{0, 1}
	};
	operators[OperatorTypes_SUM].parOperatorAppend = new bool*[operators[OperatorTypes_SUM].enumAmount]{
		new bool[operators[OperatorTypes_SUM].params[0]]{false, false},
			new bool[operators[OperatorTypes_SUM].params[1]]{false, false},
			new bool[operators[OperatorTypes_SUM].params[2]]{false, false},
			new bool[operators[OperatorTypes_SUM].params[3]]{false, false},
			new bool[operators[OperatorTypes_SUM].params[4]]{false, false},
			new bool[operators[OperatorTypes_SUM].params[5]]{false, false}
	};

	operators[OperatorTypes_SUB].keyword = "sub";
	operators[OperatorTypes_SUB].enumAmount = 10;
	operators[OperatorTypes_SUB].enums = new std::string[operators[OperatorTypes_SUB].enumAmount]; operators[OperatorTypes_SUB].enums[0] = ""; operators[OperatorTypes_SUB].enums[1] = ""; operators[OperatorTypes_SUB].enums[2] = ""; operators[OperatorTypes_SUB].enums[3] = ""; operators[OperatorTypes_SUB].enums[4] = ""; operators[OperatorTypes_SUB].enums[5] = ""; operators[OperatorTypes_SUB].enums[6] = ""; operators[OperatorTypes_SUB].enums[7] = ""; operators[OperatorTypes_SUB].enums[8] = ""; operators[OperatorTypes_SUB].enums[9] = "";
	operators[OperatorTypes_SUB].byteCodes = new unsigned char[operators[OperatorTypes_SUB].enumAmount]{opcode_SUB2CONST, opcode_SUB2CONST, opcode_SUBVAR, opcode_SUBFLOAT, opcode_SUBFLOAT, opcode_SUBFLOAT, opcode_NEGATECONST, opcode_NEGATECONST, opcode_NEGATEFLOAT, opcode_NEGATEVAR};
	operators[OperatorTypes_SUB].params = new unsigned char[operators[OperatorTypes_SUB].enumAmount]{2, 2, 2, 2, 2, 2, 1, 1, 1, 1};
	operators[OperatorTypes_SUB].minParams = new unsigned char[operators[OperatorTypes_SUB].enumAmount]{2, 2, 2, 2, 2, 2, 1, 1, 1, 1};
	operators[OperatorTypes_SUB].paramsyntax = new VarType*[operators[OperatorTypes_SUB].enumAmount]{
		new VarType[operators[OperatorTypes_SUB].params[0]]{VarType_NUMBER, VarType_NUMBER},
			new VarType[operators[OperatorTypes_SUB].params[1]]{VarType_FLOAT, VarType_FLOAT},
			new VarType[operators[OperatorTypes_SUB].params[2]]{VarType_NUMBER, VarType_NUMBER},
			new VarType[operators[OperatorTypes_SUB].params[3]]{VarType_FLOAT, VarType_FLOAT},
			new VarType[operators[OperatorTypes_SUB].params[4]]{VarType_NUMBER, VarType_FLOAT},
			new VarType[operators[OperatorTypes_SUB].params[5]]{VarType_FLOAT, VarType_NUMBER},

			new VarType[operators[OperatorTypes_SUB].params[6]]{VarType_FLOAT},
			new VarType[operators[OperatorTypes_SUB].params[7]]{VarType_NUMBER},
			new VarType[operators[OperatorTypes_SUB].params[8]]{VarType_FLOAT},
			new VarType[operators[OperatorTypes_SUB].params[9]]{VarType_NUMBER}};
	operators[OperatorTypes_SUB].paramTypes = new ParamType*[operators[OperatorTypes_SUB].enumAmount]{
		new ParamType[operators[OperatorTypes_SUB].params[0]]{ParamType_CONST, ParamType_CONST},
			new ParamType[operators[OperatorTypes_SUB].params[1]]{ParamType_CONST, ParamType_CONST},
			new ParamType[operators[OperatorTypes_SUB].params[2]]{ParamType_NA, ParamType_NA},
			new ParamType[operators[OperatorTypes_SUB].params[3]]{ParamType_NA, ParamType_NA},
			new ParamType[operators[OperatorTypes_SUB].params[4]]{ParamType_NA, ParamType_NA},
			new ParamType[operators[OperatorTypes_SUB].params[5]]{ParamType_NA, ParamType_NA},

			new ParamType[operators[OperatorTypes_SUB].params[6]]{ParamType_CONST},
			new ParamType[operators[OperatorTypes_SUB].params[7]]{ParamType_CONST},
			new ParamType[operators[OperatorTypes_SUB].params[8]]{ParamType_NA},
			new ParamType[operators[OperatorTypes_SUB].params[9]]{ParamType_NA}};
	operators[OperatorTypes_SUB].optionalPar = new bool*[operators[OperatorTypes_SUB].enumAmount]{
		new bool[operators[OperatorTypes_SUB].params[0]]{ false, false },
			new bool[operators[OperatorTypes_SUB].params[1]]{ false, false },
			new bool[operators[OperatorTypes_SUB].params[2]]{ false, false },
			new bool[operators[OperatorTypes_SUB].params[3]]{ false, false },
			new bool[operators[OperatorTypes_SUB].params[4]]{ false, false },
			new bool[operators[OperatorTypes_SUB].params[5]]{ false, false },
			new bool[operators[OperatorTypes_SUB].params[6]]{ false },
			new bool[operators[OperatorTypes_SUB].params[7]]{ false },
			new bool[operators[OperatorTypes_SUB].params[8]]{ false },
			new bool[operators[OperatorTypes_SUB].params[9]]{ false }
	};
	operators[OperatorTypes_SUB].readParam = new unsigned char[operators[OperatorTypes_SUB].enumAmount]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	operators[OperatorTypes_SUB].returns = new VarType[operators[OperatorTypes_SUB].enumAmount]{VarType_NUMBER, VarType_FLOAT, VarType_NUMBER, VarType_FLOAT, VarType_FLOAT, VarType_FLOAT, VarType_FLOAT, VarType_NUMBER, VarType_FLOAT, VarType_NUMBER};
	operators[OperatorTypes_SUB].returnType = new ParamType[operators[OperatorTypes_SUB].enumAmount]{ParamType_CONST, ParamType_CONST, ParamType_VAR, ParamType_VAR, ParamType_VAR, ParamType_VAR, ParamType_CONST, ParamType_CONST, ParamType_VAR, ParamType_VAR};
	operators[OperatorTypes_SUB].writeParam = new unsigned char[operators[OperatorTypes_SUB].enumAmount]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	operators[OperatorTypes_SUB].priority = 3;
	operators[OperatorTypes_SUB].shortHandsAmounts = 1;
	operators[OperatorTypes_SUB].shortHands = new std::string[operators[OperatorTypes_SUB].shortHandsAmounts]; operators[OperatorTypes_SUB].shortHands[0] = "-";
	operators[OperatorTypes_SUB].rightParams = new unsigned char[operators[OperatorTypes_SUB].shortHandsAmounts]{1};
	operators[OperatorTypes_SUB].leftParams = new unsigned char[operators[OperatorTypes_SUB].shortHandsAmounts]{1};
	operators[OperatorTypes_SUB].shortFlipParams = new bool[operators[OperatorTypes_SUB].shortHandsAmounts]{false};
	operators[OperatorTypes_SUB].amountParOperators = new unsigned char[operators[OperatorTypes_SUB].enumAmount]{0, 0, 0, 0, 1, 1, 0, 0, 0, 0};
	operators[OperatorTypes_SUB].parRepeatsMin = new unsigned char*[operators[OperatorTypes_SUB].enumAmount]{
		new unsigned char[operators[OperatorTypes_SUB].params[0]]{0, 0},
			new unsigned char[operators[OperatorTypes_SUB].params[1]]{0, 0},
			new unsigned char[operators[OperatorTypes_SUB].params[2]]{0, 0},
			new unsigned char[operators[OperatorTypes_SUB].params[3]]{0, 0},
			new unsigned char[operators[OperatorTypes_SUB].params[4]]{0, 0},
			new unsigned char[operators[OperatorTypes_SUB].params[5]]{0, 0},

			new unsigned char[operators[OperatorTypes_SUB].params[6]]{0},
			new unsigned char[operators[OperatorTypes_SUB].params[7]]{0},
			new unsigned char[operators[OperatorTypes_SUB].params[8]]{0},
			new unsigned char[operators[OperatorTypes_SUB].params[9]]{0}
	};
	operators[OperatorTypes_SUB].parRepeatsMax = new unsigned char*[operators[OperatorTypes_SUB].enumAmount]{
		new unsigned char[operators[OperatorTypes_SUB].params[0]]{0, 0},
			new unsigned char[operators[OperatorTypes_SUB].params[1]]{0, 0},
			new unsigned char[operators[OperatorTypes_SUB].params[2]]{0, 0},
			new unsigned char[operators[OperatorTypes_SUB].params[3]]{0, 0},
			new unsigned char[operators[OperatorTypes_SUB].params[4]]{0, 0},
			new unsigned char[operators[OperatorTypes_SUB].params[5]]{0, 0},

			new unsigned char[operators[OperatorTypes_SUB].params[6]]{0},
			new unsigned char[operators[OperatorTypes_SUB].params[7]]{0},
			new unsigned char[operators[OperatorTypes_SUB].params[8]]{0},
			new unsigned char[operators[OperatorTypes_SUB].params[9]]{0}
	};
	operators[OperatorTypes_SUB].parOperators = new OperatorTypes*[operators[OperatorTypes_SUB].enumAmount]{
		nullptr,
			nullptr,
			nullptr,
			nullptr,
			new OperatorTypes[operators[OperatorTypes_SUB].amountParOperators[4]]{OperatorTypes_CAST},
			new OperatorTypes[operators[OperatorTypes_SUB].amountParOperators[5]]{OperatorTypes_CAST},
			nullptr,
			nullptr,
			nullptr,
			nullptr
	};
	operators[OperatorTypes_SUB].parOperator = new unsigned char*[operators[OperatorTypes_SUB].enumAmount]{
		nullptr,
			nullptr,
			nullptr,
			nullptr,
			new unsigned char[operators[OperatorTypes_SUB].params[4]]{1, 0},
			new unsigned char[operators[OperatorTypes_SUB].params[5]]{0, 1},
			nullptr,
			nullptr,
			nullptr,
			nullptr
	};
	operators[OperatorTypes_SUB].parOperatorAppend = new bool*[operators[OperatorTypes_SUB].enumAmount]{
		new bool[operators[OperatorTypes_SUB].params[0]]{false, false},
			new bool[operators[OperatorTypes_SUB].params[1]]{false, false},
			new bool[operators[OperatorTypes_SUB].params[2]]{false, false},
			new bool[operators[OperatorTypes_SUB].params[3]]{false, false},
			new bool[operators[OperatorTypes_SUB].params[4]]{false, false},
			new bool[operators[OperatorTypes_SUB].params[5]]{false, false},

			new bool[operators[OperatorTypes_SUB].params[6]]{false},
			new bool[operators[OperatorTypes_SUB].params[7]]{false},
			new bool[operators[OperatorTypes_SUB].params[8]]{false},
			new bool[operators[OperatorTypes_SUB].params[9]]{false}
	};

	operators[OperatorTypes_MUL].keyword = "mul";
	operators[OperatorTypes_MUL].enumAmount = 6;
	operators[OperatorTypes_MUL].enums = new std::string[operators[OperatorTypes_MUL].enumAmount]; operators[OperatorTypes_MUL].enums[0] = ""; operators[OperatorTypes_MUL].enums[1] = ""; operators[OperatorTypes_MUL].enums[2] = ""; operators[OperatorTypes_MUL].enums[3] = ""; operators[OperatorTypes_MUL].enums[4] = ""; operators[OperatorTypes_MUL].enums[5] = "";
	operators[OperatorTypes_MUL].byteCodes = new unsigned char[operators[OperatorTypes_MUL].enumAmount]{opcode_MUL2CONST, opcode_MUL2CONST, opcode_MULVAR, opcode_MULFLOAT, opcode_MULFLOAT, opcode_MULFLOAT};
	operators[OperatorTypes_MUL].params = new unsigned char[operators[OperatorTypes_MUL].enumAmount]{2, 2, 2, 2, 2, 2};
	operators[OperatorTypes_MUL].minParams = new unsigned char[operators[OperatorTypes_MUL].enumAmount]{2, 2, 2, 2, 2, 2};
	operators[OperatorTypes_MUL].paramsyntax = new VarType*[operators[OperatorTypes_MUL].enumAmount]{
		new VarType[operators[OperatorTypes_MUL].params[0]]{VarType_NUMBER, VarType_NUMBER},
			new VarType[operators[OperatorTypes_MUL].params[1]]{VarType_FLOAT, VarType_FLOAT},
			new VarType[operators[OperatorTypes_MUL].params[2]]{VarType_NUMBER, VarType_NUMBER},
			new VarType[operators[OperatorTypes_MUL].params[3]]{VarType_FLOAT, VarType_FLOAT},
			new VarType[operators[OperatorTypes_MUL].params[4]]{VarType_NUMBER, VarType_FLOAT},
			new VarType[operators[OperatorTypes_MUL].params[5]]{VarType_FLOAT, VarType_NUMBER}};
	operators[OperatorTypes_MUL].paramTypes = new ParamType*[operators[OperatorTypes_MUL].enumAmount]{
		new ParamType[operators[OperatorTypes_MUL].params[0]]{ParamType_CONST, ParamType_CONST},
			new ParamType[operators[OperatorTypes_MUL].params[1]]{ParamType_CONST, ParamType_CONST},
			new ParamType[operators[OperatorTypes_MUL].params[2]]{ParamType_NA, ParamType_NA},
			new ParamType[operators[OperatorTypes_MUL].params[3]]{ParamType_NA, ParamType_NA},
			new ParamType[operators[OperatorTypes_MUL].params[4]]{ParamType_NA, ParamType_NA},
			new ParamType[operators[OperatorTypes_MUL].params[5]]{ParamType_NA, ParamType_NA}};
	operators[OperatorTypes_MUL].optionalPar = new bool*[operators[OperatorTypes_MUL].enumAmount]{
		new bool[operators[OperatorTypes_MUL].params[0]]{ false, false },
			new bool[operators[OperatorTypes_MUL].params[1]]{ false, false },
			new bool[operators[OperatorTypes_MUL].params[2]]{ false, false },
			new bool[operators[OperatorTypes_MUL].params[3]]{ false, false },
			new bool[operators[OperatorTypes_MUL].params[4]]{ false, false },
			new bool[operators[OperatorTypes_MUL].params[4]]{ false, false }
	};
	operators[OperatorTypes_MUL].readParam = new unsigned char[operators[OperatorTypes_MUL].enumAmount]{0, 0, 0, 0, 0, 0};
	operators[OperatorTypes_MUL].returns = new VarType[operators[OperatorTypes_MUL].enumAmount]{VarType_NUMBER, VarType_FLOAT, VarType_NUMBER, VarType_FLOAT, VarType_FLOAT, VarType_FLOAT};
	operators[OperatorTypes_MUL].returnType = new ParamType[operators[OperatorTypes_MUL].enumAmount]{ParamType_CONST, ParamType_CONST, ParamType_VAR, ParamType_VAR, ParamType_VAR, ParamType_VAR};
	operators[OperatorTypes_MUL].writeParam = new unsigned char[operators[OperatorTypes_MUL].enumAmount]{0, 0, 0, 0, 0, 0};
	operators[OperatorTypes_MUL].priority = 2;
	operators[OperatorTypes_MUL].shortHandsAmounts = 1;
	operators[OperatorTypes_MUL].shortHands = new std::string[operators[OperatorTypes_MUL].shortHandsAmounts]; operators[OperatorTypes_MUL].shortHands[0] = "*";
	operators[OperatorTypes_MUL].rightParams = new unsigned char[operators[OperatorTypes_MUL].shortHandsAmounts]{1};
	operators[OperatorTypes_MUL].leftParams = new unsigned char[operators[OperatorTypes_MUL].shortHandsAmounts]{1};
	operators[OperatorTypes_MUL].shortFlipParams = new bool[operators[OperatorTypes_MUL].shortHandsAmounts]{false};
	operators[OperatorTypes_MUL].amountParOperators = new unsigned char[operators[OperatorTypes_MUL].enumAmount]{0, 0, 0, 0, 1, 1};
	operators[OperatorTypes_MUL].parRepeatsMin = new unsigned char*[operators[OperatorTypes_MUL].enumAmount]{
		new unsigned char[operators[OperatorTypes_MUL].params[0]]{0, 0},
			new unsigned char[operators[OperatorTypes_MUL].params[1]]{0, 0},
			new unsigned char[operators[OperatorTypes_MUL].params[2]]{0, 0},
			new unsigned char[operators[OperatorTypes_MUL].params[3]]{0, 0},
			new unsigned char[operators[OperatorTypes_MUL].params[4]]{0, 0},
			new unsigned char[operators[OperatorTypes_MUL].params[5]]{0, 0}
	};
	operators[OperatorTypes_MUL].parRepeatsMax = new unsigned char*[operators[OperatorTypes_MUL].enumAmount]{
		new unsigned char[operators[OperatorTypes_MUL].params[0]]{0, 0},
			new unsigned char[operators[OperatorTypes_MUL].params[1]]{0, 0},
			new unsigned char[operators[OperatorTypes_MUL].params[2]]{0, 0},
			new unsigned char[operators[OperatorTypes_MUL].params[3]]{0, 0},
			new unsigned char[operators[OperatorTypes_MUL].params[4]]{0, 0},
			new unsigned char[operators[OperatorTypes_MUL].params[5]]{0, 0}
	};
	operators[OperatorTypes_MUL].parOperators = new OperatorTypes*[operators[OperatorTypes_MUL].enumAmount]{
		nullptr,
			nullptr,
			nullptr,
			nullptr,
			new OperatorTypes[operators[OperatorTypes_MUL].amountParOperators[4]]{OperatorTypes_CAST},
			new OperatorTypes[operators[OperatorTypes_MUL].amountParOperators[5]]{OperatorTypes_CAST}
	};
	operators[OperatorTypes_MUL].parOperator = new unsigned char*[operators[OperatorTypes_MUL].enumAmount]{
		nullptr,
			nullptr,
			nullptr,
			nullptr,
			new unsigned char[operators[OperatorTypes_MUL].params[4]]{1, 0},
			new unsigned char[operators[OperatorTypes_MUL].params[5]]{0, 1}
	};
	operators[OperatorTypes_MUL].parOperatorAppend = new bool*[operators[OperatorTypes_MUL].enumAmount]{
		new bool[operators[OperatorTypes_MUL].params[0]]{false, false},
			new bool[operators[OperatorTypes_MUL].params[1]]{false, false},
			new bool[operators[OperatorTypes_MUL].params[2]]{false, false},
			new bool[operators[OperatorTypes_MUL].params[3]]{false, false},
			new bool[operators[OperatorTypes_MUL].params[4]]{false, false},
			new bool[operators[OperatorTypes_MUL].params[5]]{false, false}
	};

	operators[OperatorTypes_DIV].keyword = "div";
	operators[OperatorTypes_DIV].enumAmount = 6;
	operators[OperatorTypes_DIV].enums = new std::string[operators[OperatorTypes_DIV].enumAmount]; operators[OperatorTypes_DIV].enums[0] = ""; operators[OperatorTypes_DIV].enums[1] = ""; operators[OperatorTypes_DIV].enums[2] = ""; operators[OperatorTypes_DIV].enums[3] = ""; operators[OperatorTypes_DIV].enums[4] = ""; operators[OperatorTypes_DIV].enums[5] = "";
	operators[OperatorTypes_DIV].byteCodes = new unsigned char[operators[OperatorTypes_DIV].enumAmount]{opcode_DIV2CONST, opcode_DIV2CONST, opcode_DIVVAR, opcode_DIVFLOAT, opcode_DIVFLOAT, opcode_DIVFLOAT};
	operators[OperatorTypes_DIV].params = new unsigned char[operators[OperatorTypes_DIV].enumAmount]{2, 2, 2, 2, 2, 2};
	operators[OperatorTypes_DIV].minParams = new unsigned char[operators[OperatorTypes_DIV].enumAmount]{2, 2, 2, 2, 2, 2};
	operators[OperatorTypes_DIV].paramsyntax = new VarType*[operators[OperatorTypes_DIV].enumAmount]{
		new VarType[operators[OperatorTypes_DIV].params[0]]{VarType_NUMBER, VarType_NUMBER},
			new VarType[operators[OperatorTypes_DIV].params[1]]{VarType_FLOAT, VarType_FLOAT},
			new VarType[operators[OperatorTypes_DIV].params[2]]{VarType_NUMBER, VarType_NUMBER},
			new VarType[operators[OperatorTypes_DIV].params[3]]{VarType_FLOAT, VarType_FLOAT},
			new VarType[operators[OperatorTypes_DIV].params[4]]{VarType_NUMBER, VarType_FLOAT},
			new VarType[operators[OperatorTypes_DIV].params[5]]{VarType_FLOAT, VarType_NUMBER}};
	operators[OperatorTypes_DIV].paramTypes = new ParamType*[operators[OperatorTypes_DIV].enumAmount]{
		new ParamType[operators[OperatorTypes_DIV].params[0]]{ParamType_CONST, ParamType_CONST},
			new ParamType[operators[OperatorTypes_DIV].params[1]]{ParamType_CONST, ParamType_CONST},
			new ParamType[operators[OperatorTypes_DIV].params[2]]{ParamType_NA, ParamType_NA},
			new ParamType[operators[OperatorTypes_DIV].params[3]]{ParamType_NA, ParamType_NA},
			new ParamType[operators[OperatorTypes_DIV].params[4]]{ParamType_NA, ParamType_NA},
			new ParamType[operators[OperatorTypes_DIV].params[5]]{ParamType_NA, ParamType_NA}};
	operators[OperatorTypes_DIV].optionalPar = new bool*[operators[OperatorTypes_DIV].enumAmount]{
		new bool[operators[OperatorTypes_DIV].params[0]]{ false, false },
			new bool[operators[OperatorTypes_DIV].params[1]]{ false, false },
			new bool[operators[OperatorTypes_DIV].params[2]]{ false, false },
			new bool[operators[OperatorTypes_DIV].params[3]]{ false, false },
			new bool[operators[OperatorTypes_DIV].params[4]]{ false, false },
			new bool[operators[OperatorTypes_DIV].params[4]]{ false, false }
	};
	operators[OperatorTypes_DIV].readParam = new unsigned char[operators[OperatorTypes_DIV].enumAmount]{0, 0, 0, 0, 0, 0};
	operators[OperatorTypes_DIV].returns = new VarType[operators[OperatorTypes_DIV].enumAmount]{VarType_NUMBER, VarType_FLOAT, VarType_NUMBER, VarType_FLOAT, VarType_FLOAT, VarType_FLOAT};
	operators[OperatorTypes_DIV].returnType = new ParamType[operators[OperatorTypes_DIV].enumAmount]{ParamType_CONST, ParamType_CONST, ParamType_VAR, ParamType_VAR, ParamType_VAR, ParamType_VAR};
	operators[OperatorTypes_DIV].writeParam = new unsigned char[operators[OperatorTypes_DIV].enumAmount]{0, 0, 0, 0, 0, 0};
	operators[OperatorTypes_DIV].priority = 2;
	operators[OperatorTypes_DIV].shortHandsAmounts = 1;
	operators[OperatorTypes_DIV].shortHands = new std::string[operators[OperatorTypes_DIV].shortHandsAmounts]; operators[OperatorTypes_DIV].shortHands[0] = "/";
	operators[OperatorTypes_DIV].rightParams = new unsigned char[operators[OperatorTypes_DIV].shortHandsAmounts]{1};
	operators[OperatorTypes_DIV].leftParams = new unsigned char[operators[OperatorTypes_DIV].shortHandsAmounts]{1};
	operators[OperatorTypes_DIV].shortFlipParams = new bool[operators[OperatorTypes_DIV].shortHandsAmounts]{false};
	operators[OperatorTypes_DIV].amountParOperators = new unsigned char[operators[OperatorTypes_DIV].enumAmount]{0, 0, 0, 0, 1, 1};
	operators[OperatorTypes_DIV].parRepeatsMin = new unsigned char*[operators[OperatorTypes_DIV].enumAmount]{
		new unsigned char[operators[OperatorTypes_DIV].params[0]]{0, 0},
			new unsigned char[operators[OperatorTypes_DIV].params[1]]{0, 0},
			new unsigned char[operators[OperatorTypes_DIV].params[2]]{0, 0},
			new unsigned char[operators[OperatorTypes_DIV].params[3]]{0, 0},
			new unsigned char[operators[OperatorTypes_DIV].params[4]]{0, 0},
			new unsigned char[operators[OperatorTypes_DIV].params[5]]{0, 0}
	};
	operators[OperatorTypes_DIV].parRepeatsMax = new unsigned char*[operators[OperatorTypes_DIV].enumAmount]{
		new unsigned char[operators[OperatorTypes_DIV].params[0]]{0, 0},
			new unsigned char[operators[OperatorTypes_DIV].params[1]]{0, 0},
			new unsigned char[operators[OperatorTypes_DIV].params[2]]{0, 0},
			new unsigned char[operators[OperatorTypes_DIV].params[3]]{0, 0},
			new unsigned char[operators[OperatorTypes_DIV].params[4]]{0, 0},
			new unsigned char[operators[OperatorTypes_DIV].params[5]]{0, 0}
	};
	operators[OperatorTypes_DIV].parOperators = new OperatorTypes*[operators[OperatorTypes_DIV].enumAmount]{
		nullptr,
			nullptr,
			nullptr,
			nullptr,
			new OperatorTypes[operators[OperatorTypes_DIV].amountParOperators[4]]{OperatorTypes_CAST},
			new OperatorTypes[operators[OperatorTypes_DIV].amountParOperators[5]]{OperatorTypes_CAST}
	};
	operators[OperatorTypes_DIV].parOperator = new unsigned char*[operators[OperatorTypes_DIV].enumAmount]{
		nullptr,
			nullptr,
			nullptr,
			nullptr,
			new unsigned char[operators[OperatorTypes_DIV].params[4]]{1, 0},
			new unsigned char[operators[OperatorTypes_DIV].params[5]]{0, 1}
	};
	operators[OperatorTypes_DIV].parOperatorAppend = new bool*[operators[OperatorTypes_DIV].enumAmount]{
		new bool[operators[OperatorTypes_DIV].params[0]]{false, false},
			new bool[operators[OperatorTypes_DIV].params[1]]{false, false},
			new bool[operators[OperatorTypes_DIV].params[2]]{false, false},
			new bool[operators[OperatorTypes_DIV].params[3]]{false, false},
			new bool[operators[OperatorTypes_DIV].params[4]]{false, false},
			new bool[operators[OperatorTypes_DIV].params[5]]{false, false}
	};
}

void CelScriptCompiler::Init(CrossHandlers::TextContainer* outText)
{

	this->outText = outText;

}

unsigned int CelScriptCompiler::getParam(std::string param) const
{

	bool isRP = false;
	std::string numb = "";
	unsigned int ret = 0;

	for (unsigned int i = 1; i < param.length() && !isRP; i++)
	{

		isRP = !(param[i] == '0' ||
			param[i] == '1' ||
			param[i] == '2' ||
			param[i] == '3' ||
			param[i] == '4' ||
			param[i] == '5' ||
			param[i] == '6' ||
			param[i] == '7' ||
			param[i] == '8' ||
			param[i] == '9') &&
			!(i == 1 && param[i] == 's');

		if (!isRP && param[i] != 's')
		{

			numb += param[i];

		}
	}

	if (!isRP)
	{

		ret = std::stoi(numb)+1;

	}

	return ret;
}

CelScriptCompiled* CelScriptCompiler::CompileSource(CelScriptSource* source, CompileError &error)
{

	CelScriptCompiled* compiledSource = nullptr;
	error.errorType = ScriptError_OK;
	CelestialList<Token>* lexedcode = new CelestialList<Token>();

	for (int i = 0; i < source->GetLines() && error.errorType == ScriptError_OK; i++)
	{

		CelestialDoubleList<Token>* lineLex = lexer->TokenizeLine(source->GetLine(i), i, error);

		if (lineLex != nullptr)
		{

			lexedcode->AddElement(lineLex);
			delete lineLex;

		}
	}

	if (error.errorType == ScriptError_OK && lexedcode->GetCount() > 0)
	{

		CelestialList<CelestialTree<Token>*>* parseTrees = parser->ParseTokens(lexedcode, error);

		if (error.errorType == ScriptError_OK)
		{

			AST abstractSyntaxTree = analyzer->AnalyzeTrees(parseTrees, error);
			
			if (error.errorType == ScriptError_OK)
			{

				CelestialList<CelestialList<unsigned char>*>* compiledCode = generator->GenerateCode(abstractSyntaxTree);
				unsigned int scriptParams = 0;
				unsigned int scriptStringParams = 0;

				for (int k = 0; k < 2; k++)
				{

					for (int i = 0; i < abstractSyntaxTree.symbols; i++)
					{

						std::string symbol = abstractSyntaxTree.symbolTable->GetValue(i).name;

						if (symbol[0] == '?')
						{

							unsigned int param = getParam(symbol);

							if (param > 0)
							{

								if (symbol[1] == 's' && scriptStringParams < param)
								{

									scriptStringParams = param;

								}
								else if (scriptParams < param)
								{

									scriptParams = param;
								}

								if (compiledSource != nullptr)
								{

									compiledSource->AddParamAdr(param - 1, abstractSyntaxTree.symbolTable->GetValue(i).address, symbol[1] == 's');

								}
							}
							else if (compiledSource != nullptr)
							{

								bool found = false;

								for (int j = 0; j < RunTimeParams_NA; j++)
								{

									found = strcmp(symbol.c_str(), rParams[j].name.c_str()) == 0;

									if (found)
									{

										compiledSource->AddSystemParamAdr(RunTimeParams(j), abstractSyntaxTree.symbolTable->GetValue(i).address);

									}
								}
							}
						}
					}

					if (k == 0)
					{
						
						compiledSource = new CelScriptCompiled(scriptParams, scriptStringParams);

					}
				}

				CelestialListNode<CelestialList<unsigned char>*>* command = compiledCode->GetFirstNode();

				while (command != nullptr)
				{

					int size = command->GetNodeObject()->GetCount();
					unsigned char* com = new unsigned char[size];
					CelestialListNode<unsigned char>* arg = command->GetNodeObject()->GetFirstNode();
					int place = 0;

					while (arg != nullptr)
					{

						com[place] = arg->GetNodeObject();
						place++;
						arg = arg->GetNext();

					}

					compiledSource->AddCommand(com, size);
					command = command->GetNext();

				}

				compiledCode->KillList();
				delete compiledCode;

			}

			abstractSyntaxTree.trees->KillList();
			delete abstractSyntaxTree.trees;

		}

		parseTrees->KillList();
		delete parseTrees;

	}

	delete lexedcode;

	if (error.errorType != Logic::ScriptError_OK)
	{

		outText->AddText("Error compiling celscript at line: " + std::to_string(error.line + 1)+ " in file: "+source->GetFile(error.line+1));

		if (error.errorType == Logic::ScriptError_BADSIG)
		{

			outText->AddText("Incompatible parameters");

		}
		else if (error.errorType == Logic::ScriptError_CONDITION)
		{

			outText->AddText("Need condition");

		}
		else if (error.errorType == Logic::ScriptError_ENUMMISPLACE)
		{

			outText->AddText("Misplaced Enumeration");

		}
		else if (error.errorType == Logic::ScriptError_HUH)
		{

			outText->AddText("Beats the fuck out of me...");

		}
		else if (error.errorType == Logic::ScriptError_INCOMPATIBLE)
		{

			outText->AddText("Type error");

		}
		else if (error.errorType == Logic::ScriptError_INCOMPATIBLEENUM)
		{

			outText->AddText("I have no idea..");

		}
		else if (error.errorType == Logic::ScriptError_INCOMPATIBLERETURN)
		{

			outText->AddText("Wrong return type");

		}
		else if (error.errorType == Logic::ScriptError_NESTEDFLOW)
		{

			outText->AddText("Flow operators cannot nest on the same lline");

		}
		else if (error.errorType == Logic::ScriptError_NOBODY)
		{

			outText->AddText("Lacking body");

		}
		else if (error.errorType == Logic::ScriptError_TOFEWPARAM)
		{

			outText->AddText("Not enough parameters");

		}
		else if (error.errorType == Logic::ScriptError_TOMANYPARAM)
		{

			outText->AddText("Too many parameters");

		}
		else if (error.errorType == Logic::ScriptError_UNCLOSED)
		{

			outText->AddText("Unclosed string");

		}
		else if (error.errorType == Logic::ScriptError_UNCLOSEDBODY)
		{

			outText->AddText("Unclosed Body");

		}
		else if (error.errorType == Logic::ScriptError_UNKNOWNKEYWORD)
		{

			outText->AddText("Expected valid Keyword");

		}
		else if (error.errorType == Logic::ScriptError_VARUNIN)
		{

			outText->AddText("Variable needs a value");

		}
		else if (error.errorType == Logic::ScriptError_PARAMWRONGPLACE)
		{

			outText->AddText("Runtime Parameter marker in the wrong place");

		}
		else if (error.errorType == Logic::ScriptError_UNKNOWNSYSTEMPARAM)
		{

			outText->AddText("Unknown System parameter");

		}
		else
		{

			outText->AddText("Unknown error, sucks to be you");

		}
	}
	else
	{
		
		outText->AddText("Script compiled ok!");
		
	}

	return compiledSource;

}

CelScriptCompiler::~CelScriptCompiler()
{

	delete parser;
	delete lexer;
	delete analyzer;
	delete generator;

	delete[] keywords;

	for (int i = 0; i < OperatorTypes_NA; i++)
	{
		
		bool delParop = false;

		for (int k = 0; k < operators[i].enumAmount; k++)
		{

			if (operators[i].params[k] > 0)
			{

				delete[] operators[i].paramTypes[k];
				delete[] operators[i].optionalPar[k];
				delete[] operators[i].paramsyntax[k];
				delete[] operators[i].parRepeatsMax[k];
				delete[] operators[i].parRepeatsMin[k];
				delete[] operators[i].parOperatorAppend[k];

				if (operators[i].amountParOperators[k] > 0)
				{

					delParop = true;
					delete[] operators[i].parOperator[k];
					delete[] operators[i].parOperators[k];

				}
			}
		}

		if( 0 < operators[i].shortHandsAmounts)
		{

			delete[] operators[i].shortHands;
			delete[] operators[i].rightParams;
			delete[] operators[i].leftParams;
			delete[] operators[i].shortFlipParams;

		}

		delete[] operators[i].amountParOperators;
		delete[] operators[i].byteCodes;
		delete[] operators[i].enums;

		if (delParop)
		{

			delete[] operators[i].parOperator;
			delete[] operators[i].parOperators;

		}

		delete[] operators[i].parOperatorAppend;
		delete[] operators[i].parRepeatsMax;
		delete[] operators[i].parRepeatsMin;
		delete[] operators[i].params;
		delete[] operators[i].minParams;
		delete[] operators[i].optionalPar;
		delete[] operators[i].paramsyntax;
		delete[] operators[i].paramTypes;
		delete[] operators[i].readParam;
		delete[] operators[i].returns;
		delete[] operators[i].returnType;
		delete[] operators[i].writeParam;

	}

	for (int i = 0; i < FlowOperator_NA; i++)
	{

		delete[] flowOps[i].maxParamArgs;
		delete[] flowOps[i].minParamArgs;
		delete[] flowOps[i].optionalParams;
		delete[] flowOps[i].parameters;

	}

	delete[] rParams;
	delete[] flowOps;
	delete[] operators;

}
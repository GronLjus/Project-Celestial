#include "stdafx.h"
#include "CelScriptCompiler.h"

using namespace Logic;
using namespace Resources;


CelScriptCompiler::CelScriptCompiler()
{

	this->outText = nullptr;
	operators = new Operator[OperatorTypes_NA];

	operators[OperatorTypes_SET].keyword = "set";
	operators[OperatorTypes_SET].enumAmount = 17;
	operators[OperatorTypes_SET].enums = new std::string[operators[OperatorTypes_SET].enumAmount]; 
	operators[OperatorTypes_SET].enums[0] = ""; 
	operators[OperatorTypes_SET].enums[1] = ""; 
	operators[OperatorTypes_SET].enums[2] = "gameboard";
	operators[OperatorTypes_SET].enums[3] = "camera";
	operators[OperatorTypes_SET].enums[4] = "leftclick";
	operators[OperatorTypes_SET].enums[5] = "out";
	operators[OperatorTypes_SET].enums[6] = "text";
	operators[OperatorTypes_SET].enums[7] = "ui";
	operators[OperatorTypes_SET].enums[8] = "content";
	operators[OperatorTypes_SET].enums[9] = "border";
	operators[OperatorTypes_SET].enums[10] = "layer";
	operators[OperatorTypes_SET].enums[11] = "middleclick";
	operators[OperatorTypes_SET].enums[12] = "rightclick";
	operators[OperatorTypes_SET].enums[13] = "leftdrag";
	operators[OperatorTypes_SET].enums[14] = "middledrag";
	operators[OperatorTypes_SET].enums[15] = "rightdrag";
	operators[OperatorTypes_SET].enums[16] = "mousewheel";
	operators[OperatorTypes_SET].byteCodes = new unsigned char[operators[OperatorTypes_SET].enumAmount]{
		opcode_SETCONST, 
		opcode_SETVAR, 
		opcode_SETGMEBRD, 
		opcode_SETCMRA, 
		opcode_SETLCLK, 
		opcode_LNKDBG, 
		opcode_STXT, 
		opcode_SETUI, 
		opcode_STCNTNT, 
		opcode_STBRDR, 
		opcode_STLYR,
		opcode_SETMCLK,
		opcode_SETRCLK,
		opcode_SETLDRG,
		opcode_SETMDRG,
		opcode_SETRDRG,
		opcode_SETMWHL };
	operators[OperatorTypes_SET].params = new unsigned char[operators[OperatorTypes_SET].enumAmount]{2, 2, 1, 1, 2, 1, 2, 1, 4, 4, 2, 2, 2, 2, 2, 2, 2};
	operators[OperatorTypes_SET].minParams = new unsigned char[operators[OperatorTypes_SET].enumAmount]{ 2, 2, 1, 1, 2, 1, 2, 1, 4, 4, 2, 2, 2, 2, 2, 2, 2};
	operators[OperatorTypes_SET].paramsyntax = new VarType*[operators[OperatorTypes_SET].enumAmount]{
		new VarType[operators[OperatorTypes_SET].params[0]]{VarType_NA, VarType_NA},
			new VarType[operators[OperatorTypes_SET].params[1]]{VarType_NA, VarType_NA},
			new VarType[operators[OperatorTypes_SET].params[2]]{VarType_NUMBER},
			new VarType[operators[OperatorTypes_SET].params[3]]{ VarType_NUMBER },
			new VarType[operators[OperatorTypes_SET].params[4]]{ VarType_NUMBER, VarType_NUMBER},
			new VarType[operators[OperatorTypes_SET].params[5]]{ VarType_NUMBER},
			new VarType[operators[OperatorTypes_SET].params[6]]{ VarType_NUMBER, VarType_STRING },
			new VarType[operators[OperatorTypes_SET].params[7]]{ VarType_NUMBER },
			new VarType[operators[OperatorTypes_SET].params[8]]{ VarType_NUMBER, VarType_FLOAT, VarType_FLOAT, VarType_FLOAT },
			new VarType[operators[OperatorTypes_SET].params[9]]{ VarType_NUMBER, VarType_FLOAT, VarType_FLOAT, VarType_FLOAT },
			new VarType[operators[OperatorTypes_SET].params[10]]{ VarType_NUMBER, VarType_NUMBER},
			new VarType[operators[OperatorTypes_SET].params[11]]{ VarType_NUMBER, VarType_NUMBER },
			new VarType[operators[OperatorTypes_SET].params[12]]{ VarType_NUMBER, VarType_NUMBER },
			new VarType[operators[OperatorTypes_SET].params[13]]{ VarType_NUMBER, VarType_NUMBER },
			new VarType[operators[OperatorTypes_SET].params[14]]{ VarType_NUMBER, VarType_NUMBER },
			new VarType[operators[OperatorTypes_SET].params[15]]{ VarType_NUMBER, VarType_NUMBER },
			new VarType[operators[OperatorTypes_SET].params[16]]{ VarType_NUMBER, VarType_NUMBER }
	};
	operators[OperatorTypes_SET].paramTypes = new ParamType*[operators[OperatorTypes_SET].enumAmount]{
		new ParamType[operators[OperatorTypes_SET].params[0]]{ParamType_VAR, ParamType_CONST}, 
			new ParamType[operators[OperatorTypes_SET].params[1]]{ParamType_VAR, ParamType_VAR},
			new ParamType[operators[OperatorTypes_SET].params[2]]{ParamType_VAR},
			new ParamType[operators[OperatorTypes_SET].params[3]]{ ParamType_VAR },
			new ParamType[operators[OperatorTypes_SET].params[4]]{ ParamType_NA, ParamType_NA },
			new ParamType[operators[OperatorTypes_SET].params[5]]{ ParamType_VAR},
			new ParamType[operators[OperatorTypes_SET].params[6]]{ ParamType_VAR, ParamType_VAR},
			new ParamType[operators[OperatorTypes_SET].params[7]]{ ParamType_VAR},
			new ParamType[operators[OperatorTypes_SET].params[8]]{ ParamType_NA, ParamType_NA, ParamType_NA, ParamType_NA},
			new ParamType[operators[OperatorTypes_SET].params[9]]{ ParamType_NA, ParamType_NA, ParamType_NA, ParamType_NA},
			new ParamType[operators[OperatorTypes_SET].params[10]]{ ParamType_NA, ParamType_NA},
			new ParamType[operators[OperatorTypes_SET].params[11]]{ ParamType_NA, ParamType_NA },
			new ParamType[operators[OperatorTypes_SET].params[12]]{ ParamType_NA, ParamType_NA },
			new ParamType[operators[OperatorTypes_SET].params[13]]{ ParamType_NA, ParamType_NA },
			new ParamType[operators[OperatorTypes_SET].params[14]]{ ParamType_NA, ParamType_NA },
			new ParamType[operators[OperatorTypes_SET].params[15]]{ ParamType_NA, ParamType_NA },
			new ParamType[operators[OperatorTypes_SET].params[16]]{ ParamType_NA, ParamType_NA }
	};
	operators[OperatorTypes_SET].optionalPar = new bool*[operators[OperatorTypes_SET].enumAmount]{
	new bool[operators[OperatorTypes_SET].params[0]]{ false, false },
	new bool[operators[OperatorTypes_SET].params[1]]{ false, false },
	new bool[operators[OperatorTypes_SET].params[2]]{ false },
	new bool[operators[OperatorTypes_SET].params[3]]{ false },
	new bool[operators[OperatorTypes_SET].params[4]]{ false, false},
	new bool[operators[OperatorTypes_SET].params[5]]{ false},
	new bool[operators[OperatorTypes_SET].params[6]]{ false, false},
	new bool[operators[OperatorTypes_SET].params[7]]{ false},
	new bool[operators[OperatorTypes_SET].params[8]]{ false, false, false, false},
	new bool[operators[OperatorTypes_SET].params[9]]{ false, false, false, false},
	new bool[operators[OperatorTypes_SET].params[10]]{ false, false},
	new bool[operators[OperatorTypes_SET].params[11]]{ false, false },
	new bool[operators[OperatorTypes_SET].params[12]]{ false, false },
	new bool[operators[OperatorTypes_SET].params[13]]{ false, false },
	new bool[operators[OperatorTypes_SET].params[14]]{ false, false },
	new bool[operators[OperatorTypes_SET].params[15]]{ false, false },
	new bool[operators[OperatorTypes_SET].params[16]]{ false, false }
	};
	operators[OperatorTypes_SET].readParam = new unsigned char[operators[OperatorTypes_SET].enumAmount]{2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2};
	operators[OperatorTypes_SET].returns = new VarType[operators[OperatorTypes_SET].enumAmount]{VarType_NA, VarType_NA, VarType_NA, VarType_NA, VarType_NA, VarType_NA, VarType_NA, VarType_NA, VarType_NA, VarType_NA, VarType_NA, VarType_NA, VarType_NA,VarType_NA,VarType_NA,VarType_NA,VarType_NA };
	operators[OperatorTypes_SET].returnType = new ParamType[operators[OperatorTypes_SET].enumAmount]{ParamType_NA, ParamType_NA, ParamType_NA, ParamType_NA, ParamType_NA, ParamType_NA, ParamType_NA, ParamType_NA, ParamType_NA, ParamType_NA, ParamType_NA, ParamType_NA, ParamType_NA,ParamType_NA,ParamType_NA,ParamType_NA,ParamType_NA };
	operators[OperatorTypes_SET].writeParam = new unsigned char[operators[OperatorTypes_SET].enumAmount]{1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	operators[OperatorTypes_SET].priority = 0;
	operators[OperatorTypes_SET].shortHandsAmounts = 1;
	operators[OperatorTypes_SET].shortHands = new std::string[operators[OperatorTypes_SET].shortHandsAmounts]; operators[OperatorTypes_SET].shortHands[0] = "=" ;
	operators[OperatorTypes_SET].rightParams = new unsigned char[operators[OperatorTypes_SET].shortHandsAmounts]{1};
	operators[OperatorTypes_SET].leftParams = new unsigned char[operators[OperatorTypes_SET].shortHandsAmounts]{1};
	operators[OperatorTypes_SET].shortFlipParams = new bool[operators[OperatorTypes_SET].shortHandsAmounts]{false};
	operators[OperatorTypes_SET].amountParOperators = new unsigned char[operators[OperatorTypes_SET].enumAmount]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	operators[OperatorTypes_SET].parRepeatsMin = new unsigned char*[operators[OperatorTypes_SET].enumAmount]{
		new unsigned char[operators[OperatorTypes_SET].params[0]]{0, 0}, 
			new unsigned char[operators[OperatorTypes_SET].params[1]]{0, 0},
			new unsigned char[operators[OperatorTypes_SET].params[2]]{0},
			new unsigned char[operators[OperatorTypes_SET].params[3]]{ 0 },
			new unsigned char[operators[OperatorTypes_SET].params[4]]{ 0, 0},
			new unsigned char[operators[OperatorTypes_SET].params[5]]{ 0},
			new unsigned char[operators[OperatorTypes_SET].params[6]]{ 0, 0},
			new unsigned char[operators[OperatorTypes_SET].params[7]]{ 0},
			new unsigned char[operators[OperatorTypes_SET].params[8]]{ 0, 0, 0, 0},
			new unsigned char[operators[OperatorTypes_SET].params[9]]{ 0, 0, 0, 0},
			new unsigned char[operators[OperatorTypes_SET].params[10]]{ 0, 0},
			new unsigned char[operators[OperatorTypes_SET].params[11]]{ 0, 0 },
			new unsigned char[operators[OperatorTypes_SET].params[12]]{ 0, 0 },
			new unsigned char[operators[OperatorTypes_SET].params[13]]{ 0, 0 },
			new unsigned char[operators[OperatorTypes_SET].params[14]]{ 0, 0 },
			new unsigned char[operators[OperatorTypes_SET].params[15]]{ 0, 0 },
			new unsigned char[operators[OperatorTypes_SET].params[16]]{ 0, 0 }
	};
	operators[OperatorTypes_SET].parRepeatsMax = new unsigned char*[operators[OperatorTypes_SET].enumAmount]{
		new unsigned char[operators[OperatorTypes_SET].params[0]]{0, 0}, 
			new unsigned char[operators[OperatorTypes_SET].params[1]]{0, 0},
			new unsigned char[operators[OperatorTypes_SET].params[2]]{0},
			new unsigned char[operators[OperatorTypes_SET].params[3]]{ 0 },
			new unsigned char[operators[OperatorTypes_SET].params[4]]{ 0, 1},
			new unsigned char[operators[OperatorTypes_SET].params[5]]{ 0},
			new unsigned char[operators[OperatorTypes_SET].params[6]]{ 0, 0},
			new unsigned char[operators[OperatorTypes_SET].params[7]]{ 0},
			new unsigned char[operators[OperatorTypes_SET].params[8]]{ 0, 0, 0, 0},
			new unsigned char[operators[OperatorTypes_SET].params[9]]{ 0, 0, 0, 0},
			new unsigned char[operators[OperatorTypes_SET].params[10]]{ 0, 0},
			new unsigned char[operators[OperatorTypes_SET].params[11]]{ 0, 0 },
			new unsigned char[operators[OperatorTypes_SET].params[12]]{ 0, 0 },
			new unsigned char[operators[OperatorTypes_SET].params[13]]{ 0, 0 },
			new unsigned char[operators[OperatorTypes_SET].params[14]]{ 0, 0 },
			new unsigned char[operators[OperatorTypes_SET].params[15]]{ 0, 0 },
			new unsigned char[operators[OperatorTypes_SET].params[16]]{ 0, 0 }
	};
	operators[OperatorTypes_SET].parOperatorAppend = new bool*[operators[OperatorTypes_SET].enumAmount]{
		new bool[operators[OperatorTypes_SET].params[0]]{false, false},
			new bool[operators[OperatorTypes_SET].params[1]]{false, false},
			new bool[operators[OperatorTypes_SET].params[2]]{false},
			new bool[operators[OperatorTypes_SET].params[3]]{ false },
			new bool[operators[OperatorTypes_SET].params[4]]{ false, false},
			new bool[operators[OperatorTypes_SET].params[5]]{ false},
			new bool[operators[OperatorTypes_SET].params[6]]{ false, false},
			new bool[operators[OperatorTypes_SET].params[7]]{ false},
			new bool[operators[OperatorTypes_SET].params[8]]{ false, false, false, false},
			new bool[operators[OperatorTypes_SET].params[9]]{ false, false, false, false},
			new bool[operators[OperatorTypes_SET].params[10]]{ false, false},
			new bool[operators[OperatorTypes_SET].params[11]]{ false, false },
			new bool[operators[OperatorTypes_SET].params[12]]{ false, false },
			new bool[operators[OperatorTypes_SET].params[13]]{ false, false },
			new bool[operators[OperatorTypes_SET].params[14]]{ false, false },
			new bool[operators[OperatorTypes_SET].params[15]]{ false, false },
			new bool[operators[OperatorTypes_SET].params[16]]{ false, false }
	};

	operators[OperatorTypes_LOAD].keyword = "load";
	operators[OperatorTypes_LOAD].enumAmount = 10;
	operators[OperatorTypes_LOAD].enums = new std::string[operators[OperatorTypes_LOAD].enumAmount]; 
	operators[OperatorTypes_LOAD].enums[0] = "light"; 
	operators[OperatorTypes_LOAD].enums[1] = "mesh";
	operators[OperatorTypes_LOAD].enums[2] = "script";
	operators[OperatorTypes_LOAD].enums[3] = "textbox";
	operators[OperatorTypes_LOAD].enums[4] = "panel";
	operators[OperatorTypes_LOAD].enums[5] = "image";
	operators[OperatorTypes_LOAD].enums[6] = "keychartrgr";
	operators[OperatorTypes_LOAD].enums[7] = "gameboard";
	operators[OperatorTypes_LOAD].enums[8] = "camera";
	operators[OperatorTypes_LOAD].enums[9] = "object";
	operators[OperatorTypes_LOAD].byteCodes = new unsigned char[operators[OperatorTypes_LOAD].enumAmount]{opcode_LOADLIGHT, opcode_LOADMESH, opcode_LOADSCRIPT, opcode_LOADTXTBX, opcode_LOADPANEL, opcode_LOADIMAGE, 0, opcode_LOADGMBRD, opcode_LOADCAM, opcode_LOADOBJCT};
	operators[OperatorTypes_LOAD].params = new unsigned char[operators[OperatorTypes_LOAD].enumAmount]{3, 1,1,0,0,1,2,2,0, 1};
	operators[OperatorTypes_LOAD].minParams = new unsigned char[operators[OperatorTypes_LOAD].enumAmount]{ 3, 1,1,0,0,1,2,1, 0, 1};
	operators[OperatorTypes_LOAD].paramsyntax = new VarType*[operators[OperatorTypes_LOAD].enumAmount]{
		new VarType[operators[OperatorTypes_LOAD].params[0]]{VarType_NUMBER, VarType_NUMBER, VarType_NUMBER}, 
			new VarType[operators[OperatorTypes_LOAD].params[1]]{VarType_STRING},
			new VarType[operators[OperatorTypes_LOAD].params[2]]{VarType_STRING},
			nullptr,nullptr,
			new VarType[operators[OperatorTypes_LOAD].params[5]]{VarType_STRING},
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
			new ParamType[operators[OperatorTypes_LOAD].params[5]]{ParamType_NA},
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
		new bool[operators[OperatorTypes_LOAD].params[5]]{ false },
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
			new unsigned char[operators[OperatorTypes_LOAD].params[5]]{0},
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
			new unsigned char[operators[OperatorTypes_LOAD].params[5]]{0},
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
			new bool[operators[OperatorTypes_LOAD].params[5]]{false},
			new bool[operators[OperatorTypes_LOAD].params[6]]{false, false},
			new bool[operators[OperatorTypes_LOAD].params[7]]{false, 0},
			nullptr,
			new bool[operators[OperatorTypes_LOAD].params[9]]{false}
	};

	operators[OperatorTypes_ADD].keyword = "add";
	operators[OperatorTypes_ADD].enumAmount = 5;
	operators[OperatorTypes_ADD].enums = new std::string[operators[OperatorTypes_ADD].enumAmount]; 
	operators[OperatorTypes_ADD].enums[0] = "object"; 
	operators[OperatorTypes_ADD].enums[1] = "ui"; 
	operators[OperatorTypes_ADD].enums[2] = "mesh";
	operators[OperatorTypes_ADD].enums[3] = "text";
	operators[OperatorTypes_ADD].enums[4] = "line";
	operators[OperatorTypes_ADD].byteCodes = new unsigned char[operators[OperatorTypes_ADD].enumAmount]{opcode_ADDOBJECT,opcode_2DADDCHLD,opcode_ADDMESH, opcode_ADDTXT, opcode_ADDLNE};
	operators[OperatorTypes_ADD].params = new unsigned char[operators[OperatorTypes_ADD].enumAmount]{1, 2, 1, 2, 2};
	operators[OperatorTypes_ADD].minParams = new unsigned char[operators[OperatorTypes_ADD].enumAmount]{1, 2, 1, 2, 2 };
	operators[OperatorTypes_ADD].paramsyntax = new VarType*[operators[OperatorTypes_ADD].enumAmount]{
		new VarType[operators[OperatorTypes_ADD].params[0]]{VarType_NUMBER}, 
			new VarType[operators[OperatorTypes_ADD].params[1]]{VarType_NUMBER, VarType_NUMBER}, 
			new VarType[operators[OperatorTypes_ADD].params[2]]{VarType_NUMBER},
			new VarType[operators[OperatorTypes_ADD].params[3]]{VarType_NUMBER, VarType_STRING},
			new VarType[operators[OperatorTypes_ADD].params[4]]{VarType_NUMBER, VarType_STRING}};
	operators[OperatorTypes_ADD].paramTypes = new ParamType*[operators[OperatorTypes_ADD].enumAmount]{
		new ParamType[operators[OperatorTypes_ADD].params[0]]{ParamType_NA}, 
			new ParamType[operators[OperatorTypes_ADD].params[1]]{ParamType_NA, ParamType_NA}, 
			new ParamType[operators[OperatorTypes_ADD].params[2]]{ParamType_NA},
			new ParamType[operators[OperatorTypes_ADD].params[3]]{ParamType_NA, ParamType_NA},
			new ParamType[operators[OperatorTypes_ADD].params[4]]{ParamType_NA, ParamType_NA}};
	operators[OperatorTypes_ADD].optionalPar = new bool*[operators[OperatorTypes_ADD].enumAmount]{
		new bool[operators[OperatorTypes_ADD].params[0]]{ false,},
		new bool[operators[OperatorTypes_ADD].params[1]]{ false, false },
		new bool[operators[OperatorTypes_ADD].params[2]]{ false},
		new bool[operators[OperatorTypes_ADD].params[3]]{ false, false },
		new bool[operators[OperatorTypes_ADD].params[4]]{ false, false }
	};
	operators[OperatorTypes_ADD].readParam = new unsigned char[operators[OperatorTypes_ADD].enumAmount]{0,0,0,0,0};
	operators[OperatorTypes_ADD].priority = 0;
	operators[OperatorTypes_ADD].returns = new VarType[operators[OperatorTypes_ADD].enumAmount]{VarType_NA, VarType_NUMBER, VarType_NA, VarType_NA, VarType_NA};
	operators[OperatorTypes_ADD].returnType = new ParamType[operators[OperatorTypes_ADD].enumAmount]{ParamType_NA, ParamType_VAR, ParamType_NA, ParamType_NA, ParamType_NA};
	operators[OperatorTypes_ADD].writeParam = new unsigned char[operators[OperatorTypes_ADD].enumAmount]{0,0,0,0,0};
	operators[OperatorTypes_ADD].shortHandsAmounts = 0;
	operators[OperatorTypes_ADD].amountParOperators = new unsigned char[operators[OperatorTypes_ADD].enumAmount]{0, 0, 0,0,0};
	operators[OperatorTypes_ADD].parRepeatsMin = new unsigned char*[operators[OperatorTypes_ADD].enumAmount]{
		new unsigned char[operators[OperatorTypes_ADD].params[0]]{0},
			new unsigned char[operators[OperatorTypes_ADD].params[1]]{0, 0},
			new unsigned char[operators[OperatorTypes_ADD].params[2]]{0},
			new unsigned char[operators[OperatorTypes_ADD].params[3]]{0, 0},
			new unsigned char[operators[OperatorTypes_ADD].params[4]]{0, 0}
	};
	operators[OperatorTypes_ADD].parRepeatsMax = new unsigned char*[operators[OperatorTypes_ADD].enumAmount]{
		new unsigned char[operators[OperatorTypes_ADD].params[0]]{0},
			new unsigned char[operators[OperatorTypes_ADD].params[1]]{0, 0},
			new unsigned char[operators[OperatorTypes_ADD].params[2]]{0},
			new unsigned char[operators[OperatorTypes_ADD].params[3]]{0, 0},
			new unsigned char[operators[OperatorTypes_ADD].params[4]]{0, 0}
	};
	operators[OperatorTypes_ADD].parOperatorAppend = new bool*[operators[OperatorTypes_ADD].enumAmount]{
		new bool[operators[OperatorTypes_ADD].params[0]]{false},
			new bool[operators[OperatorTypes_ADD].params[1]]{false, false},
			new bool[operators[OperatorTypes_ADD].params[2]]{false},
			new bool[operators[OperatorTypes_ADD].params[3]]{false, false},
			new bool[operators[OperatorTypes_ADD].params[4]]{false, false}
	};

	operators[OperatorTypes_POST].keyword = "post";
	operators[OperatorTypes_POST].enumAmount = 3;
	operators[OperatorTypes_POST].enums = new std::string[operators[OperatorTypes_POST].enumAmount]; operators[OperatorTypes_POST].enums[0] = ""; operators[OperatorTypes_POST].enums[1] = ""; operators[OperatorTypes_POST].enums[2] = "";
	operators[OperatorTypes_POST].byteCodes = new unsigned char[operators[OperatorTypes_POST].enumAmount]{opcode_POSTSTR, opcode_POSTFLOAT, opcode_POSTNMBR};
	operators[OperatorTypes_POST].params = new unsigned char[operators[OperatorTypes_POST].enumAmount]{1, 1, 1};
	operators[OperatorTypes_POST].minParams = new unsigned char[operators[OperatorTypes_POST].enumAmount]{ 1, 1, 1};
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
	operators[OperatorTypes_EQUAL].shortHands = new std::string[operators[OperatorTypes_EQUAL].shortHandsAmounts]; 
	operators[OperatorTypes_EQUAL].shortHands[0] = ":";
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

	operators[OperatorTypes_PARENT].keyword = "parent";
	operators[OperatorTypes_PARENT].enumAmount = 1;
	operators[OperatorTypes_PARENT].enums = new std::string[operators[OperatorTypes_PARENT].enumAmount]; operators[OperatorTypes_PARENT].enums[0] = "";
	operators[OperatorTypes_PARENT].byteCodes = new unsigned char[operators[OperatorTypes_PARENT].enumAmount]{opcode_PRNT};
	operators[OperatorTypes_PARENT].params = new unsigned char[operators[OperatorTypes_PARENT].enumAmount]{1};
	operators[OperatorTypes_PARENT].minParams = new unsigned char[operators[OperatorTypes_PARENT].enumAmount]{ 1 };
	operators[OperatorTypes_PARENT].paramsyntax = new VarType*[operators[OperatorTypes_PARENT].enumAmount]{
		new VarType[operators[OperatorTypes_PARENT].params[0]]{ VarType_NUMBER} };
	operators[OperatorTypes_PARENT].paramTypes = new ParamType*[operators[OperatorTypes_PARENT].enumAmount]{
		new ParamType[operators[OperatorTypes_PARENT].params[0]]{ ParamType_NA } };
	operators[OperatorTypes_PARENT].optionalPar = new bool*[operators[OperatorTypes_PARENT].enumAmount]{
		new bool[operators[OperatorTypes_PARENT].params[0]]{ false} };
	operators[OperatorTypes_PARENT].readParam = new unsigned char[operators[OperatorTypes_PARENT].enumAmount]{0};
	operators[OperatorTypes_PARENT].returns = new VarType[operators[OperatorTypes_PARENT].enumAmount]{VarType_NUMBER};
	operators[OperatorTypes_PARENT].returnType = new ParamType[operators[OperatorTypes_PARENT].enumAmount]{ParamType_VAR};
	operators[OperatorTypes_PARENT].writeParam = new unsigned char[operators[OperatorTypes_PARENT].enumAmount]{0};
	operators[OperatorTypes_PARENT].priority = 0;
	operators[OperatorTypes_PARENT].shortHandsAmounts = 0;
	operators[OperatorTypes_PARENT].amountParOperators = new unsigned char[operators[OperatorTypes_PARENT].enumAmount]{0};
	operators[OperatorTypes_PARENT].parRepeatsMin = new unsigned char*[operators[OperatorTypes_PARENT].enumAmount]{
		new unsigned char[operators[OperatorTypes_PARENT].params[0]]{ 0 } };
	operators[OperatorTypes_PARENT].parRepeatsMax = new unsigned char*[operators[OperatorTypes_PARENT].enumAmount]{
		new unsigned char[operators[OperatorTypes_PARENT].params[0]]{ 0 } };
	operators[OperatorTypes_PARENT].parOperatorAppend = new bool*[operators[OperatorTypes_PARENT].enumAmount]{
		new bool[operators[OperatorTypes_PARENT].params[0]]{ false } };

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
	operators[OperatorTypes_POS].enumAmount = 2;
	operators[OperatorTypes_POS].enums = new std::string[operators[OperatorTypes_POS].enumAmount]; operators[OperatorTypes_POS].enums[0] = ""; operators[OperatorTypes_POS].enums[1] = "";
	operators[OperatorTypes_POS].byteCodes = new unsigned char[operators[OperatorTypes_POS].enumAmount]{opcode_POS,opcode_POS };
	operators[OperatorTypes_POS].params = new unsigned char[operators[OperatorTypes_POS].enumAmount]{4,4};
	operators[OperatorTypes_POS].minParams = new unsigned char[operators[OperatorTypes_POS].enumAmount]{ 3,3 };
	operators[OperatorTypes_POS].paramsyntax = new VarType*[operators[OperatorTypes_POS].enumAmount]{
		new VarType[operators[OperatorTypes_POS].params[0]]{VarType_NUMBER, VarType_FLOAT, VarType_FLOAT, VarType_FLOAT},
		new VarType[operators[OperatorTypes_POS].params[1]]{ VarType_NUMBER, VarType_NUMBER, VarType_NUMBER, VarType_NUMBER } };
	operators[OperatorTypes_POS].paramTypes = new ParamType*[operators[OperatorTypes_POS].enumAmount]{
		new ParamType[operators[OperatorTypes_POS].params[0]]{ParamType_NA, ParamType_NA, ParamType_NA, ParamType_NA},
		new ParamType[operators[OperatorTypes_POS].params[0]]{ ParamType_NA, ParamType_NA, ParamType_NA, ParamType_NA } };
	operators[OperatorTypes_POS].optionalPar = new bool*[operators[OperatorTypes_POS].enumAmount]{
		new bool[operators[OperatorTypes_POS].params[0]]{ false, false, false, true },
		new bool[operators[OperatorTypes_POS].params[0]]{ false, false, false, true }
	};
	operators[OperatorTypes_POS].readParam = new unsigned char[operators[OperatorTypes_POS].enumAmount]{0,0};
	operators[OperatorTypes_POS].returns = new VarType[operators[OperatorTypes_POS].enumAmount]{VarType_NA,VarType_NA };
	operators[OperatorTypes_POS].returnType = new ParamType[operators[OperatorTypes_POS].enumAmount]{ParamType_NA,ParamType_NA };
	operators[OperatorTypes_POS].writeParam = new unsigned char[operators[OperatorTypes_POS].enumAmount]{0,0};
	operators[OperatorTypes_POS].priority = 0;
	operators[OperatorTypes_POS].shortHandsAmounts = 0;
	operators[OperatorTypes_POS].amountParOperators = new unsigned char[operators[OperatorTypes_POS].enumAmount]{0,1};
	operators[OperatorTypes_POS].parRepeatsMin = new unsigned char*[operators[OperatorTypes_POS].enumAmount]{
		new unsigned char[operators[OperatorTypes_POS].params[0]]{0, 0, 0, 0},
		new unsigned char[operators[OperatorTypes_POS].params[1]]{ 0, 0, 0, 0 }
	};
	operators[OperatorTypes_POS].parRepeatsMax = new unsigned char*[operators[OperatorTypes_POS].enumAmount]{
		new unsigned char[operators[OperatorTypes_POS].params[0]]{0, 0, 0, 0},
		new unsigned char[operators[OperatorTypes_POS].params[1]]{ 0, 0, 0, 0 }
	};
	operators[OperatorTypes_POS].parOperatorAppend = new bool*[operators[OperatorTypes_POS].enumAmount]{
		new bool[operators[OperatorTypes_POS].params[0]]{false, false, false, false},
		new bool[operators[OperatorTypes_POS].params[0]]{ false, false, false, false }
	};
	operators[OperatorTypes_POS].parOperators = new OperatorTypes*[operators[OperatorTypes_POS].enumAmount]{
		nullptr,
		new OperatorTypes[operators[OperatorTypes_POS].amountParOperators[1]]{ OperatorTypes_CAST }
	};
	operators[OperatorTypes_POS].parOperator = new unsigned char*[operators[OperatorTypes_POS].enumAmount]{
		nullptr,
		new unsigned char[operators[OperatorTypes_POS].params[1]]{ 0, 1, 1, 1 }
	};

	operators[OperatorTypes_MOVE].keyword = "move";
	operators[OperatorTypes_MOVE].enumAmount = 2;
	operators[OperatorTypes_MOVE].enums = new std::string[operators[OperatorTypes_MOVE].enumAmount]; operators[OperatorTypes_MOVE].enums[0] = ""; operators[OperatorTypes_MOVE].enums[1] = "";
	operators[OperatorTypes_MOVE].byteCodes = new unsigned char[operators[OperatorTypes_MOVE].enumAmount]{ opcode_MVE,opcode_MVE };
	operators[OperatorTypes_MOVE].params = new unsigned char[operators[OperatorTypes_MOVE].enumAmount]{ 4,4 };
	operators[OperatorTypes_MOVE].minParams = new unsigned char[operators[OperatorTypes_MOVE].enumAmount]{ 3,3 };
	operators[OperatorTypes_MOVE].paramsyntax = new VarType*[operators[OperatorTypes_MOVE].enumAmount]{
		new VarType[operators[OperatorTypes_MOVE].params[0]]{ VarType_NUMBER, VarType_FLOAT, VarType_FLOAT, VarType_FLOAT },
		new VarType[operators[OperatorTypes_MOVE].params[1]]{ VarType_NUMBER, VarType_NUMBER, VarType_NUMBER, VarType_NUMBER } };
	operators[OperatorTypes_MOVE].paramTypes = new ParamType*[operators[OperatorTypes_MOVE].enumAmount]{
		new ParamType[operators[OperatorTypes_MOVE].params[0]]{ ParamType_NA, ParamType_NA, ParamType_NA, ParamType_NA },
		new ParamType[operators[OperatorTypes_MOVE].params[0]]{ ParamType_NA, ParamType_NA, ParamType_NA, ParamType_NA } };
	operators[OperatorTypes_MOVE].optionalPar = new bool*[operators[OperatorTypes_MOVE].enumAmount]{
		new bool[operators[OperatorTypes_MOVE].params[0]]{ false, false, false, true },
		new bool[operators[OperatorTypes_MOVE].params[1]]{ false, false, false, true }
	};
	operators[OperatorTypes_MOVE].readParam = new unsigned char[operators[OperatorTypes_MOVE].enumAmount]{ 0,0 };
	operators[OperatorTypes_MOVE].returns = new VarType[operators[OperatorTypes_MOVE].enumAmount]{ VarType_NA,VarType_NA };
	operators[OperatorTypes_MOVE].returnType = new ParamType[operators[OperatorTypes_MOVE].enumAmount]{ ParamType_NA,ParamType_NA };
	operators[OperatorTypes_MOVE].writeParam = new unsigned char[operators[OperatorTypes_MOVE].enumAmount]{ 0,0 };
	operators[OperatorTypes_MOVE].priority = 0;
	operators[OperatorTypes_MOVE].shortHandsAmounts = 0;
	operators[OperatorTypes_MOVE].amountParOperators = new unsigned char[operators[OperatorTypes_MOVE].enumAmount]{ 0,1 };
	operators[OperatorTypes_MOVE].parRepeatsMin = new unsigned char*[operators[OperatorTypes_MOVE].enumAmount]{
		new unsigned char[operators[OperatorTypes_MOVE].params[0]]{ 0, 0, 0, 0 },
		new unsigned char[operators[OperatorTypes_MOVE].params[1]]{ 0, 0, 0, 0 }
	};
	operators[OperatorTypes_MOVE].parRepeatsMax = new unsigned char*[operators[OperatorTypes_MOVE].enumAmount]{
		new unsigned char[operators[OperatorTypes_MOVE].params[0]]{ 0, 0, 0, 0 },
		new unsigned char[operators[OperatorTypes_MOVE].params[1]]{ 0, 0, 0, 0 }
	};
	operators[OperatorTypes_MOVE].parOperatorAppend = new bool*[operators[OperatorTypes_MOVE].enumAmount]{
		new bool[operators[OperatorTypes_MOVE].params[0]]{ false, false, false, false },
		new bool[operators[OperatorTypes_MOVE].params[1]]{ false, false, false, false }
	};
	operators[OperatorTypes_MOVE].parOperators = new OperatorTypes*[operators[OperatorTypes_MOVE].enumAmount]{
		nullptr,
		new OperatorTypes[operators[OperatorTypes_MOVE].amountParOperators[1]]{ OperatorTypes_CAST }
	};
	operators[OperatorTypes_MOVE].parOperator = new unsigned char*[operators[OperatorTypes_MOVE].enumAmount]{
		nullptr,
		new unsigned char[operators[OperatorTypes_MOVE].params[1]]{ 0, 1, 1, 1 }
	};

	operators[OperatorTypes_ROTATE].keyword = "rotate";
	operators[OperatorTypes_ROTATE].enumAmount = 1;
	operators[OperatorTypes_ROTATE].enums = new std::string[operators[OperatorTypes_ROTATE].enumAmount]; operators[OperatorTypes_ROTATE].enums[0] = "";
	operators[OperatorTypes_ROTATE].byteCodes = new unsigned char[operators[OperatorTypes_ROTATE].enumAmount]{ opcode_RTE };
	operators[OperatorTypes_ROTATE].params = new unsigned char[operators[OperatorTypes_ROTATE].enumAmount]{ 4};
	operators[OperatorTypes_ROTATE].minParams = new unsigned char[operators[OperatorTypes_ROTATE].enumAmount]{ 4 };
	operators[OperatorTypes_ROTATE].paramsyntax = new VarType*[operators[OperatorTypes_ROTATE].enumAmount]{
		new VarType[operators[OperatorTypes_ROTATE].params[0]]{ VarType_NUMBER, VarType_FLOAT, VarType_FLOAT, VarType_FLOAT }};
	operators[OperatorTypes_ROTATE].paramTypes = new ParamType*[operators[OperatorTypes_ROTATE].enumAmount]{
		new ParamType[operators[OperatorTypes_ROTATE].params[0]]{ ParamType_NA, ParamType_NA, ParamType_NA, ParamType_NA }};
	operators[OperatorTypes_ROTATE].optionalPar = new bool*[operators[OperatorTypes_ROTATE].enumAmount]{
		new bool[operators[OperatorTypes_ROTATE].params[0]]{ false, false, false, false }
	};
	operators[OperatorTypes_ROTATE].readParam = new unsigned char[operators[OperatorTypes_ROTATE].enumAmount]{ 0 };
	operators[OperatorTypes_ROTATE].returns = new VarType[operators[OperatorTypes_ROTATE].enumAmount]{ VarType_NA};
	operators[OperatorTypes_ROTATE].returnType = new ParamType[operators[OperatorTypes_ROTATE].enumAmount]{ ParamType_NA};
	operators[OperatorTypes_ROTATE].writeParam = new unsigned char[operators[OperatorTypes_ROTATE].enumAmount]{ 0};
	operators[OperatorTypes_ROTATE].priority = 0;
	operators[OperatorTypes_ROTATE].shortHandsAmounts = 0;
	operators[OperatorTypes_ROTATE].amountParOperators = new unsigned char[operators[OperatorTypes_ROTATE].enumAmount]{ 0 };
	operators[OperatorTypes_ROTATE].parRepeatsMin = new unsigned char*[operators[OperatorTypes_ROTATE].enumAmount]{
		new unsigned char[operators[OperatorTypes_ROTATE].params[0]]{ 0, 0, 0, 0 }
	};
	operators[OperatorTypes_ROTATE].parRepeatsMax = new unsigned char*[operators[OperatorTypes_ROTATE].enumAmount]{
		new unsigned char[operators[OperatorTypes_ROTATE].params[0]]{ 0, 0, 0, 0 }
	};
	operators[OperatorTypes_ROTATE].parOperatorAppend = new bool*[operators[OperatorTypes_ROTATE].enumAmount]{
		new bool[operators[OperatorTypes_ROTATE].params[0]]{ false, false, false, false }
	};

	operators[OperatorTypes_ORBIT].keyword = "orbit";
	operators[OperatorTypes_ORBIT].enumAmount = 1;
	operators[OperatorTypes_ORBIT].enums = new std::string[operators[OperatorTypes_ORBIT].enumAmount]; operators[OperatorTypes_ORBIT].enums[0] = "";
	operators[OperatorTypes_ORBIT].byteCodes = new unsigned char[operators[OperatorTypes_ORBIT].enumAmount]{ opcode_ORB };
	operators[OperatorTypes_ORBIT].params = new unsigned char[operators[OperatorTypes_ORBIT].enumAmount]{ 2 };
	operators[OperatorTypes_ORBIT].minParams = new unsigned char[operators[OperatorTypes_ORBIT].enumAmount]{ 2 };
	operators[OperatorTypes_ORBIT].paramsyntax = new VarType*[operators[OperatorTypes_ORBIT].enumAmount]{
		new VarType[operators[OperatorTypes_ORBIT].params[0]]{ VarType_NUMBER, VarType_FLOAT} };
	operators[OperatorTypes_ORBIT].paramTypes = new ParamType*[operators[OperatorTypes_ORBIT].enumAmount]{
		new ParamType[operators[OperatorTypes_ORBIT].params[0]]{ ParamType_NA, ParamType_NA} };
	operators[OperatorTypes_ORBIT].optionalPar = new bool*[operators[OperatorTypes_ORBIT].enumAmount]{
		new bool[operators[OperatorTypes_ORBIT].params[0]]{ false, false}
	};
	operators[OperatorTypes_ORBIT].readParam = new unsigned char[operators[OperatorTypes_ORBIT].enumAmount]{ 0 };
	operators[OperatorTypes_ORBIT].returns = new VarType[operators[OperatorTypes_ORBIT].enumAmount]{ VarType_NA };
	operators[OperatorTypes_ORBIT].returnType = new ParamType[operators[OperatorTypes_ORBIT].enumAmount]{ ParamType_NA };
	operators[OperatorTypes_ORBIT].writeParam = new unsigned char[operators[OperatorTypes_ORBIT].enumAmount]{ 0 };
	operators[OperatorTypes_ORBIT].priority = 0;
	operators[OperatorTypes_ORBIT].shortHandsAmounts = 0;
	operators[OperatorTypes_ORBIT].amountParOperators = new unsigned char[operators[OperatorTypes_ORBIT].enumAmount]{ 0 };
	operators[OperatorTypes_ORBIT].parRepeatsMin = new unsigned char*[operators[OperatorTypes_ORBIT].enumAmount]{
		new unsigned char[operators[OperatorTypes_ORBIT].params[0]]{ 0, 0}
	};
	operators[OperatorTypes_ORBIT].parRepeatsMax = new unsigned char*[operators[OperatorTypes_ORBIT].enumAmount]{
		new unsigned char[operators[OperatorTypes_ORBIT].params[0]]{ 0, 0}
	};
	operators[OperatorTypes_ORBIT].parOperatorAppend = new bool*[operators[OperatorTypes_ORBIT].enumAmount]{
		new bool[operators[OperatorTypes_ORBIT].params[0]]{ false, false}
	};

	operators[OperatorTypes_PROPEL].keyword = "propel";
	operators[OperatorTypes_PROPEL].enumAmount = 1;
	operators[OperatorTypes_PROPEL].enums = new std::string[operators[OperatorTypes_PROPEL].enumAmount]; operators[OperatorTypes_PROPEL].enums[0] = "";
	operators[OperatorTypes_PROPEL].byteCodes = new unsigned char[operators[OperatorTypes_PROPEL].enumAmount]{ opcode_PRPL };
	operators[OperatorTypes_PROPEL].params = new unsigned char[operators[OperatorTypes_PROPEL].enumAmount]{ 2 };
	operators[OperatorTypes_PROPEL].minParams = new unsigned char[operators[OperatorTypes_PROPEL].enumAmount]{ 2 };
	operators[OperatorTypes_PROPEL].paramsyntax = new VarType*[operators[OperatorTypes_PROPEL].enumAmount]{
		new VarType[operators[OperatorTypes_PROPEL].params[0]]{ VarType_NUMBER, VarType_FLOAT } };
	operators[OperatorTypes_PROPEL].paramTypes = new ParamType*[operators[OperatorTypes_PROPEL].enumAmount]{
		new ParamType[operators[OperatorTypes_PROPEL].params[0]]{ ParamType_NA, ParamType_NA } };
	operators[OperatorTypes_PROPEL].optionalPar = new bool*[operators[OperatorTypes_PROPEL].enumAmount]{
		new bool[operators[OperatorTypes_PROPEL].params[0]]{ false, false }
	};
	operators[OperatorTypes_PROPEL].readParam = new unsigned char[operators[OperatorTypes_PROPEL].enumAmount]{ 0 };
	operators[OperatorTypes_PROPEL].returns = new VarType[operators[OperatorTypes_PROPEL].enumAmount]{ VarType_NA };
	operators[OperatorTypes_PROPEL].returnType = new ParamType[operators[OperatorTypes_PROPEL].enumAmount]{ ParamType_NA };
	operators[OperatorTypes_PROPEL].writeParam = new unsigned char[operators[OperatorTypes_PROPEL].enumAmount]{ 0 };
	operators[OperatorTypes_PROPEL].priority = 0;
	operators[OperatorTypes_PROPEL].shortHandsAmounts = 0;
	operators[OperatorTypes_PROPEL].amountParOperators = new unsigned char[operators[OperatorTypes_PROPEL].enumAmount]{ 0 };
	operators[OperatorTypes_PROPEL].parRepeatsMin = new unsigned char*[operators[OperatorTypes_PROPEL].enumAmount]{
		new unsigned char[operators[OperatorTypes_PROPEL].params[0]]{ 0, 0 }
	};
	operators[OperatorTypes_PROPEL].parRepeatsMax = new unsigned char*[operators[OperatorTypes_PROPEL].enumAmount]{
		new unsigned char[operators[OperatorTypes_PROPEL].params[0]]{ 0, 0 }
	};
	operators[OperatorTypes_PROPEL].parOperatorAppend = new bool*[operators[OperatorTypes_PROPEL].enumAmount]{
		new bool[operators[OperatorTypes_PROPEL].params[0]]{ false, false }
	};

	operators[OperatorTypes_SIZE].keyword = "size";
	operators[OperatorTypes_SIZE].enumAmount = 1;
	operators[OperatorTypes_SIZE].enums = new std::string[operators[OperatorTypes_SIZE].enumAmount]; operators[OperatorTypes_SIZE].enums[0] = "";
	operators[OperatorTypes_SIZE].byteCodes = new unsigned char[operators[OperatorTypes_SIZE].enumAmount]{opcode_SIZE};
	operators[OperatorTypes_SIZE].params = new unsigned char[operators[OperatorTypes_SIZE].enumAmount]{4};
	operators[OperatorTypes_SIZE].minParams = new unsigned char[operators[OperatorTypes_SIZE].enumAmount]{ 3 };
	operators[OperatorTypes_SIZE].paramsyntax = new VarType*[operators[OperatorTypes_SIZE].enumAmount]{
		new VarType[operators[OperatorTypes_SIZE].params[0]]{VarType_NUMBER, VarType_FLOAT, VarType_FLOAT, VarType_FLOAT}};
	operators[OperatorTypes_SIZE].paramTypes = new ParamType*[operators[OperatorTypes_SIZE].enumAmount]{
		new ParamType[operators[OperatorTypes_SIZE].params[0]]{ParamType_NA, ParamType_NA, ParamType_NA, ParamType_NA}};
	operators[OperatorTypes_SIZE].optionalPar = new bool*[operators[OperatorTypes_SIZE].enumAmount]{
		new bool[operators[OperatorTypes_SIZE].params[0]]{ false, false, false, true }
	};
	operators[OperatorTypes_SIZE].readParam = new unsigned char[operators[OperatorTypes_SIZE].enumAmount]{0};
	operators[OperatorTypes_SIZE].returns = new VarType[operators[OperatorTypes_SIZE].enumAmount]{VarType_NA};
	operators[OperatorTypes_SIZE].returnType = new ParamType[operators[OperatorTypes_SIZE].enumAmount]{ParamType_NA};
	operators[OperatorTypes_SIZE].writeParam = new unsigned char[operators[OperatorTypes_SIZE].enumAmount]{0};
	operators[OperatorTypes_SIZE].priority = 0;
	operators[OperatorTypes_SIZE].shortHandsAmounts = 0;
	operators[OperatorTypes_SIZE].amountParOperators = new unsigned char[operators[OperatorTypes_SIZE].enumAmount]{0};
	operators[OperatorTypes_SIZE].parRepeatsMin = new unsigned char*[operators[OperatorTypes_SIZE].enumAmount]{
		new unsigned char[operators[OperatorTypes_SIZE].params[0]]{0, 0, 0, 0}
	};
	operators[OperatorTypes_SIZE].parRepeatsMax = new unsigned char*[operators[OperatorTypes_SIZE].enumAmount]{
		new unsigned char[operators[OperatorTypes_SIZE].params[0]]{0, 0, 0, 0}
	};
	operators[OperatorTypes_SIZE].parOperatorAppend = new bool*[operators[OperatorTypes_SIZE].enumAmount]{
		new bool[operators[OperatorTypes_SIZE].params[0]]{false, false, false, false}
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
	operators[OperatorTypes_SETPARAM].enumAmount = 3;
	operators[OperatorTypes_SETPARAM].enums = new std::string[operators[OperatorTypes_SETPARAM].enumAmount]; operators[OperatorTypes_SETPARAM].enums[0] = "num"; operators[OperatorTypes_SETPARAM].enums[1] = "str"; operators[OperatorTypes_SETPARAM].enums[2] = "float";
	operators[OperatorTypes_SETPARAM].byteCodes = new unsigned char[operators[OperatorTypes_SETPARAM].enumAmount]{opcode_STSCRPTPRMNMBR,opcode_STSCRPTPRMSTR,opcode_STSCRPTPRMFLT};
	operators[OperatorTypes_SETPARAM].params = new unsigned char[operators[OperatorTypes_SETPARAM].enumAmount]{2,2,2};
	operators[OperatorTypes_SETPARAM].minParams = new unsigned char[operators[OperatorTypes_SETPARAM].enumAmount]{ 2,2,2 };
	operators[OperatorTypes_SETPARAM].paramsyntax = new VarType*[operators[OperatorTypes_SETPARAM].enumAmount]{
		new VarType[operators[OperatorTypes_SETPARAM].params[0]]{VarType_NUMBER, VarType_NUMBER},
			new VarType[operators[OperatorTypes_SETPARAM].params[1]]{VarType_STRING, VarType_NUMBER},
		new VarType[operators[OperatorTypes_SETPARAM].params[2]]{ VarType_FLOAT, VarType_NUMBER }
	};
	operators[OperatorTypes_SETPARAM].paramTypes = new ParamType*[operators[OperatorTypes_SETPARAM].enumAmount]{
		new ParamType[operators[OperatorTypes_SETPARAM].params[0]]{ParamType_NA, ParamType_NA},
		new ParamType[operators[OperatorTypes_SETPARAM].params[1]]{ParamType_NA, ParamType_NA},
		new ParamType[operators[OperatorTypes_SETPARAM].params[2]]{ ParamType_NA, ParamType_NA }
	};
	operators[OperatorTypes_SETPARAM].optionalPar = new bool*[operators[OperatorTypes_SETPARAM].enumAmount]{
		new bool[operators[OperatorTypes_SETPARAM].params[0]]{ false, false },
		new bool[operators[OperatorTypes_SETPARAM].params[1]]{ false, false },
		new bool[operators[OperatorTypes_SETPARAM].params[2]]{ false, false }
	};
	operators[OperatorTypes_SETPARAM].readParam = new unsigned char[operators[OperatorTypes_SETPARAM].enumAmount]{0,0,0};
	operators[OperatorTypes_SETPARAM].returns = new VarType[operators[OperatorTypes_SETPARAM].enumAmount]{VarType_NA, VarType_NA,VarType_NA };
	operators[OperatorTypes_SETPARAM].returnType = new ParamType[operators[OperatorTypes_SETPARAM].enumAmount]{ParamType_NA, ParamType_NA,ParamType_NA };
	operators[OperatorTypes_SETPARAM].writeParam = new unsigned char[operators[OperatorTypes_SETPARAM].enumAmount]{0,0,0};
	operators[OperatorTypes_SETPARAM].priority = 0;
	operators[OperatorTypes_SETPARAM].shortHandsAmounts = 0;
	operators[OperatorTypes_SETPARAM].amountParOperators = new unsigned char[operators[OperatorTypes_SETPARAM].enumAmount]{0,0,0};
	operators[OperatorTypes_SETPARAM].parRepeatsMin = new unsigned char*[operators[OperatorTypes_SETPARAM].enumAmount]{
		new unsigned char[operators[OperatorTypes_SETPARAM].params[0]]{0, 0},
			new unsigned char[operators[OperatorTypes_SETPARAM].params[1]]{0, 0},
		new unsigned char[operators[OperatorTypes_SETPARAM].params[2]]{ 0, 0 }
	};
	operators[OperatorTypes_SETPARAM].parRepeatsMax = new unsigned char*[operators[OperatorTypes_SETPARAM].enumAmount]{
		new unsigned char[operators[OperatorTypes_SETPARAM].params[0]]{0, 0},
			new unsigned char[operators[OperatorTypes_SETPARAM].params[1]]{0, 0},
		new unsigned char[operators[OperatorTypes_SETPARAM].params[2]]{ 0, 0 }
	};
	operators[OperatorTypes_SETPARAM].parOperatorAppend = new bool*[operators[OperatorTypes_SETPARAM].enumAmount]{
		new bool[operators[OperatorTypes_SETPARAM].params[0]]{false, false},
			new bool[operators[OperatorTypes_SETPARAM].params[1]]{false, false},
		new bool[operators[OperatorTypes_SETPARAM].params[2]]{ false, false }
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
		new ParamType[operators[OperatorTypes_EXPORT].params[1]]{ParamType_NA, ParamType_VAR}
	};
	operators[OperatorTypes_EXPORT].optionalPar = new bool*[operators[OperatorTypes_EXPORT].enumAmount]{
		new bool[operators[OperatorTypes_EXPORT].params[0]]{ false, false },
		new bool[operators[OperatorTypes_EXPORT].params[1]]{ false, false }
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

	operators[OperatorTypes_FOCUS].keyword = "focus";
	operators[OperatorTypes_FOCUS].enumAmount = 1;
	operators[OperatorTypes_FOCUS].enums = new std::string[operators[OperatorTypes_FOCUS].enumAmount]; operators[OperatorTypes_FOCUS].enums[0] = "ui";
	operators[OperatorTypes_FOCUS].byteCodes = new unsigned char[operators[OperatorTypes_FOCUS].enumAmount]{opcode_FCSUI };
	operators[OperatorTypes_FOCUS].params = new unsigned char[operators[OperatorTypes_FOCUS].enumAmount]{1};
	operators[OperatorTypes_FOCUS].minParams = new unsigned char[operators[OperatorTypes_FOCUS].enumAmount]{1};
	operators[OperatorTypes_FOCUS].paramsyntax = new VarType*[operators[OperatorTypes_FOCUS].enumAmount]{
		new VarType[operators[OperatorTypes_FOCUS].params[0]]{VarType_NUMBER}
	};
	operators[OperatorTypes_FOCUS].paramTypes = new ParamType*[operators[OperatorTypes_FOCUS].enumAmount]{
		new ParamType[operators[OperatorTypes_FOCUS].params[0]]{ParamType_NA}
	};
	operators[OperatorTypes_FOCUS].optionalPar = new bool*[operators[OperatorTypes_FOCUS].enumAmount]{
		new bool[operators[OperatorTypes_FOCUS].params[0]]{ false }
	};
	operators[OperatorTypes_FOCUS].readParam = new unsigned char[operators[OperatorTypes_FOCUS].enumAmount]{0};
	operators[OperatorTypes_FOCUS].returns = new VarType[operators[OperatorTypes_FOCUS].enumAmount]{VarType_NA};
	operators[OperatorTypes_FOCUS].returnType = new ParamType[operators[OperatorTypes_FOCUS].enumAmount]{ParamType_NA};
	operators[OperatorTypes_FOCUS].writeParam = new unsigned char[operators[OperatorTypes_FOCUS].enumAmount]{0};
	operators[OperatorTypes_FOCUS].priority = 0;
	operators[OperatorTypes_FOCUS].shortHandsAmounts = 0;
	operators[OperatorTypes_FOCUS].amountParOperators = new unsigned char[operators[OperatorTypes_FOCUS].enumAmount]{0};
	operators[OperatorTypes_FOCUS].parRepeatsMin = new unsigned char*[operators[OperatorTypes_FOCUS].enumAmount]{
		new unsigned char[operators[OperatorTypes_FOCUS].params[0]]{0}
	};
	operators[OperatorTypes_FOCUS].parRepeatsMax = new unsigned char*[operators[OperatorTypes_FOCUS].enumAmount]{
		new unsigned char[operators[OperatorTypes_FOCUS].params[0]]{0}
	};
	operators[OperatorTypes_FOCUS].parOperatorAppend = new bool*[operators[OperatorTypes_FOCUS].enumAmount]{
		new bool[operators[OperatorTypes_FOCUS].params[0]]{false}
	};


	operators[OperatorTypes_UNLOAD].keyword = "unload";
	operators[OperatorTypes_UNLOAD].enumAmount = 1;
	operators[OperatorTypes_UNLOAD].enums = new std::string[operators[OperatorTypes_UNLOAD].enumAmount]; operators[OperatorTypes_UNLOAD].enums[0] = "";
	operators[OperatorTypes_UNLOAD].byteCodes = new unsigned char[operators[OperatorTypes_UNLOAD].enumAmount]{opcode_UNLOAD };
	operators[OperatorTypes_UNLOAD].params = new unsigned char[operators[OperatorTypes_UNLOAD].enumAmount]{1};
	operators[OperatorTypes_UNLOAD].minParams = new unsigned char[operators[OperatorTypes_UNLOAD].enumAmount]{1};
	operators[OperatorTypes_UNLOAD].paramsyntax = new VarType*[operators[OperatorTypes_UNLOAD].enumAmount]{
		new VarType[operators[OperatorTypes_UNLOAD].params[0]]{VarType_NUMBER}
	};
	operators[OperatorTypes_UNLOAD].paramTypes = new ParamType*[operators[OperatorTypes_UNLOAD].enumAmount]{
		new ParamType[operators[OperatorTypes_UNLOAD].params[0]]{ParamType_NA}
	};
	operators[OperatorTypes_UNLOAD].optionalPar = new bool*[operators[OperatorTypes_UNLOAD].enumAmount]{
		new bool[operators[OperatorTypes_UNLOAD].params[0]]{ false }
	};
	operators[OperatorTypes_UNLOAD].readParam = new unsigned char[operators[OperatorTypes_UNLOAD].enumAmount]{0};
	operators[OperatorTypes_UNLOAD].returns = new VarType[operators[OperatorTypes_UNLOAD].enumAmount]{VarType_NA};
	operators[OperatorTypes_UNLOAD].returnType = new ParamType[operators[OperatorTypes_UNLOAD].enumAmount]{ParamType_NA};
	operators[OperatorTypes_UNLOAD].writeParam = new unsigned char[operators[OperatorTypes_UNLOAD].enumAmount]{0};
	operators[OperatorTypes_UNLOAD].priority = 0;
	operators[OperatorTypes_UNLOAD].shortHandsAmounts = 0;
	operators[OperatorTypes_UNLOAD].amountParOperators = new unsigned char[operators[OperatorTypes_UNLOAD].enumAmount]{0};
	operators[OperatorTypes_UNLOAD].parRepeatsMin = new unsigned char*[operators[OperatorTypes_UNLOAD].enumAmount]{
		new unsigned char[operators[OperatorTypes_UNLOAD].params[0]]{0}
	};
	operators[OperatorTypes_UNLOAD].parRepeatsMax = new unsigned char*[operators[OperatorTypes_UNLOAD].enumAmount]{
		new unsigned char[operators[OperatorTypes_UNLOAD].params[0]]{0}
	};
	operators[OperatorTypes_UNLOAD].parOperatorAppend = new bool*[operators[OperatorTypes_UNLOAD].enumAmount]{
		new bool[operators[OperatorTypes_UNLOAD].params[0]]{false}
	};

	operators[OperatorTypes_REMOVE].keyword = "remove";
	operators[OperatorTypes_REMOVE].enumAmount = 1;
	operators[OperatorTypes_REMOVE].enums = new std::string[operators[OperatorTypes_REMOVE].enumAmount]; operators[OperatorTypes_REMOVE].enums[0] = "";
	operators[OperatorTypes_REMOVE].byteCodes = new unsigned char[operators[OperatorTypes_REMOVE].enumAmount]{opcode_RMVE };
	operators[OperatorTypes_REMOVE].params = new unsigned char[operators[OperatorTypes_REMOVE].enumAmount]{1};
	operators[OperatorTypes_REMOVE].minParams = new unsigned char[operators[OperatorTypes_REMOVE].enumAmount]{1};
	operators[OperatorTypes_REMOVE].paramsyntax = new VarType*[operators[OperatorTypes_REMOVE].enumAmount]{
		new VarType[operators[OperatorTypes_REMOVE].params[0]]{VarType_NUMBER}
	};
	operators[OperatorTypes_REMOVE].paramTypes = new ParamType*[operators[OperatorTypes_REMOVE].enumAmount]{
		new ParamType[operators[OperatorTypes_REMOVE].params[0]]{ParamType_NA}
	};
	operators[OperatorTypes_REMOVE].optionalPar = new bool*[operators[OperatorTypes_REMOVE].enumAmount]{
		new bool[operators[OperatorTypes_REMOVE].params[0]]{ false }
	};
	operators[OperatorTypes_REMOVE].readParam = new unsigned char[operators[OperatorTypes_REMOVE].enumAmount]{0};
	operators[OperatorTypes_REMOVE].returns = new VarType[operators[OperatorTypes_REMOVE].enumAmount]{VarType_NA};
	operators[OperatorTypes_REMOVE].returnType = new ParamType[operators[OperatorTypes_REMOVE].enumAmount]{ParamType_NA};
	operators[OperatorTypes_REMOVE].writeParam = new unsigned char[operators[OperatorTypes_REMOVE].enumAmount]{0};
	operators[OperatorTypes_REMOVE].priority = 0;
	operators[OperatorTypes_REMOVE].shortHandsAmounts = 0;
	operators[OperatorTypes_REMOVE].amountParOperators = new unsigned char[operators[OperatorTypes_REMOVE].enumAmount]{0};
	operators[OperatorTypes_REMOVE].parRepeatsMin = new unsigned char*[operators[OperatorTypes_REMOVE].enumAmount]{
		new unsigned char[operators[OperatorTypes_REMOVE].params[0]]{0}
	};
	operators[OperatorTypes_REMOVE].parRepeatsMax = new unsigned char*[operators[OperatorTypes_REMOVE].enumAmount]{
		new unsigned char[operators[OperatorTypes_REMOVE].params[0]]{0}
	};
	operators[OperatorTypes_REMOVE].parOperatorAppend = new bool*[operators[OperatorTypes_REMOVE].enumAmount]{
		new bool[operators[OperatorTypes_REMOVE].params[0]]{false}
	};


	operators[OperatorTypes_HIDE].keyword = "hide";
	operators[OperatorTypes_HIDE].enumAmount = 1;
	operators[OperatorTypes_HIDE].enums = new std::string[operators[OperatorTypes_HIDE].enumAmount]; operators[OperatorTypes_HIDE].enums[0] = "";
	operators[OperatorTypes_HIDE].byteCodes = new unsigned char[operators[OperatorTypes_HIDE].enumAmount]{opcode_HDE };
	operators[OperatorTypes_HIDE].params = new unsigned char[operators[OperatorTypes_HIDE].enumAmount]{1};
	operators[OperatorTypes_HIDE].minParams = new unsigned char[operators[OperatorTypes_HIDE].enumAmount]{1};
	operators[OperatorTypes_HIDE].paramsyntax = new VarType*[operators[OperatorTypes_HIDE].enumAmount]{
		new VarType[operators[OperatorTypes_HIDE].params[0]]{VarType_NUMBER}
	};
	operators[OperatorTypes_HIDE].paramTypes = new ParamType*[operators[OperatorTypes_HIDE].enumAmount]{
		new ParamType[operators[OperatorTypes_HIDE].params[0]]{ParamType_NA}
	};
	operators[OperatorTypes_HIDE].optionalPar = new bool*[operators[OperatorTypes_HIDE].enumAmount]{
		new bool[operators[OperatorTypes_HIDE].params[0]]{ false }
	};
	operators[OperatorTypes_HIDE].readParam = new unsigned char[operators[OperatorTypes_HIDE].enumAmount]{0};
	operators[OperatorTypes_HIDE].returns = new VarType[operators[OperatorTypes_HIDE].enumAmount]{VarType_NA};
	operators[OperatorTypes_HIDE].returnType = new ParamType[operators[OperatorTypes_HIDE].enumAmount]{ParamType_NA};
	operators[OperatorTypes_HIDE].writeParam = new unsigned char[operators[OperatorTypes_HIDE].enumAmount]{0};
	operators[OperatorTypes_HIDE].priority = 0;
	operators[OperatorTypes_HIDE].shortHandsAmounts = 0;
	operators[OperatorTypes_HIDE].amountParOperators = new unsigned char[operators[OperatorTypes_HIDE].enumAmount]{0};
	operators[OperatorTypes_HIDE].parRepeatsMin = new unsigned char*[operators[OperatorTypes_HIDE].enumAmount]{
		new unsigned char[operators[OperatorTypes_HIDE].params[0]]{0}
	};
	operators[OperatorTypes_HIDE].parRepeatsMax = new unsigned char*[operators[OperatorTypes_HIDE].enumAmount]{
		new unsigned char[operators[OperatorTypes_HIDE].params[0]]{0}
	};
	operators[OperatorTypes_HIDE].parOperatorAppend = new bool*[operators[OperatorTypes_HIDE].enumAmount]{
		new bool[operators[OperatorTypes_HIDE].params[0]]{false}
	};

	operators[OperatorTypes_SHOW].keyword = "show";
	operators[OperatorTypes_SHOW].enumAmount = 1;
	operators[OperatorTypes_SHOW].enums = new std::string[operators[OperatorTypes_SHOW].enumAmount]; operators[OperatorTypes_SHOW].enums[0] = "";
	operators[OperatorTypes_SHOW].byteCodes = new unsigned char[operators[OperatorTypes_SHOW].enumAmount]{opcode_SHW };
	operators[OperatorTypes_SHOW].params = new unsigned char[operators[OperatorTypes_SHOW].enumAmount]{1};
	operators[OperatorTypes_SHOW].minParams = new unsigned char[operators[OperatorTypes_SHOW].enumAmount]{1};
	operators[OperatorTypes_SHOW].paramsyntax = new VarType*[operators[OperatorTypes_SHOW].enumAmount]{
		new VarType[operators[OperatorTypes_SHOW].params[0]]{VarType_NUMBER}
	};
	operators[OperatorTypes_SHOW].paramTypes = new ParamType*[operators[OperatorTypes_SHOW].enumAmount]{
		new ParamType[operators[OperatorTypes_SHOW].params[0]]{ParamType_NA}
	};
	operators[OperatorTypes_SHOW].optionalPar = new bool*[operators[OperatorTypes_SHOW].enumAmount]{
		new bool[operators[OperatorTypes_SHOW].params[0]]{ false }
	};
	operators[OperatorTypes_SHOW].readParam = new unsigned char[operators[OperatorTypes_SHOW].enumAmount]{0};
	operators[OperatorTypes_SHOW].returns = new VarType[operators[OperatorTypes_SHOW].enumAmount]{VarType_NA};
	operators[OperatorTypes_SHOW].returnType = new ParamType[operators[OperatorTypes_SHOW].enumAmount]{ParamType_NA};
	operators[OperatorTypes_SHOW].writeParam = new unsigned char[operators[OperatorTypes_SHOW].enumAmount]{0};
	operators[OperatorTypes_SHOW].priority = 0;
	operators[OperatorTypes_SHOW].shortHandsAmounts = 0;
	operators[OperatorTypes_SHOW].amountParOperators = new unsigned char[operators[OperatorTypes_SHOW].enumAmount]{0};
	operators[OperatorTypes_SHOW].parRepeatsMin = new unsigned char*[operators[OperatorTypes_SHOW].enumAmount]{
		new unsigned char[operators[OperatorTypes_SHOW].params[0]]{0}
	};
	operators[OperatorTypes_SHOW].parRepeatsMax = new unsigned char*[operators[OperatorTypes_SHOW].enumAmount]{
		new unsigned char[operators[OperatorTypes_SHOW].params[0]]{0}
	};
	operators[OperatorTypes_SHOW].parOperatorAppend = new bool*[operators[OperatorTypes_SHOW].enumAmount]{
		new bool[operators[OperatorTypes_SHOW].params[0]]{false}
	};

	operators[OperatorTypes_INCREMENT].keyword = "increment";
	operators[OperatorTypes_INCREMENT].enumAmount = 1;
	operators[OperatorTypes_INCREMENT].enums = new std::string[operators[OperatorTypes_INCREMENT].enumAmount]; operators[OperatorTypes_INCREMENT].enums[0] = "layer";
	operators[OperatorTypes_INCREMENT].byteCodes = new unsigned char[operators[OperatorTypes_INCREMENT].enumAmount]{opcode_INCRMNTLYR};
	operators[OperatorTypes_INCREMENT].params = new unsigned char[operators[OperatorTypes_INCREMENT].enumAmount]{1};
	operators[OperatorTypes_INCREMENT].minParams = new unsigned char[operators[OperatorTypes_INCREMENT].enumAmount]{1};
	operators[OperatorTypes_INCREMENT].paramsyntax = new VarType*[operators[OperatorTypes_INCREMENT].enumAmount]{
		new VarType[operators[OperatorTypes_INCREMENT].params[0]]{VarType_NUMBER}
	};
	operators[OperatorTypes_INCREMENT].paramTypes = new ParamType*[operators[OperatorTypes_INCREMENT].enumAmount]{
		new ParamType[operators[OperatorTypes_INCREMENT].params[0]]{ParamType_NA}
	};
	operators[OperatorTypes_INCREMENT].optionalPar = new bool*[operators[OperatorTypes_INCREMENT].enumAmount]{
		new bool[operators[OperatorTypes_INCREMENT].params[0]]{ false }
	};
	operators[OperatorTypes_INCREMENT].readParam = new unsigned char[operators[OperatorTypes_INCREMENT].enumAmount]{0};
	operators[OperatorTypes_INCREMENT].returns = new VarType[operators[OperatorTypes_INCREMENT].enumAmount]{VarType_NA};
	operators[OperatorTypes_INCREMENT].returnType = new ParamType[operators[OperatorTypes_INCREMENT].enumAmount]{ParamType_NA};
	operators[OperatorTypes_INCREMENT].writeParam = new unsigned char[operators[OperatorTypes_INCREMENT].enumAmount]{0};
	operators[OperatorTypes_INCREMENT].priority = 0;
	operators[OperatorTypes_INCREMENT].shortHandsAmounts = 0;
	operators[OperatorTypes_INCREMENT].amountParOperators = new unsigned char[operators[OperatorTypes_INCREMENT].enumAmount]{0};
	operators[OperatorTypes_INCREMENT].parRepeatsMin = new unsigned char*[operators[OperatorTypes_INCREMENT].enumAmount]{
		new unsigned char[operators[OperatorTypes_INCREMENT].params[0]]{0}
	};
	operators[OperatorTypes_INCREMENT].parRepeatsMax = new unsigned char*[operators[OperatorTypes_INCREMENT].enumAmount]{
		new unsigned char[operators[OperatorTypes_INCREMENT].params[0]]{0}
	};
	operators[OperatorTypes_INCREMENT].parOperatorAppend = new bool*[operators[OperatorTypes_INCREMENT].enumAmount]{
		new bool[operators[OperatorTypes_INCREMENT].params[0]]{false}
	};

	operators[OperatorTypes_DECREMENT].keyword = "decrement";
	operators[OperatorTypes_DECREMENT].enumAmount = 1;
	operators[OperatorTypes_DECREMENT].enums = new std::string[operators[OperatorTypes_DECREMENT].enumAmount]; operators[OperatorTypes_DECREMENT].enums[0] = "layer";
	operators[OperatorTypes_DECREMENT].byteCodes = new unsigned char[operators[OperatorTypes_DECREMENT].enumAmount]{opcode_DCRMNLYR};
	operators[OperatorTypes_DECREMENT].params = new unsigned char[operators[OperatorTypes_DECREMENT].enumAmount]{1};
	operators[OperatorTypes_DECREMENT].minParams = new unsigned char[operators[OperatorTypes_DECREMENT].enumAmount]{1};
	operators[OperatorTypes_DECREMENT].paramsyntax = new VarType*[operators[OperatorTypes_DECREMENT].enumAmount]{
		new VarType[operators[OperatorTypes_DECREMENT].params[0]]{VarType_NUMBER}
	};
	operators[OperatorTypes_DECREMENT].paramTypes = new ParamType*[operators[OperatorTypes_DECREMENT].enumAmount]{
		new ParamType[operators[OperatorTypes_DECREMENT].params[0]]{ParamType_NA}
	};
	operators[OperatorTypes_DECREMENT].optionalPar = new bool*[operators[OperatorTypes_DECREMENT].enumAmount]{
		new bool[operators[OperatorTypes_DECREMENT].params[0]]{ false }
	};
	operators[OperatorTypes_DECREMENT].readParam = new unsigned char[operators[OperatorTypes_DECREMENT].enumAmount]{0};
	operators[OperatorTypes_DECREMENT].returns = new VarType[operators[OperatorTypes_DECREMENT].enumAmount]{VarType_NA};
	operators[OperatorTypes_DECREMENT].returnType = new ParamType[operators[OperatorTypes_DECREMENT].enumAmount]{ParamType_NA};
	operators[OperatorTypes_DECREMENT].writeParam = new unsigned char[operators[OperatorTypes_DECREMENT].enumAmount]{0};
	operators[OperatorTypes_DECREMENT].priority = 0;
	operators[OperatorTypes_DECREMENT].shortHandsAmounts = 0;
	operators[OperatorTypes_DECREMENT].amountParOperators = new unsigned char[operators[OperatorTypes_DECREMENT].enumAmount]{0};
	operators[OperatorTypes_DECREMENT].parRepeatsMin = new unsigned char*[operators[OperatorTypes_DECREMENT].enumAmount]{
		new unsigned char[operators[OperatorTypes_DECREMENT].params[0]]{0}
	};
	operators[OperatorTypes_DECREMENT].parRepeatsMax = new unsigned char*[operators[OperatorTypes_DECREMENT].enumAmount]{
		new unsigned char[operators[OperatorTypes_DECREMENT].params[0]]{0}
	};
	operators[OperatorTypes_DECREMENT].parOperatorAppend = new bool*[operators[OperatorTypes_DECREMENT].enumAmount]{
		new bool[operators[OperatorTypes_DECREMENT].params[0]]{false}
	};

	operators[OperatorTypes_IGNORE].keyword = "ignore";
	operators[OperatorTypes_IGNORE].enumAmount = 3;
	operators[OperatorTypes_IGNORE].enums = new std::string[operators[OperatorTypes_IGNORE].enumAmount]; operators[OperatorTypes_IGNORE].enums[0] = "mouse"; operators[OperatorTypes_IGNORE].enums[1] = "keys"; operators[OperatorTypes_IGNORE].enums[2] = "input";
	operators[OperatorTypes_IGNORE].byteCodes = new unsigned char[operators[OperatorTypes_IGNORE].enumAmount]{opcode_IGNRMS, opcode_IGNRKY, opcode_IGNRIN};
	operators[OperatorTypes_IGNORE].params = new unsigned char[operators[OperatorTypes_IGNORE].enumAmount]{1, 1, 1};
	operators[OperatorTypes_IGNORE].minParams = new unsigned char[operators[OperatorTypes_IGNORE].enumAmount]{1, 1, 1};
	operators[OperatorTypes_IGNORE].paramsyntax = new VarType*[operators[OperatorTypes_IGNORE].enumAmount]{
		new VarType[operators[OperatorTypes_IGNORE].params[0]]{VarType_NUMBER},
		new VarType[operators[OperatorTypes_IGNORE].params[1]]{VarType_NUMBER},
		new VarType[operators[OperatorTypes_IGNORE].params[2]]{VarType_NUMBER}
	};
	operators[OperatorTypes_IGNORE].paramTypes = new ParamType*[operators[OperatorTypes_IGNORE].enumAmount]{
		new ParamType[operators[OperatorTypes_IGNORE].params[0]]{ParamType_NA},
			new ParamType[operators[OperatorTypes_IGNORE].params[1]]{ParamType_NA},
			new ParamType[operators[OperatorTypes_IGNORE].params[2]]{ParamType_NA}
	};
	operators[OperatorTypes_IGNORE].optionalPar = new bool*[operators[OperatorTypes_IGNORE].enumAmount]{
		new bool[operators[OperatorTypes_IGNORE].params[0]]{ false },
			new bool[operators[OperatorTypes_IGNORE].params[1]]{ false },
			new bool[operators[OperatorTypes_IGNORE].params[1]]{ false }
	};
	operators[OperatorTypes_IGNORE].readParam = new unsigned char[operators[OperatorTypes_IGNORE].enumAmount]{0,0,0};
	operators[OperatorTypes_IGNORE].returns = new VarType[operators[OperatorTypes_IGNORE].enumAmount]{VarType_NA, VarType_NA, VarType_NA};
	operators[OperatorTypes_IGNORE].returnType = new ParamType[operators[OperatorTypes_IGNORE].enumAmount]{ParamType_NA, ParamType_NA, ParamType_NA};
	operators[OperatorTypes_IGNORE].writeParam = new unsigned char[operators[OperatorTypes_IGNORE].enumAmount]{0, 0, 0};
	operators[OperatorTypes_IGNORE].priority = 0;
	operators[OperatorTypes_IGNORE].shortHandsAmounts = 0;
	operators[OperatorTypes_IGNORE].amountParOperators = new unsigned char[operators[OperatorTypes_IGNORE].enumAmount]{0, 0, 0};
	operators[OperatorTypes_IGNORE].parRepeatsMin = new unsigned char*[operators[OperatorTypes_IGNORE].enumAmount]{
		new unsigned char[operators[OperatorTypes_IGNORE].params[0]]{0},
		new unsigned char[operators[OperatorTypes_IGNORE].params[1]]{0},
		new unsigned char[operators[OperatorTypes_IGNORE].params[2]]{0}
	};
	operators[OperatorTypes_IGNORE].parRepeatsMax = new unsigned char*[operators[OperatorTypes_IGNORE].enumAmount]{
		new unsigned char[operators[OperatorTypes_IGNORE].params[0]]{0},
			new unsigned char[operators[OperatorTypes_IGNORE].params[1]]{0},
			new unsigned char[operators[OperatorTypes_IGNORE].params[2]]{0}
	};
	operators[OperatorTypes_IGNORE].parOperatorAppend = new bool*[operators[OperatorTypes_IGNORE].enumAmount]{
		new bool[operators[OperatorTypes_IGNORE].params[0]]{false},
			new bool[operators[OperatorTypes_IGNORE].params[1]]{false},
			new bool[operators[OperatorTypes_IGNORE].params[2]]{false}
	};

	operators[OperatorTypes_ACCEPT].keyword = "accept";
	operators[OperatorTypes_ACCEPT].enumAmount = 3;
	operators[OperatorTypes_ACCEPT].enums = new std::string[operators[OperatorTypes_ACCEPT].enumAmount]; operators[OperatorTypes_ACCEPT].enums[0] = "mouse"; operators[OperatorTypes_ACCEPT].enums[1] = "keys"; operators[OperatorTypes_ACCEPT].enums[2] = "input";
	operators[OperatorTypes_ACCEPT].byteCodes = new unsigned char[operators[OperatorTypes_ACCEPT].enumAmount]{opcode_ACPTMS, opcode_ACPTKY, opcode_ACPTIN};
	operators[OperatorTypes_ACCEPT].params = new unsigned char[operators[OperatorTypes_ACCEPT].enumAmount]{1, 1, 1};
	operators[OperatorTypes_ACCEPT].minParams = new unsigned char[operators[OperatorTypes_ACCEPT].enumAmount]{1, 1, 1};
	operators[OperatorTypes_ACCEPT].paramsyntax = new VarType*[operators[OperatorTypes_ACCEPT].enumAmount]{
		new VarType[operators[OperatorTypes_ACCEPT].params[0]]{VarType_NUMBER},
			new VarType[operators[OperatorTypes_ACCEPT].params[1]]{VarType_NUMBER},
			new VarType[operators[OperatorTypes_ACCEPT].params[2]]{VarType_NUMBER}
	};
	operators[OperatorTypes_ACCEPT].paramTypes = new ParamType*[operators[OperatorTypes_ACCEPT].enumAmount]{
		new ParamType[operators[OperatorTypes_ACCEPT].params[0]]{ParamType_NA},
			new ParamType[operators[OperatorTypes_ACCEPT].params[1]]{ParamType_NA},
			new ParamType[operators[OperatorTypes_ACCEPT].params[2]]{ParamType_NA}
	};
	operators[OperatorTypes_ACCEPT].optionalPar = new bool*[operators[OperatorTypes_ACCEPT].enumAmount]{
		new bool[operators[OperatorTypes_ACCEPT].params[0]]{ false },
			new bool[operators[OperatorTypes_ACCEPT].params[1]]{ false },
			new bool[operators[OperatorTypes_ACCEPT].params[2]]{ false }
	};
	operators[OperatorTypes_ACCEPT].readParam = new unsigned char[operators[OperatorTypes_ACCEPT].enumAmount]{0, 0, 0};
	operators[OperatorTypes_ACCEPT].returns = new VarType[operators[OperatorTypes_ACCEPT].enumAmount]{VarType_NA, VarType_NA, VarType_NA};
	operators[OperatorTypes_ACCEPT].returnType = new ParamType[operators[OperatorTypes_ACCEPT].enumAmount]{ParamType_NA, ParamType_NA, ParamType_NA};
	operators[OperatorTypes_ACCEPT].writeParam = new unsigned char[operators[OperatorTypes_ACCEPT].enumAmount]{0, 0, 0};
	operators[OperatorTypes_ACCEPT].priority = 0;
	operators[OperatorTypes_ACCEPT].shortHandsAmounts = 0;
	operators[OperatorTypes_ACCEPT].amountParOperators = new unsigned char[operators[OperatorTypes_ACCEPT].enumAmount]{0, 0, 0};
	operators[OperatorTypes_ACCEPT].parRepeatsMin = new unsigned char*[operators[OperatorTypes_ACCEPT].enumAmount]{
		new unsigned char[operators[OperatorTypes_ACCEPT].params[0]]{0},
			new unsigned char[operators[OperatorTypes_ACCEPT].params[1]]{0},
			new unsigned char[operators[OperatorTypes_ACCEPT].params[2]]{0}
	};
	operators[OperatorTypes_ACCEPT].parRepeatsMax = new unsigned char*[operators[OperatorTypes_ACCEPT].enumAmount]{
		new unsigned char[operators[OperatorTypes_ACCEPT].params[0]]{0},
			new unsigned char[operators[OperatorTypes_ACCEPT].params[1]]{0},
			new unsigned char[operators[OperatorTypes_ACCEPT].params[2]]{0}
	};
	operators[OperatorTypes_ACCEPT].parOperatorAppend = new bool*[operators[OperatorTypes_ACCEPT].enumAmount]{
		new bool[operators[OperatorTypes_ACCEPT].params[0]]{false},
			new bool[operators[OperatorTypes_ACCEPT].params[1]]{false},
			new bool[operators[OperatorTypes_ACCEPT].params[2]]{false}
	};

	operators[OperatorTypes_LINK].keyword = "link";
	operators[OperatorTypes_LINK].enumAmount = 2;
	operators[OperatorTypes_LINK].enums = new std::string[operators[OperatorTypes_LINK].enumAmount]; operators[OperatorTypes_LINK].enums[0] = "target"; operators[OperatorTypes_LINK].enums[1] = "out";
	operators[OperatorTypes_LINK].byteCodes = new unsigned char[operators[OperatorTypes_LINK].enumAmount]{ opcode_LNKTRGT, opcode_LNKDBG};
	operators[OperatorTypes_LINK].params = new unsigned char[operators[OperatorTypes_LINK].enumAmount]{ 2, 1};
	operators[OperatorTypes_LINK].minParams = new unsigned char[operators[OperatorTypes_LINK].enumAmount]{ 2, 1};
	operators[OperatorTypes_LINK].paramsyntax = new VarType*[operators[OperatorTypes_LINK].enumAmount]{
		new VarType[operators[OperatorTypes_LINK].params[0]]{ VarType_NUMBER, VarType_NUMBER },
		new VarType[operators[OperatorTypes_LINK].params[1]]{ VarType_NUMBER }
	};
	operators[OperatorTypes_LINK].paramTypes = new ParamType*[operators[OperatorTypes_LINK].enumAmount]{
		new ParamType[operators[OperatorTypes_LINK].params[0]]{ ParamType_NA, ParamType_NA },
		new ParamType[operators[OperatorTypes_LINK].params[1]]{ ParamType_NA }
	};
	operators[OperatorTypes_LINK].optionalPar = new bool*[operators[OperatorTypes_LINK].enumAmount]{
		new bool[operators[OperatorTypes_LINK].params[0]]{ false, false },
		new bool[operators[OperatorTypes_LINK].params[1]]{ false }
	};
	operators[OperatorTypes_LINK].readParam = new unsigned char[operators[OperatorTypes_LINK].enumAmount]{ 0, 0 };
	operators[OperatorTypes_LINK].returns = new VarType[operators[OperatorTypes_LINK].enumAmount]{ VarType_NA, VarType_NA };
	operators[OperatorTypes_LINK].returnType = new ParamType[operators[OperatorTypes_LINK].enumAmount]{ ParamType_NA, ParamType_NA };
	operators[OperatorTypes_LINK].writeParam = new unsigned char[operators[OperatorTypes_LINK].enumAmount]{ 0, 0};
	operators[OperatorTypes_LINK].priority = 0;
	operators[OperatorTypes_LINK].shortHandsAmounts = 0;
	operators[OperatorTypes_LINK].amountParOperators = new unsigned char[operators[OperatorTypes_LINK].enumAmount]{ 0, 0};
	operators[OperatorTypes_LINK].parRepeatsMin = new unsigned char*[operators[OperatorTypes_LINK].enumAmount]{
		new unsigned char[operators[OperatorTypes_LINK].params[0]]{ 0, 0 },
		new unsigned char[operators[OperatorTypes_LINK].params[1]]{ 0 }
	};
	operators[OperatorTypes_LINK].parRepeatsMax = new unsigned char*[operators[OperatorTypes_LINK].enumAmount]{
		new unsigned char[operators[OperatorTypes_LINK].params[0]]{ 0, 0 },
		new unsigned char[operators[OperatorTypes_LINK].params[1]]{ 0 }
	};
	operators[OperatorTypes_LINK].parOperatorAppend = new bool*[operators[OperatorTypes_LINK].enumAmount]{
		new bool[operators[OperatorTypes_LINK].params[0]]{ false, false },
		new bool[operators[OperatorTypes_LINK].params[1]]{ false }
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
			!((i == 1 && param[i] == 's') || (i == 1 && param[i] == 'f'));

		if (!isRP && param[i] != 's' && param[i] != 'f')
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
				unsigned int scriptFloatParams = 0;

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
								else if (symbol[1] == 'f' && scriptStringParams < param)
								{

									scriptFloatParams = param;

								}
								else if (scriptParams < param)
								{

									scriptParams = param;

								}

								if (compiledSource != nullptr)
								{

									compiledSource->AddParamAdr(param - 1, abstractSyntaxTree.symbolTable->GetValue(i).address, symbol[1]);

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
						
						compiledSource = new CelScriptCompiled(scriptParams, scriptStringParams, scriptFloatParams);

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

		outText->AddTextLine("Error compiling celscript at line: " + std::to_string(error.line + 1)+ " in file: "+source->GetFile(error.line+1));

		if (error.errorType == Logic::ScriptError_BADSIG)
		{

			outText->AddTextLine("Incompatible parameters");

		}
		else if (error.errorType == Logic::ScriptError_CONDITION)
		{

			outText->AddTextLine("Need condition");

		}
		else if (error.errorType == Logic::ScriptError_ENUMMISPLACE)
		{

			outText->AddTextLine("Misplaced Enumeration");

		}
		else if (error.errorType == Logic::ScriptError_HUH)
		{

			outText->AddTextLine("Beats the fuck out of me...");

		}
		else if (error.errorType == Logic::ScriptError_INCOMPATIBLE)
		{

			outText->AddTextLine("Type error");

		}
		else if (error.errorType == Logic::ScriptError_INCOMPATIBLEENUM)
		{

			outText->AddTextLine("I have no idea..");

		}
		else if (error.errorType == Logic::ScriptError_INCOMPATIBLERETURN)
		{

			outText->AddTextLine("Wrong return type");

		}
		else if (error.errorType == Logic::ScriptError_NESTEDFLOW)
		{

			outText->AddTextLine("Flow operators cannot nest on the same lline");

		}
		else if (error.errorType == Logic::ScriptError_NOBODY)
		{

			outText->AddTextLine("Lacking body");

		}
		else if (error.errorType == Logic::ScriptError_TOFEWPARAM)
		{

			outText->AddTextLine("Not enough parameters");

		}
		else if (error.errorType == Logic::ScriptError_TOMANYPARAM)
		{

			outText->AddTextLine("Too many parameters");

		}
		else if (error.errorType == Logic::ScriptError_UNCLOSED)
		{

			outText->AddTextLine("Unclosed string");

		}
		else if (error.errorType == Logic::ScriptError_UNCLOSEDBODY)
		{

			outText->AddTextLine("Unclosed Body");

		}
		else if (error.errorType == Logic::ScriptError_UNKNOWNKEYWORD)
		{

			outText->AddTextLine("Expected valid Keyword");

		}
		else if (error.errorType == Logic::ScriptError_VARUNIN)
		{

			outText->AddTextLine("Variable needs a value");

		}
		else if (error.errorType == Logic::ScriptError_PARAMWRONGPLACE)
		{

			outText->AddTextLine("Runtime Parameter marker in the wrong place");

		}
		else if (error.errorType == Logic::ScriptError_UNKNOWNSYSTEMPARAM)
		{

			outText->AddTextLine("Unknown System parameter");

		}
		else
		{

			outText->AddTextLine("Unknown error, sucks to be you");

		}
	}
	else
	{
		
		outText->AddTextLine("Script compiled ok!");
		
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
#include "KubLingCompiler.h"

using namespace Logic;
using namespace Resources;


KubLingCompiler::KubLingCompiler()
{

	this->outText = nullptr;
	operators = new Operator[OperatorTypes_NA];

	operators[OperatorTypes_SET].keyword = "set";
	operators[OperatorTypes_SET].enumAmount = 21;
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
	operators[OperatorTypes_SET].enums[17] = "mouseupdown";
	operators[OperatorTypes_SET].enums[18] = "cursor";
	operators[OperatorTypes_SET].enums[19] = "nodearrival";
	operators[OperatorTypes_SET].enums[20] = "splitscript";
	operators[OperatorTypes_SET].byteCodes = new unsigned char[operators[OperatorTypes_SET].enumAmount]{
		bytecode_SETCONST, 
		bytecode_SETVAR, 
		bytecode_SETGMEBRD, 
		bytecode_SETCMRA, 
		bytecode_SETLCLK, 
		bytecode_LNKDBG, 
		bytecode_STXT, 
		bytecode_SETUI, 
		bytecode_STCNTNT, 
		bytecode_STBRDR, 
		bytecode_STLYR,
		bytecode_SETMCLK,
		bytecode_SETRCLK,
		bytecode_SETLDRG,
		bytecode_SETMDRG,
		bytecode_SETRDRG,
		bytecode_SETMWHL,
		bytecode_SETUD,
		bytecode_SETCRS,
		bytecode_SETNDEARRVL,
		bytecode_SETSPLT};
	operators[OperatorTypes_SET].params = new unsigned char[operators[OperatorTypes_SET].enumAmount]{2, 2, 1, 1, 2, 1, 2, 1, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2};
	operators[OperatorTypes_SET].minParams = new unsigned char[operators[OperatorTypes_SET].enumAmount]{ 2, 2, 1, 1, 2, 1, 2, 1, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2};
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
			new VarType[operators[OperatorTypes_SET].params[16]]{ VarType_NUMBER, VarType_NUMBER },
			new VarType[operators[OperatorTypes_SET].params[17]]{ VarType_NUMBER, VarType_NUMBER },
			new VarType[operators[OperatorTypes_SET].params[18]]{ VarType_NUMBER },
			new VarType[operators[OperatorTypes_SET].params[19]]{ VarType_NUMBER, VarType_NUMBER },
			new VarType[operators[OperatorTypes_SET].params[20]]{ VarType_NUMBER, VarType_NUMBER }
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
			new ParamType[operators[OperatorTypes_SET].params[16]]{ ParamType_NA, ParamType_NA },
			new ParamType[operators[OperatorTypes_SET].params[17]]{ ParamType_NA, ParamType_NA },
			new ParamType[operators[OperatorTypes_SET].params[18]]{ ParamType_NA},
			new ParamType[operators[OperatorTypes_SET].params[19]]{ ParamType_NA, ParamType_NA },
			new ParamType[operators[OperatorTypes_SET].params[20]]{ ParamType_NA, ParamType_NA }
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
		new bool[operators[OperatorTypes_SET].params[16]]{ false, false },
		new bool[operators[OperatorTypes_SET].params[17]]{ false, false },
		new bool[operators[OperatorTypes_SET].params[18]]{ false},
		new bool[operators[OperatorTypes_SET].params[19]]{ false, false },
		new bool[operators[OperatorTypes_SET].params[20]]{ false, false }
	};
	operators[OperatorTypes_SET].readParam = new unsigned char[operators[OperatorTypes_SET].enumAmount]{2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	operators[OperatorTypes_SET].returns = new VarType[operators[OperatorTypes_SET].enumAmount]{VarType_NA, VarType_NA, VarType_NA, VarType_NA, VarType_NA, VarType_NA, VarType_NA, VarType_NA, VarType_NA, VarType_NA, VarType_NA, VarType_NA, VarType_NA,VarType_NA,VarType_NA,VarType_NA,VarType_NA,VarType_NA,VarType_NA,VarType_NA,VarType_NA };
	operators[OperatorTypes_SET].returnType = new ParamType[operators[OperatorTypes_SET].enumAmount]{ParamType_NA, ParamType_NA, ParamType_NA, ParamType_NA, ParamType_NA, ParamType_NA, ParamType_NA, ParamType_NA, ParamType_NA, ParamType_NA, ParamType_NA, ParamType_NA, ParamType_NA,ParamType_NA,ParamType_NA,ParamType_NA,ParamType_NA,ParamType_NA,ParamType_NA,ParamType_NA,ParamType_NA };
	operators[OperatorTypes_SET].writeParam = new unsigned char[operators[OperatorTypes_SET].enumAmount]{1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0};
	operators[OperatorTypes_SET].priority = 0;
	operators[OperatorTypes_SET].shortHandsAmounts = 1;
	operators[OperatorTypes_SET].shortHands = new std::string[operators[OperatorTypes_SET].shortHandsAmounts]; operators[OperatorTypes_SET].shortHands[0] = "=" ;
	operators[OperatorTypes_SET].rightParams = new unsigned char[operators[OperatorTypes_SET].shortHandsAmounts]{1};
	operators[OperatorTypes_SET].leftParams = new unsigned char[operators[OperatorTypes_SET].shortHandsAmounts]{1};
	operators[OperatorTypes_SET].shortFlipParams = new bool[operators[OperatorTypes_SET].shortHandsAmounts]{false};
	operators[OperatorTypes_SET].amountParOperators = new unsigned char[operators[OperatorTypes_SET].enumAmount]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0};
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
		new unsigned char[operators[OperatorTypes_SET].params[16]]{ 0, 0 },
		new unsigned char[operators[OperatorTypes_SET].params[17]]{ 0, 0 },
		new unsigned char[operators[OperatorTypes_SET].params[18]]{ 0},
		new unsigned char[operators[OperatorTypes_SET].params[19]]{ 0, 0 },
		new unsigned char[operators[OperatorTypes_SET].params[20]]{ 0, 0 }
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
		new unsigned char[operators[OperatorTypes_SET].params[16]]{ 0, 0 },
		new unsigned char[operators[OperatorTypes_SET].params[17]]{ 0, 0 },
		new unsigned char[operators[OperatorTypes_SET].params[18]]{ 0},
		new unsigned char[operators[OperatorTypes_SET].params[19]]{ 0, 0 },
		new unsigned char[operators[OperatorTypes_SET].params[20]]{ 0, 0 }
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
		new bool[operators[OperatorTypes_SET].params[16]]{ false, false },
		new bool[operators[OperatorTypes_SET].params[17]]{ false, false },
		new bool[operators[OperatorTypes_SET].params[18]]{ false},
		new bool[operators[OperatorTypes_SET].params[19]]{ false, false },
		new bool[operators[OperatorTypes_SET].params[20]]{ false, false }
	};

	operators[OperatorTypes_LOAD].keyword = "load";
	operators[OperatorTypes_LOAD].enumAmount = 12;
	operators[OperatorTypes_LOAD].enums = new std::string[operators[OperatorTypes_LOAD].enumAmount]; 
	operators[OperatorTypes_LOAD].enums[0] = "light"; 
	operators[OperatorTypes_LOAD].enums[1] = "mesh";
	operators[OperatorTypes_LOAD].enums[2] = "script";
	operators[OperatorTypes_LOAD].enums[3] = "textbox";
	operators[OperatorTypes_LOAD].enums[4] = "panel";
	operators[OperatorTypes_LOAD].enums[5] = "image";
	operators[OperatorTypes_LOAD].enums[6] = "task";
	operators[OperatorTypes_LOAD].enums[7] = "gameboard";
	operators[OperatorTypes_LOAD].enums[8] = "camera";
	operators[OperatorTypes_LOAD].enums[9] = "object";
	operators[OperatorTypes_LOAD].enums[10] = "copyobject";
	operators[OperatorTypes_LOAD].enums[11] = "state";
	operators[OperatorTypes_LOAD].byteCodes = new unsigned char[operators[OperatorTypes_LOAD].enumAmount]{bytecode_LOADLIGHT, bytecode_LOADMESH, bytecode_LOADSCRIPT, bytecode_LOADTXTBX, bytecode_LOADPANEL, bytecode_LOADIMAGE, bytecode_LOADTSK, bytecode_LOADGMBRD, bytecode_LOADCAM, bytecode_LOADOBJCT,bytecode_LOADCPY, bytecode_LDSTT };
	operators[OperatorTypes_LOAD].params = new unsigned char[operators[OperatorTypes_LOAD].enumAmount]{3,1,1, 0,0,1,
		2,2,0, 2, 1, 2};
	operators[OperatorTypes_LOAD].minParams = new unsigned char[operators[OperatorTypes_LOAD].enumAmount]{ 3, 1,1, 0,0,1,
		1,1, 0, 2, 1, 2};
	operators[OperatorTypes_LOAD].paramsyntax = new VarType*[operators[OperatorTypes_LOAD].enumAmount]{
		new VarType[operators[OperatorTypes_LOAD].params[0]]{VarType_NUMBER, VarType_NUMBER, VarType_NUMBER}, 
			new VarType[operators[OperatorTypes_LOAD].params[1]]{VarType_STRING},
			new VarType[operators[OperatorTypes_LOAD].params[2]]{VarType_STRING},
			nullptr,nullptr,
			new VarType[operators[OperatorTypes_LOAD].params[5]]{VarType_STRING},
			new VarType[operators[OperatorTypes_LOAD].params[6]]{VarType_NUMBER, VarType_NA},
			new VarType[operators[OperatorTypes_LOAD].params[7]]{VarType_NUMBER, VarType_NUMBER},
			nullptr,
			new VarType[operators[OperatorTypes_LOAD].params[9]]{VarType_NUMBER, VarType_NUMBER },
			new VarType[operators[OperatorTypes_LOAD].params[10]]{ VarType_NUMBER },
			new VarType[operators[OperatorTypes_LOAD].params[11]]{ VarType_NUMBER, VarType_STRING } };
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
			new ParamType[operators[OperatorTypes_LOAD].params[9]]{ParamType_NA, ParamType_NA },
			new ParamType[operators[OperatorTypes_LOAD].params[10]]{ ParamType_NA },
			new ParamType[operators[OperatorTypes_LOAD].params[11]]{ ParamType_NA, ParamType_NA } };
	operators[OperatorTypes_LOAD].optionalPar = new bool*[operators[OperatorTypes_LOAD].enumAmount]{
		new bool[operators[OperatorTypes_LOAD].params[0]]{false, false, false },
		new bool[operators[OperatorTypes_LOAD].params[1]]{ false },
		new bool[operators[OperatorTypes_LOAD].params[2]]{ false},
		nullptr,
		nullptr,
		new bool[operators[OperatorTypes_LOAD].params[5]]{ false },
		new bool[operators[OperatorTypes_LOAD].params[6]]{ false, true },
		new bool[operators[OperatorTypes_LOAD].params[7]]{ false, true },
		nullptr,
		new bool[operators[OperatorTypes_LOAD].params[9]]{ false, false },
		new bool[operators[OperatorTypes_LOAD].params[10]]{ false },
		new bool[operators[OperatorTypes_LOAD].params[11]]{ false, false }
	};
	operators[OperatorTypes_LOAD].readParam = new unsigned char[operators[OperatorTypes_LOAD].enumAmount]{0,0,0,0,0,0,0,0,0,0,0,0};
	operators[OperatorTypes_LOAD].priority = 0;
	operators[OperatorTypes_LOAD].returns = new VarType[operators[OperatorTypes_LOAD].enumAmount]{VarType_NUMBER, VarType_NUMBER, VarType_NUMBER, VarType_NUMBER, VarType_NUMBER, VarType_NUMBER, VarType_NUMBER, VarType_NUMBER, VarType_NUMBER, VarType_NUMBER,VarType_NUMBER,VarType_NUMBER };
	operators[OperatorTypes_LOAD].returnType = new ParamType[operators[OperatorTypes_LOAD].enumAmount]{ParamType_VAR, ParamType_VAR, ParamType_VAR, ParamType_VAR, ParamType_VAR, ParamType_VAR, ParamType_VAR, ParamType_VAR, ParamType_VAR, ParamType_VAR,ParamType_VAR,ParamType_VAR };
	operators[OperatorTypes_LOAD].writeParam = new unsigned char[operators[OperatorTypes_LOAD].enumAmount]{0, 0,0,0,0,0,0,0, 0,0,0, 0};
	operators[OperatorTypes_LOAD].shortHandsAmounts = 0;
	operators[OperatorTypes_LOAD].amountParOperators = new unsigned char[operators[OperatorTypes_LOAD].enumAmount]{0, 0, 0, 0, 0, 0,
		1,0, 0,0,0,0};
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
			new unsigned char[operators[OperatorTypes_LOAD].params[9]]{0, 0},
			new unsigned char[operators[OperatorTypes_LOAD].params[10]]{ 0 },
			new unsigned char[operators[OperatorTypes_LOAD].params[11]]{ 0, 0 }
	};
	operators[OperatorTypes_LOAD].parRepeatsMax = new unsigned char*[operators[OperatorTypes_LOAD].enumAmount]{
		new unsigned char[operators[OperatorTypes_LOAD].params[0]]{0, 0, 0},
			new unsigned char[operators[OperatorTypes_LOAD].params[1]]{0},
			new unsigned char[operators[OperatorTypes_LOAD].params[2]]{0},
			nullptr,
			nullptr,
			new unsigned char[operators[OperatorTypes_LOAD].params[5]]{0},
			new unsigned char[operators[OperatorTypes_LOAD].params[6]]{0, 16},
			new unsigned char[operators[OperatorTypes_LOAD].params[7]]{0,0},
			nullptr,
			new unsigned char[operators[OperatorTypes_LOAD].params[9]]{0, 0},
			new unsigned char[operators[OperatorTypes_LOAD].params[10]]{ 0 },
			new unsigned char[operators[OperatorTypes_LOAD].params[11]]{ 0, 0 }
	};

	operators[OperatorTypes_LOAD].parOperators = new OperatorTypes*[operators[OperatorTypes_LOAD].enumAmount]{
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		new OperatorTypes[operators[OperatorTypes_LOAD].amountParOperators[6]]{ OperatorTypes_ADDTASKPARAM },
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
	};

	operators[OperatorTypes_LOAD].parOperator = new unsigned char*[operators[OperatorTypes_LOAD].enumAmount]{ 
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		new unsigned char[operators[OperatorTypes_LOAD].params[6]]{ 0,1 },
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
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
			new bool[operators[OperatorTypes_LOAD].params[9]]{false, false },
			new bool[operators[OperatorTypes_LOAD].params[10]]{ false },
			new bool[operators[OperatorTypes_LOAD].params[11]]{ false, false }
	};

	operators[OperatorTypes_ADDTASKPARAM].keyword = "//addtskprm";
	operators[OperatorTypes_ADDTASKPARAM].enumAmount = 3;
	operators[OperatorTypes_ADDTASKPARAM].enums = new std::string[operators[OperatorTypes_ADDTASKPARAM].enumAmount]; operators[OperatorTypes_ADDTASKPARAM].enums[0] = "num"; operators[OperatorTypes_ADDTASKPARAM].enums[1] = "str"; operators[OperatorTypes_ADDTASKPARAM].enums[2] = "float";
	operators[OperatorTypes_ADDTASKPARAM].byteCodes = new unsigned char[operators[OperatorTypes_ADDTASKPARAM].enumAmount]{ bytecode_ADDTSKPARNUM,bytecode_ADDTSKPARSTR,bytecode_ADDTSKPARFLT };
	operators[OperatorTypes_ADDTASKPARAM].params = new unsigned char[operators[OperatorTypes_ADDTASKPARAM].enumAmount]{ 2,2,2 };
	operators[OperatorTypes_ADDTASKPARAM].minParams = new unsigned char[operators[OperatorTypes_ADDTASKPARAM].enumAmount]{ 1,1,1 };
	operators[OperatorTypes_ADDTASKPARAM].paramsyntax = new VarType*[operators[OperatorTypes_ADDTASKPARAM].enumAmount]{
		new VarType[operators[OperatorTypes_ADDTASKPARAM].params[0]]{ VarType_NUMBER,VarType_NUMBER },
		new VarType[operators[OperatorTypes_ADDTASKPARAM].params[1]]{ VarType_STRING, VarType_NUMBER },
		new VarType[operators[OperatorTypes_ADDTASKPARAM].params[2]]{ VarType_FLOAT, VarType_NUMBER }
	};
	operators[OperatorTypes_ADDTASKPARAM].paramTypes = new ParamType*[operators[OperatorTypes_ADDTASKPARAM].enumAmount]{
		new ParamType[operators[OperatorTypes_ADDTASKPARAM].params[0]]{ ParamType_NA, ParamType_NA },
		new ParamType[operators[OperatorTypes_ADDTASKPARAM].params[1]]{ ParamType_NA, ParamType_NA },
		new ParamType[operators[OperatorTypes_ADDTASKPARAM].params[2]]{ ParamType_NA, ParamType_NA }
	};
	operators[OperatorTypes_ADDTASKPARAM].optionalPar = new bool*[operators[OperatorTypes_ADDTASKPARAM].enumAmount]{
		new bool[operators[OperatorTypes_ADDTASKPARAM].params[0]]{ false, true },
		new bool[operators[OperatorTypes_ADDTASKPARAM].params[1]]{ false, true },
		new bool[operators[OperatorTypes_ADDTASKPARAM].params[2]]{ false, true }
	};
	operators[OperatorTypes_ADDTASKPARAM].readParam = new unsigned char[operators[OperatorTypes_ADDTASKPARAM].enumAmount]{ 0,0,0 };
	operators[OperatorTypes_ADDTASKPARAM].returns = new VarType[operators[OperatorTypes_ADDTASKPARAM].enumAmount]{ VarType_NA, VarType_NA,VarType_NA };
	operators[OperatorTypes_ADDTASKPARAM].returnType = new ParamType[operators[OperatorTypes_ADDTASKPARAM].enumAmount]{ ParamType_NA, ParamType_NA,ParamType_NA };
	operators[OperatorTypes_ADDTASKPARAM].writeParam = new unsigned char[operators[OperatorTypes_ADDTASKPARAM].enumAmount]{ 0,0,0 };
	operators[OperatorTypes_ADDTASKPARAM].priority = 0;
	operators[OperatorTypes_ADDTASKPARAM].shortHandsAmounts = 0;
	operators[OperatorTypes_ADDTASKPARAM].amountParOperators = new unsigned char[operators[OperatorTypes_ADDTASKPARAM].enumAmount]{ 0,0,0 };
	operators[OperatorTypes_ADDTASKPARAM].parRepeatsMin = new unsigned char*[operators[OperatorTypes_ADDTASKPARAM].enumAmount]{
		new unsigned char[operators[OperatorTypes_ADDTASKPARAM].params[0]]{ 0, 0 },
		new unsigned char[operators[OperatorTypes_ADDTASKPARAM].params[1]]{ 0, 0 },
		new unsigned char[operators[OperatorTypes_ADDTASKPARAM].params[2]]{ 0 , 0 }
	};
	operators[OperatorTypes_ADDTASKPARAM].parRepeatsMax = new unsigned char*[operators[OperatorTypes_ADDTASKPARAM].enumAmount]{
		new unsigned char[operators[OperatorTypes_ADDTASKPARAM].params[0]]{ 0, 0 },
		new unsigned char[operators[OperatorTypes_ADDTASKPARAM].params[1]]{ 0, 0 },
		new unsigned char[operators[OperatorTypes_ADDTASKPARAM].params[2]]{ 0, 0 }
	};
	operators[OperatorTypes_ADDTASKPARAM].parOperatorAppend = new bool*[operators[OperatorTypes_ADDTASKPARAM].enumAmount]{
		new bool[operators[OperatorTypes_ADDTASKPARAM].params[0]]{ false, false },
		new bool[operators[OperatorTypes_ADDTASKPARAM].params[1]]{ false, false },
		new bool[operators[OperatorTypes_ADDTASKPARAM].params[2]]{ false, false }
	};

	operators[OperatorTypes_ADD].keyword = "add";
	operators[OperatorTypes_ADD].enumAmount = 9;
	operators[OperatorTypes_ADD].enums = new std::string[operators[OperatorTypes_ADD].enumAmount]; 
	operators[OperatorTypes_ADD].enums[0] = "object"; 
	operators[OperatorTypes_ADD].enums[1] = "ui"; 
	operators[OperatorTypes_ADD].enums[2] = "mesh";
	operators[OperatorTypes_ADD].enums[3] = "text";
	operators[OperatorTypes_ADD].enums[4] = "line";
	operators[OperatorTypes_ADD].enums[5] = "routegoal";
	operators[OperatorTypes_ADD].enums[6] = "keytrigger";
	operators[OperatorTypes_ADD].enums[7] = "systemtask";
	operators[OperatorTypes_ADD].enums[8] = "clocktask";
	operators[OperatorTypes_ADD].byteCodes = new unsigned char[operators[OperatorTypes_ADD].enumAmount]{bytecode_ADDOBJECT,bytecode_2DADDCHLD,bytecode_ADDMESH, bytecode_ADDTXT, bytecode_ADDLNE,
		bytecode_ADDRTOBJ, bytecode_ADDTRGGR, bytecode_QSTSK, bytecode_QCTSK };
	operators[OperatorTypes_ADD].params = new unsigned char[operators[OperatorTypes_ADD].enumAmount]{1, 2, 1, 2, 2, 4, 3, 2, 2};
	operators[OperatorTypes_ADD].minParams = new unsigned char[operators[OperatorTypes_ADD].enumAmount]{1, 2, 1, 2, 2, 4, 3, 2, 2};
	operators[OperatorTypes_ADD].paramsyntax = new VarType*[operators[OperatorTypes_ADD].enumAmount]{
		new VarType[operators[OperatorTypes_ADD].params[0]]{VarType_NUMBER}, 
			new VarType[operators[OperatorTypes_ADD].params[1]]{VarType_NUMBER, VarType_NUMBER}, 
			new VarType[operators[OperatorTypes_ADD].params[2]]{VarType_NUMBER},
			new VarType[operators[OperatorTypes_ADD].params[3]]{VarType_NUMBER, VarType_STRING},
			new VarType[operators[OperatorTypes_ADD].params[4]]{VarType_NUMBER, VarType_STRING},
			new VarType[operators[OperatorTypes_ADD].params[5]]{ VarType_FLOAT, VarType_FLOAT, VarType_FLOAT, VarType_NUMBER },
			new VarType[operators[OperatorTypes_ADD].params[6]]{ VarType_NUMBER, VarType_NUMBER, VarType_NUMBER },
			new VarType[operators[OperatorTypes_ADD].params[7]]{ VarType_NUMBER, VarType_NUMBER},
			new VarType[operators[OperatorTypes_ADD].params[8]]{ VarType_NUMBER, VarType_NUMBER } };
	operators[OperatorTypes_ADD].paramTypes = new ParamType*[operators[OperatorTypes_ADD].enumAmount]{
		new ParamType[operators[OperatorTypes_ADD].params[0]]{ParamType_NA}, 
			new ParamType[operators[OperatorTypes_ADD].params[1]]{ParamType_NA, ParamType_NA}, 
			new ParamType[operators[OperatorTypes_ADD].params[2]]{ParamType_NA},
			new ParamType[operators[OperatorTypes_ADD].params[3]]{ParamType_NA, ParamType_NA},
			new ParamType[operators[OperatorTypes_ADD].params[4]]{ParamType_NA, ParamType_NA},
			new ParamType[operators[OperatorTypes_ADD].params[5]]{ ParamType_NA, ParamType_NA, ParamType_NA, ParamType_NA },
			new ParamType[operators[OperatorTypes_ADD].params[6]]{ ParamType_NA, ParamType_NA, ParamType_NA },
			new ParamType[operators[OperatorTypes_ADD].params[7]]{ ParamType_NA, ParamType_NA },
			new ParamType[operators[OperatorTypes_ADD].params[8]]{ ParamType_NA, ParamType_NA } };
	operators[OperatorTypes_ADD].optionalPar = new bool*[operators[OperatorTypes_ADD].enumAmount]{
		new bool[operators[OperatorTypes_ADD].params[0]]{ false,},
		new bool[operators[OperatorTypes_ADD].params[1]]{ false, false },
		new bool[operators[OperatorTypes_ADD].params[2]]{ false},
		new bool[operators[OperatorTypes_ADD].params[3]]{ false, false },
		new bool[operators[OperatorTypes_ADD].params[4]]{ false, false },
		new bool[operators[OperatorTypes_ADD].params[5]]{ false, false, false, false },
		new bool[operators[OperatorTypes_ADD].params[6]]{ false, false, false },
		new bool[operators[OperatorTypes_ADD].params[7]]{ false, false },
		new bool[operators[OperatorTypes_ADD].params[8]]{ false, false }
	};
	operators[OperatorTypes_ADD].readParam = new unsigned char[operators[OperatorTypes_ADD].enumAmount]{0,0,0,0,0,0,0,0,0};
	operators[OperatorTypes_ADD].priority = 0;
	operators[OperatorTypes_ADD].returns = new VarType[operators[OperatorTypes_ADD].enumAmount]{VarType_NA, VarType_NUMBER, VarType_NA, VarType_NA, VarType_NA,VarType_NUMBER,VarType_NA,VarType_NA,VarType_NA };
	operators[OperatorTypes_ADD].returnType = new ParamType[operators[OperatorTypes_ADD].enumAmount]{ParamType_NA, ParamType_VAR, ParamType_NA, ParamType_NA, ParamType_NA,ParamType_VAR,ParamType_NA,ParamType_NA,ParamType_NA };
	operators[OperatorTypes_ADD].writeParam = new unsigned char[operators[OperatorTypes_ADD].enumAmount]{0,0,0,0,0,0,0,0,0};
	operators[OperatorTypes_ADD].shortHandsAmounts = 0;
	operators[OperatorTypes_ADD].amountParOperators = new unsigned char[operators[OperatorTypes_ADD].enumAmount]{0, 0, 0,0,0,0,0,0,0};
	operators[OperatorTypes_ADD].parRepeatsMin = new unsigned char*[operators[OperatorTypes_ADD].enumAmount]{
		new unsigned char[operators[OperatorTypes_ADD].params[0]]{0},
			new unsigned char[operators[OperatorTypes_ADD].params[1]]{0, 0},
			new unsigned char[operators[OperatorTypes_ADD].params[2]]{0},
			new unsigned char[operators[OperatorTypes_ADD].params[3]]{0, 0},
			new unsigned char[operators[OperatorTypes_ADD].params[4]]{0, 0},
			new unsigned char[operators[OperatorTypes_ADD].params[5]]{ 0, 0, 0, 0 },
			new unsigned char[operators[OperatorTypes_ADD].params[6]]{ 0, 0, 0 },
			new unsigned char[operators[OperatorTypes_ADD].params[7]]{ 0, 0 },
			new unsigned char[operators[OperatorTypes_ADD].params[8]]{ 0, 0 }
	};
	operators[OperatorTypes_ADD].parRepeatsMax = new unsigned char*[operators[OperatorTypes_ADD].enumAmount]{
		new unsigned char[operators[OperatorTypes_ADD].params[0]]{0},
			new unsigned char[operators[OperatorTypes_ADD].params[1]]{0, 0},
			new unsigned char[operators[OperatorTypes_ADD].params[2]]{0},
			new unsigned char[operators[OperatorTypes_ADD].params[3]]{0, 0},
			new unsigned char[operators[OperatorTypes_ADD].params[4]]{0, 0},
			new unsigned char[operators[OperatorTypes_ADD].params[5]]{ 0, 0, 0, 0 },
			new unsigned char[operators[OperatorTypes_ADD].params[6]]{ 0, 0, 0 },
			new unsigned char[operators[OperatorTypes_ADD].params[7]]{ 0, 0 },
			new unsigned char[operators[OperatorTypes_ADD].params[8]]{ 0, 0 }
	};
	operators[OperatorTypes_ADD].parOperatorAppend = new bool*[operators[OperatorTypes_ADD].enumAmount]{
		new bool[operators[OperatorTypes_ADD].params[0]]{false},
			new bool[operators[OperatorTypes_ADD].params[1]]{false, false},
			new bool[operators[OperatorTypes_ADD].params[2]]{false},
			new bool[operators[OperatorTypes_ADD].params[3]]{false, false},
			new bool[operators[OperatorTypes_ADD].params[4]]{false, false},
			new bool[operators[OperatorTypes_ADD].params[5]]{ false, false, false, false },
			new bool[operators[OperatorTypes_ADD].params[6]]{ false, false, false },
			new bool[operators[OperatorTypes_ADD].params[7]]{ false, false },
			new bool[operators[OperatorTypes_ADD].params[8]]{ false, false }
	};

	operators[OperatorTypes_POST].keyword = "post";
	operators[OperatorTypes_POST].enumAmount = 3;
	operators[OperatorTypes_POST].enums = new std::string[operators[OperatorTypes_POST].enumAmount]; operators[OperatorTypes_POST].enums[0] = ""; operators[OperatorTypes_POST].enums[1] = ""; operators[OperatorTypes_POST].enums[2] = "";
	operators[OperatorTypes_POST].byteCodes = new unsigned char[operators[OperatorTypes_POST].enumAmount]{bytecode_POSTSTR, bytecode_POSTFLOAT, bytecode_POSTNMBR};
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
	operators[OperatorTypes_CAST].enumAmount = 2;
	operators[OperatorTypes_CAST].enums = new std::string[operators[OperatorTypes_CAST].enumAmount]; operators[OperatorTypes_CAST].enums[0] = ""; operators[OperatorTypes_CAST].enums[1] = "str";
	operators[OperatorTypes_CAST].byteCodes = new unsigned char[operators[OperatorTypes_CAST].enumAmount]{bytecode_CASTFLOAT,bytecode_CASTTSTR};
	operators[OperatorTypes_CAST].params = new unsigned char[operators[OperatorTypes_CAST].enumAmount]{1,1};
	operators[OperatorTypes_CAST].minParams = new unsigned char[operators[OperatorTypes_CAST].enumAmount]{1,1};
	operators[OperatorTypes_CAST].paramsyntax = new VarType*[operators[OperatorTypes_CAST].enumAmount]{
		new VarType[operators[OperatorTypes_CAST].params[0]]{VarType_NUMBER},
		new VarType[operators[OperatorTypes_CAST].params[1]]{ VarType_NUMBER } };
	operators[OperatorTypes_CAST].paramTypes = new ParamType*[operators[OperatorTypes_CAST].enumAmount]{
		new ParamType[operators[OperatorTypes_CAST].params[0]]{ParamType_NA},
		new ParamType[operators[OperatorTypes_CAST].params[1]]{ ParamType_NA } };
	operators[OperatorTypes_CAST].optionalPar = new bool*[operators[OperatorTypes_CAST].enumAmount]{
		new bool[operators[OperatorTypes_CAST].params[0]]{false},
		new bool[operators[OperatorTypes_CAST].params[1]]{ false }
	};
	operators[OperatorTypes_CAST].readParam = new unsigned char[operators[OperatorTypes_CAST].enumAmount]{0, 0};
	operators[OperatorTypes_CAST].priority = 0;
	operators[OperatorTypes_CAST].returns = new VarType[operators[OperatorTypes_CAST].enumAmount]{VarType_FLOAT, VarType_STRING};
	operators[OperatorTypes_CAST].returnType = new ParamType[operators[OperatorTypes_CAST].enumAmount]{ParamType_VAR,ParamType_VAR };
	operators[OperatorTypes_CAST].writeParam = new unsigned char[operators[OperatorTypes_CAST].enumAmount]{0,0};
	operators[OperatorTypes_CAST].shortHandsAmounts = 0;
	operators[OperatorTypes_CAST].amountParOperators = new unsigned char[operators[OperatorTypes_CAST].enumAmount]{0, 0};
	operators[OperatorTypes_CAST].parRepeatsMin = new unsigned char*[operators[OperatorTypes_CAST].enumAmount]{
		new unsigned char[operators[OperatorTypes_CAST].params[0]]{0},
		new unsigned char[operators[OperatorTypes_CAST].params[1]]{ 0 }
	};
	operators[OperatorTypes_CAST].parRepeatsMax = new unsigned char*[operators[OperatorTypes_CAST].enumAmount]{
		new unsigned char[operators[OperatorTypes_CAST].params[0]]{0},
		new unsigned char[operators[OperatorTypes_CAST].params[1]]{ 0 }
	};
	operators[OperatorTypes_CAST].parOperatorAppend = new bool*[operators[OperatorTypes_CAST].enumAmount]{
		new bool[operators[OperatorTypes_CAST].params[0]]{false},
		new bool[operators[OperatorTypes_CAST].params[1]]{ false }
	};

	setupArithmeticOps();

	operators[OperatorTypes_EQUAL].keyword = "eql";
	operators[OperatorTypes_EQUAL].enumAmount = 8;
	operators[OperatorTypes_EQUAL].enums = new std::string[operators[OperatorTypes_EQUAL].enumAmount]; operators[OperatorTypes_EQUAL].enums[0] = ""; operators[OperatorTypes_EQUAL].enums[1] = ""; operators[OperatorTypes_EQUAL].enums[2] = "", operators[OperatorTypes_EQUAL].enums[3] = ""; operators[OperatorTypes_EQUAL].enums[4] = ""; operators[OperatorTypes_EQUAL].enums[5] = ""; operators[OperatorTypes_EQUAL].enums[6] = ""; operators[OperatorTypes_EQUAL].enums[7] = "";
	operators[OperatorTypes_EQUAL].byteCodes = new unsigned char[operators[OperatorTypes_EQUAL].enumAmount]{bytecode_NUMEQUAL2CONST, bytecode_NUMEQUAL2CONST, bytecode_NUMEQUALVAR, bytecode_NUMEQUALFLOAT, bytecode_NUMEQUALFLOAT, bytecode_NUMEQUALFLOAT, bytecode_STREQUAL2CONST, bytecode_STREQUALVAR};
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
	operators[OperatorTypes_GREATER].byteCodes = new unsigned char[operators[OperatorTypes_GREATER].enumAmount]{bytecode_NUMGRT2CONST, bytecode_NUMGRT2CONST, bytecode_NUMGRTFLOAT, bytecode_NUMGRTVAR, bytecode_NUMGRTFLOAT, bytecode_NUMGRTFLOAT};
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

	operators[OperatorTypes_AND].keyword = "and";
	operators[OperatorTypes_AND].enumAmount = 1;
	operators[OperatorTypes_AND].enums = new std::string[operators[OperatorTypes_AND].enumAmount]; operators[OperatorTypes_AND].enums[0] = "num";
	operators[OperatorTypes_AND].byteCodes = new unsigned char[operators[OperatorTypes_AND].enumAmount]{ bytecode_AND };
	operators[OperatorTypes_AND].params = new unsigned char[operators[OperatorTypes_AND].enumAmount]{ 2 };
	operators[OperatorTypes_AND].minParams = new unsigned char[operators[OperatorTypes_AND].enumAmount]{ 2 };
	operators[OperatorTypes_AND].paramsyntax = new VarType*[operators[OperatorTypes_AND].enumAmount]{
		new VarType[operators[OperatorTypes_AND].params[0]]{ VarType_NUMBER, VarType_NUMBER }
	};
	operators[OperatorTypes_AND].paramTypes = new ParamType*[operators[OperatorTypes_AND].enumAmount]{
		new ParamType[operators[OperatorTypes_AND].params[0]]{ ParamType_NA, ParamType_NA }
	};
	operators[OperatorTypes_AND].optionalPar = new bool*[operators[OperatorTypes_AND].enumAmount]{
		new bool[operators[OperatorTypes_AND].params[0]]{ false, false }
	};
	operators[OperatorTypes_AND].readParam = new unsigned char[operators[OperatorTypes_AND].enumAmount]{ 0 };
	operators[OperatorTypes_AND].returns = new VarType[operators[OperatorTypes_AND].enumAmount]{ VarType_NUMBER };
	operators[OperatorTypes_AND].returnType = new ParamType[operators[OperatorTypes_AND].enumAmount]{ ParamType_VAR };
	operators[OperatorTypes_AND].writeParam = new unsigned char[operators[OperatorTypes_AND].enumAmount]{ 0 };
	operators[OperatorTypes_AND].priority = 5;
	operators[OperatorTypes_AND].shortHandsAmounts = 1;
	operators[OperatorTypes_AND].shortHands = new std::string[operators[OperatorTypes_AND].shortHandsAmounts]; operators[OperatorTypes_AND].shortHands[0] = "&";
	operators[OperatorTypes_AND].rightParams = new unsigned char[operators[OperatorTypes_AND].shortHandsAmounts]{ 1 };
	operators[OperatorTypes_AND].leftParams = new unsigned char[operators[OperatorTypes_AND].shortHandsAmounts]{ 1 };
	operators[OperatorTypes_AND].shortFlipParams = new bool[operators[OperatorTypes_AND].shortHandsAmounts]{ false };

	operators[OperatorTypes_AND].amountParOperators = new unsigned char[operators[OperatorTypes_AND].enumAmount]{ 0 };
	operators[OperatorTypes_AND].parRepeatsMin = new unsigned char*[operators[OperatorTypes_AND].enumAmount]{
		new unsigned char[operators[OperatorTypes_AND].params[0]]{ 0, 0 }
	};
	operators[OperatorTypes_AND].parRepeatsMax = new unsigned char*[operators[OperatorTypes_AND].enumAmount]{
		new unsigned char[operators[OperatorTypes_AND].params[0]]{ 0, 0 }
	};
	operators[OperatorTypes_AND].parOperatorAppend = new bool*[operators[OperatorTypes_AND].enumAmount]{
		new bool[operators[OperatorTypes_AND].params[0]]{ false, false }
	};

	operators[OperatorTypes_OR].keyword = "or";
	operators[OperatorTypes_OR].enumAmount = 1;
	operators[OperatorTypes_OR].enums = new std::string[operators[OperatorTypes_OR].enumAmount]; operators[OperatorTypes_OR].enums[0] = "num";
	operators[OperatorTypes_OR].byteCodes = new unsigned char[operators[OperatorTypes_OR].enumAmount]{ bytecode_OR };
	operators[OperatorTypes_OR].params = new unsigned char[operators[OperatorTypes_OR].enumAmount]{ 2 };
	operators[OperatorTypes_OR].minParams = new unsigned char[operators[OperatorTypes_OR].enumAmount]{ 2 };
	operators[OperatorTypes_OR].paramsyntax = new VarType*[operators[OperatorTypes_OR].enumAmount]{
		new VarType[operators[OperatorTypes_OR].params[0]]{ VarType_NUMBER, VarType_NUMBER }
	};
	operators[OperatorTypes_OR].paramTypes = new ParamType*[operators[OperatorTypes_OR].enumAmount]{
		new ParamType[operators[OperatorTypes_OR].params[0]]{ ParamType_NA, ParamType_NA }
	};
	operators[OperatorTypes_OR].optionalPar = new bool*[operators[OperatorTypes_OR].enumAmount]{
		new bool[operators[OperatorTypes_OR].params[0]]{ false, false }
	};
	operators[OperatorTypes_OR].readParam = new unsigned char[operators[OperatorTypes_OR].enumAmount]{ 0 };
	operators[OperatorTypes_OR].returns = new VarType[operators[OperatorTypes_OR].enumAmount]{ VarType_NUMBER };
	operators[OperatorTypes_OR].returnType = new ParamType[operators[OperatorTypes_OR].enumAmount]{ ParamType_VAR };
	operators[OperatorTypes_OR].writeParam = new unsigned char[operators[OperatorTypes_OR].enumAmount]{ 0 };
	operators[OperatorTypes_OR].priority = 5;
	operators[OperatorTypes_OR].shortHandsAmounts = 1;
	operators[OperatorTypes_OR].shortHands = new std::string[operators[OperatorTypes_OR].shortHandsAmounts]; operators[OperatorTypes_OR].shortHands[0] = "|";
	operators[OperatorTypes_OR].rightParams = new unsigned char[operators[OperatorTypes_OR].shortHandsAmounts]{ 1 };
	operators[OperatorTypes_OR].leftParams = new unsigned char[operators[OperatorTypes_OR].shortHandsAmounts]{ 1 };
	operators[OperatorTypes_OR].shortFlipParams = new bool[operators[OperatorTypes_OR].shortHandsAmounts]{ false };

	operators[OperatorTypes_OR].amountParOperators = new unsigned char[operators[OperatorTypes_OR].enumAmount]{ 0 };
	operators[OperatorTypes_OR].parRepeatsMin = new unsigned char*[operators[OperatorTypes_OR].enumAmount]{
		new unsigned char[operators[OperatorTypes_OR].params[0]]{ 0, 0 }
	};
	operators[OperatorTypes_OR].parRepeatsMax = new unsigned char*[operators[OperatorTypes_OR].enumAmount]{
		new unsigned char[operators[OperatorTypes_OR].params[0]]{ 0, 0 }
	};
	operators[OperatorTypes_OR].parOperatorAppend = new bool*[operators[OperatorTypes_OR].enumAmount]{
		new bool[operators[OperatorTypes_OR].params[0]]{ false, false }
	};

	operators[OperatorTypes_PARENT].keyword = "parent";
	operators[OperatorTypes_PARENT].enumAmount = 1;
	operators[OperatorTypes_PARENT].enums = new std::string[operators[OperatorTypes_PARENT].enumAmount]; operators[OperatorTypes_PARENT].enums[0] = "";
	operators[OperatorTypes_PARENT].byteCodes = new unsigned char[operators[OperatorTypes_PARENT].enumAmount]{bytecode_PRNT};
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
	operators[OperatorTypes_SNAP].byteCodes = new unsigned char[operators[OperatorTypes_SNAP].enumAmount]{bytecode_RESNAP};
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
	operators[OperatorTypes_POS].byteCodes = new unsigned char[operators[OperatorTypes_POS].enumAmount]{bytecode_POS,bytecode_POS };
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
	operators[OperatorTypes_MOVE].byteCodes = new unsigned char[operators[OperatorTypes_MOVE].enumAmount]{ bytecode_MVE,bytecode_MVE };
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
	operators[OperatorTypes_ROTATE].byteCodes = new unsigned char[operators[OperatorTypes_ROTATE].enumAmount]{ bytecode_RTE };
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
	operators[OperatorTypes_ORBIT].byteCodes = new unsigned char[operators[OperatorTypes_ORBIT].enumAmount]{ bytecode_ORB };
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
	operators[OperatorTypes_PROPEL].byteCodes = new unsigned char[operators[OperatorTypes_PROPEL].enumAmount]{ bytecode_PRPL };
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
	operators[OperatorTypes_SIZE].byteCodes = new unsigned char[operators[OperatorTypes_SIZE].enumAmount]{bytecode_SIZE};
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

	operators[OperatorTypes_SCALE].keyword = "scale";
	operators[OperatorTypes_SCALE].enumAmount = 1;
	operators[OperatorTypes_SCALE].enums = new std::string[operators[OperatorTypes_SCALE].enumAmount]; operators[OperatorTypes_SCALE].enums[0] = "";
	operators[OperatorTypes_SCALE].byteCodes = new unsigned char[operators[OperatorTypes_SCALE].enumAmount]{ bytecode_SCL };
	operators[OperatorTypes_SCALE].params = new unsigned char[operators[OperatorTypes_SCALE].enumAmount]{ 4 };
	operators[OperatorTypes_SCALE].minParams = new unsigned char[operators[OperatorTypes_SCALE].enumAmount]{ 3 };
	operators[OperatorTypes_SCALE].paramsyntax = new VarType*[operators[OperatorTypes_SCALE].enumAmount]{
		new VarType[operators[OperatorTypes_SCALE].params[0]]{ VarType_NUMBER, VarType_FLOAT, VarType_FLOAT, VarType_FLOAT } };
	operators[OperatorTypes_SCALE].paramTypes = new ParamType*[operators[OperatorTypes_SCALE].enumAmount]{
		new ParamType[operators[OperatorTypes_SCALE].params[0]]{ ParamType_NA, ParamType_NA, ParamType_NA, ParamType_NA } };
	operators[OperatorTypes_SCALE].optionalPar = new bool*[operators[OperatorTypes_SCALE].enumAmount]{
		new bool[operators[OperatorTypes_SCALE].params[0]]{ false, false, false, true }
	};
	operators[OperatorTypes_SCALE].readParam = new unsigned char[operators[OperatorTypes_SCALE].enumAmount]{ 0 };
	operators[OperatorTypes_SCALE].returns = new VarType[operators[OperatorTypes_SCALE].enumAmount]{ VarType_NA };
	operators[OperatorTypes_SCALE].returnType = new ParamType[operators[OperatorTypes_SCALE].enumAmount]{ ParamType_NA };
	operators[OperatorTypes_SCALE].writeParam = new unsigned char[operators[OperatorTypes_SCALE].enumAmount]{ 0 };
	operators[OperatorTypes_SCALE].priority = 0;
	operators[OperatorTypes_SCALE].shortHandsAmounts = 0;
	operators[OperatorTypes_SCALE].amountParOperators = new unsigned char[operators[OperatorTypes_SCALE].enumAmount]{ 0 };
	operators[OperatorTypes_SCALE].parRepeatsMin = new unsigned char*[operators[OperatorTypes_SCALE].enumAmount]{
		new unsigned char[operators[OperatorTypes_SCALE].params[0]]{ 0, 0, 0, 0 }
	};
	operators[OperatorTypes_SCALE].parRepeatsMax = new unsigned char*[operators[OperatorTypes_SCALE].enumAmount]{
		new unsigned char[operators[OperatorTypes_SCALE].params[0]]{ 0, 0, 0, 0 }
	};
	operators[OperatorTypes_SCALE].parOperatorAppend = new bool*[operators[OperatorTypes_SCALE].enumAmount]{
		new bool[operators[OperatorTypes_SCALE].params[0]]{ false, false, false, false }
	};

	operators[OperatorTypes_RUN].keyword = "run";
	operators[OperatorTypes_RUN].enumAmount = 1;
	operators[OperatorTypes_RUN].enums = new std::string[operators[OperatorTypes_RUN].enumAmount]; operators[OperatorTypes_RUN].enums[0] = "script";
	operators[OperatorTypes_RUN].byteCodes = new unsigned char[operators[OperatorTypes_RUN].enumAmount]{bytecode_RNSCRPT};
	operators[OperatorTypes_RUN].params = new unsigned char[operators[OperatorTypes_RUN].enumAmount]{2};
	operators[OperatorTypes_RUN].minParams = new unsigned char[operators[OperatorTypes_RUN].enumAmount]{ 1 };
	operators[OperatorTypes_RUN].paramsyntax = new VarType*[operators[OperatorTypes_RUN].enumAmount]{
		new VarType[operators[OperatorTypes_RUN].params[0]]{VarType_STRING, VarType_NA}};
	operators[OperatorTypes_RUN].paramTypes = new ParamType*[operators[OperatorTypes_RUN].enumAmount]{
		new ParamType[operators[OperatorTypes_RUN].params[0]]{ParamType_CONST, ParamType_NA}};
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
	operators[OperatorTypes_SETPARAM].byteCodes = new unsigned char[operators[OperatorTypes_SETPARAM].enumAmount]{bytecode_STSCRPTPRMNMBR,bytecode_STSCRPTPRMSTR,bytecode_STSCRPTPRMFLT};
	operators[OperatorTypes_SETPARAM].params = new unsigned char[operators[OperatorTypes_SETPARAM].enumAmount]{2,2,2};
	operators[OperatorTypes_SETPARAM].minParams = new unsigned char[operators[OperatorTypes_SETPARAM].enumAmount]{ 2,2,2 };
	operators[OperatorTypes_SETPARAM].paramsyntax = new VarType*[operators[OperatorTypes_SETPARAM].enumAmount]{
		new VarType[operators[OperatorTypes_SETPARAM].params[0]]{VarType_NUMBER, VarType_STRING},
			new VarType[operators[OperatorTypes_SETPARAM].params[1]]{VarType_STRING, VarType_STRING },
		new VarType[operators[OperatorTypes_SETPARAM].params[2]]{ VarType_FLOAT, VarType_STRING }
	};
	operators[OperatorTypes_SETPARAM].paramTypes = new ParamType*[operators[OperatorTypes_SETPARAM].enumAmount]{
		new ParamType[operators[OperatorTypes_SETPARAM].params[0]]{ParamType_NA, ParamType_CONST },
		new ParamType[operators[OperatorTypes_SETPARAM].params[1]]{ParamType_NA, ParamType_CONST },
		new ParamType[operators[OperatorTypes_SETPARAM].params[2]]{ ParamType_NA, ParamType_CONST }
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
	operators[OperatorTypes_EXPORT].enumAmount = 3;
	operators[OperatorTypes_EXPORT].enums = new std::string[operators[OperatorTypes_EXPORT].enumAmount]; operators[OperatorTypes_EXPORT].enums[0] = "num"; operators[OperatorTypes_EXPORT].enums[1] = "float"; operators[OperatorTypes_EXPORT].enums[2] = "str";
	operators[OperatorTypes_EXPORT].byteCodes = new unsigned char[operators[OperatorTypes_EXPORT].enumAmount]{bytecode_EXPRTNUM, bytecode_EXPRTNUM, bytecode_EXPRTSTR};
	operators[OperatorTypes_EXPORT].params = new unsigned char[operators[OperatorTypes_EXPORT].enumAmount]{2, 2, 2};
	operators[OperatorTypes_EXPORT].minParams = new unsigned char[operators[OperatorTypes_EXPORT].enumAmount]{2, 2, 2};
	operators[OperatorTypes_EXPORT].paramsyntax = new VarType*[operators[OperatorTypes_EXPORT].enumAmount]{
		new VarType[operators[OperatorTypes_EXPORT].params[0]]{VarType_STRING, VarType_NUMBER},
		new VarType[operators[OperatorTypes_EXPORT].params[1]]{VarType_STRING, VarType_FLOAT},
		new VarType[operators[OperatorTypes_EXPORT].params[2]]{ VarType_STRING, VarType_STRING }
	};
	operators[OperatorTypes_EXPORT].paramTypes = new ParamType*[operators[OperatorTypes_EXPORT].enumAmount]{
		new ParamType[operators[OperatorTypes_EXPORT].params[0]]{ ParamType_CONST, ParamType_NA },
		new ParamType[operators[OperatorTypes_EXPORT].params[1]]{ ParamType_CONST, ParamType_NA },
		new ParamType[operators[OperatorTypes_EXPORT].params[2]]{ ParamType_CONST, ParamType_NA }
	};
	operators[OperatorTypes_EXPORT].optionalPar = new bool*[operators[OperatorTypes_EXPORT].enumAmount]{
		new bool[operators[OperatorTypes_EXPORT].params[0]]{ false, false },
		new bool[operators[OperatorTypes_EXPORT].params[1]]{ false, false },
		new bool[operators[OperatorTypes_EXPORT].params[2]]{ false, false }
	};
	operators[OperatorTypes_EXPORT].readParam = new unsigned char[operators[OperatorTypes_EXPORT].enumAmount]{0,0,0};
	operators[OperatorTypes_EXPORT].returns = new VarType[operators[OperatorTypes_EXPORT].enumAmount]{VarType_NA, VarType_NA,VarType_NA };
	operators[OperatorTypes_EXPORT].returnType = new ParamType[operators[OperatorTypes_EXPORT].enumAmount]{ParamType_NA, ParamType_NA,ParamType_NA };
	operators[OperatorTypes_EXPORT].writeParam = new unsigned char[operators[OperatorTypes_EXPORT].enumAmount]{0, 0};
	operators[OperatorTypes_EXPORT].priority = 0;
	operators[OperatorTypes_EXPORT].shortHandsAmounts = 0;
	operators[OperatorTypes_EXPORT].amountParOperators = new unsigned char[operators[OperatorTypes_EXPORT].enumAmount]{0, 0,0};
	operators[OperatorTypes_EXPORT].parRepeatsMin = new unsigned char*[operators[OperatorTypes_EXPORT].enumAmount]{
		new unsigned char[operators[OperatorTypes_EXPORT].params[0]]{0, 0},
		new unsigned char[operators[OperatorTypes_EXPORT].params[1]]{0, 0},
		new unsigned char[operators[OperatorTypes_EXPORT].params[2]]{ 0, 0 }
	};
	operators[OperatorTypes_EXPORT].parRepeatsMax = new unsigned char*[operators[OperatorTypes_EXPORT].enumAmount]{
		new unsigned char[operators[OperatorTypes_EXPORT].params[0]]{0, 0},
		new unsigned char[operators[OperatorTypes_EXPORT].params[1]]{0, 0},
		new unsigned char[operators[OperatorTypes_EXPORT].params[2]]{ 0, 0 }
	};
	operators[OperatorTypes_EXPORT].parOperatorAppend = new bool*[operators[OperatorTypes_EXPORT].enumAmount]{
		new bool[operators[OperatorTypes_EXPORT].params[0]]{false, false},
		new bool[operators[OperatorTypes_EXPORT].params[1]]{false, false},
		new bool[operators[OperatorTypes_EXPORT].params[2]]{ false, false }
	};

	operators[OperatorTypes_IMPORT].keyword = "import";
	operators[OperatorTypes_IMPORT].enumAmount = 3;
	operators[OperatorTypes_IMPORT].enums = new std::string[operators[OperatorTypes_IMPORT].enumAmount]; operators[OperatorTypes_IMPORT].enums[0] = "num"; operators[OperatorTypes_IMPORT].enums[1] = "float"; operators[OperatorTypes_IMPORT].enums[2] = "string";
	operators[OperatorTypes_IMPORT].byteCodes = new unsigned char[operators[OperatorTypes_IMPORT].enumAmount]{bytecode_IMPRTNUM, bytecode_IMPRTNUM, bytecode_IMPRTSTR};
	operators[OperatorTypes_IMPORT].params = new unsigned char[operators[OperatorTypes_IMPORT].enumAmount]{1, 1, 1};
	operators[OperatorTypes_IMPORT].minParams = new unsigned char[operators[OperatorTypes_IMPORT].enumAmount]{1, 1, 1};
	operators[OperatorTypes_IMPORT].paramsyntax = new VarType*[operators[OperatorTypes_IMPORT].enumAmount]{
		new VarType[operators[OperatorTypes_IMPORT].params[0]]{VarType_STRING},
		new VarType[operators[OperatorTypes_IMPORT].params[1]]{VarType_STRING},
		new VarType[operators[OperatorTypes_IMPORT].params[2]]{VarType_STRING}
	};
	operators[OperatorTypes_IMPORT].paramTypes = new ParamType*[operators[OperatorTypes_IMPORT].enumAmount]{
		new ParamType[operators[OperatorTypes_IMPORT].params[0]]{ParamType_CONST},
		new ParamType[operators[OperatorTypes_IMPORT].params[1]]{ ParamType_CONST },
		new ParamType[operators[OperatorTypes_IMPORT].params[2]]{ ParamType_CONST }
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
	operators[OperatorTypes_FOCUS].byteCodes = new unsigned char[operators[OperatorTypes_FOCUS].enumAmount]{bytecode_FCSUI };
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
	operators[OperatorTypes_UNLOAD].byteCodes = new unsigned char[operators[OperatorTypes_UNLOAD].enumAmount]{bytecode_UNLOAD };
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
	operators[OperatorTypes_REMOVE].enumAmount = 2;
	operators[OperatorTypes_REMOVE].enums = new std::string[operators[OperatorTypes_REMOVE].enumAmount]; operators[OperatorTypes_REMOVE].enums[0] = ""; operators[OperatorTypes_REMOVE].enums[1] = "task";
	operators[OperatorTypes_REMOVE].byteCodes = new unsigned char[operators[OperatorTypes_REMOVE].enumAmount]{bytecode_RMVE, bytecode_RMTSK };
	operators[OperatorTypes_REMOVE].params = new unsigned char[operators[OperatorTypes_REMOVE].enumAmount]{1, 1};
	operators[OperatorTypes_REMOVE].minParams = new unsigned char[operators[OperatorTypes_REMOVE].enumAmount]{1, 1};
	operators[OperatorTypes_REMOVE].paramsyntax = new VarType*[operators[OperatorTypes_REMOVE].enumAmount]{
		new VarType[operators[OperatorTypes_REMOVE].params[0]]{ VarType_NUMBER },
		new VarType[operators[OperatorTypes_REMOVE].params[1]]{ VarType_NUMBER }
	};
	operators[OperatorTypes_REMOVE].paramTypes = new ParamType*[operators[OperatorTypes_REMOVE].enumAmount]{
		new ParamType[operators[OperatorTypes_REMOVE].params[0]]{ParamType_NA},
		new ParamType[operators[OperatorTypes_REMOVE].params[1]]{ ParamType_NA }
	};
	operators[OperatorTypes_REMOVE].optionalPar = new bool*[operators[OperatorTypes_REMOVE].enumAmount]{
		new bool[operators[OperatorTypes_REMOVE].params[0]]{ false },
		new bool[operators[OperatorTypes_REMOVE].params[1]]{ false }
	};
	operators[OperatorTypes_REMOVE].readParam = new unsigned char[operators[OperatorTypes_REMOVE].enumAmount]{0,0};
	operators[OperatorTypes_REMOVE].returns = new VarType[operators[OperatorTypes_REMOVE].enumAmount]{VarType_NA,VarType_NA };
	operators[OperatorTypes_REMOVE].returnType = new ParamType[operators[OperatorTypes_REMOVE].enumAmount]{ParamType_NA, ParamType_NA };
	operators[OperatorTypes_REMOVE].writeParam = new unsigned char[operators[OperatorTypes_REMOVE].enumAmount]{0, 0};
	operators[OperatorTypes_REMOVE].priority = 0;
	operators[OperatorTypes_REMOVE].shortHandsAmounts = 0;
	operators[OperatorTypes_REMOVE].amountParOperators = new unsigned char[operators[OperatorTypes_REMOVE].enumAmount]{0, 0};
	operators[OperatorTypes_REMOVE].parRepeatsMin = new unsigned char*[operators[OperatorTypes_REMOVE].enumAmount]{
		new unsigned char[operators[OperatorTypes_REMOVE].params[0]]{0},
		new unsigned char[operators[OperatorTypes_REMOVE].params[1]]{ 0 }
	};
	operators[OperatorTypes_REMOVE].parRepeatsMax = new unsigned char*[operators[OperatorTypes_REMOVE].enumAmount]{
		new unsigned char[operators[OperatorTypes_REMOVE].params[0]]{0},
		new unsigned char[operators[OperatorTypes_REMOVE].params[1]]{ 0 }
	};
	operators[OperatorTypes_REMOVE].parOperatorAppend = new bool*[operators[OperatorTypes_REMOVE].enumAmount]{
		new bool[operators[OperatorTypes_REMOVE].params[0]]{false},
		new bool[operators[OperatorTypes_REMOVE].params[1]]{ false }
	};


	operators[OperatorTypes_HIDE].keyword = "hide";
	operators[OperatorTypes_HIDE].enumAmount = 2;
	operators[OperatorTypes_HIDE].enums = new std::string[operators[OperatorTypes_HIDE].enumAmount]; operators[OperatorTypes_HIDE].enums[0] = ""; operators[OperatorTypes_HIDE].enums[1] = "cursor";
	operators[OperatorTypes_HIDE].byteCodes = new unsigned char[operators[OperatorTypes_HIDE].enumAmount]{bytecode_HDE,bytecode_HDECRS };
	operators[OperatorTypes_HIDE].params = new unsigned char[operators[OperatorTypes_HIDE].enumAmount]{1,0};
	operators[OperatorTypes_HIDE].minParams = new unsigned char[operators[OperatorTypes_HIDE].enumAmount]{1,0};
	operators[OperatorTypes_HIDE].paramsyntax = new VarType*[operators[OperatorTypes_HIDE].enumAmount]{
		new VarType[operators[OperatorTypes_HIDE].params[0]]{VarType_NUMBER},
		nullptr
	};
	operators[OperatorTypes_HIDE].paramTypes = new ParamType*[operators[OperatorTypes_HIDE].enumAmount]{
		new ParamType[operators[OperatorTypes_HIDE].params[0]]{ParamType_NA},
		nullptr
	};
	operators[OperatorTypes_HIDE].optionalPar = new bool*[operators[OperatorTypes_HIDE].enumAmount]{
		new bool[operators[OperatorTypes_HIDE].params[0]]{ false },
		nullptr
	};
	operators[OperatorTypes_HIDE].readParam = new unsigned char[operators[OperatorTypes_HIDE].enumAmount]{0,0};
	operators[OperatorTypes_HIDE].returns = new VarType[operators[OperatorTypes_HIDE].enumAmount]{VarType_NA,VarType_NA };
	operators[OperatorTypes_HIDE].returnType = new ParamType[operators[OperatorTypes_HIDE].enumAmount]{ParamType_NA,ParamType_NA };
	operators[OperatorTypes_HIDE].writeParam = new unsigned char[operators[OperatorTypes_HIDE].enumAmount]{0,0};
	operators[OperatorTypes_HIDE].priority = 0;
	operators[OperatorTypes_HIDE].shortHandsAmounts = 0;
	operators[OperatorTypes_HIDE].amountParOperators = new unsigned char[operators[OperatorTypes_HIDE].enumAmount]{0,0};
	operators[OperatorTypes_HIDE].parRepeatsMin = new unsigned char*[operators[OperatorTypes_HIDE].enumAmount]{
		new unsigned char[operators[OperatorTypes_HIDE].params[0]]{0},
		nullptr
	};
	operators[OperatorTypes_HIDE].parRepeatsMax = new unsigned char*[operators[OperatorTypes_HIDE].enumAmount]{
		new unsigned char[operators[OperatorTypes_HIDE].params[0]]{0},
		nullptr
	};
	operators[OperatorTypes_HIDE].parOperatorAppend = new bool*[operators[OperatorTypes_HIDE].enumAmount]{
		new bool[operators[OperatorTypes_HIDE].params[0]]{false},
		nullptr
	};

	operators[OperatorTypes_SHOW].keyword = "show";
	operators[OperatorTypes_SHOW].enumAmount = 2;
	operators[OperatorTypes_SHOW].enums = new std::string[operators[OperatorTypes_SHOW].enumAmount]; operators[OperatorTypes_SHOW].enums[0] = ""; operators[OperatorTypes_SHOW].enums[1] = "cursor";
	operators[OperatorTypes_SHOW].byteCodes = new unsigned char[operators[OperatorTypes_SHOW].enumAmount]{bytecode_SHW,bytecode_SHWCRS };
	operators[OperatorTypes_SHOW].params = new unsigned char[operators[OperatorTypes_SHOW].enumAmount]{1,0};
	operators[OperatorTypes_SHOW].minParams = new unsigned char[operators[OperatorTypes_SHOW].enumAmount]{1,0};
	operators[OperatorTypes_SHOW].paramsyntax = new VarType*[operators[OperatorTypes_SHOW].enumAmount]{
		new VarType[operators[OperatorTypes_SHOW].params[0]]{VarType_NUMBER},
		nullptr
	};
	operators[OperatorTypes_SHOW].paramTypes = new ParamType*[operators[OperatorTypes_SHOW].enumAmount]{
		new ParamType[operators[OperatorTypes_SHOW].params[0]]{ParamType_NA},
		nullptr
	};
	operators[OperatorTypes_SHOW].optionalPar = new bool*[operators[OperatorTypes_SHOW].enumAmount]{
		new bool[operators[OperatorTypes_SHOW].params[0]]{ false },
		nullptr
	};
	operators[OperatorTypes_SHOW].readParam = new unsigned char[operators[OperatorTypes_SHOW].enumAmount]{0,0};
	operators[OperatorTypes_SHOW].returns = new VarType[operators[OperatorTypes_SHOW].enumAmount]{VarType_NA, VarType_NA };
	operators[OperatorTypes_SHOW].returnType = new ParamType[operators[OperatorTypes_SHOW].enumAmount]{ParamType_NA, ParamType_NA };
	operators[OperatorTypes_SHOW].writeParam = new unsigned char[operators[OperatorTypes_SHOW].enumAmount]{0,0};
	operators[OperatorTypes_SHOW].priority = 0;
	operators[OperatorTypes_SHOW].shortHandsAmounts = 0;
	operators[OperatorTypes_SHOW].amountParOperators = new unsigned char[operators[OperatorTypes_SHOW].enumAmount]{0,0};
	operators[OperatorTypes_SHOW].parRepeatsMin = new unsigned char*[operators[OperatorTypes_SHOW].enumAmount]{
		new unsigned char[operators[OperatorTypes_SHOW].params[0]]{0},
		nullptr
	};
	operators[OperatorTypes_SHOW].parRepeatsMax = new unsigned char*[operators[OperatorTypes_SHOW].enumAmount]{
		new unsigned char[operators[OperatorTypes_SHOW].params[0]]{0},
		nullptr
	};
	operators[OperatorTypes_SHOW].parOperatorAppend = new bool*[operators[OperatorTypes_SHOW].enumAmount]{
		new bool[operators[OperatorTypes_SHOW].params[0]]{false},
		nullptr
	};

	operators[OperatorTypes_TRACK].keyword = "track";
	operators[OperatorTypes_TRACK].enumAmount = 1;
	operators[OperatorTypes_TRACK].enums = new std::string[operators[OperatorTypes_TRACK].enumAmount]; operators[OperatorTypes_TRACK].enums[0] = "";
	operators[OperatorTypes_TRACK].byteCodes = new unsigned char[operators[OperatorTypes_TRACK].enumAmount]{ bytecode_TRCK };
	operators[OperatorTypes_TRACK].params = new unsigned char[operators[OperatorTypes_TRACK].enumAmount]{ 1 };
	operators[OperatorTypes_TRACK].minParams = new unsigned char[operators[OperatorTypes_TRACK].enumAmount]{ 1 };
	operators[OperatorTypes_TRACK].paramsyntax = new VarType*[operators[OperatorTypes_TRACK].enumAmount]{
		new VarType[operators[OperatorTypes_TRACK].params[0]]{ VarType_NUMBER }
	};
	operators[OperatorTypes_TRACK].paramTypes = new ParamType*[operators[OperatorTypes_TRACK].enumAmount]{
		new ParamType[operators[OperatorTypes_TRACK].params[0]]{ ParamType_NA }
	};
	operators[OperatorTypes_TRACK].optionalPar = new bool*[operators[OperatorTypes_TRACK].enumAmount]{
		new bool[operators[OperatorTypes_TRACK].params[0]]{ false }
	};
	operators[OperatorTypes_TRACK].readParam = new unsigned char[operators[OperatorTypes_TRACK].enumAmount]{ 0};
	operators[OperatorTypes_TRACK].returns = new VarType[operators[OperatorTypes_TRACK].enumAmount]{ VarType_NA };
	operators[OperatorTypes_TRACK].returnType = new ParamType[operators[OperatorTypes_TRACK].enumAmount]{ ParamType_NA };
	operators[OperatorTypes_TRACK].writeParam = new unsigned char[operators[OperatorTypes_TRACK].enumAmount]{ 0 };
	operators[OperatorTypes_TRACK].priority = 0;
	operators[OperatorTypes_TRACK].shortHandsAmounts = 0;
	operators[OperatorTypes_TRACK].amountParOperators = new unsigned char[operators[OperatorTypes_TRACK].enumAmount]{ 0 };
	operators[OperatorTypes_TRACK].parRepeatsMin = new unsigned char*[operators[OperatorTypes_TRACK].enumAmount]{
		new unsigned char[operators[OperatorTypes_TRACK].params[0]]{ 0 }
	};
	operators[OperatorTypes_TRACK].parRepeatsMax = new unsigned char*[operators[OperatorTypes_TRACK].enumAmount]{
		new unsigned char[operators[OperatorTypes_TRACK].params[0]]{ 0 }
	};
	operators[OperatorTypes_TRACK].parOperatorAppend = new bool*[operators[OperatorTypes_TRACK].enumAmount]{
		new bool[operators[OperatorTypes_TRACK].params[0]]{ false }
	};

	operators[OperatorTypes_CLEAR].keyword = "clear";
	operators[OperatorTypes_CLEAR].enumAmount = 2;
	operators[OperatorTypes_CLEAR].enums = new std::string[operators[OperatorTypes_CLEAR].enumAmount]; operators[OperatorTypes_CLEAR].enums[0] = "tracking"; operators[OperatorTypes_CLEAR].enums[1] = "gameboard";
	operators[OperatorTypes_CLEAR].byteCodes = new unsigned char[operators[OperatorTypes_CLEAR].enumAmount]{ bytecode_CRLTRCK, bytecode_CLRBRD };
	operators[OperatorTypes_CLEAR].params = new unsigned char[operators[OperatorTypes_CLEAR].enumAmount]{ 0, 1 };
	operators[OperatorTypes_CLEAR].minParams = new unsigned char[operators[OperatorTypes_CLEAR].enumAmount]{ 0, 1 };
	operators[OperatorTypes_CLEAR].paramsyntax = new VarType*[operators[OperatorTypes_CLEAR].enumAmount]{
		nullptr,
		new VarType[operators[OperatorTypes_CLEAR].params[1]]{ VarType_NUMBER }
	};
	operators[OperatorTypes_CLEAR].paramTypes = new ParamType*[operators[OperatorTypes_CLEAR].enumAmount]{
		nullptr,
		new ParamType[operators[OperatorTypes_CLEAR].params[1]]{ ParamType_NA }
	};
	operators[OperatorTypes_CLEAR].optionalPar = new bool*[operators[OperatorTypes_CLEAR].enumAmount]{
		nullptr,
		new bool[operators[OperatorTypes_CLEAR].params[1]]{ false }
	};
	operators[OperatorTypes_CLEAR].readParam = new unsigned char[operators[OperatorTypes_CLEAR].enumAmount]{ 0,0,};
	operators[OperatorTypes_CLEAR].returns = new VarType[operators[OperatorTypes_CLEAR].enumAmount]{ VarType_NA, VarType_NA };
	operators[OperatorTypes_CLEAR].returnType = new ParamType[operators[OperatorTypes_CLEAR].enumAmount]{ ParamType_NA, ParamType_NA };
	operators[OperatorTypes_CLEAR].writeParam = new unsigned char[operators[OperatorTypes_CLEAR].enumAmount]{ 0, 0 };
	operators[OperatorTypes_CLEAR].priority = 0;
	operators[OperatorTypes_CLEAR].shortHandsAmounts = 0;
	operators[OperatorTypes_CLEAR].amountParOperators = new unsigned char[operators[OperatorTypes_CLEAR].enumAmount]{ 0, 0};
	operators[OperatorTypes_CLEAR].parRepeatsMin = new unsigned char*[operators[OperatorTypes_CLEAR].enumAmount]{
		nullptr,
		new unsigned char[operators[OperatorTypes_CLEAR].params[1]]{ 0 }
	};
	operators[OperatorTypes_CLEAR].parRepeatsMax = new unsigned char*[operators[OperatorTypes_CLEAR].enumAmount]{
		nullptr,
		new unsigned char[operators[OperatorTypes_CLEAR].params[1]]{ 0 }
	};
	operators[OperatorTypes_CLEAR].parOperatorAppend = new bool*[operators[OperatorTypes_CLEAR].enumAmount]{
		nullptr,
		new bool[operators[OperatorTypes_CLEAR].params[1]]{ 0 }
	};

	operators[OperatorTypes_HOOK].keyword = "hook";
	operators[OperatorTypes_HOOK].enumAmount = 1;
	operators[OperatorTypes_HOOK].enums = new std::string[operators[OperatorTypes_HOOK].enumAmount]; operators[OperatorTypes_HOOK].enums[0] = "tracking";
	operators[OperatorTypes_HOOK].byteCodes = new unsigned char[operators[OperatorTypes_HOOK].enumAmount]{ bytecode_HCKTRCK};
	operators[OperatorTypes_HOOK].params = new unsigned char[operators[OperatorTypes_HOOK].enumAmount]{ 0 };
	operators[OperatorTypes_HOOK].minParams = new unsigned char[operators[OperatorTypes_HOOK].enumAmount]{ 0 };
	operators[OperatorTypes_HOOK].paramsyntax = new VarType*[operators[OperatorTypes_HOOK].enumAmount]{
		nullptr
	};
	operators[OperatorTypes_HOOK].paramTypes = new ParamType*[operators[OperatorTypes_HOOK].enumAmount]{
		nullptr
	};
	operators[OperatorTypes_HOOK].optionalPar = new bool*[operators[OperatorTypes_HOOK].enumAmount]{
		nullptr
	};
	operators[OperatorTypes_HOOK].readParam = new unsigned char[operators[OperatorTypes_HOOK].enumAmount]{ 0, };
	operators[OperatorTypes_HOOK].returns = new VarType[operators[OperatorTypes_HOOK].enumAmount]{ VarType_NA };
	operators[OperatorTypes_HOOK].returnType = new ParamType[operators[OperatorTypes_HOOK].enumAmount]{ ParamType_NA };
	operators[OperatorTypes_HOOK].writeParam = new unsigned char[operators[OperatorTypes_HOOK].enumAmount]{ 0 };
	operators[OperatorTypes_HOOK].priority = 0;
	operators[OperatorTypes_HOOK].shortHandsAmounts = 0;
	operators[OperatorTypes_HOOK].amountParOperators = new unsigned char[operators[OperatorTypes_HOOK].enumAmount]{ 0 };
	operators[OperatorTypes_HOOK].parRepeatsMin = new unsigned char*[operators[OperatorTypes_HOOK].enumAmount]{
		nullptr
	};
	operators[OperatorTypes_HOOK].parRepeatsMax = new unsigned char*[operators[OperatorTypes_HOOK].enumAmount]{
		nullptr
	};
	operators[OperatorTypes_HOOK].parOperatorAppend = new bool*[operators[OperatorTypes_HOOK].enumAmount]{
		nullptr
	};

	operators[OperatorTypes_GLUE].keyword = "glue";
	operators[OperatorTypes_GLUE].enumAmount = 1;
	operators[OperatorTypes_GLUE].enums = new std::string[operators[OperatorTypes_GLUE].enumAmount]; operators[OperatorTypes_GLUE].enums[0] = "";
	operators[OperatorTypes_GLUE].byteCodes = new unsigned char[operators[OperatorTypes_GLUE].enumAmount]{ bytecode_GLUOBJCT };
	operators[OperatorTypes_GLUE].params = new unsigned char[operators[OperatorTypes_GLUE].enumAmount]{ 5 };
	operators[OperatorTypes_GLUE].minParams = new unsigned char[operators[OperatorTypes_GLUE].enumAmount]{ 5 };
	operators[OperatorTypes_GLUE].paramsyntax = new VarType*[operators[OperatorTypes_GLUE].enumAmount]{
		new VarType[operators[OperatorTypes_GLUE].params[0]]{ VarType_NUMBER, VarType_NUMBER, VarType_FLOAT, VarType_FLOAT, VarType_FLOAT }
	};
	operators[OperatorTypes_GLUE].paramTypes = new ParamType*[operators[OperatorTypes_GLUE].enumAmount]{
		new ParamType[operators[OperatorTypes_GLUE].params[0]]{ ParamType_NA, ParamType_NA, ParamType_NA, ParamType_NA, ParamType_NA }
	};
	operators[OperatorTypes_GLUE].optionalPar = new bool*[operators[OperatorTypes_GLUE].enumAmount]{
		new bool[operators[OperatorTypes_GLUE].params[0]]{ false, false, false, false, false }
	};
	operators[OperatorTypes_GLUE].readParam = new unsigned char[operators[OperatorTypes_GLUE].enumAmount]{ 0, };
	operators[OperatorTypes_GLUE].returns = new VarType[operators[OperatorTypes_GLUE].enumAmount]{ VarType_NA };
	operators[OperatorTypes_GLUE].returnType = new ParamType[operators[OperatorTypes_GLUE].enumAmount]{ ParamType_NA };
	operators[OperatorTypes_GLUE].writeParam = new unsigned char[operators[OperatorTypes_GLUE].enumAmount]{ 0 };
	operators[OperatorTypes_GLUE].priority = 0;
	operators[OperatorTypes_GLUE].shortHandsAmounts = 0;
	operators[OperatorTypes_GLUE].amountParOperators = new unsigned char[operators[OperatorTypes_GLUE].enumAmount]{ 0 };
	operators[OperatorTypes_GLUE].parRepeatsMin = new unsigned char*[operators[OperatorTypes_GLUE].enumAmount]{
		new unsigned char[operators[OperatorTypes_GLUE].params[0]]{ 0, 0, 0, 0, 0 }
	};
	operators[OperatorTypes_GLUE].parRepeatsMax = new unsigned char*[operators[OperatorTypes_GLUE].enumAmount]{
		new unsigned char[operators[OperatorTypes_GLUE].params[0]]{ 0, 0, 0, 0, 0 }
	};
	operators[OperatorTypes_GLUE].parOperatorAppend = new bool*[operators[OperatorTypes_GLUE].enumAmount]{
		new bool[operators[OperatorTypes_GLUE].params[0]]{ false, false, false, false, false }
	};

	operators[OperatorTypes_INCREMENT].keyword = "increment";
	operators[OperatorTypes_INCREMENT].enumAmount = 1;
	operators[OperatorTypes_INCREMENT].enums = new std::string[operators[OperatorTypes_INCREMENT].enumAmount]; operators[OperatorTypes_INCREMENT].enums[0] = "layer";
	operators[OperatorTypes_INCREMENT].byteCodes = new unsigned char[operators[OperatorTypes_INCREMENT].enumAmount]{bytecode_INCRMNTLYR};
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
	operators[OperatorTypes_DECREMENT].byteCodes = new unsigned char[operators[OperatorTypes_DECREMENT].enumAmount]{bytecode_DCRMNLYR};
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
	operators[OperatorTypes_IGNORE].byteCodes = new unsigned char[operators[OperatorTypes_IGNORE].enumAmount]{bytecode_IGNRMS, bytecode_IGNRKY, bytecode_IGNRIN};
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
	operators[OperatorTypes_ACCEPT].byteCodes = new unsigned char[operators[OperatorTypes_ACCEPT].enumAmount]{bytecode_ACPTMS, bytecode_ACPTKY, bytecode_ACPTIN};
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
	operators[OperatorTypes_LINK].byteCodes = new unsigned char[operators[OperatorTypes_LINK].enumAmount]{ bytecode_LNKTRGT, bytecode_LNKDBG};
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

	operators[OperatorTypes_SPAWN].keyword = "spawn";
	operators[OperatorTypes_SPAWN].enumAmount = 1;
	operators[OperatorTypes_SPAWN].enums = new std::string[operators[OperatorTypes_SPAWN].enumAmount]; operators[OperatorTypes_SPAWN].enums[0] = "object";
	operators[OperatorTypes_SPAWN].byteCodes = new unsigned char[operators[OperatorTypes_SPAWN].enumAmount]{ bytecode_SPWN };
	operators[OperatorTypes_SPAWN].params = new unsigned char[operators[OperatorTypes_SPAWN].enumAmount]{ 2 };
	operators[OperatorTypes_SPAWN].minParams = new unsigned char[operators[OperatorTypes_SPAWN].enumAmount]{ 2 };
	operators[OperatorTypes_SPAWN].paramsyntax = new VarType*[operators[OperatorTypes_SPAWN].enumAmount]{
		new VarType[operators[OperatorTypes_SPAWN].params[0]]{ VarType_NUMBER, VarType_NUMBER }
	};
	operators[OperatorTypes_SPAWN].paramTypes = new ParamType*[operators[OperatorTypes_SPAWN].enumAmount]{
		new ParamType[operators[OperatorTypes_SPAWN].params[0]]{ ParamType_NA, ParamType_NA }
	};
	operators[OperatorTypes_SPAWN].optionalPar = new bool*[operators[OperatorTypes_SPAWN].enumAmount]{
		new bool[operators[OperatorTypes_SPAWN].params[0]]{ false, false }
	};
	operators[OperatorTypes_SPAWN].readParam = new unsigned char[operators[OperatorTypes_SPAWN].enumAmount]{ 0 };
	operators[OperatorTypes_SPAWN].returns = new VarType[operators[OperatorTypes_SPAWN].enumAmount]{ VarType_NA };
	operators[OperatorTypes_SPAWN].returnType = new ParamType[operators[OperatorTypes_SPAWN].enumAmount]{ ParamType_NA };
	operators[OperatorTypes_SPAWN].writeParam = new unsigned char[operators[OperatorTypes_SPAWN].enumAmount]{ 0 };
	operators[OperatorTypes_SPAWN].priority = 0;
	operators[OperatorTypes_SPAWN].shortHandsAmounts = 0;
	operators[OperatorTypes_SPAWN].amountParOperators = new unsigned char[operators[OperatorTypes_SPAWN].enumAmount]{ 0 };
	operators[OperatorTypes_SPAWN].parRepeatsMin = new unsigned char*[operators[OperatorTypes_SPAWN].enumAmount]{
		new unsigned char[operators[OperatorTypes_SPAWN].params[0]]{ 0,0 }
	};
	operators[OperatorTypes_SPAWN].parRepeatsMax = new unsigned char*[operators[OperatorTypes_SPAWN].enumAmount]{
		new unsigned char[operators[OperatorTypes_SPAWN].params[0]]{ 0,0 }
	};
	operators[OperatorTypes_SPAWN].parOperatorAppend = new bool*[operators[OperatorTypes_SPAWN].enumAmount]{
		new bool[operators[OperatorTypes_SPAWN].params[0]]{ false, false }
	};

	operators[OperatorTypes_TRAVEL].keyword = "travel";
	operators[OperatorTypes_TRAVEL].enumAmount = 1;
	operators[OperatorTypes_TRAVEL].enums = new std::string[operators[OperatorTypes_TRAVEL].enumAmount]; operators[OperatorTypes_TRAVEL].enums[0] = "object";
	operators[OperatorTypes_TRAVEL].byteCodes = new unsigned char[operators[OperatorTypes_TRAVEL].enumAmount]{ bytecode_TRVL };
	operators[OperatorTypes_TRAVEL].params = new unsigned char[operators[OperatorTypes_TRAVEL].enumAmount]{ 2 };
	operators[OperatorTypes_TRAVEL].minParams = new unsigned char[operators[OperatorTypes_TRAVEL].enumAmount]{ 2 };
	operators[OperatorTypes_TRAVEL].paramsyntax = new VarType*[operators[OperatorTypes_TRAVEL].enumAmount]{
		new VarType[operators[OperatorTypes_TRAVEL].params[0]]{ VarType_NUMBER, VarType_NUMBER }
	};
	operators[OperatorTypes_TRAVEL].paramTypes = new ParamType*[operators[OperatorTypes_TRAVEL].enumAmount]{
		new ParamType[operators[OperatorTypes_TRAVEL].params[0]]{ ParamType_NA, ParamType_NA }
	};
	operators[OperatorTypes_TRAVEL].optionalPar = new bool*[operators[OperatorTypes_TRAVEL].enumAmount]{
		new bool[operators[OperatorTypes_TRAVEL].params[0]]{ false, false }
	};
	operators[OperatorTypes_TRAVEL].readParam = new unsigned char[operators[OperatorTypes_TRAVEL].enumAmount]{ 0 };
	operators[OperatorTypes_TRAVEL].returns = new VarType[operators[OperatorTypes_TRAVEL].enumAmount]{ VarType_NA };
	operators[OperatorTypes_TRAVEL].returnType = new ParamType[operators[OperatorTypes_TRAVEL].enumAmount]{ ParamType_NA };
	operators[OperatorTypes_TRAVEL].writeParam = new unsigned char[operators[OperatorTypes_TRAVEL].enumAmount]{ 0 };
	operators[OperatorTypes_TRAVEL].priority = 0;
	operators[OperatorTypes_TRAVEL].shortHandsAmounts = 0;
	operators[OperatorTypes_TRAVEL].amountParOperators = new unsigned char[operators[OperatorTypes_TRAVEL].enumAmount]{ 0 };
	operators[OperatorTypes_TRAVEL].parRepeatsMin = new unsigned char*[operators[OperatorTypes_TRAVEL].enumAmount]{
		new unsigned char[operators[OperatorTypes_TRAVEL].params[0]]{ 0,0 }
	};
	operators[OperatorTypes_TRAVEL].parRepeatsMax = new unsigned char*[operators[OperatorTypes_TRAVEL].enumAmount]{
		new unsigned char[operators[OperatorTypes_TRAVEL].params[0]]{ 0,0 }
	};
	operators[OperatorTypes_TRAVEL].parOperatorAppend = new bool*[operators[OperatorTypes_TRAVEL].enumAmount]{
		new bool[operators[OperatorTypes_TRAVEL].params[0]]{ false, false }
	};


	operators[OperatorTypes_SPLIT].keyword = "split";
	operators[OperatorTypes_SPLIT].enumAmount = 1;
	operators[OperatorTypes_SPLIT].enums = new std::string[operators[OperatorTypes_SPLIT].enumAmount]; operators[OperatorTypes_SPLIT].enums[0] = "route";
	operators[OperatorTypes_SPLIT].byteCodes = new unsigned char[operators[OperatorTypes_SPLIT].enumAmount]{ bytecode_SPLTRTE };
	operators[OperatorTypes_SPLIT].params = new unsigned char[operators[OperatorTypes_SPLIT].enumAmount]{ 5 };
	operators[OperatorTypes_SPLIT].minParams = new unsigned char[operators[OperatorTypes_SPLIT].enumAmount]{ 5 };
	operators[OperatorTypes_SPLIT].paramsyntax = new VarType*[operators[OperatorTypes_SPLIT].enumAmount]{
		new VarType[operators[OperatorTypes_SPLIT].params[0]]{ VarType_NUMBER, VarType_FLOAT,VarType_FLOAT,VarType_FLOAT,VarType_FLOAT }
	};
	operators[OperatorTypes_SPLIT].paramTypes = new ParamType*[operators[OperatorTypes_SPLIT].enumAmount]{
		new ParamType[operators[OperatorTypes_SPLIT].params[0]]{ ParamType_NA, ParamType_NA,ParamType_NA,ParamType_NA,ParamType_NA }
	};
	operators[OperatorTypes_SPLIT].optionalPar = new bool*[operators[OperatorTypes_SPLIT].enumAmount]{
		new bool[operators[OperatorTypes_SPLIT].params[0]]{ false, false, false, false, false }
	};
	operators[OperatorTypes_SPLIT].readParam = new unsigned char[operators[OperatorTypes_SPLIT].enumAmount]{ 0 };
	operators[OperatorTypes_SPLIT].returns = new VarType[operators[OperatorTypes_SPLIT].enumAmount]{ VarType_NA };
	operators[OperatorTypes_SPLIT].returnType = new ParamType[operators[OperatorTypes_SPLIT].enumAmount]{ ParamType_NA };
	operators[OperatorTypes_SPLIT].writeParam = new unsigned char[operators[OperatorTypes_SPLIT].enumAmount]{ 0 };
	operators[OperatorTypes_SPLIT].priority = 0;
	operators[OperatorTypes_SPLIT].shortHandsAmounts = 0;
	operators[OperatorTypes_SPLIT].amountParOperators = new unsigned char[operators[OperatorTypes_SPLIT].enumAmount]{ 0 };
	operators[OperatorTypes_SPLIT].parRepeatsMin = new unsigned char*[operators[OperatorTypes_SPLIT].enumAmount]{
		new unsigned char[operators[OperatorTypes_SPLIT].params[0]]{ 0, 0,0,0,0 }
	};
	operators[OperatorTypes_SPLIT].parRepeatsMax = new unsigned char*[operators[OperatorTypes_SPLIT].enumAmount]{
		new unsigned char[operators[OperatorTypes_SPLIT].params[0]]{ 0, 0,0,0,0 }
	};
	operators[OperatorTypes_SPLIT].parOperatorAppend = new bool*[operators[OperatorTypes_SPLIT].enumAmount]{
		new bool[operators[OperatorTypes_SPLIT].params[0]]{ false, false,false, false, false }
	};

	operators[OperatorTypes_SCOPE].keyword = "scope";
	operators[OperatorTypes_SCOPE].enumAmount = 1;
	operators[OperatorTypes_SCOPE].enums = new std::string[operators[OperatorTypes_SCOPE].enumAmount]; operators[OperatorTypes_SCOPE].enums[0] = "sve";
	operators[OperatorTypes_SCOPE].byteCodes = new unsigned char[operators[OperatorTypes_SCOPE].enumAmount]{ bytecode_SVESCPE };
	operators[OperatorTypes_SCOPE].params = new unsigned char[operators[OperatorTypes_SCOPE].enumAmount]{ 2 };
	operators[OperatorTypes_SCOPE].minParams = new unsigned char[operators[OperatorTypes_SCOPE].enumAmount]{ 2 };
	operators[OperatorTypes_SCOPE].paramsyntax = new VarType*[operators[OperatorTypes_SCOPE].enumAmount]{
		new VarType[operators[OperatorTypes_SCOPE].params[0]]{ VarType_NUMBER, VarType_NUMBER }
	};
	operators[OperatorTypes_SCOPE].paramTypes = new ParamType*[operators[OperatorTypes_SCOPE].enumAmount]{
		new ParamType[operators[OperatorTypes_SCOPE].params[0]]{ ParamType_NA, ParamType_NA }
	};
	operators[OperatorTypes_SCOPE].optionalPar = new bool*[operators[OperatorTypes_SCOPE].enumAmount]{
		new bool[operators[OperatorTypes_SCOPE].params[0]]{ false, false }
	};
	operators[OperatorTypes_SCOPE].readParam = new unsigned char[operators[OperatorTypes_SCOPE].enumAmount]{ 0 };
	operators[OperatorTypes_SCOPE].returns = new VarType[operators[OperatorTypes_SCOPE].enumAmount]{ VarType_NA };
	operators[OperatorTypes_SCOPE].returnType = new ParamType[operators[OperatorTypes_SCOPE].enumAmount]{ ParamType_NA };
	operators[OperatorTypes_SCOPE].writeParam = new unsigned char[operators[OperatorTypes_SCOPE].enumAmount]{ 0 };
	operators[OperatorTypes_SCOPE].priority = 0;
	operators[OperatorTypes_SCOPE].shortHandsAmounts = 0;
	operators[OperatorTypes_SCOPE].amountParOperators = new unsigned char[operators[OperatorTypes_SCOPE].enumAmount]{ 0 };
	operators[OperatorTypes_SCOPE].parRepeatsMin = new unsigned char*[operators[OperatorTypes_SCOPE].enumAmount]{
		new unsigned char[operators[OperatorTypes_SCOPE].params[0]]{ 0, 0 }
	};
	operators[OperatorTypes_SCOPE].parRepeatsMax = new unsigned char*[operators[OperatorTypes_SCOPE].enumAmount]{
		new unsigned char[operators[OperatorTypes_SCOPE].params[0]]{ 0, 0 }
	};
	operators[OperatorTypes_SCOPE].parOperatorAppend = new bool*[operators[OperatorTypes_SCOPE].enumAmount]{
		new bool[operators[OperatorTypes_SCOPE].params[0]]{ false, false }
	};

	operators[OperatorTypes_SAVE].keyword = "save";
	operators[OperatorTypes_SAVE].enumAmount = 1;
	operators[OperatorTypes_SAVE].enums = new std::string[operators[OperatorTypes_SAVE].enumAmount]; operators[OperatorTypes_SAVE].enums[0] = "state";
	operators[OperatorTypes_SAVE].byteCodes = new unsigned char[operators[OperatorTypes_SAVE].enumAmount]{ bytecode_SVESTT };
	operators[OperatorTypes_SAVE].params = new unsigned char[operators[OperatorTypes_SAVE].enumAmount]{ 2 };
	operators[OperatorTypes_SAVE].minParams = new unsigned char[operators[OperatorTypes_SAVE].enumAmount]{ 2 };
	operators[OperatorTypes_SAVE].paramsyntax = new VarType*[operators[OperatorTypes_SAVE].enumAmount]{
		new VarType[operators[OperatorTypes_SAVE].params[0]]{ VarType_NUMBER, VarType_STRING }
	};
	operators[OperatorTypes_SAVE].paramTypes = new ParamType*[operators[OperatorTypes_SAVE].enumAmount]{
		new ParamType[operators[OperatorTypes_SAVE].params[0]]{ ParamType_NA, ParamType_NA }
	};
	operators[OperatorTypes_SAVE].optionalPar = new bool*[operators[OperatorTypes_SAVE].enumAmount]{
		new bool[operators[OperatorTypes_SAVE].params[0]]{ false, false }
	};
	operators[OperatorTypes_SAVE].readParam = new unsigned char[operators[OperatorTypes_SAVE].enumAmount]{ 0 };
	operators[OperatorTypes_SAVE].returns = new VarType[operators[OperatorTypes_SAVE].enumAmount]{ VarType_NA };
	operators[OperatorTypes_SAVE].returnType = new ParamType[operators[OperatorTypes_SAVE].enumAmount]{ ParamType_NA };
	operators[OperatorTypes_SAVE].writeParam = new unsigned char[operators[OperatorTypes_SAVE].enumAmount]{ 0 };
	operators[OperatorTypes_SAVE].priority = 0;
	operators[OperatorTypes_SAVE].shortHandsAmounts = 0;
	operators[OperatorTypes_SAVE].amountParOperators = new unsigned char[operators[OperatorTypes_SAVE].enumAmount]{ 0 };
	operators[OperatorTypes_SAVE].parRepeatsMin = new unsigned char*[operators[OperatorTypes_SAVE].enumAmount]{
		new unsigned char[operators[OperatorTypes_SAVE].params[0]]{ 0, 0 }
	};
	operators[OperatorTypes_SAVE].parRepeatsMax = new unsigned char*[operators[OperatorTypes_SAVE].enumAmount]{
		new unsigned char[operators[OperatorTypes_SAVE].params[0]]{ 0, 0 }
	};
	operators[OperatorTypes_SAVE].parOperatorAppend = new bool*[operators[OperatorTypes_SAVE].enumAmount]{
		new bool[operators[OperatorTypes_SAVE].params[0]]{ false, false }
	};

	operators[OperatorTypes_PAUSE].keyword = "pause";
	operators[OperatorTypes_PAUSE].enumAmount = 1;
	operators[OperatorTypes_PAUSE].enums = new std::string[operators[OperatorTypes_PAUSE].enumAmount]; operators[OperatorTypes_PAUSE].enums[0] = "game";
	operators[OperatorTypes_PAUSE].byteCodes = new unsigned char[operators[OperatorTypes_PAUSE].enumAmount]{ bytecode_PSEGME };
	operators[OperatorTypes_PAUSE].params = new unsigned char[operators[OperatorTypes_PAUSE].enumAmount]{ 0 };
	operators[OperatorTypes_PAUSE].minParams = new unsigned char[operators[OperatorTypes_PAUSE].enumAmount]{ 0 };
	operators[OperatorTypes_PAUSE].paramsyntax = new VarType*[operators[OperatorTypes_PAUSE].enumAmount]{
		nullptr
	};
	operators[OperatorTypes_PAUSE].paramTypes = new ParamType*[operators[OperatorTypes_PAUSE].enumAmount]{
		nullptr
	};
	operators[OperatorTypes_PAUSE].optionalPar = new bool*[operators[OperatorTypes_PAUSE].enumAmount]{
		nullptr
	};
	operators[OperatorTypes_PAUSE].readParam = new unsigned char[operators[OperatorTypes_PAUSE].enumAmount]{ 0 };
	operators[OperatorTypes_PAUSE].returns = new VarType[operators[OperatorTypes_PAUSE].enumAmount]{ VarType_NA };
	operators[OperatorTypes_PAUSE].returnType = new ParamType[operators[OperatorTypes_PAUSE].enumAmount]{ ParamType_NA };
	operators[OperatorTypes_PAUSE].writeParam = new unsigned char[operators[OperatorTypes_PAUSE].enumAmount]{ 0 };
	operators[OperatorTypes_PAUSE].priority = 0;
	operators[OperatorTypes_PAUSE].shortHandsAmounts = 0;
	operators[OperatorTypes_PAUSE].amountParOperators = new unsigned char[operators[OperatorTypes_PAUSE].enumAmount]{ 0 };
	operators[OperatorTypes_PAUSE].parRepeatsMin = new unsigned char*[operators[OperatorTypes_PAUSE].enumAmount]{
		nullptr
	};
	operators[OperatorTypes_PAUSE].parRepeatsMax = new unsigned char*[operators[OperatorTypes_PAUSE].enumAmount]{
		nullptr
	};
	operators[OperatorTypes_PAUSE].parOperatorAppend = new bool*[operators[OperatorTypes_PAUSE].enumAmount]{
		nullptr
	};

	operators[OperatorTypes_RESUME].keyword = "resume";
	operators[OperatorTypes_RESUME].enumAmount = 1;
	operators[OperatorTypes_RESUME].enums = new std::string[operators[OperatorTypes_RESUME].enumAmount]; operators[OperatorTypes_RESUME].enums[0] = "game";
	operators[OperatorTypes_RESUME].byteCodes = new unsigned char[operators[OperatorTypes_RESUME].enumAmount]{ bytecode_RSMGME };
	operators[OperatorTypes_RESUME].params = new unsigned char[operators[OperatorTypes_RESUME].enumAmount]{ 0 };
	operators[OperatorTypes_RESUME].minParams = new unsigned char[operators[OperatorTypes_RESUME].enumAmount]{ 0 };
	operators[OperatorTypes_RESUME].paramsyntax = new VarType*[operators[OperatorTypes_RESUME].enumAmount]{
		nullptr
	};
	operators[OperatorTypes_RESUME].paramTypes = new ParamType*[operators[OperatorTypes_RESUME].enumAmount]{
		nullptr
	};
	operators[OperatorTypes_RESUME].optionalPar = new bool*[operators[OperatorTypes_RESUME].enumAmount]{
		nullptr
	};
	operators[OperatorTypes_RESUME].readParam = new unsigned char[operators[OperatorTypes_RESUME].enumAmount]{ 0 };
	operators[OperatorTypes_RESUME].returns = new VarType[operators[OperatorTypes_RESUME].enumAmount]{ VarType_NA };
	operators[OperatorTypes_RESUME].returnType = new ParamType[operators[OperatorTypes_RESUME].enumAmount]{ ParamType_NA };
	operators[OperatorTypes_RESUME].writeParam = new unsigned char[operators[OperatorTypes_RESUME].enumAmount]{ 0 };
	operators[OperatorTypes_RESUME].priority = 0;
	operators[OperatorTypes_RESUME].shortHandsAmounts = 0;
	operators[OperatorTypes_RESUME].amountParOperators = new unsigned char[operators[OperatorTypes_RESUME].enumAmount]{ 0 };
	operators[OperatorTypes_RESUME].parRepeatsMin = new unsigned char*[operators[OperatorTypes_RESUME].enumAmount]{
		nullptr
	};
	operators[OperatorTypes_RESUME].parRepeatsMax = new unsigned char*[operators[OperatorTypes_RESUME].enumAmount]{
		nullptr
	};
	operators[OperatorTypes_RESUME].parOperatorAppend = new bool*[operators[OperatorTypes_RESUME].enumAmount]{
		nullptr
	};

	operators[OperatorTypes_OFFSET].keyword = "¤offset";
	operators[OperatorTypes_OFFSET].enumAmount = 3;
	operators[OperatorTypes_OFFSET].enums = new std::string[operators[OperatorTypes_OFFSET].enumAmount]; operators[OperatorTypes_OFFSET].enums[0] = ""; operators[OperatorTypes_OFFSET].enums[1] = ""; operators[OperatorTypes_OFFSET].enums[2] = "";
	operators[OperatorTypes_OFFSET].byteCodes = new unsigned char[operators[OperatorTypes_OFFSET].enumAmount]{ bytecode_NUMOFFST,bytecode_NUMOFFST,bytecode_STROFFST };
	operators[OperatorTypes_OFFSET].params = new unsigned char[operators[OperatorTypes_OFFSET].enumAmount]{ 2,2,2 };
	operators[OperatorTypes_OFFSET].minParams = new unsigned char[operators[OperatorTypes_OFFSET].enumAmount]{ 2,2,2 };
	operators[OperatorTypes_OFFSET].paramsyntax = new VarType*[operators[OperatorTypes_OFFSET].enumAmount]{
		new VarType[operators[OperatorTypes_OFFSET].params[0]]{ VarType_NUMBER, VarType_NUMBER },
		new VarType[operators[OperatorTypes_OFFSET].params[1]]{ VarType_NUMBER, VarType_FLOAT },
		new VarType[operators[OperatorTypes_OFFSET].params[2]]{ VarType_NUMBER, VarType_STRING }
	};
	operators[OperatorTypes_OFFSET].paramTypes = new ParamType*[operators[OperatorTypes_OFFSET].enumAmount]{
		new ParamType[operators[OperatorTypes_OFFSET].params[0]]{ ParamType_NA, ParamType_VAR},
		new ParamType[operators[OperatorTypes_OFFSET].params[1]]{ ParamType_NA, ParamType_VAR },
		new ParamType[operators[OperatorTypes_OFFSET].params[2]]{ ParamType_NA, ParamType_VAR }
	};
	operators[OperatorTypes_OFFSET].optionalPar = new bool*[operators[OperatorTypes_OFFSET].enumAmount]{
		new bool[operators[OperatorTypes_OFFSET].params[0]]{ false, false },
		new bool[operators[OperatorTypes_OFFSET].params[1]]{ false, false },
		new bool[operators[OperatorTypes_OFFSET].params[2]]{ false, false }
	};
	operators[OperatorTypes_OFFSET].readParam = new unsigned char[operators[OperatorTypes_OFFSET].enumAmount]{ 0, 0, 0};
	operators[OperatorTypes_OFFSET].returns = new VarType[operators[OperatorTypes_OFFSET].enumAmount]{ VarType_NA,VarType_NA,VarType_NA };
	operators[OperatorTypes_OFFSET].returnType = new ParamType[operators[OperatorTypes_OFFSET].enumAmount]{ ParamType_NA,ParamType_NA ,ParamType_NA };
	operators[OperatorTypes_OFFSET].writeParam = new unsigned char[operators[OperatorTypes_OFFSET].enumAmount]{ 0,0,0 };
	operators[OperatorTypes_OFFSET].priority = 0;
	operators[OperatorTypes_OFFSET].shortHandsAmounts = 0;
	operators[OperatorTypes_OFFSET].amountParOperators = new unsigned char[operators[OperatorTypes_OFFSET].enumAmount]{ 0, 0, 0 };
	operators[OperatorTypes_OFFSET].parRepeatsMin = new unsigned char*[operators[OperatorTypes_OFFSET].enumAmount]{
		new unsigned char[operators[OperatorTypes_OFFSET].params[0]]{ 0,0 },
		new unsigned char[operators[OperatorTypes_OFFSET].params[1]]{ 0,0 },
		new unsigned char[operators[OperatorTypes_OFFSET].params[2]]{ 0,0 }
	};
	operators[OperatorTypes_OFFSET].parRepeatsMax = new unsigned char*[operators[OperatorTypes_OFFSET].enumAmount]{
		new unsigned char[operators[OperatorTypes_OFFSET].params[0]]{ 0, 0 },
		new unsigned char[operators[OperatorTypes_OFFSET].params[1]]{ 0,0 },
		new unsigned char[operators[OperatorTypes_OFFSET].params[2]]{ 0,0 }
	};
	operators[OperatorTypes_OFFSET].parOperatorAppend = new bool*[operators[OperatorTypes_OFFSET].enumAmount]{
		new bool[operators[OperatorTypes_OFFSET].params[0]]{ false, false },
		new bool[operators[OperatorTypes_OFFSET].params[1]]{ false, false },
		new bool[operators[OperatorTypes_OFFSET].params[2]]{ false, false }
	};

	operators[OperatorTypes_ARRAY].keyword = "array";
	operators[OperatorTypes_ARRAY].enumAmount = 3;
	operators[OperatorTypes_ARRAY].enums = new std::string[operators[OperatorTypes_ARRAY].enumAmount]; operators[OperatorTypes_ARRAY].enums[0] = "num"; operators[OperatorTypes_ARRAY].enums[1] = "float"; operators[OperatorTypes_ARRAY].enums[2] = "string";
	operators[OperatorTypes_ARRAY].byteCodes = new unsigned char[operators[OperatorTypes_ARRAY].enumAmount]{ bytecode_NUMARR, bytecode_NUMARR,bytecode_STRARR };
	operators[OperatorTypes_ARRAY].params = new unsigned char[operators[OperatorTypes_ARRAY].enumAmount]{ 1, 1, 1 };
	operators[OperatorTypes_ARRAY].minParams = new unsigned char[operators[OperatorTypes_ARRAY].enumAmount]{ 1, 1, 1 };
	operators[OperatorTypes_ARRAY].paramsyntax = new VarType*[operators[OperatorTypes_ARRAY].enumAmount]{
		new VarType[operators[OperatorTypes_ARRAY].params[0]]{ VarType_NUMBER },
		new VarType[operators[OperatorTypes_ARRAY].params[1]]{ VarType_NUMBER },
		new VarType[operators[OperatorTypes_ARRAY].params[2]]{ VarType_NUMBER }
	};
	operators[OperatorTypes_ARRAY].paramTypes = new ParamType*[operators[OperatorTypes_ARRAY].enumAmount]{
		new ParamType[operators[OperatorTypes_ARRAY].params[0]]{ ParamType_NA },
		new ParamType[operators[OperatorTypes_ARRAY].params[1]]{ ParamType_NA },
		new ParamType[operators[OperatorTypes_ARRAY].params[2]]{ ParamType_NA }
	};
	operators[OperatorTypes_ARRAY].optionalPar = new bool*[operators[OperatorTypes_ARRAY].enumAmount]{
		new bool[operators[OperatorTypes_ARRAY].params[0]]{ false },
		new bool[operators[OperatorTypes_ARRAY].params[1]]{ false },
		new bool[operators[OperatorTypes_ARRAY].params[2]]{ false }
	};
	operators[OperatorTypes_ARRAY].readParam = new unsigned char[operators[OperatorTypes_ARRAY].enumAmount]{ 0,0,0 };
	operators[OperatorTypes_ARRAY].returns = new VarType[operators[OperatorTypes_ARRAY].enumAmount]{ VarType_NUMBER,VarType_FLOAT,VarType_STRING };
	operators[OperatorTypes_ARRAY].returnType = new ParamType[operators[OperatorTypes_ARRAY].enumAmount]{ ParamType_VAR, ParamType_VAR,ParamType_VAR };
	operators[OperatorTypes_ARRAY].writeParam = new unsigned char[operators[OperatorTypes_ARRAY].enumAmount]{ 0,0,0 };
	operators[OperatorTypes_ARRAY].priority = 0;
	operators[OperatorTypes_ARRAY].shortHandsAmounts = 0;
	operators[OperatorTypes_ARRAY].amountParOperators = new unsigned char[operators[OperatorTypes_ARRAY].enumAmount]{ 0,0,0 };
	operators[OperatorTypes_ARRAY].parRepeatsMin = new unsigned char*[operators[OperatorTypes_ARRAY].enumAmount]{
		new unsigned char[operators[OperatorTypes_ARRAY].params[0]]{ 0 },
		new unsigned char[operators[OperatorTypes_ARRAY].params[1]]{ 0 },
		new unsigned char[operators[OperatorTypes_ARRAY].params[2]]{ 0 }
	};
	operators[OperatorTypes_ARRAY].parRepeatsMax = new unsigned char*[operators[OperatorTypes_ARRAY].enumAmount]{
		new unsigned char[operators[OperatorTypes_ARRAY].params[0]]{ 0 },
		new unsigned char[operators[OperatorTypes_ARRAY].params[1]]{ 0 },
		new unsigned char[operators[OperatorTypes_ARRAY].params[2]]{ 0 }
	};
	operators[OperatorTypes_ARRAY].parOperatorAppend = new bool*[operators[OperatorTypes_ARRAY].enumAmount]{
		new bool[operators[OperatorTypes_ARRAY].params[0]]{ false },
		new bool[operators[OperatorTypes_ARRAY].params[1]]{ false },
		new bool[operators[OperatorTypes_ARRAY].params[2]]{ false }
	};

	operators[OperatorTypes_ROUTE].keyword = "route";
	operators[OperatorTypes_ROUTE].enumAmount = 1;
	operators[OperatorTypes_ROUTE].enums = new std::string[operators[OperatorTypes_ROUTE].enumAmount]; operators[OperatorTypes_ROUTE].enums[0] = "";
	operators[OperatorTypes_ROUTE].byteCodes = new unsigned char[operators[OperatorTypes_ROUTE].enumAmount]{ bytecode_RUTE};
	operators[OperatorTypes_ROUTE].params = new unsigned char[operators[OperatorTypes_ROUTE].enumAmount]{ 1};
	operators[OperatorTypes_ROUTE].minParams = new unsigned char[operators[OperatorTypes_ROUTE].enumAmount]{ 1};
	operators[OperatorTypes_ROUTE].paramsyntax = new VarType*[operators[OperatorTypes_ROUTE].enumAmount]{
		new VarType[operators[OperatorTypes_ROUTE].params[0]]{ VarType_NUMBER }
	};
	operators[OperatorTypes_ROUTE].paramTypes = new ParamType*[operators[OperatorTypes_ROUTE].enumAmount]{
		new ParamType[operators[OperatorTypes_ROUTE].params[0]]{ ParamType_NA }
	};
	operators[OperatorTypes_ROUTE].optionalPar = new bool*[operators[OperatorTypes_ROUTE].enumAmount]{
		new bool[operators[OperatorTypes_ROUTE].params[0]]{ false }
	};
	operators[OperatorTypes_ROUTE].readParam = new unsigned char[operators[OperatorTypes_ROUTE].enumAmount]{ 0 };
	operators[OperatorTypes_ROUTE].returns = new VarType[operators[OperatorTypes_ROUTE].enumAmount]{ VarType_NUMBER};
	operators[OperatorTypes_ROUTE].returnType = new ParamType[operators[OperatorTypes_ROUTE].enumAmount]{ ParamType_VAR};
	operators[OperatorTypes_ROUTE].writeParam = new unsigned char[operators[OperatorTypes_ROUTE].enumAmount]{ 0};
	operators[OperatorTypes_ROUTE].priority = 0;
	operators[OperatorTypes_ROUTE].shortHandsAmounts = 0;
	operators[OperatorTypes_ROUTE].amountParOperators = new unsigned char[operators[OperatorTypes_ROUTE].enumAmount]{ 0};
	operators[OperatorTypes_ROUTE].parRepeatsMin = new unsigned char*[operators[OperatorTypes_ROUTE].enumAmount]{
		new unsigned char[operators[OperatorTypes_ROUTE].params[0]]{ 0 }
	};
	operators[OperatorTypes_ROUTE].parRepeatsMax = new unsigned char*[operators[OperatorTypes_ROUTE].enumAmount]{
		new unsigned char[operators[OperatorTypes_ROUTE].params[0]]{ 0 }
	};
	operators[OperatorTypes_ROUTE].parOperatorAppend = new bool*[operators[OperatorTypes_ROUTE].enumAmount]{
		new bool[operators[OperatorTypes_ROUTE].params[0]]{ false }
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

	parser = new KubLingParser(keywords, keyWordsSize, flowOps, FlowOperator_NA);
	lexer = new KubLingLexer(keywords, keyWordsSize, operators, OperatorTypes_NA);
	analyzer = new KubLingAnalyzer(keywords, keyWordsSize, operators, OperatorTypes_NA, flowOps, FlowOperator_NA,rParams,RunTimeParams_NA);
	generator = new KubLingCodeGenerator(keywords, keyWordsSize);

}

void KubLingCompiler::setupArithmeticOps()
{

	operators[OperatorTypes_SUM].keyword = "sum";
	operators[OperatorTypes_SUM].enumAmount = 7;
	operators[OperatorTypes_SUM].enums = new std::string[operators[OperatorTypes_SUM].enumAmount]; operators[OperatorTypes_SUM].enums[0] = ""; operators[OperatorTypes_SUM].enums[1] = ""; operators[OperatorTypes_SUM].enums[2] = ""; operators[OperatorTypes_SUM].enums[3] = ""; operators[OperatorTypes_SUM].enums[4] = ""; operators[OperatorTypes_SUM].enums[5] = ""; operators[OperatorTypes_SUM].enums[6] = "";
	operators[OperatorTypes_SUM].byteCodes = new unsigned char[operators[OperatorTypes_SUM].enumAmount]{bytecode_SUM2CONST, bytecode_SUM2CONST, bytecode_SUMVAR, bytecode_SUMFLOAT, bytecode_SUMFLOAT, bytecode_SUMFLOAT, bytecode_SUMSTR};
	operators[OperatorTypes_SUM].params = new unsigned char[operators[OperatorTypes_SUM].enumAmount]{2, 2, 2, 2, 2, 2, 2};
	operators[OperatorTypes_SUM].minParams = new unsigned char[operators[OperatorTypes_SUM].enumAmount]{2, 2, 2, 2, 2, 2, 2};
	operators[OperatorTypes_SUM].paramsyntax = new VarType*[operators[OperatorTypes_SUM].enumAmount]{
		new VarType[operators[OperatorTypes_SUM].params[0]]{VarType_NUMBER, VarType_NUMBER},
			new VarType[operators[OperatorTypes_SUM].params[1]]{VarType_FLOAT, VarType_FLOAT},
			new VarType[operators[OperatorTypes_SUM].params[2]]{VarType_NUMBER, VarType_NUMBER},
			new VarType[operators[OperatorTypes_SUM].params[3]]{VarType_FLOAT, VarType_FLOAT},
			new VarType[operators[OperatorTypes_SUM].params[4]]{VarType_NUMBER, VarType_FLOAT},
			new VarType[operators[OperatorTypes_SUM].params[5]]{VarType_FLOAT, VarType_NUMBER},
			new VarType[operators[OperatorTypes_SUM].params[6]]{ VarType_STRING, VarType_STRING } };
	operators[OperatorTypes_SUM].paramTypes = new ParamType*[operators[OperatorTypes_SUM].enumAmount]{
		new ParamType[operators[OperatorTypes_SUM].params[0]]{ParamType_CONST, ParamType_CONST},
			new ParamType[operators[OperatorTypes_SUM].params[1]]{ParamType_CONST, ParamType_CONST},
			new ParamType[operators[OperatorTypes_SUM].params[2]]{ParamType_NA, ParamType_NA},
			new ParamType[operators[OperatorTypes_SUM].params[3]]{ParamType_NA, ParamType_NA},
			new ParamType[operators[OperatorTypes_SUM].params[4]]{ParamType_NA, ParamType_NA},
			new ParamType[operators[OperatorTypes_SUM].params[5]]{ParamType_NA, ParamType_NA},
			new ParamType[operators[OperatorTypes_SUM].params[6]]{ ParamType_NA, ParamType_NA } };
	operators[OperatorTypes_SUM].optionalPar = new bool*[operators[OperatorTypes_SUM].enumAmount]{
		new bool[operators[OperatorTypes_SUM].params[0]]{ false, false },
			new bool[operators[OperatorTypes_SUM].params[1]]{ false, false },
			new bool[operators[OperatorTypes_SUM].params[2]]{ false, false },
			new bool[operators[OperatorTypes_SUM].params[3]]{ false, false },
			new bool[operators[OperatorTypes_SUM].params[4]]{ false, false },
			new bool[operators[OperatorTypes_SUM].params[5]]{ false, false },
			new bool[operators[OperatorTypes_SUM].params[6]]{ false, false }
	};
	operators[OperatorTypes_SUM].readParam = new unsigned char[operators[OperatorTypes_SUM].enumAmount]{0, 0, 0, 0, 0, 0, 0};
	operators[OperatorTypes_SUM].returns = new VarType[operators[OperatorTypes_SUM].enumAmount]{VarType_NUMBER, VarType_FLOAT, VarType_NUMBER, VarType_FLOAT, VarType_FLOAT, VarType_FLOAT, VarType_STRING };
	operators[OperatorTypes_SUM].returnType = new ParamType[operators[OperatorTypes_SUM].enumAmount]{ParamType_CONST, ParamType_CONST, ParamType_VAR, ParamType_VAR, ParamType_VAR, ParamType_VAR, ParamType_VAR };
	operators[OperatorTypes_SUM].writeParam = new unsigned char[operators[OperatorTypes_SUM].enumAmount]{0, 0, 0, 0, 0, 0, 0};
	operators[OperatorTypes_SUM].priority = 3;
	operators[OperatorTypes_SUM].shortHandsAmounts = 1;
	operators[OperatorTypes_SUM].shortHands = new std::string[operators[OperatorTypes_SUM].shortHandsAmounts]; operators[OperatorTypes_SUM].shortHands[0] = "+";
	operators[OperatorTypes_SUM].rightParams = new unsigned char[operators[OperatorTypes_SUM].shortHandsAmounts]{1};
	operators[OperatorTypes_SUM].leftParams = new unsigned char[operators[OperatorTypes_SUM].shortHandsAmounts]{1};
	operators[OperatorTypes_SUM].shortFlipParams = new bool[operators[OperatorTypes_SUM].shortHandsAmounts]{false};
	operators[OperatorTypes_SUM].amountParOperators = new unsigned char[operators[OperatorTypes_SUM].enumAmount]{0, 0, 0, 0, 1, 1, 0};
	operators[OperatorTypes_SUM].parRepeatsMin = new unsigned char*[operators[OperatorTypes_SUM].enumAmount]{
		new unsigned char[operators[OperatorTypes_SUM].params[0]]{0, 0},
			new unsigned char[operators[OperatorTypes_SUM].params[1]]{0, 0},
			new unsigned char[operators[OperatorTypes_SUM].params[2]]{0, 0},
			new unsigned char[operators[OperatorTypes_SUM].params[3]]{0, 0},
			new unsigned char[operators[OperatorTypes_SUM].params[4]]{0, 0},
			new unsigned char[operators[OperatorTypes_SUM].params[5]]{0, 0},
			new unsigned char[operators[OperatorTypes_SUM].params[6]]{ 0, 0 }
	};
	operators[OperatorTypes_SUM].parRepeatsMax = new unsigned char*[operators[OperatorTypes_SUM].enumAmount]{
		new unsigned char[operators[OperatorTypes_SUM].params[0]]{0, 0},
			new unsigned char[operators[OperatorTypes_SUM].params[1]]{0, 0},
			new unsigned char[operators[OperatorTypes_SUM].params[2]]{0, 0},
			new unsigned char[operators[OperatorTypes_SUM].params[3]]{0, 0},
			new unsigned char[operators[OperatorTypes_SUM].params[4]]{0, 0},
			new unsigned char[operators[OperatorTypes_SUM].params[5]]{0, 0},
			new unsigned char[operators[OperatorTypes_SUM].params[6]]{ 0, 0 }
	};
	operators[OperatorTypes_SUM].parOperators = new OperatorTypes*[operators[OperatorTypes_SUM].enumAmount]{
		nullptr,
			nullptr,
			nullptr,
			nullptr,
			new OperatorTypes[operators[OperatorTypes_SUM].amountParOperators[4]]{OperatorTypes_CAST},
			new OperatorTypes[operators[OperatorTypes_SUM].amountParOperators[5]]{OperatorTypes_CAST},
		nullptr
	};
	operators[OperatorTypes_SUM].parOperator = new unsigned char*[operators[OperatorTypes_SUM].enumAmount]{
		nullptr,
			nullptr,
			nullptr,
			nullptr,
			new unsigned char[operators[OperatorTypes_SUM].params[4]]{1, 0},
			new unsigned char[operators[OperatorTypes_SUM].params[5]]{0, 1},
			nullptr
	};
	operators[OperatorTypes_SUM].parOperatorAppend = new bool*[operators[OperatorTypes_SUM].enumAmount]{
		new bool[operators[OperatorTypes_SUM].params[0]]{false, false},
			new bool[operators[OperatorTypes_SUM].params[1]]{false, false},
			new bool[operators[OperatorTypes_SUM].params[2]]{false, false},
			new bool[operators[OperatorTypes_SUM].params[3]]{false, false},
			new bool[operators[OperatorTypes_SUM].params[4]]{false, false},
			new bool[operators[OperatorTypes_SUM].params[5]]{false, false},
			new bool[operators[OperatorTypes_SUM].params[6]]{ false, false }
	};

	operators[OperatorTypes_SUB].keyword = "sub";
	operators[OperatorTypes_SUB].enumAmount = 10;
	operators[OperatorTypes_SUB].enums = new std::string[operators[OperatorTypes_SUB].enumAmount]; operators[OperatorTypes_SUB].enums[0] = ""; operators[OperatorTypes_SUB].enums[1] = ""; operators[OperatorTypes_SUB].enums[2] = ""; operators[OperatorTypes_SUB].enums[3] = ""; operators[OperatorTypes_SUB].enums[4] = ""; operators[OperatorTypes_SUB].enums[5] = ""; operators[OperatorTypes_SUB].enums[6] = ""; operators[OperatorTypes_SUB].enums[7] = ""; operators[OperatorTypes_SUB].enums[8] = ""; operators[OperatorTypes_SUB].enums[9] = "";
	operators[OperatorTypes_SUB].byteCodes = new unsigned char[operators[OperatorTypes_SUB].enumAmount]{bytecode_SUB2CONST, bytecode_SUB2CONST, bytecode_SUBVAR, bytecode_SUBFLOAT, bytecode_SUBFLOAT, bytecode_SUBFLOAT, bytecode_NEGATECONST, bytecode_NEGATECONST, bytecode_NEGATEFLOAT, bytecode_NEGATEVAR};
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
	operators[OperatorTypes_MUL].byteCodes = new unsigned char[operators[OperatorTypes_MUL].enumAmount]{bytecode_MUL2CONST, bytecode_MUL2CONST, bytecode_MULVAR, bytecode_MULFLOAT, bytecode_MULFLOAT, bytecode_MULFLOAT};
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
	operators[OperatorTypes_DIV].byteCodes = new unsigned char[operators[OperatorTypes_DIV].enumAmount]{bytecode_DIV2CONST, bytecode_DIV2CONST, bytecode_DIVVAR, bytecode_DIVFLOAT, bytecode_DIVFLOAT, bytecode_DIVFLOAT};
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

	operators[OperatorTypes_MOD].keyword = "mod";
	operators[OperatorTypes_MOD].enumAmount = 1;
	operators[OperatorTypes_MOD].enums = new std::string[operators[OperatorTypes_MOD].enumAmount]; operators[OperatorTypes_MOD].enums[0] = ""; 
	operators[OperatorTypes_MOD].byteCodes = new unsigned char[operators[OperatorTypes_MOD].enumAmount]{ bytecode_MODVAR };
	operators[OperatorTypes_MOD].params = new unsigned char[operators[OperatorTypes_MOD].enumAmount]{ 2 };
	operators[OperatorTypes_MOD].minParams = new unsigned char[operators[OperatorTypes_MOD].enumAmount]{ 2};
	operators[OperatorTypes_MOD].paramsyntax = new VarType*[operators[OperatorTypes_MOD].enumAmount]{
		new VarType[operators[OperatorTypes_MOD].params[0]]{ VarType_NUMBER, VarType_NUMBER } };
	operators[OperatorTypes_MOD].paramTypes = new ParamType*[operators[OperatorTypes_MOD].enumAmount]{
		new ParamType[operators[OperatorTypes_MOD].params[0]]{ ParamType_NA, ParamType_NA } };
	operators[OperatorTypes_MOD].optionalPar = new bool*[operators[OperatorTypes_MOD].enumAmount]{
		new bool[operators[OperatorTypes_MOD].params[0]]{ false, false }
	};
	operators[OperatorTypes_MOD].readParam = new unsigned char[operators[OperatorTypes_MOD].enumAmount]{ 0};
	operators[OperatorTypes_MOD].returns = new VarType[operators[OperatorTypes_MOD].enumAmount]{ VarType_NUMBER};
	operators[OperatorTypes_MOD].returnType = new ParamType[operators[OperatorTypes_MOD].enumAmount]{ ParamType_VAR};
	operators[OperatorTypes_MOD].writeParam = new unsigned char[operators[OperatorTypes_MOD].enumAmount]{ 0 };
	operators[OperatorTypes_MOD].priority = 4;
	operators[OperatorTypes_MOD].shortHandsAmounts = 1;
	operators[OperatorTypes_MOD].shortHands = new std::string[operators[OperatorTypes_MOD].shortHandsAmounts]; operators[OperatorTypes_MOD].shortHands[0] = "%";
	operators[OperatorTypes_MOD].rightParams = new unsigned char[operators[OperatorTypes_MOD].shortHandsAmounts]{ 1 };
	operators[OperatorTypes_MOD].leftParams = new unsigned char[operators[OperatorTypes_MOD].shortHandsAmounts]{ 1 };
	operators[OperatorTypes_MOD].shortFlipParams = new bool[operators[OperatorTypes_MOD].shortHandsAmounts]{ false };
	operators[OperatorTypes_MOD].amountParOperators = new unsigned char[operators[OperatorTypes_MOD].enumAmount]{ 0 };
	operators[OperatorTypes_MOD].parRepeatsMin = new unsigned char*[operators[OperatorTypes_MOD].enumAmount]{
		new unsigned char[operators[OperatorTypes_MOD].params[0]]{ 0, 0 }
	};
	operators[OperatorTypes_MOD].parRepeatsMax = new unsigned char*[operators[OperatorTypes_MOD].enumAmount]{
		new unsigned char[operators[OperatorTypes_MOD].params[0]]{ 0, 0 }
	};
	operators[OperatorTypes_MOD].parOperatorAppend = new bool*[operators[OperatorTypes_MOD].enumAmount]{
		new bool[operators[OperatorTypes_MOD].params[0]]{ false, false }
	};
}

void KubLingCompiler::Init(CrossHandlers::TextContainer* outText)
{

	this->outText = outText;

}

unsigned int KubLingCompiler::getParam(std::string param) const
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

KubLingCompiled* KubLingCompiler::CompileSource(KubLingSource* source, CompileError &error)
{

	KubLingCompiled* compiledSource = nullptr;
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

				unsigned int maxVars = 0;
				CelestialList<CelestialList<unsigned char>*>* compiledCode = generator->GenerateCode(abstractSyntaxTree, maxVars);
				unsigned int scriptParams = 0;
				unsigned int scriptStringParams = 0;
				unsigned int scriptFloatParams = 0;

				for (int k = 0; k < 2; k++)
				{

					for (int i = 0; i < abstractSyntaxTree.symbolTable->GetHighest(); i++)
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
										compiledSource->SetMaxVar(maxVars);

									}
								}
							}
						}
					}

					if (k == 0)
					{
						
						compiledSource = new KubLingCompiled(scriptParams, scriptStringParams, scriptFloatParams, source->GetName());

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

KubLingCompiler::~KubLingCompiler()
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
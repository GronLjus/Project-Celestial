#pragma once

namespace Logic
{
	///<summary>The code for each operator</summary>
	enum opcode : unsigned char {
		opcode_SETCONST, opcode_SETVAR,

		opcode_LOADLIGHT, opcode_LOADMESH, opcode_LOADSCRIPT, opcode_LOADKYTRGR, opcode_LOADCHRKYTRGR, opcode_LOADTXTBX, opcode_LOADPANEL, opcode_LOADGMBRD, opcode_LOADCAM, opcode_LOADOBJCT,

		opcode_ADDOBJECT, opcode_ADDMESH, 
		opcode_SETGMEBRD, opcode_SETCMRA, opcode_SETLCLK,

		opcode_GETSCRNX, opcode_GETSCRNY,

		opcode_RESNAP, opcode_2DPOS, opcode_2DSIZE, opcode_2DADDASNODE, opcode_2DADDKYTRGR,

		opcode_POSTSTR, opcode_POSTNMBR, opcode_POSTFLOAT,

		opcode_NEGATECONST, opcode_NEGATEVAR, opcode_NEGATEFLOAT,
		opcode_SUM2CONST, opcode_SUMFLOAT, opcode_SUMVAR,
		opcode_SUB2CONST, opcode_SUBFLOAT, opcode_SUBVAR,
		opcode_MUL2CONST, opcode_MULFLOAT, opcode_MULVAR,
		opcode_DIV2CONST, opcode_DIVFLOAT, opcode_DIVVAR,
		opcode_CASTFLOAT, 

		opcode_NUMEQUAL2CONST, opcode_NUMEQUALFLOAT, opcode_NUMEQUALVAR,
		opcode_STREQUAL2CONST, opcode_STREQUALVAR,

		opcode_NUMGRT2CONST, opcode_NUMGRTFLOAT, opcode_NUMGRTVAR,

		opcode_RNSCRPT, opcode_STSCRPTPRMNMBR, opcode_STSCRPTPRMSTR,

		opcode_EXPRTCNST, opcode_EXPRTVAR, opcode_IMPRT,

		opcode_JMPINVVAR,opcode_JMPNOW,opCode_WTFRVR,

		opcode_NA
	};

	///<summary>The different types of runtime errors</summary>
	enum RunTimeError{
		RunTimeError_OK, RunTimeError_MSGFULL,
		RunTimeError_BADPARAMS, RunTimeError_ABORT, 
		RunTimeError_WAITINGFORWAR, RunTimeError_OHNO, 
		RunTimeError_HALT, RunTimeError_NA
	};


	enum RunTimeParams
	{
		RunTimeParams_ETIME, RunTimeParams_NA

	};
}
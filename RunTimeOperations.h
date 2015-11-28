#pragma once

namespace Logic
{
	///<summary>The code for each operator</summary>
	enum opcode : unsigned char {
		opcode_SETCONST, opcode_SETVAR,

		opcode_LOADLIGHT, opcode_LOADMESH, opcode_LOADSCRIPT, opcode_LOADKYTRGR, opcode_LOADCHRKYTRGR, opcode_LOADTXTBX, opcode_LOADPANEL,

		opcode_ADDOBJECT,

		opcode_GETSCRNX, opcode_GETSCRNY,

		opcode_RESNAP, opcode_2DPOS, opcode_2DSIZE, opcode_2DADDASNODE, opcode_2DADDKYTRGR,

		opcode_POSTSTR, opcode_POSTNMBR, opcode_POSTNMBRCONST,

		opcode_SUM2CONST, opcode_SUM1CONST, opcode_SUMCONST1, opcode_SUMVAR,
		opcode_SUB2CONST, opcode_SUB1CONST, opcode_SUBCONST1, opcode_SUBVAR,
		opcode_MUL2CONST, opcode_MUL1CONST, opcode_MULCONST1, opcode_MULVAR,
		opcode_DIV2CONST, opcode_DIV1CONST, opcode_DIVCONST1, opcode_DIVVAR,

		opcode_NUMEQUAL2CONST, opcode_NUMEQUALCONST1, opcode_NUMEQUAL1CONST, opcode_NUMEQUALVAR,

		opcode_STREQUAL2CONST, opcode_STREQUALCONST1, opcode_STREQUAL1CONST, opcode_STREQUALVAR,

		opcode_NUMGRT2CONST, opcode_NUMGRTCONST1, opcode_NUMGRT1CONST, opcode_NUMGRTVAR,

		opcode_RNSCRPT, opcode_STSCRPTPRMNMBR, opcode_STSCRPTPRMSTR,

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
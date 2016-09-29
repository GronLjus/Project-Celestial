#pragma once

namespace Logic
{
	///<summary>The code for each operator</summary>
	enum bytecode : unsigned char {
		bytecode_SETCONST, bytecode_SETVAR,

		bytecode_LOADLIGHT, bytecode_LOADMESH, bytecode_LOADSCRIPT,
		bytecode_LOADTXTBX, bytecode_LOADPANEL, bytecode_LOADIMAGE,
		bytecode_LOADGMBRD, bytecode_LOADCAM, bytecode_LOADOBJCT,
		bytecode_LOADCPY,

		bytecode_UNLOAD,

		bytecode_ADDOBJECT, bytecode_ADDMESH,
		bytecode_ADDRTOBJ,

		bytecode_RMVE,
		bytecode_SETGMEBRD, bytecode_SETCMRA,
		bytecode_SETLCLK, bytecode_SETMCLK, bytecode_SETRCLK,
		bytecode_SETLDRG, bytecode_SETMDRG, bytecode_SETRDRG,
		bytecode_SETMWHL, bytecode_SETUD,
		bytecode_SETNDEARRVL,
		bytecode_SETUI,
		bytecode_SETCRS,
		bytecode_FCSUI,

		bytecode_GETSCRNY,
		bytecode_PRNT,

		bytecode_RESNAP,
		bytecode_POS, bytecode_MVE, bytecode_PRPL,
		bytecode_RTE, bytecode_ORB,
		bytecode_SIZE, bytecode_SCL,
		bytecode_2DADDCHLD,

		bytecode_POSTSTR, bytecode_POSTNMBR, bytecode_POSTFLOAT,

		bytecode_IGNRMS, bytecode_IGNRKY, bytecode_IGNRIN,
		bytecode_ACPTMS, bytecode_ACPTKY, bytecode_ACPTIN,
		bytecode_INCRMNTLYR, bytecode_DCRMNLYR, bytecode_STLYR,

		bytecode_NEGATECONST, bytecode_NEGATEVAR, bytecode_NEGATEFLOAT,
		bytecode_SUM2CONST, bytecode_SUMFLOAT, bytecode_SUMVAR,
		bytecode_SUB2CONST, bytecode_SUBFLOAT, bytecode_SUBVAR,
		bytecode_MUL2CONST, bytecode_MULFLOAT, bytecode_MULVAR,
		bytecode_DIV2CONST, bytecode_DIVFLOAT, bytecode_DIVVAR,
		bytecode_CASTFLOAT,

		bytecode_NUMEQUAL2CONST, bytecode_NUMEQUALFLOAT, bytecode_NUMEQUALVAR,
		bytecode_STREQUAL2CONST, bytecode_STREQUALVAR,

		bytecode_NUMGRT2CONST, bytecode_NUMGRTFLOAT, bytecode_NUMGRTVAR,

		bytecode_RNSCRPT, bytecode_STSCRPTPRMNMBR, bytecode_STSCRPTPRMSTR, bytecode_STSCRPTPRMFLT,

		bytecode_EXPRTCNST, bytecode_EXPRTVAR, bytecode_IMPRT,
		bytecode_HDE, bytecode_SHW,
		bytecode_HDECRS, bytecode_SHWCRS,

		bytecode_LNKDBG, bytecode_LNKTRGT,
		bytecode_STXT, bytecode_ADDTXT, bytecode_ADDLNE,
		bytecode_STCNTNT, bytecode_STBRDR,
		
		bytecode_SPLTRTE,

		bytecode_TRCK, bytecode_CRLTRCK, bytecode_HCKTRCK, 
		bytecode_SPWN, bytecode_TRVL,
		bytecode_GLUOBJCT,

		bytecode_SVESCPE,
		bytecode_SVESTT, bytecode_LDSTT,
		bytecode_CLRBRD,
		bytecode_PSEGME, bytecode_RSMGME,

		bytecode_JMPINVVAR,bytecode_JMPNOW,opCode_WTFRVR,

		bytecode_NA
	};

	enum opcode : unsigned char
	{

		opcode_STOP,
		
		opcode_HEAP,

		opcode_PLACE,
		opcode_LOAD,
		opcode_STORE,
		opcode_SAVE,
		opcode_MOV,
		
		opcode_SEND,
		opcode_STPRM,

		opcode_ADD,
		opcode_SUB,
		opcode_MUL,
		opcode_DIV,

		opcode_CMPRE,
		opcode_BITCMP,

		opcode_RETURN,

		opcode_ALLOC,
		opcode_MALLOC,
		
		opcode_JMP,
		opcode_JMPINV

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
#pragma once

namespace Logic
{
	///<summary>The code for each operator</summary>
	enum opcode : unsigned char {
		opcode_SETCONST, opcode_SETVAR,

		opcode_LOADLIGHT, opcode_LOADMESH, opcode_LOADSCRIPT, 
		opcode_LOADTXTBX, opcode_LOADPANEL, opcode_LOADIMAGE,
		opcode_LOADGMBRD, opcode_LOADCAM, opcode_LOADOBJCT,
		opcode_LOADRTOBJ,
		opcode_LOADCPY,

		opcode_UNLOAD,

		opcode_ADDOBJECT, opcode_ADDMESH,
		opcode_ADDRTOBJ,

		opcode_RMVE,
		opcode_SETGMEBRD, opcode_SETCMRA, 
		opcode_SETLCLK, opcode_SETMCLK, opcode_SETRCLK,
		opcode_SETLDRG, opcode_SETMDRG, opcode_SETRDRG,
		opcode_SETMWHL, opcode_SETUD,
		opcode_SETUI,
		opcode_SETCRS,
		opcode_FCSUI,

		opcode_GETSCRNY,
		opcode_PRNT,

		opcode_RESNAP, 
		opcode_POS, opcode_MVE, opcode_PRPL,
		opcode_RTE, opcode_ORB,
		opcode_SIZE, opcode_SCL, 
		opcode_2DADDCHLD,

		opcode_POSTSTR, opcode_POSTNMBR, opcode_POSTFLOAT,

		opcode_IGNRMS, opcode_IGNRKY, opcode_IGNRIN,
		opcode_ACPTMS, opcode_ACPTKY, opcode_ACPTIN,
		opcode_INCRMNTLYR, opcode_DCRMNLYR, opcode_STLYR,

		opcode_NEGATECONST, opcode_NEGATEVAR, opcode_NEGATEFLOAT,
		opcode_SUM2CONST, opcode_SUMFLOAT, opcode_SUMVAR,
		opcode_SUB2CONST, opcode_SUBFLOAT, opcode_SUBVAR,
		opcode_MUL2CONST, opcode_MULFLOAT, opcode_MULVAR,
		opcode_DIV2CONST, opcode_DIVFLOAT, opcode_DIVVAR,
		opcode_CASTFLOAT, 

		opcode_NUMEQUAL2CONST, opcode_NUMEQUALFLOAT, opcode_NUMEQUALVAR,
		opcode_STREQUAL2CONST, opcode_STREQUALVAR,

		opcode_NUMGRT2CONST, opcode_NUMGRTFLOAT, opcode_NUMGRTVAR,

		opcode_RNSCRPT, opcode_STSCRPTPRMNMBR, opcode_STSCRPTPRMSTR, opcode_STSCRPTPRMFLT,

		opcode_EXPRTCNST, opcode_EXPRTVAR, opcode_IMPRT,
		opcode_HDE, opcode_SHW,
		opcode_HDECRS, opcode_SHWCRS,

		opcode_LNKDBG, opcode_LNKTRGT,
		opcode_STXT, opcode_ADDTXT, opcode_ADDLNE,
		opcode_STCNTNT, opcode_STBRDR,

		opcode_TRCK, opcode_CRLTRCK, opcode_HCKTRCK, 
		opcode_GLUOBJCT,

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
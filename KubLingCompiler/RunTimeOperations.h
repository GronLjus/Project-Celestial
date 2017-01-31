#pragma once
#include <string>

namespace Logic
{
	///<summary>The code for each operator</summary>
	enum bytecode : unsigned char {
		bytecode_SETCONST, bytecode_SETVAR,

		bytecode_LOADLIGHT, bytecode_LOADMESH, bytecode_LOADSCRIPT,
		bytecode_LOADTXTBX, bytecode_LOADPANEL, bytecode_LOADIMAGE,
		bytecode_LOADGMBRD, bytecode_LOADCAM, bytecode_LOADOBJCT,
		bytecode_LOADCPY, bytecode_LOADTSK,

		bytecode_UNLOAD,

		bytecode_ADDOBJECT, bytecode_ADDMESH,
		bytecode_ADDRTOBJ, bytecode_ADDTRGGR,

		bytecode_RMVE,
		bytecode_SETGMEBRD, bytecode_SETCMRA,
		bytecode_SETLCLK, bytecode_SETMCLK, bytecode_SETRCLK,
		bytecode_SETLDRG, bytecode_SETMDRG, bytecode_SETRDRG,
		bytecode_SETMWHL, bytecode_SETUD,
		bytecode_SETNDEARRVL, bytecode_SETSPLT,
		bytecode_SETUI,
		bytecode_SETCRS,
		bytecode_FCSUI,
	
		bytecode_FLGRD,
		
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
		bytecode_SUM2CONST, bytecode_SUMFLOAT, bytecode_SUMVAR, bytecode_SUMSTR,
		bytecode_SUB2CONST, bytecode_SUBFLOAT, bytecode_SUBVAR,
		bytecode_MUL2CONST, bytecode_MULFLOAT, bytecode_MULVAR,
		bytecode_DIV2CONST, bytecode_DIVFLOAT, bytecode_DIVVAR,
		bytecode_MODVAR,

		bytecode_CASTFLOAT, bytecode_CASTTSTR,

		bytecode_NUMEQUAL2CONST, bytecode_NUMEQUALFLOAT, bytecode_NUMEQUALVAR,
		bytecode_STREQUAL2CONST, bytecode_STREQUALVAR,

		bytecode_NUMGRT2CONST, bytecode_NUMGRTFLOAT, bytecode_NUMGRTVAR,

		bytecode_RNSCRPT, bytecode_STSCRPTPRMNMBR, bytecode_STSCRPTPRMSTR, bytecode_STSCRPTPRMFLT,

		bytecode_EXPRTSTR, bytecode_EXPRTNUM, bytecode_IMPRTSTR, bytecode_IMPRTNUM,
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
		bytecode_AND, bytecode_OR,

		bytecode_ADDTSKPARNUM, bytecode_ADDTSKPARFLT, bytecode_ADDTSKPARSTR,
		bytecode_QSTSK, bytecode_QCTSK, bytecode_RMTSK,
		
		bytecode_RUTE,

		bytecode_NUMARR, bytecode_STRARR,bytecode_NUMOFFST, bytecode_STROFFST,

		bytecode_JMPINVVAR,bytecode_JMPNOW, bytecode_WTFRVR,

		bytecode_NA
	};

	enum opcode : unsigned char
	{

		opcode_STOP,
		
		opcode_TO,
		opcode_CAST,
		opcode_PLACE,
		opcode_PLACEADR,
		opcode_LOAD,
		opcode_STORE,

		opcode_COPY,
		opcode_SAVE,
		
		opcode_SEND,
		opcode_STPRM,
		opcode_PRM,

		opcode_ADD,
		opcode_SUB,
		opcode_MUL,
		opcode_DIV,
		opcode_MOD,

		opcode_CMPRE,
		opcode_CMPREBYTES,
		opcode_GRTR,
		opcode_BITCMP,

		opcode_RETURN,

		opcode_FTS,

		opcode_ADR,
		opcode_ALLOC,
		opcode_DALLOC,
		
		opcode_INV,
		opcode_JMP,
		opcode_JMPIF,
		opcode_JMPINV

	};

	///<summary>The different types of runtime errors</summary>
	enum RunTimeError{
		RunTimeError_OK, RunTimeError_MSGFULL,
		RunTimeError_BADPARAMS, RunTimeError_ABORT, 
		RunTimeError_WAITINGFORWAR, RunTimeError_OHNO, 
		RunTimeError_HALT, RunTimeError_NA
	};

	struct SystemVar
	{
		SystemVar(std::string name, unsigned int var) : name(name), var(var) {}
		std::string name;
		unsigned int var;

	};

	const SystemVar SystemVars[]{ SystemVar("£null", 0),SystemVar("£clock", 1), SystemVar("£time", 2), SystemVar("£fps1", 3), SystemVar("£fps2", 4) };

	enum SystemMem{ SystemMem_CLOCK = 1, SystemMem_TIME, SystemMem_FPS1, SystemMem_FPS2, SystemMem_NA
	};

	enum RunTimeParams
	{
		RunTimeParams_ETIME, RunTimeParams_NA

	};
}
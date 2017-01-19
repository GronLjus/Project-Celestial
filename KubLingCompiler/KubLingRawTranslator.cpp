#include "KubLingRawTranslator.h"
#include "Message.h"
#include "GUIEnums.h"
#include "TaskEnums.h"
#include <cmath>

using namespace Logic;
using namespace Resources;

#pragma region operators

const unsigned int placeAdrLines = 6;

const unsigned int waitingLines = 3;
const unsigned int sendLines = 4 + placeAdrLines;
const unsigned int commonAddMessLines = 3;


const unsigned int addMessParLinesNO = 1 + commonAddMessLines;
const unsigned int addMessParLinesO = placeAdrLines + commonAddMessLines;
const unsigned int addMessStrParLines = 7 + placeAdrLines;
const unsigned int addMessStrParLinesNO = 7 ;


const unsigned int commonArrayLines = 9;
const unsigned int commonConditionalLines = 9 + placeAdrLines * 3;

const unsigned int addMessTempParLines = 3 + addMessParLinesNO;
const unsigned int castIToSLines = 34 + placeAdrLines*2;
const unsigned int commonMathLines = 4 + waitingLines + placeAdrLines*3;
const unsigned int setObjectMessLines = addMessParLinesO + sendLines;
const unsigned int setParamaterLines = 11 + placeAdrLines;

//3
void startWaiting(unsigned int varAdr, rawCode* currentCode)
{

	rawCode::line line;
	//Place the var in 11 so we can set it to 0
	line.code = opcode_PLACE;
	line.r1 = 0;
	line.type = 3;
	line.scale = varAdr;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Place 0 in c1
	line.code = opcode_PLACE;
	line.r1 = 0;
	line.type = 2;
	line.scale = 0;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Store c1 in the stack at adr i1
	line.code = opcode_STORE;
	line.r1 = 0;
	line.r2 = 0;
	line.type = 2;
	line.scale = 0;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;

}

//3
void finishWaiting(unsigned int varAdr, rawCode* currentCode)
{

	rawCode::line line;
	//Place the var in i1 so we can set it to 1
	line.code = opcode_PLACE;
	line.r1 = 0;
	line.type = 3;
	line.scale = varAdr;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Place 255 in c1
	line.code = opcode_PLACE;
	line.r1 = 0;
	line.type = 2;
	line.scale = 255;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Store c1 in the stack at adr i1
	line.code = opcode_STORE;
	line.r1 = 0;
	line.r2 = 0;
	line.type = 2;
	line.scale = 0;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
}

//6
void placeAdr(unsigned int var, unsigned int reg, unsigned int typ, runTimeVal &rtv, rawCode* currentCode)
{

	MemoryPool::offset offset = rtv.memory->GetOffset(var);
	rawCode::line line;

	if (offset.size > 0)
	{

		//Place the adress of the offsetvar in a1
		line.code = opcode_PLACE;
		line.r1 = 0;
		line.type = 3;
		line.scale = rtv.memory->GetVarAdr(offset.offsetVar);
		currentCode->code[currentCode->codeSize] = line;
		currentCode->codeSize++;
		//Load the value from adr a1 to i2
		line.code = opcode_LOAD;
		line.r1 = 1;
		line.r2 = 0;
		line.type = 0;
		currentCode->code[currentCode->codeSize] = line;
		currentCode->codeSize++;
		//Place the size of the offst in i1
		line.code = opcode_PLACE;
		line.r1 = 0;
		line.type = 0;
		line.scale = offset.size;
		currentCode->code[currentCode->codeSize] = line;
		currentCode->codeSize++;
		//Multiply i1 with i2 and store in i3
		line.code = opcode_MUL;
		line.r1 = 1;
		line.r2 = 0;
		line.r3 = 2;
		line.type = 0;
		currentCode->code[currentCode->codeSize] = line;
		currentCode->codeSize++;

	}
	else
	{

		//Place the size of the offst in i3
		line.code = opcode_PLACE;
		line.r1 = 2;
		line.type = 0;
		line.scale = 0;
		currentCode->code[currentCode->codeSize] = line;
		currentCode->codeSize++;

	}

	//Place the adr off the var in i1
	line.code = opcode_PLACE;
	line.r1 = 0;
	line.type = 0;
	line.scale = rtv.memory->GetVarAdr(var);
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Get the adr of i1 and i3 and place in reg
	line.code = opcode_PLACEADR;
	line.r1 = 0;
	line.r2 = 2;
	line.r3 = reg;
	line.type = typ;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;

}

//5
void sendMessageOut(Message& mess, runTimeVal &rtv, rawCode* currentCode)
{

	unsigned int retPar = mess.returnParam;

	if (mess.returnParam > 0)
	{

		//Place the return adr in a3
		placeAdr(mess.returnParam - 1, 2, 0, rtv, currentCode);

		retPar--;

	}

	rawCode::line line;
	//Place the destination in i1
	line.code = opcode_PLACE;
	line.r1 = 0;
	line.type = 0;
	line.scale = mess.destination;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Place the message in i2
	line.code = opcode_PLACE;
	line.r1 = 1;
	line.type = 0;
	line.scale = mess.mess;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Place the return var in c1
	line.code = opcode_PLACE;
	line.r1 = 0;
	line.type = 2;
	line.scale = retPar;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Send the message in i2 to i1 with the return adr in a3
	line.code = opcode_SEND;
	line.r1 = 1;
	line.r2 = 0;
	line.r3 = 2;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;

}

//3
void commonAddMessStack(unsigned int messOffset, unsigned int size, runTimeVal &rtv, rawCode* currentCode)
{

	rawCode::line line;
	//Place the size of the var in i2
	line.code = opcode_PLACE;
	line.r1 = 1;
	line.type = 0;
	line.scale = size;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Place the offset in i3
	line.code = opcode_PLACE;
	line.r1 = 2;
	line.type = 0;
	line.scale = messOffset;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Move data from the stack at adr a1 with size i2 to the currentmessage with offset i3
	line.code = opcode_STPRM;
	line.r1 = 0;
	line.r2 = 2;
	line.r3 = 1;
	line.type = 0;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;

}

//4
void addMessStackParamNO(unsigned int var, unsigned int messOffset, unsigned int size, runTimeVal &rtv, rawCode* currentCode)
{

	rawCode::line line;
	//Place the adress of the var in i1
	line.code = opcode_PLACE;
	line.r1 = 0;
	line.type = 0;
	line.scale = rtv.memory->GetVarAdr(var);
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	commonAddMessStack(messOffset, size, rtv, currentCode);
}

//9
void addMessStackParamO(unsigned int var, unsigned int messOffset, unsigned int size, runTimeVal &rtv, rawCode* currentCode)
{

	//Place the adress of the var in i1
	placeAdr(var, 0, 0, rtv, currentCode);
	commonAddMessStack(messOffset, size, rtv, currentCode);
}

//7 + placeadr
void addMessStackStringParamO(unsigned int var, unsigned int messOffset, unsigned int size, runTimeVal &rtv, rawCode* currentCode)
{

	rawCode::line line;
	//Place the adress of the var in a1
	placeAdr(var, 0, 0, rtv, currentCode);
	//Copy the value in a1 to i1
	//Load the size of the var in i2 from the stack at a1
	line.code = opcode_LOAD;
	line.r1 = 1;
	line.r2 = 0;
	line.type = 0;
	line.scale = size;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//4 in i3
	line.code = opcode_PLACE;
	line.r1 = 2;
	line.type = 0;
	line.scale = 4;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Add i3 to a1 and store in a1
	line.code = opcode_ADD;
	line.r1 = 0;
	line.r2 = 2;
	line.r3 = 0;
	line.type = 3;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Place the offset in i3
	line.code = opcode_PLACE;
	line.r1 = 2;
	line.type = 0;
	line.scale = messOffset;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Move data from the stack at adr a1 with size i2 to the currentmessage with offset i3
	line.code = opcode_STPRM;
	line.r1 = 0;
	line.r2 = 2;
	line.r3 = 1;
	line.type = 0;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Add i2 to i3 and store in i3
	line.code = opcode_ADD;
	line.r1 = 2;
	line.r2 = 1;
	line.r3 = 2;
	line.type = 0;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Add 0 to the message at offset i3
	line.code = opcode_PRM;
	line.r1 = 2;
	line.scale = 0;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;

}

//7
void addMessStackStringParamNO(unsigned int var, unsigned int messOffset, unsigned int size, runTimeVal &rtv, rawCode* currentCode)
{

	rawCode::line line;
	//Place the adress of the var in a1
	line.code = opcode_PLACE;
	line.r1 = 0;
	line.type = 3;
	line.scale = rtv.memory->GetVarAdr(var);
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Load the size of the var in i2 from the stack at a1
	line.code = opcode_LOAD;
	line.r1 = 1;
	line.r2 = 0;
	line.type = 0;
	line.scale = size;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Place the adress + 4 of the var in a1
	line.code = opcode_PLACE;
	line.r1 = 0;
	line.type = 3;
	line.scale = rtv.memory->GetVarAdr(var) + 4;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Place the offset in i3
	line.code = opcode_PLACE;
	line.r1 = 2;
	line.type = 0;
	line.scale = messOffset;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Move data from the stack at adr a1 with size i2 to the currentmessage with offset i3
	line.code = opcode_STPRM;
	line.r1 = 0;
	line.r2 = 2;
	line.r3 = 1;
	line.type = 0;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Add i2 to i3 and store in i3
	line.code = opcode_ADD;
	line.r1 = 2;
	line.r2 = 1;
	line.r3 = 2;
	line.type = 0;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Add 0 to the message at offset i3
	line.code = opcode_PRM;
	line.r1 = 2;
	line.scale = 0;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;

}

//7
void addMessTempStack(unsigned int messOffset, unsigned int data, unsigned int messLength, runTimeVal &rtv, rawCode* currentCode)
{

	rawCode::line line;
	//Place the adress of the tempoary var in a1
	line.code = opcode_PLACE;
	line.r1 = 0;
	line.type = 3;
	line.scale = rtv.memory->GetVarAdr(rtv.memory->GetMaxVar());
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Place the size of the char in i2
	line.code = opcode_PLACE;
	line.r1 = 1;
	line.type = 0;
	line.scale = 4;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Save the scalar in the stack at adr a1 with size i2
	line.code = opcode_SAVE;
	line.r1 = 0;
	line.r2 = 1;
	line.scale = data;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;

	addMessStackParamNO(rtv.memory->GetMaxVar(),messOffset, messLength, rtv, currentCode);

}

std::string getString(char* data)
{

	unsigned int size = 0;
	memcpy(&size, data, sizeof(unsigned int));
	return std::string(&data[sizeof(unsigned int)], size);

}

unsigned int getCode(KubLingCompiled** codes, unsigned int totalCodes, std::string name)
{

	unsigned int retVal = 0;

	for (unsigned int i = 0; i < totalCodes && retVal == 0; i++)
	{

		if (codes[i]->GetName() == name)
		{

			retVal = i + 1;

		}
	}

	if (retVal > 0)
	{

		return retVal - 1;

	}

	return 0;

}

unsigned int getHeapVal(runTimeVal &rtv, std::string name, bool &inited)
{

	unsigned int retVal = 0;

	for (unsigned int i = 0; i < rtv.heap->GetHighest() && retVal == 0; i++)
	{

		std::string localName = rtv.heap->GetValue(i).name;

		if (localName == name)
		{

			retVal = i + 1;

		}
	}

	inited = retVal > 0;

	if (inited)
	{

		return retVal - 1;

	}

	heapVar var;
	var.name = name;
	var.var = rtv.heap->Add(var);

	return var.var;

}

void commonLoad(unsigned int returnVar, runTimeVal &rtv, unsigned char op)
{

	if (rtv.memory->GetVarAdr(returnVar) <= rtv.memory->GetMaxStack())
	{

		rtv.memory->AddVariable(returnVar, 
			op == Logic::bytecode_CASTTSTR ||
			op == Logic::bytecode_SUMSTR ? 64 : 
			4);

	}
}

void commonOffset(unsigned int arr, unsigned int var, unsigned int memSize, runTimeVal &rtv)
{

	rtv.memory->AddOffset(arr, var, memSize);

}

//26
void convertIntToString(unsigned int var, unsigned int target, unsigned int offset, runTimeVal &rtv, rawCode* currentCode)
{


	//The source in a1, the current target in a2 and the first target in a3

	unsigned int code = 34;
	rawCode::line line;
	//Place the adress of the number in a1
	placeAdr(var, 0, 0, rtv, currentCode);
	//Place the adress of the target var in a3
	placeAdr(target, 2, 0, rtv, currentCode);

	//Move a3 to i1
	line.code = opcode_TO;
	line.r1 = 0;
	line.r2 = 2;
	line.type = 4;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Move i1 to a2
	line.code = opcode_TO;
	line.r1 = 1;
	line.r2 = 0;
	line.type = 3;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;

	//Place 0 in c3
	line.code = opcode_PLACE;
	line.r1 = 0;
	line.type = 2;
	line.scale = 0;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Place 4 in i2
	line.code = opcode_PLACE;
	line.r1 = 1;
	line.type = 0;
	line.scale = 4;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Add i2 to a2 and store in a2
	line.code = opcode_ADD;
	line.r1 = 1;
	line.r2 = 1;
	line.r3 = 1;
	line.type = 3;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;

	//48 in c1 and 1 in i2, the number of digits in c3

	//Place the base in i3
	line.code = opcode_PLACE;
	line.r1 = 2;
	line.type = 0;
	line.scale = 10;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;

	//Place 48 in c1
	line.code = opcode_PLACE;
	line.r1 = 0;
	line.type = 2;
	line.scale = 48;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Load the value on stack a1 to i1
	line.code = opcode_LOAD;
	line.r1 = 0;
	line.r2 = 0;
	line.type = 0;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;

	unsigned int castStart = offset + currentCode->codeSize;

	//Cast the int

	//Cast each digit --12--
	//Set the remainder of i1 / i3 in i2
	line.code = opcode_MOD;
	line.r1 = 0;
	line.r2 = 2;
	line.r3 = 1;
	line.type = 0;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Cast the value in i2 to c2
	line.code = opcode_CAST;
	line.r1 = 1;
	line.r2 = 1;
	line.type = 2;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Add c1 to c2 and store in c2
	line.code = opcode_ADD;
	line.r1 = 0;
	line.r2 = 1;
	line.r2 = 1;
	line.type = 2;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Save the value in c2 to the stack at adr a2
	line.code = opcode_STORE;
	line.r1 = 1;
	line.r2 = 1;
	line.type = 2;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Place 1 in i2
	line.code = opcode_PLACE;
	line.r1 = 1;
	line.type = 0;
	line.scale = 1;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;

	//Add i2 to a2 and store in a2
	line.code = opcode_ADD;
	line.r1 = 1;
	line.r2 = 1;
	line.r3 = 1;
	line.type = 3;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Set the fraction of i1 / i3 in i1
	line.code = opcode_DIV;
	line.r1 = 0;
	line.r2 = 2;
	line.r3 = 0;
	line.type = 0;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Place 1 in c2
	line.code = opcode_PLACE;
	line.r1 = 1;
	line.type = 2;
	line.scale = 1;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Add c2 to c3 and store in c3
	line.code = opcode_ADD;
	line.r1 = 2;
	line.r2 = 1;
	line.r3 = 2;
	line.type = 2;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Move i1 to c2
	line.code = opcode_TO;
	line.r1 = 1;
	line.r2 = 0;
	line.type = 2;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;

	//Place the start of the digit loop in i2
	line.code = opcode_PLACE;
	line.r1 = 1;
	line.type = 0;
	line.scale = castStart;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Jump back to i2 if c2 != 0
	line.code = opcode_JMPIF;
	line.r1 = 1;
	line.r2 = 1;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;


	//Start saving the new string --2--
	//Cast c3 to i1
	line.code = opcode_TO;
	line.r1 = 0;
	line.r2 = 2;
	line.type = 0;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Store i1 at a3
	line.code = opcode_STORE;
	line.r1 = 0;
	line.r2 = 2;
	line.type = 0;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;


	//Reverse the sequence --13--
	//Place 4 in i1
	line.code = opcode_PLACE;
	line.r1 = 0;
	line.type = 0;
	line.scale = 4;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Add i1 to a3 and store in a3
	line.code = opcode_ADD;
	line.r1 = 2;
	line.r2 = 0;
	line.r3 = 2;
	line.type = 3;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;


	//Place 1 in i2
	line.code = opcode_PLACE;
	line.r1 = 1;
	line.type = 0;
	line.scale = 1;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Subtract i2 from a2 and store in a2
	line.code = opcode_SUB;
	line.r1 = 1;
	line.r2 = 1;
	line.r3 = 1;
	line.type = 3;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;

	unsigned int reverseStart = offset + currentCode->codeSize + 1;
	//Place The reversestart in i3
	line.code = opcode_PLACE;
	line.r1 = 2;
	line.type = 0;
	line.scale = reverseStart ;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Load the value from the stack at adr a2 and store it in c1
	line.code = opcode_LOAD;
	line.r1 = 0;
	line.r2 = 1;
	line.type = 2;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Load the value from the stack at adr a3 and store it in c2
	line.code = opcode_LOAD;
	line.r1 = 1;
	line.r2 = 2;
	line.type = 2;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Save the value in c1 to the stack at adr a3
	line.code = opcode_STORE;
	line.r1 = 0;
	line.r2 = 2;
	line.type = 2;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Save the value in c2 to the stack at adr a2
	line.code = opcode_STORE;
	line.r1 = 1;
	line.r2 = 1;
	line.type = 2;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;

	//Add i2 to a3 and store in a3
	line.code = opcode_ADD;
	line.r1 = 2;
	line.r2 = 1;
	line.r3 = 2;
	line.type = 3;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Subtract i2 from a2 and store in a2
	line.code = opcode_SUB;
	line.r1 = 1;
	line.r2 = 1;
	line.r3 = 1;
	line.type = 3;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Compare a3 to a2 and store the result in c3
	line.code = opcode_GRTR;
	line.r1 = 2;
	line.r2 = 1;
	line.r3 = 2;
	line.type = 3;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Jump to the start of the loop if c3 == 0
	line.code = opcode_JMPINV;
	line.r1 = 2;
	line.r2 = 2;
	line.type = 2;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;

}

//10
void performArithemitc(unsigned int var, unsigned int var2, unsigned int returnVar, runTimeVal &rtv, opcode code, unsigned long long type, rawCode* raw)
{

	rawCode::line line;
	//Place the adress of the var2 in a1
	placeAdr(var2, 0, 0, rtv, raw);
	//Place the adress of the var in a2
	placeAdr(var, 1, 0, rtv, raw);
	//Place the adress of the returnvar in a3
	placeAdr(returnVar, 2, 0, rtv, raw);

	//Load the number to r3 from the adress in a1
	line.code = opcode_LOAD;
	line.r1 = 2;
	line.r2 = 0;
	line.type = type;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Load the number to r2 from the adress in a2
	line.code = opcode_LOAD;
	line.r1 = 1;
	line.r2 = 1;
	line.type = type;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Perform the operation
	line.code = code;
	line.r1 = 1;
	line.r2 = 2;
	line.r3 = 0;
	line.type = type;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Store the number in r1 to adr a3
	line.code = opcode_STORE;
	line.r1 = 0;
	line.r2 = 2;
	line.type = opcode_CMPRE == code || opcode_GRTR == code ? 2 : type;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;

	finishWaiting(returnVar, raw);

}

//7
void setCommon(unsigned int var, unsigned int var2, unsigned int message, runTimeVal &rtv, rawCode* currentCode)
{

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = message;
	mess.returnParam = var;

	addMessStackParamO(var2 - 1, 0, 4, rtv, currentCode);
	sendMessageOut(mess, rtv, currentCode);

}

//9
void commonConditional(unsigned int var, unsigned int var2, unsigned int returnVal, unsigned int cmpVal, runTimeVal &rtv, rawCode* raw)
{

	//Place the adress of the var2 in a1
	placeAdr(var2, 0, 0, rtv, raw);
	//Place the adress of the var in a2
	placeAdr(var, 1, 0, rtv, raw);
	//Place the adress of the returnvar in a3
	placeAdr(returnVal, 2, 0, rtv, raw);

	rawCode::line line;
	//Place -1 in c3
	line.code = opcode_PLACE;
	line.r1 = 1;
	line.type = 2;
	line.scale = -1;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Load a1 in c1
	line.code = opcode_LOAD;
	line.r1 = 0;
	line.r2 = 0;
	line.type = 2;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Compare c1 to c3 and store in c1
	line.code = opcode_CMPRE;
	line.r1 = 0;
	line.r2 = 2;
	line.r3 = 0;
	line.type = 2;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Load a2 in c2
	line.code = opcode_LOAD;
	line.r1 = 1;
	line.r2 = 1;
	line.type = 2;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Compare c2 to c3 and store in c2
	line.code = opcode_CMPRE;
	line.r1 = 1;
	line.r2 = 2;
	line.r3 = 1;
	line.type = 2;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Add c1 to c2 and store in c3
	line.code = opcode_ADD;
	line.r1 = 0;
	line.r2 = 1;
	line.r3 = 2;
	line.type = 2;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Place the compare-value in c2
	line.code = opcode_PLACE;
	line.r1 = 1;
	line.type = 2;
	line.scale = cmpVal;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Compare c2 to c3 and store in c1
	line.code = opcode_GRTR;
	line.r1 = 1;
	line.r2 = 2;
	line.r3 = 0;
	line.type = 2;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Store c1 at adr a3
	line.code = opcode_STORE;
	line.r1 = 0;
	line.r2 = 2;
	line.type = 2;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;

}

//9 for 'n' and 'f' 12 for 's' 
void commonPar(unsigned int var, unsigned int scriptValue, unsigned int offset, unsigned char type, runTimeVal &rtv, rawCode* currentCode)
{

	unsigned char currentPar = rtv.byteCodes[scriptValue]->GetCurrentParam(type);
	rtv.byteCodes[scriptValue]->UpParam(type);

	rawCode::line line;
	//Place the adress of the var in a1
	placeAdr(var - 1,0 , 0, rtv, currentCode);
	//Place the adress of the current par in a2
	line.code = opcode_PLACE;
	line.r1 = 1;
	line.type = 3;
	line.scale = rtv.byteCodes[scriptValue]->GetAdr(currentPar, type);
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Place the memoffset of the current code to run in i3
	rtv.byteCodes[rtv.current]->AddMemOffsetPlaceHolder(offset + currentCode->codeSize);
	line.code = opcode_PLACE;
	line.r1 = 1;
	line.type = 0;
	line.scale = 0;//PlaceHolder
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Add i5 to a1 and store it in a1
	line.code = opcode_ADD;
	line.r1 = 0;
	line.r2 = 4;
	line.r3 = 0;
	line.type = 3;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Add i3 to a2 and store it in a2
	line.code = opcode_ADD;
	line.r1 = 1;
	line.r2 = 2;
	line.r3 = 1;
	line.type = 3;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;

	if (type == 's')
	{

		//Place 4 in i5
		line.code = opcode_PLACE;
		line.r1 = 4;
		line.type = 3;
		line.scale = 4;
		currentCode->code[currentCode->codeSize] = line;
		currentCode->codeSize++;
		//Load the first 4 bytes from the stack at a1 to i3
		line.code = opcode_LOAD;
		line.r1 = 2;
		line.type = 0;
		currentCode->code[currentCode->codeSize] = line;
		currentCode->codeSize++;
		//Add i5 to i3 and store it in i3
		line.code = opcode_ADD;
		line.r1 = 2;
		line.r2 = 4;
		line.r3 = 2;
		line.type = 0;
		currentCode->code[currentCode->codeSize] = line;
		currentCode->codeSize++;

	}
	else
	{

		//Place the size in i3
		line.code = opcode_PLACE;
		line.r1 = 2;
		line.type = 0;
		line.scale = 4;
		currentCode->code[currentCode->codeSize] = line;
		currentCode->codeSize++;

	}

	//Place 0 in i5, our offsets are precalculated
	line.code = opcode_PLACE;
	line.r1 = 4;
	line.type = 3;
	line.scale = 0;//PlaceHolder
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Copy the value from the stack at adr a1 to the stack at adr a2
	line.code = opcode_COPY;
	line.r1 = 1;
	line.r2 = 0;
	line.type = 0;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;

	//Place the memoffset of the current code to run in i5
	rtv.byteCodes[rtv.current]->AddMemOffsetPlaceHolder(offset + currentCode->codeSize);
	line.code = opcode_PLACE;
	line.r1 = 4;
	line.type = 3;
	line.scale = 0;//PlaceHolder
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;

}

//9
void commonArray(unsigned int var, unsigned int returnVar, unsigned int memSize, runTimeVal &rtv, rawCode* currentCode)
{

	bool init;
	std::string arrName = "¤array" + std::to_string(rtv.heap->GetHighest());
	unsigned int arrayVar = getHeapVal(rtv, arrName, init);

	rtv.memory->AddVariable(returnVar, rtv.memory->GetMaxStack() + arrayVar, 4);

	rawCode::line line;
	//Place the adress of the var in a1
	line.code = opcode_PLACE;
	line.r1 = 0;
	line.type = 3;
	line.scale = rtv.memory->GetVarAdr(var);
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Load the value from a1 and place it in i1
	line.code = opcode_LOAD;
	line.r1 = 0;
	line.r2 = 0;
	line.type = 0;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Place the size of each element in i2
	line.code = opcode_PLACE;
	line.r1 = 1;
	line.type = 0;
	line.scale = memSize;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Multiply i1 with i2 and store in i3
	line.code = opcode_MUL;
	line.r1 = 0;
	line.r2 = 1;
	line.r3 = 2;
	line.type = 0;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Place the arrayvar in i2
	line.code = opcode_PLACE;
	line.r1 = 1;
	line.type = 0;
	line.scale = arrayVar;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;
	//Allocate the var in i2 with size i3
	line.code = opcode_ALLOC;
	line.r1 = 1;
	line.r2 = 2;
	currentCode->code[currentCode->codeSize] = line;
	currentCode->codeSize++;

	finishWaiting(returnVar, currentCode);

}


RunTimeError AcceptInputOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesNO + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_LOCKINPUT;

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));

	mess.returnParam = var;

	addMessTempStack(0, 1, 1, rtv, raw);
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError AcceptKeyboardOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesNO + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_LOCKKEYS;

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	mess.returnParam = var;

	addMessTempStack(0, 1, 1, rtv, raw);
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError AcceptMouseOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessTempParLines + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_LOCKMOUSE;

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	mess.returnParam = var;

	addMessTempStack(0, 1, 1, rtv, raw);
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError Add2DOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO * 2 + sendLines + 1 + 2;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.destination = MessageSource_GUIENTITIES;
	mess.type = MessageType_GUIENTITIES;
	mess.mess = GUIMess_ADDCHILD;
	mess.returnParam = 0;

	//The child object
	unsigned int var = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	addMessStackParamO(var - 1, 4, 4, rtv, raw);

	//The parent object
	var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	addMessStackParamO(var - 1, 0, 4, rtv, raw);


	sendMessageOut(mess, rtv, raw);

	rawCode::line line;
	//Place a halt message on i1
	line.code = opcode_PLACE;
	line.r1 = 0;
	line.type = 0;
	line.scale = RunTimeError_HALT;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Return i1
	line.code = opcode_RETURN;
	line.r1 = 0;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;


	return RunTimeError_OK;

}

RunTimeError AddMeshOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var1 = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));

	Message mess;
	mess.destination = MessageSource_ENTITIES;
	mess.type = MessageType_ENTITIES;
	mess.mess = GameBoardMess_ADDMESH;
	mess.returnParam = 0;
	addMessStackParamO(var1 - 1, 0, 4, rtv, raw);
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError AddObjectOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO + sendLines + waitingLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var1 = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));

	Message mess;
	mess.destination = MessageSource_ENTITIES;
	mess.type = MessageType_ENTITIES;
	mess.mess = GameBoardMess_ADDOBJECT;
	mess.returnParam = 0;
	addMessStackParamO(var1 - 1, 0, 4, rtv, raw);

	sendMessageOut(mess, rtv, raw);
	startWaiting(returnVar - 1, raw);

	return RunTimeError_OK;

}

RunTimeError AddRouteObjectOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 16)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO * 4 + sendLines + (returnVar > 0 ? waitingLines : 0);
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.returnParam = returnVar;
	mess.destination = MessageSource_ENTITIES;
	mess.type = MessageType_ENTITIES;
	mess.mess = GameBoardMess_ADDROUTE;

	unsigned int var1 = (params[12] | ((int)params[13] << 8) | ((int)params[14] << 16) | ((int)params[15] << 24));
	addMessStackParamO(var1 - 1, 12, 4, rtv, raw);
	var1 = (params[8] | ((int)params[9] << 8) | ((int)params[10] << 16) | ((int)params[11] << 24));
	addMessStackParamO(var1 - 1, 8, 4, rtv, raw);
	var1 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	addMessStackParamO(var1 - 1, 4, 4, rtv, raw);
	var1 = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	addMessStackParamO(var1 - 1, 0, 4, rtv, raw);

	if (returnVar > 0)
	{

		startWaiting(returnVar - 1, raw);

	}

	sendMessageOut(mess, rtv, raw);
	return RunTimeError_OK;

}

RunTimeError AddTaskParamFloatOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO + addMessTempParLines + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	addMessTempStack( 0, 1, 4, rtv, raw);
	unsigned int var1 = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	addMessStackParamO(var1 - 1, 4, 4, rtv, raw);
	Message mess;
	mess.returnParam = 0;
	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_ADDTASKPAR;
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError AddTaskParamNumOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO + addMessTempParLines + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	addMessTempStack(0, 0, 4, rtv, raw);
	unsigned int var1 = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	addMessStackParamO(var1 - 1, 4, 4, rtv, raw);
	Message mess;
	mess.returnParam = 0;
	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_ADDTASKPAR;
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError AddTaskParamStrOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessStrParLines + addMessTempParLines + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	addMessTempStack(0, 2, 4, rtv, raw);
	unsigned int var1 = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	addMessStackStringParamO(var1 - 1, 4, 4, rtv, raw);
	Message mess;
	mess.returnParam = 0;
	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_ADDTASKPAR;
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError AddTriggerOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 12)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO*2 + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var1 = (params[8] | ((int)params[9] << 8) | ((int)params[10] << 16) | ((int)params[11] << 24));
	addMessStackParamO(var1 - 1, 4, 4, rtv, raw);
	var1 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	addMessStackParamO(var1 - 1, 0, 4, rtv, raw);
	var1 = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	Message mess;
	mess.returnParam = var1;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_ADDKEYSCRPT;
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError AddTextLineOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessStrParLines + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_APPENDTEXTLINE;

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	mess.returnParam = var;

	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));

	addMessStackStringParamO(var2 - 1, 0, 4, rtv, raw);
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError AndOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = commonConditionalLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));

	commonConditional(var - 1, var2 - 1, returnVar - 1, -1, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError AppendTextOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessStrParLines + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_APPENDTEXT;

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	mess.returnParam = var;

	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));

	addMessStackStringParamO(var2 - 1, 0, 4, rtv, raw);
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError ArrayNumOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}


	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	raw->maxLines = commonArrayLines;
	raw->code = new rawCode::line[raw->maxLines];

	commonArray(var - 1, returnVar - 1, 4, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError ArrayStrOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}


	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	raw->maxLines = commonArrayLines;
	raw->code = new rawCode::line[raw->maxLines];

	commonArray(var - 1, returnVar - 1, 64, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError CastFloatOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = 3 + placeAdrLines*2 + waitingLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));

	rawCode::line line;
	//Place the adress of the var in a1
	placeAdr(var - 1, 0, 0, rtv, raw);
	//Place the adress of the return var in a3
	placeAdr(returnVar - 1, 2, 0, rtv, raw);

	//Load the number to i2 from the adress in a1
	line.code = opcode_LOAD;
	line.r1 = 1;
	line.r2 = 0;
	line.type = 0;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Cast the number in i2 to f1
	line.code = opcode_CAST;
	line.r1 = 0;
	line.r2 = 1;
	line.type = 1;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Store the value in f1 to the stack at adr a3
	line.code = opcode_STORE;
	line.r1 = 0;
	line.r2 = 2;
	line.type = 1;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;

	finishWaiting(returnVar - 1, raw);
	return RunTimeError_OK;

}

RunTimeError CastNumToStringOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = castIToSLines + waitingLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	convertIntToString(var - 1, returnVar - 1, offset, rtv, raw);

	finishWaiting(returnVar - 1, raw);

	return RunTimeError_OK;

}

RunTimeError ClearBoardOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO + waitingLines + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var1 = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));

	Message mess;
	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_CLEARBOARD;
	mess.returnParam = 0;
	addMessStackParamO(var1 - 1, 0, 4, rtv, raw);

	sendMessageOut(mess, rtv, raw);
	startWaiting(returnVar - 1, raw);

	return RunTimeError_OK;

}

RunTimeError ClearTrackingOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	raw->maxLines = sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.destination = MessageSource_ENTITIES;
	mess.type = MessageType_ENTITIES;
	mess.mess = GameBoardMess_CLEARTRACK;
	mess.returnParam = 0;
	
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError DecrementLayerOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_DECREMENTLAYER;

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	mess.returnParam = var;

	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError DivFloatOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = commonMathLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	performArithemitc(var - 1, var2 - 1, returnVar - 1, rtv, opcode_DIV, 1, raw);

	return RunTimeError_OK;

}

RunTimeError DivVarOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = commonMathLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	performArithemitc(var - 1, 
		var2 - 1, 
		returnVar - 1, rtv,
		opcode_DIV, 0, raw);

	return RunTimeError_OK;

}

RunTimeError ExportNumOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 9)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int size = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	std::string name = getString((char*)params);
	unsigned int sValue = (params[size + 4] | ((int)params[size + 4 + 1] << 8) | ((int)params[size + 4 + 2] << 16) | ((int)params[size + 4 + 3] << 24));
	bool init = false;
	unsigned int hVar = getHeapVal(rtv, name, init);

	raw->maxLines = 5 + placeAdrLines;
	raw->code = new rawCode::line[raw->maxLines];

	rawCode::line line;
	//Place the const adr in a1
	placeAdr(sValue - 1, 0, 0, rtv, raw);
	//Place 4 in i2
	line.code = opcode_PLACE;
	line.r1 = 1;
	line.type = 0;
	line.scale = 4;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Place the heap var in i1
	line.code = opcode_PLACE;
	line.r1 = 0;
	line.type = 0;
	line.scale = hVar;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Deallocate the adr at i1 with the size i2
	line.code = opcode_DALLOC;
	line.r1 = 0;
	line.r2 = 1;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;

	//Allocate and adress in the heap for the var in i1 with the size i2 and store in a3
	line.code = opcode_ALLOC;
	line.r1 = 0;
	line.r2 = 1;
	line.r3 = 2;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;

	//Copy i2 bytes at a1 to a3
	line.code = opcode_COPY;
	line.r1 = 2;
	line.r2 = 0;
	line.r3 = 1;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;

	return RunTimeError_OK;

}

RunTimeError ExportStrOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 9)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int size = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	std::string name = getString((char*)params);
	unsigned int sValue = (params[size+4] | ((int)params[size + 4 + 1] << 8) | ((int)params[size + 4 + 2] << 16) | ((int)params[size + 4 + 3] << 24));
	bool init = false;
	unsigned int hVar = getHeapVal(rtv, name, init);

	raw->maxLines = 9 + placeAdrLines;
	raw->code = new rawCode::line[raw->maxLines];

	rawCode::line line;
	//Place the stack var in a2
	placeAdr(sValue - 1, 1, 0, rtv, raw);
	//Place 64 in i1
	line.code = opcode_PLACE;
	line.r1 = 0;
	line.type = 0;
	line.scale = 64;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Place the heap var in i2
	line.code = opcode_PLACE;
	line.r1 = 1;
	line.type = 0;
	line.scale = hVar;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Deallocate the adr at i2 with the size i1
	line.code = opcode_DALLOC;
	line.r1 = 1;
	line.r2 = 0;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Allocate and adress in the heap for the var in i2 with the size i1 and store in i3
	line.code = opcode_ALLOC;
	line.r1 = 1;
	line.r2 = 0;
	line.r3 = 2;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;

	//Place 4 in c1
	line.code = opcode_PLACE;
	line.r1 = 0;
	line.type = 2;
	line.scale = 4;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Load from i2 to c2
	line.code = opcode_LOAD;
	line.r1 = 1;
	line.r2 = 1;
	line.type = 2;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Add c1 to c2 and store in c1
	line.code = opcode_ADD;
	line.r1 = 0;
	line.r1 = 1;
	line.r1 = 0;
	line.type = 2;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Transfer c1 to i1
	line.code = opcode_TO;
	line.r1 = 0;
	line.r2 = 0;
	line.type = 2;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Copy i1 bytes from a2 to a3
	line.code = opcode_COPY;
	line.r1 = 2;
	line.r2 = 1;
	line.r3 = 0;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;

	return RunTimeError_OK;

}

RunTimeError FloatEqlVarOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = commonMathLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	performArithemitc(var - 1, 
		var2 - 1,
		returnVar - 1, rtv,
		opcode_CMPRE, 1, raw);

	return RunTimeError_OK;

}

RunTimeError FocusUIOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));


	Message mess;
	mess.destination = MessageSource_GUIENTITIES;
	mess.type = MessageType_GUIENTITIES;
	mess.mess = GUIMess_FOCUS;
	mess.returnParam = 0;
	addMessStackParamO(var - 1, 0, 4, rtv, raw);
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;


}

RunTimeError GetScreenHeightOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	raw->maxLines = addMessTempParLines + waitingLines + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.returnParam = returnVar;
	mess.destination = MessageSource_GRAPHICS;
	mess.type = MessageType_GRAPHICS;
	mess.mess = GraphicMess_GETSCREEN;
	mess.returnParam = 0;

	addMessTempStack(0, 0, 4, rtv, raw);
	sendMessageOut(mess, rtv, raw);
	startWaiting(returnVar - 1, raw);

	return RunTimeError_OK;

}

RunTimeError GetScreenWidthOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	raw->maxLines = addMessTempParLines + waitingLines + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.returnParam = returnVar;
	mess.destination = MessageSource_GRAPHICS;
	mess.type = MessageType_GRAPHICS;
	mess.mess = GraphicMess_GETSCREEN;
	mess.returnParam = 0;

	addMessTempStack(0, 0, 4, rtv, raw);
	startWaiting(returnVar - 1, raw);
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError GlueObjectOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 20)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO * 5 + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.destination = MessageSource_ENTITIES;
	mess.type = MessageType_ENTITIES;
	mess.mess = GameBoardMess_GLUEOBJECT;
	mess.returnParam = 0;
	

	unsigned int zVar = (params[16] | ((int)params[17] << 8) | ((int)params[18] << 16) | ((int)params[19] << 24));
	addMessStackParamO(zVar - 1, 16, 4, rtv, raw);

	unsigned int yVar = (params[12] | ((int)params[13] << 8) | ((int)params[14] << 16) | ((int)params[15] << 24));
	addMessStackParamO(yVar - 1, 12, 4, rtv, raw);

	//The sub position
	unsigned int xVar = (params[8] | ((int)params[9] << 8) | ((int)params[10] << 16) | ((int)params[11] << 24));
	addMessStackParamO(xVar - 1, 8, 4, rtv, raw);


	//The child object
	unsigned int var = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	addMessStackParamO(var - 1, 4, 4, rtv, raw);

	//The parent object
	var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	addMessStackParamO(var - 1, 0, 4, rtv, raw);

	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError HideCursorOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	raw->maxLines = sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.destination = MessageSource_SYSTEM;
	mess.type = MessageType_SYSTEM;
	mess.mess = SystemMess_HIDECURSOR;
	mess.returnParam = 0;
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError HideOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_HIDE;
	mess.returnParam = var;

	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError HockTrackingOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	raw->maxLines = sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.destination = MessageSource_ENTITIES;
	mess.type = MessageType_ENTITIES;
	mess.mess = GameBoardMess_HOOKTRACK;
	mess.returnParam = 0;

	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError IgnoreInputOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = sendLines + addMessTempParLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_LOCKINPUT;

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	mess.returnParam = var;

	addMessTempStack(0, 0, 1, rtv, raw);
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError IgnoreKeyboardOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessTempParLines + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_LOCKKEYS;

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	mess.returnParam = var;

	addMessTempStack(0, 0, 1, rtv, raw);
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError IgnoreMouseOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessTempParLines + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_LOCKKEYS;

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	mess.returnParam = var;

	addMessTempStack(0, 0, 1, rtv, raw);
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError ImportNumOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 5)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = 5 + waitingLines + placeAdrLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int size = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	std::string name = getString((char*)params);

	bool init = false;
	unsigned int hVar = getHeapVal(rtv, name, init);

	rawCode::line line;
	//Place the adr of the stack var in a1
	placeAdr(returnVar - 1, 0, 0, rtv, raw);
	//Place the adr of the var in i1
	line.code = opcode_PLACE;
	line.r1 = 0;
	line.type = 0;
	line.scale = hVar + rtv.memory->GetMaxStack();
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Place 0 in i2
	line.code = opcode_PLACE;
	line.r1 = 1;
	line.type = 0;
	line.scale = 0;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Place the heap adr at i1 vith at a3
	line.code = opcode_PLACEADR;
	line.r1 = 0;
	line.r2 = 1;
	line.r3 = 2;
	line.type = 0;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Place 4 in i2
	line.code = opcode_PLACE;
	line.r1 = 1;
	line.type = 0;
	line.scale = 4;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Copy i2 bytes from a3 to a1
	line.code = opcode_COPY;
	line.r1 = 0;
	line.r2 = 2;
	line.r3 = 1;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;

	finishWaiting(returnVar - 1,raw);
	return RunTimeError_OK;

}

RunTimeError ImportStrOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 5)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = 7 + waitingLines + placeAdrLines;
	raw->code = new rawCode::line[raw->maxLines];

	rtv.memory->AddVariable(returnVar - 1, 64);
	unsigned int size = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	std::string name = getString((char*)params);

	bool init = false;
	unsigned int hVar = getHeapVal(rtv, name, init);

	rawCode::line line;
	//Place the adr of the stack var in 12
	placeAdr(returnVar - 1, 1, 0, rtv, raw);
	//Place the adr of the var in i1
	line.code = opcode_PLACE;
	line.r1 = 0;
	line.type = 0;
	line.scale = hVar + rtv.memory->GetMaxStack();
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	raw->codeSize++;
	//Place 0 in i2
	line.code = opcode_PLACE;
	line.r1 = 1;
	line.type = 0;
	line.scale = 0;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Place the heap adr at i1 vith at a3
	line.code = opcode_PLACEADR;
	line.r1 = 0;
	line.r2 = 1;
	line.r3 = 2;
	line.type = 0;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Place 4 in i2
	line.code = opcode_PLACE;
	line.r1 = 1;
	line.type = 0;
	line.scale = 4;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Load the value from a3 to i1
	line.code = opcode_LOAD;
	line.r1 = 0;
	line.r2 = 2;
	line.type = 0;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Add i1 to i2 and store it in 11
	line.code = opcode_ADD;
	line.r1 = 1;
	line.r2 = 0;
	line.r2 = 1;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;

	//Copy i1 bytes from i3 to i2
	line.code = opcode_COPY;
	line.r1 = 1;
	line.r2 = 2;
	line.r3 = 0;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;

	finishWaiting(returnVar - 1, raw);
	return RunTimeError_OK;

}

RunTimeError IncrementLayerOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_INCREMENTLAYER;

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	mess.returnParam = var;

	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError JumpNow(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = 2;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int location = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));

	rawCode::line line;
	//Place the location to jump to in i1
	rtv.jmpPlaceHolders[byteLine] = offset + raw->codeSize + 1;
	line.code = opcode_PLACE;
	line.r1 = 0;
	line.type = 0;
	line.scale = location;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Jump to i1 
	line.code = opcode_JMP;
	line.r1 = 0;
	line.r2 = 0;
	line.type = 0;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;

	return RunTimeError_OK;


}

RunTimeError JumpInv(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = 3 + placeAdrLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int location = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));

	rawCode::line line;
	//Place the adr of the var in a1
	/*line.code = opcode_PLACE;
	line.r1 = 0;
	line.type = 0;
	line.scale = rtv.memory->GetVarAdr(var - 1);
	raw->code[raw->codeSize] = line;
	raw->codeSize++;*/
	placeAdr(var - 1, 0, 0, rtv, raw);
	//Load value from the stack with adr a1 and store it in c1
	line.code = opcode_LOAD;
	line.r1 = 0;
	line.r2 = 0;
	line.type = 2;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Place the location to jump to in i1
	rtv.jmpPlaceHolders[byteLine] = offset + raw->codeSize + 1;
	line.code = opcode_PLACE;
	line.r1 = 0;
	line.type = 0;
	line.scale = location;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Jump to i1 if c1 != 0 
	line.code = opcode_JMPINV;
	line.r1 = 0;
	line.r2 = 0;
	line.scale = location;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;

	return RunTimeError_OK;

}

RunTimeError LinkDBGOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var1 = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));

	Message mess;
	mess.destination = MessageSource_MASTER;
	mess.type = MessageType_SYSTEM;
	mess.mess = EventMess_LNKDBG;
	mess.returnParam = 0;

	addMessStackParamO(var1 - 1, 0, 4, rtv, raw);
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError LinkTargetOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = setObjectMessLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));

	setCommon(var, var2, ObjectMess_SETSCRPTTRGT, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError LoadCameraOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	raw->maxLines = sendLines + waitingLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.returnParam = returnVar;
	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_LOADCAMERA;

	sendMessageOut(mess, rtv, raw);
	startWaiting(returnVar - 1, raw);

	return RunTimeError_OK;

}

RunTimeError LoadGameBoardOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned char hasMore = paramSize == 8 ? 1 : 0;

	raw->maxLines = sendLines + addMessParLinesO * 2 + addMessTempParLines + waitingLines;
	raw->code = new rawCode::line[raw->maxLines];


	Message mess;
	mess.returnParam = returnVar;
	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_LOADGAMEBOARD;

	if (paramSize == 8)
	{

		unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
		addMessStackParamO(var2 - 1,5, 4, rtv, raw);

	}

	addMessStackParamO(var - 1, 0, 4, rtv, raw);
	addMessTempStack(4, hasMore, 1, rtv, raw);

	startWaiting(returnVar - 1, raw);
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError LoadImageOperator(rawCode* raw,unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if(paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessTempParLines + addMessParLinesO + waitingLines + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var1 = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	Message mess;
	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_LOADGUI;
	int enu = GUIObjects_IMAGE;
	mess.returnParam = returnVar;
	addMessTempStack(0, enu, 4, rtv, raw);
	addMessStackStringParamO(var1 - 1, 4, 4, rtv, raw);

	sendMessageOut(mess, rtv, raw);
	startWaiting(returnVar - 1, raw);

	return RunTimeError_OK;

}

RunTimeError LoadLightOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 12)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO * 3 + waitingLines + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var1 = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	unsigned int var3 = (params[8] | ((int)params[9] << 8) | ((int)params[10] << 16) | ((int)params[11] << 24));
	Message mess;

	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_LOADLIGHT;
	mess.returnParam = returnVar;
	addMessStackParamO(var3 - 1, 8, 4, rtv, raw);
	addMessStackParamO(var2 - 1, 4, 4, rtv, raw);
	addMessStackParamO(var1 - 1, 0, 4, rtv, raw);

	sendMessageOut(mess, rtv, raw);
	startWaiting(returnVar - 1, raw);

	return RunTimeError_OK;

}

RunTimeError LoadMeshOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessStrParLines + sendLines + waitingLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));

	Message mess;
	mess.returnParam = returnVar;
	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_LOADMESH;
	addMessStackStringParamO(var - 1, 0, 4, rtv, raw);
	sendMessageOut(mess, rtv, raw);
	startWaiting(returnVar - 1, raw);

	return RunTimeError_OK;

}

RunTimeError LoadObjectOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO * 2 + waitingLines + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.returnParam = returnVar;
	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_LOADOBJECT;

	unsigned int var = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	addMessStackParamO(var - 1, 4, 4, rtv, raw);

	var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	addMessStackParamO(var - 1, 0, 4, rtv, raw);

	sendMessageOut(mess, rtv, raw);
	startWaiting(returnVar - 1, raw);

	return RunTimeError_OK;

}

RunTimeError LoadObjectCopyOperator(rawCode* raw,unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = sendLines + waitingLines + addMessParLinesO;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));

	Message mess;
	mess.returnParam = returnVar;
	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_LOADCOPYOBJECT;

	addMessStackParamO(var - 1, 0, 4, rtv, raw);
	sendMessageOut(mess, rtv, raw);

	startWaiting(returnVar - 1, raw);

	return RunTimeError_OK;

}

RunTimeError LoadPanelOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	raw->maxLines = addMessTempParLines * 2 + waitingLines + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_LOADGUI;
	int enu = GUIObjects_LAYOUT;
	mess.returnParam = returnVar;
	addMessTempStack(0, enu, 4, rtv, raw);
	addMessTempStack(4, ' ', 1, rtv, raw);

	sendMessageOut(mess, rtv, raw);
	startWaiting(returnVar - 1, raw);

	return RunTimeError_OK;


}

RunTimeError LoadStateOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessStrParLines + addMessParLinesO + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));

	Message mess;
	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_LOADBOARD;
	mess.returnParam = 0;

	addMessStackStringParamO(var2 - 1, 4, 4, rtv, raw);
	addMessStackParamO(var - 1, 0, 4, rtv, raw);

	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError LoadScriptOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessStrParLines + waitingLines + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	Message mess;

	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_LOADSCRIPT;
	mess.returnParam = returnVar;
	
	addMessStackStringParamO(var - 1, 0, 4, rtv, raw);
	sendMessageOut(mess, rtv, raw);

	startWaiting(returnVar - 1, raw);

	return RunTimeError_OK;

}

RunTimeError LoadTaskOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = sendLines + waitingLines + addMessParLinesO;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));

	Message mess;
	mess.returnParam = returnVar;
	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_LOADTASK;

	addMessStackParamO(var - 1, 0, 4, rtv, raw);
	sendMessageOut(mess, rtv, raw);

	startWaiting(returnVar - 1, raw);

	return RunTimeError_OK;

}

RunTimeError LoadTextBoxOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	raw->maxLines = addMessTempParLines * 2 + waitingLines + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_LOADGUI;
	mess.returnParam = returnVar;
	int enu = GUIObjects_TEXTBOX;
	addMessTempStack(0, enu, 4, rtv, raw);
	addMessTempStack(4, ' ', 1, rtv, raw);

	startWaiting(returnVar - 1, raw);
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError ModulusVarOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = commonMathLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	performArithemitc(var - 1,
		var2 - 1,
		returnVar - 1, rtv,
		opcode_MOD, 0, raw);

	return RunTimeError_OK;

}

RunTimeError MoveOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 12)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO * (paramSize == 16 ? 3 : 2) + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_MOVE;

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	mess.returnParam = var; 

	if (paramSize == 16)
	{

		unsigned int zVar = (params[12] | ((int)params[13] << 8) | ((int)params[14] << 16) | ((int)params[15] << 24));
		addMessStackParamO(zVar - 1, 8, 4, rtv, raw);

	}

	unsigned int xVar = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	unsigned int yVar = (params[8] | ((int)params[9] << 8) | ((int)params[10] << 16) | ((int)params[11] << 24));

	addMessStackParamO(yVar - 1, 4, 4, rtv, raw);
	addMessStackParamO(xVar - 1, 0, 4, rtv, raw);

	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError MulFloatOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}
	
	raw->maxLines = commonMathLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	performArithemitc(var - 1,
		var2 - 1,
		returnVar - 1, rtv,
		opcode_MUL, 1, raw);

	return RunTimeError_OK;

}

RunTimeError MulVarOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = commonMathLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	performArithemitc(var - 1,
		var2 - 1,
		returnVar - 1, rtv,
		opcode_MUL, 0, raw);

	return RunTimeError_OK;

}

RunTimeError NegateFloatOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}


	raw->maxLines = 4 + waitingLines + placeAdrLines * 2;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));

	rawCode::line line;
	//Place the adress of the var in a1
	placeAdr(var - 1, 0, 0, rtv, raw);
	//Place the returnadress of the in a2
	placeAdr(returnVar - 1, 1, 0, rtv, raw);
	//Place -1 in f2
	line.code = opcode_PLACE;
	line.r1 = 1;
	line.type = 1;
	line.scale = -1;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Load the number to f3 from the adress in a1
	line.code = opcode_LOAD;
	line.r1 = 2;
	line.r2 = 0;
	line.type = 1;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Multiply f2 to f3 and store in f3
	line.code = opcode_MUL;
	line.r1 = 2;
	line.r2 = 1;
	line.r3 = 2;
	line.type = 1;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Store the value in f3 to the stack at adr a2
	line.code = opcode_STORE;
	line.r1 = 2;
	line.r2 = 1;
	line.type = 1;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;

	finishWaiting(returnVar - 1, raw);
	return RunTimeError_OK;

}

RunTimeError NegateVarOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}


	raw->maxLines = 4 + waitingLines + placeAdrLines * 2;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	
	rawCode::line line;
	//Place the adress of the var in a1
	placeAdr(var - 1, 0, 0, rtv, raw);
	//Place the returnadress of the in a2
	placeAdr(returnVar - 1, 1, 0, rtv, raw);
	//Place -1 in i2
	line.code = opcode_PLACE;
	line.r1 = 1;
	line.type = 0;
	line.scale = -1;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Load the number to i3 from the adress in i1
	line.code = opcode_LOAD;
	line.r1 = 2;
	line.r2 = 0;
	line.type = 0;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Multiply i2 to i3 and store in i3
	line.code = opcode_MUL;
	line.r1 = 2;
	line.r2 = 1;
	line.r3 = 2;
	line.type = 0;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Store the value in i3 to the stack at adr i2
	line.code = opcode_STORE;
	line.r1 = 2;
	line.r2 = 1;
	line.type = 0;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;

	finishWaiting(returnVar - 1, raw);
	return RunTimeError_OK;

}

RunTimeError NumGrtFloaOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = commonMathLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	performArithemitc(var2 - 1,
		var - 1,
		returnVar - 1, rtv,
		opcode_GRTR, 1, raw);

	return RunTimeError_OK;

}

RunTimeError NumGrtVarOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = commonMathLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	performArithemitc(var - 1,
		var2 - 1,
		returnVar - 1, rtv,
		opcode_GRTR, 0, raw);

	return RunTimeError_OK;

}

RunTimeError NumEqlVarOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = commonMathLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	performArithemitc(var - 1,
		var2 - 1,
		returnVar - 1, rtv,
		opcode_CMPRE, 0, raw);

	return RunTimeError_OK;

}

RunTimeError OffsetNumOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = 1;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int arr = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));

	commonOffset(arr-1, var - 1, 4, rtv);
	return RunTimeError_OK;

}

RunTimeError OffsetStrOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = 1;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int arr = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));

	commonOffset(arr-1,var - 1, 64, rtv);
	return RunTimeError_OK;

}

RunTimeError OrOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = commonConditionalLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));

	commonConditional(var - 1, var2 - 1, returnVar - 1, 0, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError OrbitOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO * 2 + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.destination = MessageSource_ENTITIES;
	mess.type = MessageType_ENTITIES;
	mess.mess = GameBoardMess_ORBITOBJECT;
	mess.returnParam = 0;

	unsigned int fVar = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	addMessStackParamO(fVar - 1, 4, 4, rtv, raw);

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	addMessStackParamO(var - 1, 0, 4, rtv, raw);

	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError ParentOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO + waitingLines + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));

	Message mess;
	mess.destination = MessageSource_ENTITIES;
	mess.type = MessageType_ENTITIES;
	mess.mess = GameBoardMess_GETPARENT;
	mess.returnParam = returnVar;

	addMessStackParamO(var - 1, 0, 4, rtv, raw);
	sendMessageOut(mess, rtv, raw);
	startWaiting(returnVar - 1, raw);

	return RunTimeError_OK;

}

RunTimeError PauseGameOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	raw->maxLines = addMessTempParLines + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.destination = MessageSource_TASKS;
	mess.type = MessageType_TASKING;
	mess.mess = TaskMess_PAUSECLOCK;
	mess.returnParam = 0;
	addMessTempStack(0, 1, 1, rtv, raw);
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError PosOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 12)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO * (paramSize == 16 ? 3 : 2) + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_POS;

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	mess.returnParam = var;

	if (paramSize == 16)
	{

		unsigned int zVar = (params[12] | ((int)params[13] << 8) | ((int)params[14] << 16) | ((int)params[15] << 24));
		addMessStackParamO(zVar - 1, 8, 4, rtv, raw);

	}

	unsigned int xVar = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	unsigned int yVar = (params[8] | ((int)params[9] << 8) | ((int)params[10] << 16) | ((int)params[11] << 24));

	addMessStackParamO(yVar - 1, 4, 4, rtv, raw);
	addMessStackParamO(xVar - 1, 0, 4, rtv, raw);


	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError PostFloatOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessStrParLines + sendLines + 3 + placeAdrLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));

	rawCode::line line;
	//Place the adress of the float var in a1
	placeAdr(var - 1, 0, 0, rtv, raw);
	//Load the float the adress in a1 to f1
	line.code = opcode_LOAD;
	line.r1 = 0;
	line.r2 = 0;
	line.type = 1;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Place the adress of the temp var in a2
	line.code = opcode_PLACE;
	line.r1 = 1;
	line.type = 3;
	line.scale = rtv.memory->GetVarAdr(rtv.memory->GetMaxVar());
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Cast the float in f1 to a string at adr a2
	line.code = opcode_FTS;
	line.r1 = 0;
	line.r2 = 1;
	line.type = 0;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;

	Message mess;
	mess.destination = MessageSource_GUIENTITIES;
	mess.type = MessageType_GUIENTITIES;
	mess.mess = GUIMess_POST;
	mess.returnParam = 0;

	addMessStackStringParamNO(rtv.memory->GetMaxVar(), 0, 4, rtv, raw);
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError PostNumOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessStrParLinesNO + sendLines + castIToSLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	convertIntToString(var - 1, rtv.memory->GetMaxVar(), offset, rtv, raw);

	Message mess;
	mess.destination = MessageSource_GUIENTITIES;
	mess.type = MessageType_GUIENTITIES;
	mess.mess = GUIMess_POST;
	mess.returnParam = 0;

	addMessStackStringParamNO(rtv.memory->GetMaxVar(), 0, 4, rtv, raw);
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError PostStrOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessStrParLines + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));

	Message mess;
	mess.destination = MessageSource_GUIENTITIES;
	mess.type = MessageType_GUIENTITIES;
	mess.mess = GUIMess_POST;
	mess.returnParam = 0;

	addMessStackStringParamO(var - 1, 0, 4, rtv, raw);
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError PropelOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO * (paramSize == 16 ? 3 : 2) + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_PROPEL;

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	mess.returnParam = var;

	unsigned int fVar = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	addMessStackParamO(fVar - 1, 0, 4, rtv, raw);

	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError QueueClockTaskOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO * 2 + addMessTempParLines + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.destination = MessageSource_TASKS;
	mess.type = MessageType_TASKING;
	mess.mess = TaskMess_ADDTASK;
	mess.returnParam = 0;

	unsigned int fTime = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	addMessStackParamO(fTime - 1, 8, 4, rtv, raw);

	addMessTempStack(4, Resources::TaskClass_CLOCK, 4, rtv, raw);

	unsigned int task = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	addMessStackParamO(task - 1, 0, 4, rtv, raw);

	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError QueueSystemTaskOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO * 2 + addMessTempParLines + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.destination = MessageSource_TASKS;
	mess.type = MessageType_TASKING;
	mess.mess = TaskMess_ADDTASK;
	mess.returnParam = 0;

	unsigned int fTime = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	addMessStackParamO(fTime - 1, 8, 4, rtv, raw);

	addMessTempStack(4, Resources::TaskClass_SYSTEM, 4, rtv, raw);

	unsigned int task = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	addMessStackParamO(task - 1, 0, 4, rtv, raw);

	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError RemoveOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_REMOVE;

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	mess.returnParam = var;

	sendMessageOut(mess, rtv, raw);
	return RunTimeError_OK;

}

RunTimeError RemoveTaskOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.destination = MessageSource_TASKS;
	mess.type = MessageType_TASKING;
	mess.mess = TaskMess_REMOVETASK;
	mess.returnParam = 0;

	unsigned int task = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	addMessStackParamO(task - 1, 0, 4, rtv, raw);

	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError ReSnapOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 12)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO * 2 + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_SETSNAP;

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	mess.returnParam = var ;

	unsigned int hV = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	unsigned int snapEnum = (params[8] | ((int)params[9] << 8) | ((int)params[10] << 16) | ((int)params[11] << 24));

	addMessStackParamO(snapEnum - 1, 1, 1, rtv, raw);
	addMessStackParamO(hV - 1, 0, 1, rtv, raw);

	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError ResumeGameOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{


	raw->maxLines = addMessTempParLines + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.destination = MessageSource_TASKS;
	mess.type = MessageType_TASKING;
	mess.mess = TaskMess_PAUSECLOCK;
	mess.returnParam = 0;
	addMessTempStack(0, 0, 1, rtv, raw);
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError RotateOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 16)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO * 3 + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_ROTATE;

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	mess.returnParam = var;

	unsigned int xVar = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	unsigned int yVar = (params[8] | ((int)params[9] << 8) | ((int)params[10] << 16) | ((int)params[11] << 24));
	unsigned int zVar = (params[12] | ((int)params[13] << 8) | ((int)params[14] << 16) | ((int)params[15] << 24));

	addMessStackParamO(zVar - 1, 8, 4, rtv, raw);
	addMessStackParamO(yVar - 1, 4, 4, rtv, raw);
	addMessStackParamO(xVar - 1, 0, 4, rtv, raw);

	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError RouteOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO * 3 + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	Message mess;
	mess.destination = MessageSource_ENTITIES;
	mess.type = MessageType_ENTITIES;
	mess.mess = GameBoardMess_ROUTEOBJECT;
	mess.returnParam = 0;

	addMessStackParamO(var - 1, 0, 4, rtv, raw);
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError RunScriptOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 5)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = 9;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int codeToRun = getCode(rtv.byteCodes, rtv.compiled, getString((char*)params));

	rtv.byteCodes[codeToRun]->ResetParam('n');
	rtv.byteCodes[codeToRun]->ResetParam('s');
	rtv.byteCodes[codeToRun]->ResetParam('f');

	unsigned int returnAdr = offset + 6;

	rawCode::line line;
	//Place the adress of the temp value in a1
	line.code = opcode_PLACE;
	line.r1 = 0;
	line.type = 3;
	line.scale = rtv.memory->GetVarAdr(rtv.memory->GetMaxVar());
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Save the value in i4 to the adr in a1
	line.code = opcode_STORE;
	line.r1 = 3;
	line.r2 = 0;
	line.type = 0;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Place the memoffset of the code to run in i5
	rtv.byteCodes[codeToRun]->AddMemOffsetPlaceHolder(offset + raw->codeSize + 1);
	line.code = opcode_PLACE;
	line.r1 = 4;
	line.type = 4;
	line.scale = 0;//PlaceHolder
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Place the line of the code to run in i1
	rtv.byteCodes[codeToRun]->AddStackPlaceHolder(offset + raw->codeSize + 1);
	line.code = opcode_PLACE;
	line.r1 = 0;
	line.type = 0;
	line.scale = 0;//PlaceHolder
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Place the return adr in i4
	line.code = opcode_PLACE;
	line.r1 = 3;
	line.type = 0;
	line.scale = returnAdr;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Jump to the adr in i1
	line.code = opcode_JMP;
	line.r1 = 0;
	line.r2 = 0;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;

	//Place the memoffset of the current code to run in i5
	rtv.byteCodes[rtv.current]->AddMemOffsetPlaceHolder(offset + raw->codeSize + 1);
	line.code = opcode_PLACE;
	line.r1 = 4;
	line.type = 4;
	line.scale = 0;//PlaceHolder
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Place the adress of the temp value in a1
	line.code = opcode_PLACE;
	line.r1 = 0;
	line.type = 3;
	line.scale = rtv.memory->GetVarAdr(rtv.memory->GetMaxVar());
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Load the value saved in the stack at adr a1 to i4
	line.code = opcode_LOAD;
	line.r1 = 3;
	line.r2 = 0;
	line.type = 0;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;

	return RunTimeError_OK;

}

RunTimeError SaveStateOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessStrParLines + addMessParLinesO + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	
	Message mess;
	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_SAVEBOARD;
	mess.returnParam = 0;

	addMessStackStringParamO(var2 - 1, 4, 4, rtv, raw);
	addMessStackParamO(var - 1, 0, 4, rtv, raw);
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError ScaleOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 12)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO * (paramSize == 16 ? 3 : 2) + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_SCALE;

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	mess.returnParam = var;

	unsigned int xVar = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	unsigned int yVar = (params[8] | ((int)params[9] << 8) | ((int)params[10] << 16) | ((int)params[11] << 24));

	if (paramSize == 16)
	{

		unsigned int zVar = (params[12] | ((int)params[13] << 8) | ((int)params[14] << 16) | ((int)params[15] << 24));
		addMessStackParamO(zVar - 1, 8, 4, rtv, raw);

	}

	addMessStackParamO(yVar - 1, 4, 4, rtv, raw);
	addMessStackParamO(xVar - 1, 0, 4, rtv, raw);

	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError ScopeSaveOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_SAVESCOPE;
	mess.returnParam = var;

	addMessStackParamO(var2 - 1, 0, 4, rtv, raw);
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;


}

RunTimeError ShowCursorOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	raw->maxLines = sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.destination = MessageSource_SYSTEM;
	mess.type = MessageType_SYSTEM;
	mess.mess = SystemMess_SHOWCURSOR;
	mess.returnParam = 0;
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError ShowOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_SHOW;
	mess.returnParam = var;

	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError SetBorderOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 16)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO * 4 + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.returnParam = 0;
	mess.destination = MessageSource_GRAPHICS;
	mess.type = MessageType_GRAPHICS;
	mess.mess = GraphicMess_SETBORDERBRUSH;

	unsigned int zVar = (params[12] | ((int)params[13] << 8) | ((int)params[14] << 16) | ((int)params[15] << 24));
	addMessStackParamO(zVar - 1, 12, 4, rtv, raw);

	unsigned int yVar = (params[8] | ((int)params[9] << 8) | ((int)params[10] << 16) | ((int)params[11] << 24));
	addMessStackParamO(yVar - 1, 8, 4, rtv, raw);

	unsigned int xVar = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	addMessStackParamO(xVar - 1, 4, 4, rtv, raw);

	unsigned int obj = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	addMessStackParamO(obj - 1, 0, 4, rtv, raw);

	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError SetCameraOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));

	Message mess;
	mess.destination = MessageSource_ENTITIES;
	mess.type = MessageType_ENTITIES;
	mess.mess = GameBoardMess_SETCAM;
	mess.returnParam = 0;

	addMessStackParamO(var - 1, 0, 4, rtv, raw);
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;


}

RunTimeError SetConstValOperator(rawCode* raw,unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 5)
	{

		return RunTimeError_BADPARAMS;

	}


	unsigned int var = params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24);

	if (rtv.memory->GetVarAdr(var - 1) <= rtv.memory->GetMaxStack())
	{

		rtv.memory->AddVariable(var - 1, paramSize - 4);

	}

	unsigned int bytes = paramSize - 4;
	unsigned int writtenBytes = 0;

	raw->maxLines = ceil((float)bytes / 4.0f) * 2 + waitingLines + placeAdrLines + 1;
	raw->code = new rawCode::line[raw->maxLines];
	rawCode::line line;
	//Place the starting adress in a1
	placeAdr(var - 1, 0, 0, rtv, raw);
	//Place 4 in i2
	line.code = opcode_PLACE;
	line.r1 = 1;
	line.type = 0;
	line.scale = 4;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;

	while(bytes > 0)
	{

		unsigned int bytesToRead = fmin(bytes, 4);
		//Save 4 bytes to the stack adr in a1
		line.code = opcode_SAVE;
		line.r1 = 0;
		line.r2 = 1;
		memcpy(&line.scale, &params[4 + writtenBytes], bytesToRead);
		raw->code[raw->codeSize] = line;
		raw->codeSize++;
		//Add i2 to a1 and store in a1
		line.code = opcode_ADD;
		line.r1 = 0;
		line.r2 = 1;
		line.r3 = 0;
		line.type = 3;
		raw->code[raw->codeSize] = line;
		raw->codeSize++;

		writtenBytes += bytesToRead;
		bytes -= bytesToRead;

	}

	finishWaiting(var - 1, raw);

	return RunTimeError_OK;

}

RunTimeError SetContentOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 16)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO * 4 + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.destination = MessageSource_GRAPHICS;
	mess.type = MessageType_GRAPHICS;
	mess.mess = GraphicMess_SETCONTENTBRUSH;
	mess.returnParam = 0;

	unsigned int zVar = (params[12] | ((int)params[13] << 8) | ((int)params[14] << 16) | ((int)params[15] << 24));
	addMessStackParamO(zVar - 1, 12, 4, rtv, raw);

	unsigned int yVar = (params[8] | ((int)params[9] << 8) | ((int)params[10] << 16) | ((int)params[11] << 24));
	addMessStackParamO(yVar - 1, 8, 4, rtv, raw);

	unsigned int xVar = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	addMessStackParamO(xVar - 1, 4, 4, rtv, raw);

	unsigned int obj = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	addMessStackParamO(obj - 1, 0, 4, rtv, raw);

	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError SetGameBoardOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));

	Message mess;
	mess.destination = MessageSource_ENTITIES;
	mess.type = MessageType_ENTITIES;
	mess.mess = GameBoardMess_SETGAMEBOARD;
	mess.returnParam = 0;

	addMessStackParamO(var - 1, 0, 4, rtv, raw);
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError SetLayerOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_LOCKINPUT;

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int layer = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));

	mess.returnParam = var;
	addMessStackParamO(layer - 1, 0, 4, rtv, raw);
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError SetLeftClickOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));

	setCommon(var, var2, ObjectMess_SETLCSCRPT, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError SetLeftDragOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));

	setCommon(var, var2, ObjectMess_SETLDSCRPT, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError SetMiddleClickOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = setObjectMessLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));

	setCommon(var, var2, ObjectMess_SETMCSCRPT, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError SetMiddleDragOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = setObjectMessLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));

	setCommon(var, var2, ObjectMess_SETMDSCRPT, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError SetMouseCursor(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));

	Message mess;
	mess.destination = MessageSource_SYSTEM;
	mess.type = MessageType_SYSTEM;
	mess.mess = SystemMess_SETCURSOR;
	mess.returnParam = 0;

	addMessStackParamO(var - 1, 0, 4, rtv, raw);
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;


}

RunTimeError SetMouseUpDownOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = setObjectMessLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));

	setCommon(var, var2, ObjectMess_SETUPDWNSCRPT, rtv, raw);
	return RunTimeError_OK;

}

RunTimeError SetMouseWheelOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = setObjectMessLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));

	setCommon(var, var2, ObjectMess_SETWHLSCRPT, rtv, raw);
	return RunTimeError_OK;

}

RunTimeError SetNodeArrivalOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = setObjectMessLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));

	setCommon(var, var2, ObjectMess_SETTRVLNDESCRPT, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError SetNodeSplitOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = setObjectMessLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));

	setCommon(var, var2, ObjectMess_SETSPLTSCRPT, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError SetRightClickOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = setObjectMessLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));

	setCommon(var, var2, ObjectMess_SETRCSCRPT, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError SetRightDragOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = setObjectMessLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));

	setCommon(var, var2, ObjectMess_SETRDSCRPT, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError SetScriptParFloatOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 9)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = setParamaterLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	std::string script = getString((char*)&params[4]);
	unsigned int scriptValue = getCode(rtv.byteCodes, rtv.compiled, script);

	commonPar(var, scriptValue, offset, 'f', rtv, raw);

	return RunTimeError_OK;

}

RunTimeError SetScriptParNumOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 9)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = setParamaterLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	std::string script = getString((char*)&params[4]);
	unsigned int scriptValue = getCode(rtv.byteCodes, rtv.compiled, script);

	commonPar(var, scriptValue, offset, 'n', rtv, raw);

	return RunTimeError_OK;

}

RunTimeError SetScriptParStrOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 9)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = setParamaterLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	std::string script = getString((char*)&params[4]);
	unsigned int scriptValue = getCode(rtv.byteCodes, rtv.compiled, script);

	commonPar(var, scriptValue, offset, 's', rtv, raw);

	return RunTimeError_OK;

}

RunTimeError SetTextOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessStrParLines + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_SETTEXT;

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	mess.returnParam = var;

	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));

	addMessStackStringParamO(var2 - 1, 0, 4, rtv, raw);
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError SetVarValOperator(rawCode* raw,unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->start = offset;
	raw->maxLines = 2 + waitingLines + placeAdrLines * 2;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var1 = params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24);
	unsigned int var2 = params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24);

	rawCode::line line;
	//Place the adress of var2 in a2
	placeAdr(var2 - 1, 1, 0, rtv, raw);
	//Place the adress of var1 in a1
	placeAdr(var1 - 1, 0, 0, rtv, raw);
	//Place the size of the variable in i3
	line.code = opcode_PLACE;
	line.r1 = 2;
	line.type = 0;
	line.scale = rtv.memory->GetVarSize(var2 - 1);
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Copy a3 bytes from a2 to a1
	line.code = opcode_COPY;
	line.r1 = 0;
	line.r2 = 1;
	line.r3 = 2;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;

	finishWaiting(var1 - 1, raw);

	return RunTimeError_OK;

}

RunTimeError SetUIOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));

	Message mess;
	mess.destination = MessageSource_GUIENTITIES;
	mess.type = MessageType_GUIENTITIES;
	mess.mess = GUIMess_SETUI;
	mess.returnParam = 0;

	addMessStackParamO(var - 1, 0, 4, rtv, raw);
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError SizeOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 12)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO * (paramSize == 16 ? 3 : 2) + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_SIZE;

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	mess.returnParam = var; 
	
	unsigned int xVar = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	unsigned int yVar = (params[8] | ((int)params[9] << 8) | ((int)params[10] << 16) | ((int)params[11] << 24));

	if (paramSize == 16)
	{

		unsigned int zVar = (params[12] | ((int)params[13] << 8) | ((int)params[14] << 16) | ((int)params[15] << 24));
		addMessStackParamO(zVar - 1, 8, 4, rtv, raw);

	}

	addMessStackParamO(yVar - 1, 4, 4, rtv, raw);
	addMessStackParamO(xVar - 1, 0, 4, rtv, raw);

	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError SpawnObjectOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO * 2 + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));

	Message mess;
	mess.destination = MessageSource_ENTITIES;
	mess.type = MessageType_ENTITIES;
	mess.mess = GameBoardMess_SPAWN;
	mess.returnParam = 0;

	addMessStackParamO(var2 - 1, 4, 4, rtv, raw);
	addMessStackParamO(var - 1, 0, 4, rtv, raw);
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError SplitRouteOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO * 5 + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int obj = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));

	unsigned int posX = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	unsigned int posY = (params[8] | ((int)params[9] << 8) | ((int)params[10] << 16) | ((int)params[11] << 24));
	unsigned int posZ = (params[12] | ((int)params[13] << 8) | ((int)params[14] << 16) | ((int)params[15] << 24));

	unsigned int width = (params[16] | ((int)params[17] << 8) | ((int)params[18] << 16) | ((int)params[19] << 24));

	Message mess;
	mess.destination = MessageSource_ENTITIES;
	mess.type = MessageType_ENTITIES;
	mess.mess = GameBoardMess_SPLITOBJECT;
	mess.returnParam = 0;

	addMessStackParamO(width - 1, 16, 4, rtv, raw);

	addMessStackParamO(posZ - 1, 12, 4, rtv, raw);
	addMessStackParamO(posY - 1, 8, 4, rtv, raw);
	addMessStackParamO(posX - 1, 4, 4, rtv, raw);

	addMessStackParamO(obj - 1, 0, 4, rtv, raw);

	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError StrEqlVarOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = placeAdrLines * 3 + 3;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	
	rawCode::line line;
	//Place the var2 in a2
	placeAdr(var2 - 1, 1, 0, rtv, raw);
	//Place the adr of var in a1
	placeAdr(var - 1, 0, 0, rtv, raw);
	//Place the adress of the returnvar in a3
	placeAdr(returnVar-1, 2, 0, rtv, raw);
	//Load the size of the first string in i3 from a1
	line.code = opcode_LOAD;
	line.r1 = 2;
	line.r2 = 0;
	line.type = 0;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Compare the memory at a1 with a2 and size i3 and place it in c3
	line.code = opcode_CMPREBYTES;
	line.r1 = 0;
	line.r2 = 1;
	line.r3 = 2;
	raw->codeSize++;
	//Store the number in c3 to the stack with adr a3
	line.code = opcode_STORE;
	line.r1 = 2;
	line.r2 = 2;
	line.type = 2;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;

	return RunTimeError_OK;

}

RunTimeError SubFloatOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = commonMathLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	performArithemitc(var - 1,
		var2 - 1,
		returnVar - 1, rtv,
		opcode_SUB, 1, raw);

	return RunTimeError_OK;

}

RunTimeError SubVarOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = commonMathLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	performArithemitc(var - 1,
		var2 - 1,
		returnVar - 1, rtv,
		opcode_SUB, 0, raw);

	return RunTimeError_OK;

}

RunTimeError SumStringOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = 11 + placeAdrLines * 3 + waitingLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));

	//Place the adr of var2 in a2
	placeAdr(var2 - 1, 1, 0, rtv, raw);
	//Place the adr of var1 in a1
	placeAdr(var - 1, 0, 0, rtv, raw);
	//Place the adr of returnvar in a3
	placeAdr(returnVar - 1, 2, 0, rtv, raw);


	rawCode::line line;
	//Load the size of the first string in i1
	line.code = opcode_LOAD;
	line.r1 = 0;
	line.r2 = 0;
	line.type = 0;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Load the size of the second string in i2
	line.code = opcode_LOAD;
	line.r1 = 1;
	line.r2 = 1;
	line.type = 0;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;


	//Add i2 to i1 and store in i3
	line.code = opcode_ADD;
	line.r1 = 0;
	line.r2 = 1;
	line.r3 = 2;
	line.type = 0;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//store i3 in a3
	line.code = opcode_STORE;
	line.r1 = 2;
	line.r2 = 2;
	line.type = 0;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;

	//Place 4 in i3
	line.code = opcode_PLACE;
	line.r1 = 2;
	line.type = 0;
	line.scale = 4;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Add i3 to a3 and store in a3
	line.code = opcode_ADD;
	line.r1 = 2;
	line.r2 = 2;
	line.r3 = 2;
	line.type = 3;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Add i3 to a2 and store in a2
	line.code = opcode_ADD;
	line.r1 = 1;
	line.r2 = 2;
	line.r3 = 1;
	line.type = 3;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Add i3 to a1 and store in a1
	line.code = opcode_ADD;
	line.r1 = 0;
	line.r2 = 2;
	line.r3 = 0;
	line.type = 3;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;

	//Copy the value from a1 to a3 with size i1
	line.code = opcode_COPY;
	line.r1 = 2;
	line.r2 = 0;
	line.r3 = 0;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Add i1 to a3 and store in a3
	line.code = opcode_ADD;
	line.r1 = 2;
	line.r2 = 0;
	line.r3 = 2;
	line.type = 3;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Copy the value from a2 to a3 with size i2
	line.code = opcode_COPY;
	line.r1 = 2;
	line.r2 = 1;
	line.r3 = 1;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;


	finishWaiting(returnVar - 1, raw);
	return RunTimeError_OK;

}

RunTimeError SumFloatOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = commonMathLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	performArithemitc(var - 1,
		var2 - 1,
		returnVar - 1, rtv, 
		opcode_ADD, 1, raw);


	return RunTimeError_OK;

}

RunTimeError SumVarOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = commonMathLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	performArithemitc(var - 1,
		var2 - 1,
		returnVar - 1, rtv,
		opcode_ADD, 0, raw);

	return RunTimeError_OK;

}

RunTimeError TrackObjectOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));

	Message mess;
	mess.destination = MessageSource_ENTITIES;
	mess.type = MessageType_ENTITIES;
	mess.mess = GameBoardMess_SETTRACKING;
	mess.returnParam = 0;

	addMessStackParamO(var - 1, 0, 4, rtv, raw);
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError TravelObjectOperator(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO * 2 + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));

	Message mess;
	mess.destination = MessageSource_ENTITIES;
	mess.type = MessageType_ENTITIES;
	mess.mess = GameBoardMess_TRAVEL;
	mess.returnParam = 0;

	addMessStackParamO(var2 - 1, 4, 4, rtv, raw);
	addMessStackParamO(var - 1, 0, 4, rtv, raw);
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError UnLoadOperator(rawCode* raw,unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = addMessParLinesO + sendLines;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));

	Message mess;
	mess.returnParam = returnVar;
	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_UNLOADOBJECT;

	addMessStackParamO(var - 1, 0, 4, rtv, raw);
	sendMessageOut(mess, rtv, raw);

	return RunTimeError_OK;

}

RunTimeError WaitForVar(rawCode* raw, unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	raw->maxLines = 11;
	raw->code = new rawCode::line[raw->maxLines];

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int goodLine = offset + raw->maxLines;

	rawCode::line line;
	//Place the var in a1
	line.code = opcode_PLACE;
	line.r1 = 0;
	line.type = 3;
	line.scale = var-1;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Load the var from the stack at adr a1 to c1
	line.code = opcode_LOAD;
	line.r1 = 0;
	line.r2 = 0;
	line.type = 2;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Place 1 in c2 
	line.code = opcode_PLACE;
	line.r1 = 1;
	line.type = 2;
	line.scale = 255;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Compare c1 to c2 and store in c3
	line.code = opcode_CMPRE;
	line.r1 = 0;
	line.r2 = 1;
	line.r3 = 2;
	line.type = 2;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Invert c3
	line.code = opcode_INV;
	line.r1 = 2;
	line.type = 2;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Place the line to go to if the var is good in i1 
	line.code = opcode_PLACE;
	line.r1 = 0;
	line.type = 0;
	line.scale = goodLine;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Jump to i1 if c3 == 0
	line.code = opcode_JMPINV;
	line.r1 = 2;
	line.r2 = 0;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Place the starting line in i2
	line.code = opcode_PLACE;
	line.r1 = 1;
	line.type = 0;
	line.scale = offset;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Place the waitingerr in i1
	line.code = opcode_PLACE;
	line.r1 = 0;
	line.type = 0;
	line.scale = RunTimeError_WAITINGFORWAR;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Return i1 and halt the machine
	line.code = opcode_RETURN;
	line.r1 = 0;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;
	//Jump back to the start of the code
	line.code = opcode_JMP;
	line.r1 = 0;
	line.r2 = 1;
	raw->code[raw->codeSize] = line;
	raw->codeSize++;

	return RunTimeError_OK;
	
}


RunTimeError NegateConstOperator(rawCode* raw,unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	return RunTimeError_OK;

}

RunTimeError SumConst2Operator(rawCode* raw,unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	return RunTimeError_OK;

}

RunTimeError SubConst2Operator(rawCode* raw,unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	return RunTimeError_OK;

}

RunTimeError MulConst2Operator(rawCode* raw,unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	return RunTimeError_OK;

}

RunTimeError DivConst2Operator(rawCode* raw,unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	return RunTimeError_OK;

}

RunTimeError NumEqlConst2Operator(rawCode* raw,unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	return RunTimeError_OK;

}

RunTimeError StrEqlConst2Operator(rawCode* raw,unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	return RunTimeError_OK;

}

RunTimeError NumGrtConst2Operator(rawCode* raw,unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int offset, unsigned int byteLine, runTimeVal &rtv)
{

	return RunTimeError_OK;

}



#pragma endregion

KubLingRawTranslator::KubLingRawTranslator()
{

	translator = new TranslateByteCode[bytecode_NA];

	translator[bytecode_SETCONST] = SetConstValOperator;
	translator[bytecode_SETVAR] = SetVarValOperator;

	translator[bytecode_LOADLIGHT] = LoadLightOperator;
	translator[bytecode_LOADMESH] = LoadMeshOperator;
	translator[bytecode_LOADSCRIPT] = LoadScriptOperator;
	translator[bytecode_LOADTXTBX] = LoadTextBoxOperator;
	translator[bytecode_LOADPANEL] = LoadPanelOperator;
	translator[bytecode_LOADIMAGE] = LoadImageOperator;
	translator[bytecode_LOADGMBRD] = LoadGameBoardOperator;
	translator[bytecode_LOADCAM] = LoadCameraOperator;
	translator[bytecode_LOADOBJCT] = LoadObjectOperator;
	translator[bytecode_LOADCPY] = LoadObjectCopyOperator;
	translator[bytecode_LOADTSK] = LoadTaskOperator;

	translator[bytecode_UNLOAD] = UnLoadOperator;

	translator[bytecode_ADDOBJECT] = AddObjectOperator;
	translator[bytecode_ADDMESH] = AddMeshOperator;
	translator[bytecode_ADDRTOBJ] = AddRouteObjectOperator;
	translator[bytecode_ADDTRGGR] = AddTriggerOperator;

	translator[bytecode_RMVE] = RemoveOperator;

	translator[bytecode_SETGMEBRD] = SetGameBoardOperator;
	translator[bytecode_SETCMRA] = SetCameraOperator;

	translator[bytecode_SETLCLK] = SetLeftClickOperator;
	translator[bytecode_SETMCLK] = SetMiddleClickOperator;
	translator[bytecode_SETRCLK] = SetRightClickOperator;
	translator[bytecode_SETLDRG] = SetLeftDragOperator;
	translator[bytecode_SETMDRG] = SetMiddleDragOperator;
	translator[bytecode_SETRDRG] = SetRightDragOperator;
	translator[bytecode_SETMWHL] = SetMouseWheelOperator;
	translator[bytecode_SETUD] = SetMouseUpDownOperator;
	translator[bytecode_SETNDEARRVL] = SetNodeArrivalOperator;
	translator[bytecode_SETSPLT] = SetNodeSplitOperator;

	translator[bytecode_SETUI] = SetUIOperator;
	translator[bytecode_SETCRS] = SetMouseCursor;
	translator[bytecode_FCSUI] = FocusUIOperator;

	translator[bytecode_GETSCRNY] = GetScreenHeightOperator;
	translator[bytecode_PRNT] = ParentOperator;

	translator[bytecode_RESNAP] = ReSnapOperator;
	translator[bytecode_POS] = PosOperator;
	translator[bytecode_MVE] = MoveOperator;
	translator[bytecode_PRPL] = PropelOperator;
	translator[bytecode_RTE] = RotateOperator;
	translator[bytecode_ORB] = OrbitOperator;
	translator[bytecode_SIZE] = SizeOperator;
	translator[bytecode_SCL] = ScaleOperator;
	translator[bytecode_2DADDCHLD] = Add2DOperator;

	translator[bytecode_POSTSTR] = PostStrOperator;
	translator[bytecode_POSTNMBR] = PostNumOperator;
	translator[bytecode_POSTFLOAT] = PostFloatOperator;

	translator[bytecode_IGNRMS] = IgnoreMouseOperator;
	translator[bytecode_IGNRKY] = IgnoreKeyboardOperator;
	translator[bytecode_IGNRIN] = IgnoreInputOperator;

	translator[bytecode_ACPTMS] = AcceptMouseOperator;
	translator[bytecode_ACPTKY] = AcceptKeyboardOperator;
	translator[bytecode_ACPTIN] = AcceptInputOperator;

	translator[bytecode_INCRMNTLYR] = IncrementLayerOperator;
	translator[bytecode_DCRMNLYR] = DecrementLayerOperator;
	translator[bytecode_STLYR] = SetLayerOperator;

	translator[bytecode_NEGATECONST] = NegateConstOperator;
	translator[bytecode_NEGATEVAR] = NegateVarOperator;
	translator[bytecode_NEGATEFLOAT] = NegateFloatOperator;

	translator[bytecode_SUM2CONST] = SumConst2Operator;
	translator[bytecode_SUMFLOAT] = SumFloatOperator;
	translator[bytecode_SUMVAR] = SumVarOperator;
	translator[bytecode_SUMSTR] = SumStringOperator;

	translator[bytecode_SUB2CONST] = SubConst2Operator;
	translator[bytecode_SUBFLOAT] = SubFloatOperator;
	translator[bytecode_SUBVAR] = SubVarOperator;

	translator[bytecode_MUL2CONST] = MulConst2Operator;
	translator[bytecode_MULFLOAT] = MulFloatOperator;
	translator[bytecode_MULVAR] = MulVarOperator;

	translator[bytecode_DIV2CONST] = DivConst2Operator;
	translator[bytecode_DIVFLOAT] = DivFloatOperator;
	translator[bytecode_DIVVAR] = DivVarOperator;
	translator[bytecode_MODVAR] = ModulusVarOperator;

	translator[bytecode_CASTFLOAT] = CastFloatOperator;
	translator[bytecode_CASTTSTR] = CastNumToStringOperator;

	translator[bytecode_NUMEQUAL2CONST] = NumEqlConst2Operator;
	translator[bytecode_NUMEQUALFLOAT] = FloatEqlVarOperator;
	translator[bytecode_NUMEQUALVAR] = NumEqlVarOperator;


	translator[bytecode_STREQUAL2CONST] = StrEqlConst2Operator;
	translator[bytecode_STREQUALVAR] = StrEqlVarOperator;

	translator[bytecode_NUMGRT2CONST] = NumGrtConst2Operator;
	translator[bytecode_NUMGRTFLOAT] = NumGrtFloaOperator;
	translator[bytecode_NUMGRTVAR] = NumGrtVarOperator;

	translator[bytecode_RNSCRPT] = RunScriptOperator;
	translator[bytecode_STSCRPTPRMNMBR] = SetScriptParNumOperator;
	translator[bytecode_STSCRPTPRMSTR] = SetScriptParStrOperator;
	translator[bytecode_STSCRPTPRMFLT] = SetScriptParFloatOperator;

	translator[bytecode_EXPRTSTR] = ExportStrOperator;
	translator[bytecode_EXPRTNUM] = ExportNumOperator;
	translator[bytecode_IMPRTSTR] = ImportStrOperator;
	translator[bytecode_IMPRTNUM] = ImportNumOperator;

	translator[bytecode_HDE] = HideOperator;
	translator[bytecode_SHW] = ShowOperator;
	translator[bytecode_HDECRS] = HideCursorOperator;
	translator[bytecode_SHWCRS] = ShowCursorOperator;

	translator[bytecode_LNKDBG] = LinkDBGOperator;
	translator[bytecode_LNKTRGT] = LinkTargetOperator;


	translator[bytecode_STXT] = SetTextOperator;
	translator[bytecode_ADDTXT] = AppendTextOperator;
	translator[bytecode_ADDLNE] = AddTextLineOperator;
	translator[bytecode_STCNTNT] = SetContentOperator;
	translator[bytecode_STBRDR] = SetBorderOperator;

	translator[bytecode_SPLTRTE] = SplitRouteOperator;

	translator[bytecode_TRCK] = TrackObjectOperator;
	translator[bytecode_CRLTRCK] = ClearTrackingOperator;
	translator[bytecode_HCKTRCK] = HockTrackingOperator;
	translator[bytecode_SPWN] = SpawnObjectOperator;
	translator[bytecode_TRVL] = TravelObjectOperator;
	translator[bytecode_GLUOBJCT] = GlueObjectOperator;

	translator[bytecode_SVESCPE] = ScopeSaveOperator;
	translator[bytecode_SVESTT] = SaveStateOperator;
	translator[bytecode_LDSTT] = LoadStateOperator;
	translator[bytecode_CLRBRD] = ClearBoardOperator;

	translator[bytecode_PSEGME] = PauseGameOperator;
	translator[bytecode_RSMGME] = ResumeGameOperator;

	translator[bytecode_AND] = AndOperator;
	translator[bytecode_OR] = OrOperator;

	translator[bytecode_ADDTSKPARNUM] = AddTaskParamNumOperator;
	translator[bytecode_ADDTSKPARFLT] = AddTaskParamFloatOperator;
	translator[bytecode_ADDTSKPARSTR] = AddTaskParamStrOperator;

	translator[bytecode_QSTSK] = QueueSystemTaskOperator;
	translator[bytecode_QCTSK] = QueueClockTaskOperator;
	translator[bytecode_RMTSK] = RemoveTaskOperator;

	translator[bytecode_RUTE] = RouteOperator;

	translator[bytecode_NUMARR] = ArrayNumOperator;
	translator[bytecode_STRARR] = ArrayStrOperator;
	translator[bytecode_NUMOFFST] = OffsetNumOperator;
	translator[bytecode_STROFFST] = OffsetStrOperator;

	translator[bytecode_JMPINVVAR] = JumpInv;
	translator[bytecode_JMPNOW] = JumpNow;

	translator[bytecode_WTFRVR] = WaitForVar;

}

void KubLingRawTranslator::SetRTV(CelestialSlicedList<heapVar>* heap,
	unsigned int* jmpPlaceHolders,
	unsigned int* translation,
	KubLingCompiled** byteCodes,
	unsigned int compiled,
	unsigned int current,
	MemoryPool* memPool,
	CelestialStack<unsigned int>* memoffsetPH)
{

	rtv.byteCodes = byteCodes;
	rtv.compiled = compiled;
	rtv.current = current;
	rtv.heap = heap;
	rtv.jmpPlaceHolders = jmpPlaceHolders;
	rtv.memory = memPool;
	rtv.translation = translation;
	rtv.memoffsetPH = memoffsetPH;

}

rawCode KubLingRawTranslator::Translate(unsigned char* bytes,
	unsigned int byteSize,
	unsigned int codeOffset,
	unsigned int line)
{

	unsigned int retVal = bytes[0] | ((int)bytes[1] << 8) | ((int)bytes[2] << 16) | ((int)bytes[3] << 24);
	rawCode raw;
	raw.codeSize = 0;
	raw.maxLines = 0;
	raw.start = codeOffset;

	if (retVal > 0)
	{

		commonLoad(retVal - 1, rtv, bytes[4]);

	}

	RunTimeError re = translator[bytes[4]](&raw, retVal, &bytes[5], byteSize - 5, codeOffset, line,rtv);
	return raw;

}


KubLingRawTranslator::~KubLingRawTranslator()
{

	delete[] translator;

}
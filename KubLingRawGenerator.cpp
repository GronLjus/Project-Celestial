#include "stdafx.h"
#include "KubLingRawGenerator.h"

using namespace Logic;
using namespace Resources;

KubLingRawGenerator::KubLingRawGenerator()
{

	maxLabels = 0;
	totalLabels = 0;
	labels = nullptr;
	expandLabels();
	totalCode = 0;
	translator = new KubLingRawTranslator();

}

void KubLingRawGenerator::expandLabels()
{

	maxLabels += 10;
	KubLingLabel* newLabels = new KubLingLabel[maxLabels];

	if (labels != nullptr)
	{

		for (unsigned int i = 0; i < totalLabels; i++)
		{

			newLabels[i] = KubLingLabel(labels[i]);

		}

		delete[] labels;


	}

	labels = newLabels;
		 
}

rawCode KubLingRawGenerator::expandBlock(rawCode code, unsigned int newMax)
{

	rawCode newBlock;
	newBlock.codeSize = code.codeSize;
	newBlock.start = code.start;
	newBlock.maxLines = code.maxLines = newMax;
	newBlock.code = new rawCode::line[newBlock.maxLines];

	memcpy(newBlock.code, code.code, code.codeSize * sizeof(rawCode::line));

	delete[] code.code;

	return newBlock;

}

rawCode KubLingRawGenerator::translateBlock(KubLingCompiled* byteCode,
	CelestialSlicedList<heapVar>* heap,
	unsigned int start,
	unsigned stop,
	unsigned int* jmpPlaceHolder,
	unsigned int* translations,
	unsigned int current,
	MemoryPool* memPool)
{

	rawCode block;
	block.codeSize = 0;
	block.maxLines = 100;
	block.code = new rawCode::line[block.maxLines];
	block.start = totalCode;
	translator->SetRTV(heap, jmpPlaceHolder, translations, byteCodes, compiled, current, memPool);

	for (unsigned int i = start; i < stop; i++)
	{

		translations[i] = totalCode;

		int size;
		unsigned char* line = byteCode->GetCode(size, i);
		rawCode code = translator->Translate(line, size, totalCode, i);

		if (code.codeSize > block.maxLines - block.codeSize)//Expand the block
		{

			block = expandBlock(block, block.maxLines + code.codeSize + 100);

		}

		memcpy(&block.code[block.codeSize], code.code, code.codeSize * sizeof(rawCode::line));
		block.codeSize += code.codeSize;
		delete[] code.code;
		totalCode += code.codeSize;

	}

	return block;

}

rawCode KubLingRawGenerator::createInitLowerFoot(heapVar initData, heapVar initDataSize, MemoryPool* memPol)
{

	rawCode lowerFooter;
	lowerFooter.start = 0;
	lowerFooter.codeSize = 9;
	totalCode += lowerFooter.codeSize;
	lowerFooter.maxLines = lowerFooter.codeSize;
	lowerFooter.code = new rawCode::line[lowerFooter.maxLines];

	rawCode::line line;
	// This is where the machine will jump when the script is already inited
	//Move the inited block from the heap to the stack

	//Place The initblocksize var in i2
	line.code = opcode_PLACE;
	line.r1 = 1;
	line.type = 0;
	line.scale = initDataSize.var;
	lowerFooter.code[0] = line;
	//Get the address of the var in i2 and place it in i2
	line.code = opcode_ADR;
	line.r1 = 1;
	line.r2 = 1;
	lowerFooter.code[1] = line;
	//Place the starting adr on i3 so we can move memory from the heap
	line.code = opcode_PLACE;
	line.r1 = 2;
	line.type = 0;
	line.scale = memPol->GetStartingAdr();
	lowerFooter.code[2] = line;
	//Place 4 on i1 so we can move memory from the heap
	line.code = opcode_PLACE;
	line.r1 = 0;
	line.type = 0;
	line.scale = 4;
	lowerFooter.code[3] = line;
	//Load data from the heap on adr i2 to the stack with adr i3 and the size i0
	line.code = opcode_MOV;
	line.r1 = 2;
	line.r2 = 1;
	line.r3 = 0;
	line.type = 0;
	line.scale = 0;
	lowerFooter.code[4] = line;
	//Move data from the stack with adr i3 to i2
	line.code = opcode_LOAD;
	line.r1 = 1;
	line.r2 = 0;
	line.type = 0;
	lowerFooter.code[5] = line;
	//Place The initblock var in i1
	line.code = opcode_PLACE;
	line.r1 = 0;
	line.type = 0;
	line.scale = initData.var;
	lowerFooter.code[6] = line;
	//Load the adr of var in i1 to i1
	line.code = opcode_ADR;
	line.r1 = 0;
	line.r2 = 0;
	lowerFooter.code[7] = line;
	//Move data from the address in i1 to the place in i3 with the size in i2
	line.code = opcode_MOV;
	line.r1 = 2;
	line.r2 = 0;
	line.r3 = 1;
	lowerFooter.code[8] = line;

	return lowerFooter;

}

rawCode KubLingRawGenerator::createInitUpperFoot(heapVar initData, heapVar initDataSize, MemoryPool* memPool)
{

	rawCode upperFooter;
	upperFooter.start = 0;
	upperFooter.codeSize = 11;
	totalCode += upperFooter.codeSize;
	upperFooter.maxLines = upperFooter.codeSize;
	upperFooter.code = new rawCode::line[upperFooter.maxLines];

	rawCode::line line;
	//This is the footer code to save the block on the heap
	//Place 4 on i1 so we can save the initblocksize on the heap
	line.code = opcode_PLACE;
	line.r1 = 0;
	line.type = 0;
	line.scale = 4;
	upperFooter.code[0] = line;
	//Place the initBlockSize var on i3
	line.code = opcode_PLACE;
	line.r1 = 2;
	line.type = 0;
	line.scale = initDataSize.var;
	upperFooter.code[1] = line;
	//Allocate the adress for the variable in i3 with size i1 and store it in i3
	line.code = opcode_ALLOC;
	line.r1 = 2;
	line.r2 = 0;
	line.r3 = 2;
	upperFooter.code[2] = line;
	//Place the stackadr in i2
	line.code = opcode_PLACE;
	line.r1 = 1;
	line.type = 0;
	line.scale = memPool->GetMemorySize();
	upperFooter.code[3] = line;
	//Save the scaler value to the stackadr in i2 with the size in i1
	line.code = opcode_SAVE;
	line.r1 = 2;
	line.r2 = 0;
	line.type = 0;
	line.scale = memPool->GetMemorySize();
	upperFooter.code[4] = line;
	//Save the value on the adr in i2 to the adr in i3 with the size in i1
	line.code = opcode_HEAP;
	line.r1 = 2;
	line.r2 = 1;
	line.r3 = 0;
	upperFooter.code[5] = line;

	//Place the heap var on i2
	line.code = opcode_PLACE;
	line.r1 = 1;
	line.type = 0;
	line.scale = initData.var;
	upperFooter.code[6] = line;
	//Place the size of the stack memory on i3 so we can allocate the init
	line.code = opcode_PLACE;
	line.r1 = 2;
	line.type = 0;
	line.scale = memPool->GetMemorySize();
	upperFooter.code[7] = line;
	//Allocate the adress for the variable in i2 with size i3 and store it in i1
	line.code = opcode_ALLOC;
	line.r1 = 1;
	line.r2 = 2;
	line.r3 = 0;
	upperFooter.code[8] = line;
	//Place the memory adr in i2
	line.code = opcode_PLACE;
	line.r1 = 1;
	line.type = 0;
	line.scale = memPool->GetStartingAdr();
	upperFooter.code[9] = line;
	//Save the stack on place i2 on the heapadr i1 with the size in i3
	line.code = opcode_HEAP;
	line.r1 = 0;
	line.r2 = 1;
	line.r2 = 2;
	upperFooter.code[10] = line;

	return upperFooter;

}

rawCode KubLingRawGenerator::createInitHeader(heapVar initData)
{

	rawCode headerBlock;
	headerBlock.start = 0;
	headerBlock.codeSize = 6;
	headerBlock.maxLines = headerBlock.codeSize;
	headerBlock.code = new rawCode::line[headerBlock.maxLines];

	//Place the heap var on i1
	rawCode::line line;
	line.code = opcode_PLACE;
	line.r1 = 0;
	line.type = 0;
	line.scale = initData.var;
	headerBlock.code[0] = line;
	//Get the address of the var on i1 to i2
	line.code = opcode_ADR;
	line.r1 = 0;
	line.r2 = 1;
	line.type = 0;
	headerBlock.code[1] = line;
	//Place 0 on i3 so we can compare it to i2
	line.code = opcode_PLACE;
	line.r1 = 2;
	line.type = 0;
	line.scale = 0;
	headerBlock.code[2] = line;
	//Compare i3 to i2 and place the result in c1
	line.code = opcode_CMPRE;
	line.r1 = 2;
	line.r2 = 1;
	line.r3 = 0;
	line.type = 0;
	headerBlock.code[3] = line;
	//Place the footer adr in i1 
	line.code = opcode_PLACE;
	line.r1 = 0;
	line.type = 0;
	line.scale = 0;
	headerBlock.code[4] = line;
	//Jump over the initblock if the value in c1 is not 0
	line.code = opcode_JMPINV;
	line.r1 = 0;
	line.r2 = 0;
	headerBlock.code[5] = line;

	return headerBlock;

}

rawCode KubLingRawGenerator::createInitBlock(KubLingCompiled* byteCode,
	unsigned int current,
	CelestialSlicedList<heapVar>* heap,
	unsigned int initLength,
	unsigned int* jmpPlaceHolders,
	unsigned int* translation,
	MemoryPool* memPool)
{

	rawCode initBlock;
	initBlock.start = totalCode;

	heapVar isInit;
	isInit.name = "§init" + byteCode->GetName();
	isInit.var = heap->Add(isInit);
	heapVar initBlockSize;
	initBlockSize.name = "§initSize" + byteCode->GetName();
	initBlockSize.var = heap->Add(initBlockSize);

	rawCode initHeader = createInitHeader(isInit);
	totalCode += initHeader.codeSize;
	rawCode userInit = translateBlock(byteCode, heap, 1, initLength, jmpPlaceHolders, translation,current, memPool);
	rawCode initUFooter = createInitUpperFoot(isInit, initBlockSize, memPool);
	initHeader.code[4].scale = totalCode;
	rawCode initLFooter = createInitLowerFoot(isInit, initBlockSize, memPool);


	initBlock.codeSize = userInit.codeSize + initHeader.codeSize + initUFooter.codeSize + initLFooter.codeSize;
	initBlock.maxLines = initBlock.codeSize + 1;
	initBlock.code = new rawCode::line[initBlock.maxLines];

	memcpy(&initBlock.code[0], initHeader.code, initHeader.codeSize * sizeof(rawCode::line));
	memcpy(&initBlock.code[initHeader.codeSize], userInit.code, userInit.codeSize * sizeof(rawCode::line));
	memcpy(&initBlock.code[initHeader.codeSize + userInit.codeSize], initUFooter.code, initUFooter.codeSize * sizeof(rawCode::line));
	memcpy(&initBlock.code[initHeader.codeSize + userInit.codeSize + initUFooter.codeSize], initLFooter.code, initLFooter.codeSize * sizeof(rawCode::line));

	delete[] initHeader.code;
	delete[] userInit.code;
	delete[] initUFooter.code;
	delete[] initLFooter.code;

	return initBlock;

}

rawCode KubLingRawGenerator::createFooterBlock()
{

	rawCode footer;
	footer.start = 0;
	footer.codeSize = 4;
	totalCode += footer.codeSize;
	footer.maxLines = footer.codeSize;
	footer.code = new rawCode::line[footer.maxLines];

	rawCode::line line;
	//This is the footer code to return control up or exit
	//Place 0 on i1 so we can compare it with i4
	line.code = opcode_PLACE;
	line.r1 = 0;
	line.type = 0;
	line.scale = 0;
	footer.code[0] = line;
	//Compare i1 with i4 and store result in c1
	line.code = opcode_CMPRE;
	line.r1 = 0;
	line.r2 = 3;
	line.r3 = 0;
	line.type = 0;
	footer.code[1] = line;
	//Jump to adr in i4 if c1 isn't 0
	line.code = opcode_JMPINV;
	line.r1 = 0;
	line.r2 = 3;
	footer.code[2] = line;
	//Halt execution
	line.code = opcode_STOP;
	line.r1 = 0;
	line.r2 = 0;
	line.type = 0;
	footer.code[3] = line;

	return footer;

}

void KubLingRawGenerator::addLabel(KubLingCompiled* byteCode)
{

	if (totalLabels >= maxLabels)
	{

		expandLabels();

	}

	unsigned int* tempParams[4];
	tempParams[0] = byteCode->GetMaxParams('n') > 0 ? new unsigned int[byteCode->GetMaxParams('n')] : nullptr;
	tempParams[1] = byteCode->GetMaxParams('f') > 0 ? new unsigned int[byteCode->GetMaxParams('f')] : nullptr;;
	tempParams[2] = byteCode->GetMaxParams('s') > 0 ? new unsigned int[byteCode->GetMaxParams('n')] : nullptr;;
	tempParams[3] = new unsigned int[RunTimeParams_NA];
	
	for (unsigned int i = 0; i < byteCode->GetMaxParams('n'); i++)
	{

		tempParams[0][i] = byteCode->GetAdr(i, 'n');

	}

	for (unsigned int i = 0; i < byteCode->GetMaxParams('f'); i++)
	{

		tempParams[1][i] = byteCode->GetAdr(i, 'f');

	}

	for (unsigned int i = 0; i < byteCode->GetMaxParams('s'); i++)
	{

		tempParams[2][i] = byteCode->GetAdr(i, 's');

	}

	for (unsigned int i = 0; i < RunTimeParams_NA; i++)
	{

		tempParams[3][i] = byteCode->GetAdr(RunTimeParams(i));

	}

	labels[totalLabels] = KubLingLabel(byteCode->GetName(), totalCode, tempParams);
	totalLabels++;

}

rawCode KubLingRawGenerator::assemble(KubLingCompiled* byteCode, unsigned int current , CelestialSlicedList<heapVar>* heap)
{

	unsigned int* jmpPlaceHolders = new unsigned int[byteCode->GetCodeSize()];
	unsigned int* lineTranslation = new unsigned int[byteCode->GetCodeSize()];

	for (unsigned int i = 0; i < byteCode->GetCodeSize(); i++)
	{

		jmpPlaceHolders[i] = 0;

	}

	rawCode assembled;
	assembled.maxLines = 100;
	assembled.codeSize = 0;
	assembled.start = totalCode;
	assembled.code = new rawCode::line[assembled.maxLines];

	MemoryPool* stackMem = new MemoryPool(1024);
	stackMem->AddSystemMem(byteCode);

	addLabel(byteCode);

	int size;
	unsigned char* line = byteCode->GetCode(size, 0);
	unsigned int scriptStart;
	memcpy(&scriptStart, line, size);

	if (scriptStart != 0)
	{

		rawCode initBlock = createInitBlock(byteCode, current, heap, scriptStart, jmpPlaceHolders, lineTranslation, stackMem);

		if (initBlock.codeSize > assembled.maxLines - assembled.codeSize)//Expand the block
		{

			assembled = expandBlock(assembled, assembled.maxLines + initBlock.codeSize + 100);

		}

		memcpy(&assembled.code[assembled.codeSize], initBlock.code, initBlock.codeSize * sizeof(rawCode::line));
		assembled.codeSize += initBlock.codeSize;
		delete[] initBlock.code;
		
	}

	rawCode code = translateBlock(byteCode, heap, scriptStart, byteCode->GetCodeSize(), jmpPlaceHolders, lineTranslation, current, stackMem);
	rawCode footer = createFooterBlock();

	if (code.codeSize + footer.codeSize > assembled.maxLines - assembled.codeSize)//Expand the block
	{

		assembled = expandBlock(assembled, assembled.maxLines + code.codeSize + footer.codeSize);

	}

	memcpy(&assembled.code[assembled.codeSize], code.code, code.codeSize * sizeof(rawCode::line));
	assembled.codeSize += code.codeSize;
	memcpy(&assembled.code[assembled.codeSize], footer.code, footer.codeSize * sizeof(rawCode::line));
	assembled.codeSize += footer.codeSize;

	totalOffset += stackMem->GetMemorySize();
	byteCode->SetMemOffset(totalOffset);

	delete[] code.code;
	delete[] footer.code;

	//Replace the jump placeholders
	for (unsigned int i = 0; i < byteCode->GetCodeSize(); i++)
	{

		if (jmpPlaceHolders[i] != 0)
		{

			unsigned int line = jmpPlaceHolders[i] - 1 - assembled.start;
			assembled.code[line].scale = lineTranslation[assembled.code[line].scale];

		}
	}

	delete[] lineTranslation;
	delete[] jmpPlaceHolders;
	delete stackMem;

	return assembled;

}

KubLingRaw* KubLingRawGenerator::Assemble(KubLingCompiled** byteCodes, unsigned int compiled)
{

	totalOffset = 0;
	totalCode = 0;
	totalLabels = 0;

	this->byteCodes = byteCodes;
	this->compiled = compiled;

	rawCode* raws = new rawCode[compiled];
	CelestialSlicedList<heapVar>* heap = new CelestialSlicedList<heapVar>(32);

	//Generate code
	for (unsigned int i = 0; i < compiled;i++)
	{ 
	
		raws[i] = assemble(byteCodes[i], i, heap);

	}

	rawCode::line* lines = new rawCode::line[totalCode];
	unsigned int line = 0;

	//Combine code
	for (unsigned int i = 0; i < compiled; i++)
	{

		memcpy(&lines[line], raws[i].code, raws[i].codeSize * sizeof(rawCode::line));
		delete[] raws[i].code;
		line += raws[i].codeSize;

	}

	//Replace placeholders
	for (unsigned int i = 0; i < compiled; i++)
	{

		unsigned int ph = byteCodes[i]->GetStackPHLine();

		while (ph != 0)
		{

			ph--;
			lines[ph].scale = raws[i].start;
			ph = byteCodes[i]->GetStackPHLine();

		}

		ph = byteCodes[i]->GetMemOffsetPHLine();

		while (ph != 0)
		{

			ph--;
			lines[ph].scale = byteCodes[i]->GetMemOffset();
			ph = byteCodes[i]->GetMemOffsetPHLine();

		}
	}

	unsigned long long* code = new unsigned long long[totalCode];

	//Compile code
	for (unsigned int i = 0; i < totalCode; i++)
	{

		unsigned long long line = (lines[i].code << 59) |
			(lines[i].r1 << 62) >> 5 |
			(lines[i].r2 << 62) >> 7 |
			(lines[i].r3 << 62) >> 9 |
			(lines[i].type << 62) >> 11 |
			(lines[i].scale << 32) >> 32;
		code[i] = line;
		i++;

	}

	KubLingLabel* rawLabels = new KubLingLabel[totalLabels];

	for (unsigned int i = 0; i < totalLabels; i++)
	{

		rawLabels[i] = KubLingLabel(labels[i]);
		rawLabels[i].Reset();
		unsigned int x = 0;

	}

	KubLingRaw* retVal = new KubLingRaw(code, totalCode, rawLabels, totalLabels, heap->GetHighest());

	delete[] raws;
	delete heap;
	totalOffset = 0;
	totalCode = 0;
	totalLabels = 0;

	return retVal;

}

KubLingRawGenerator::~KubLingRawGenerator()
{

	delete[] labels;
	delete translator;

}
#include "stdafx.h"
#include "KubLingRawGenerator.h"

using namespace Logic;
using namespace Resources;

KubLingRawGenerator::KubLingRawGenerator(unsigned int maxStack)
{

	maxLabels = 0;
	totalLabels = 0;
	labels = nullptr;
	expandLabels();
	totalCode = 0;
	translator = new KubLingRawTranslator();
	this->maxStack = maxStack;

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
	unsigned int stop,
	unsigned int* translations)
{

	rawCode block;
	block.codeSize = 0;
	block.maxLines = 100;
	block.code = new rawCode::line[block.maxLines];
	block.start = totalCode;

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

	translations[stop] = totalCode;
	return block;

}

rawCode KubLingRawGenerator::createInitHeader(heapVar initData)
{

	rawCode headerBlock;
	headerBlock.start = 0;
	headerBlock.codeSize = 9;
	headerBlock.maxLines = headerBlock.codeSize;
	headerBlock.code = new rawCode::line[headerBlock.maxLines];

	rawCode::line line;
	//Place the heap var on a1
	line.code = opcode_PLACE;
	line.r1 = 0;
	line.type = 3;
	line.scale = initData.var;
	headerBlock.code[0] = line;
	//Get the address of the var on a1 to i2
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
	//Place 1 in i2 
	line.code = opcode_PLACE;
	line.r1 = 1;
	line.type = 0;
	line.scale = 1;
	headerBlock.code[6] = line;
	//Place the heap var on i1
	line.code = opcode_PLACE;
	line.r1 = 0;
	line.type = 0;
	line.scale = initData.var;
	headerBlock.code[7] = line;
	//Allocate i2 bytes for the var in i1
	line.code = opcode_ALLOC;
	line.r1 = 0;
	line.r2 = 1;
	line.r3 = 2;
	headerBlock.code[8] = line;

	return headerBlock;

}

rawCode KubLingRawGenerator::createInitBlock(KubLingCompiled* byteCode,
	CelestialSlicedList<heapVar>* heap,
	unsigned int initLength,
	unsigned int* translation)
{

	rawCode initBlock;
	initBlock.start = totalCode;

	heapVar isInit;
	isInit.name = "§init" + byteCode->GetName();
	isInit.var = heap->Add(isInit);

	rawCode initHeader = createInitHeader(isInit);
	totalCode += initHeader.codeSize;
	rawCode userInit = translateBlock(byteCode, heap, 1, initLength, translation);
	initHeader.code[4].scale = totalCode;


	initBlock.codeSize = userInit.codeSize + initHeader.codeSize;
	initBlock.maxLines = initBlock.codeSize + 1;
	initBlock.code = new rawCode::line[initBlock.maxLines];

	memcpy(&initBlock.code[0], initHeader.code, initHeader.codeSize * sizeof(rawCode::line));
	memcpy(&initBlock.code[initHeader.codeSize], userInit.code, userInit.codeSize * sizeof(rawCode::line));

	delete[] initHeader.code;
	delete[] userInit.code;

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
	//Jump to adr in i4 if c1 is 0
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
	tempParams[1] = byteCode->GetMaxParams('f') > 0 ? new unsigned int[byteCode->GetMaxParams('f')] : nullptr;
	tempParams[2] = byteCode->GetMaxParams('s') > 0 ? new unsigned int[byteCode->GetMaxParams('n')] : nullptr;
	tempParams[3] = new unsigned int[RunTimeParams_NA];

	unsigned int tempMax[4];
	tempMax[0] = byteCode->GetMaxParams('n');
	tempMax[1] = byteCode->GetMaxParams('f');
	tempMax[2] = byteCode->GetMaxParams('s');
	tempMax[3] = RunTimeParams_NA;
	
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

	labels[totalLabels] = KubLingLabel(byteCode->GetName(), totalCode, tempParams, tempMax);
	totalLabels++;

}

rawCode KubLingRawGenerator::assemble(KubLingCompiled* byteCode, unsigned int current, CelestialSlicedList<heapVar>* heap)
{

	unsigned int* jmpPlaceHolders = new unsigned int[byteCode->GetCodeSize()];
	unsigned int* lineTranslation = new unsigned int[byteCode->GetCodeSize() + 1];

	for (unsigned int i = 0; i < byteCode->GetCodeSize(); i++)
	{

		jmpPlaceHolders[i] = 0;

	}

	rawCode assembled;
	assembled.maxLines = 100;
	assembled.codeSize = 0;
	assembled.start = totalCode;
	assembled.code = new rawCode::line[assembled.maxLines];

	MemoryPool* stackMem = new MemoryPool(maxStack);
	stackMem->AddSystemMem(byteCode);

	addLabel(byteCode);

	int size;
	unsigned char* line = byteCode->GetCode(size, 0);
	unsigned int scriptStart;
	memcpy(&scriptStart, line, size);

	translator->SetRTV(heap, jmpPlaceHolders, lineTranslation, byteCodes, compiled, current, stackMem, nullptr);

	if (scriptStart != 1)
	{

		rawCode initBlock = createInitBlock(byteCode, heap, scriptStart, lineTranslation);

		if (initBlock.codeSize > assembled.maxLines - assembled.codeSize)//Expand the block
		{

			assembled = expandBlock(assembled, assembled.maxLines + initBlock.codeSize + 100);

		}

		memcpy(&assembled.code[assembled.codeSize], initBlock.code, initBlock.codeSize * sizeof(rawCode::line));
		assembled.codeSize += initBlock.codeSize;
		delete[] initBlock.code;
		
	}

	rawCode code = translateBlock(byteCode, heap, scriptStart, byteCode->GetCodeSize(), lineTranslation);
	rawCode footer = createFooterBlock();

	if (code.codeSize + footer.codeSize > assembled.maxLines - assembled.codeSize)//Expand the block
	{

		assembled = expandBlock(assembled, assembled.maxLines + code.codeSize + footer.codeSize);

	}

	memcpy(&assembled.code[assembled.codeSize], code.code, code.codeSize * sizeof(rawCode::line));
	assembled.codeSize += code.codeSize;
	memcpy(&assembled.code[assembled.codeSize], footer.code, footer.codeSize * sizeof(rawCode::line));
	assembled.codeSize += footer.codeSize;

	byteCode->SetMemOffset(totalOffset);
	totalOffset += stackMem->GetMemorySize();

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

	//Set up the system variables
	CelestialSlicedList<heapVar>* heap = new CelestialSlicedList<heapVar>(32);

	for (unsigned int i = 0; i < SystemMem_NA; i++)
	{

		heapVar var;
		var.var = SystemVars[i].var;
		var.name = SystemVars[i].name;

		heap->Add(var);

	}

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

	KubLingLabel* rawLabels = new KubLingLabel[totalLabels];
	unsigned int mOffset = 0;

	for (unsigned int i = 0; i < totalLabels; i++)
	{

		rawLabels[i] = KubLingLabel(labels[i]);
		rawLabels[i].Reset();
		bool stop = false;

		for (unsigned int k = 0; k < compiled && !stop; k++)
		{

			stop = labels[i].GetName() == byteCodes[k]->GetName();

			if (stop)
			{

				rawLabels[i].SetMemOffset(mOffset);
				unsigned int ph = byteCodes[k]->GetStackPHLine();

				//Replace Placeholders
				while (ph != 0)
				{

					ph--;
					lines[ph].scale = rawLabels[i].GetStart();
					ph = byteCodes[k]->GetStackPHLine();

				}

				ph = byteCodes[k]->GetMemOffsetPHLine();

				while (ph != 0)
				{

					ph--;
					lines[ph].scale = mOffset;
					ph = byteCodes[k]->GetMemOffsetPHLine();

				}

				mOffset = byteCodes[k]->GetMemOffset();

			}
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
			(lines[i].type << 43) >> 11 |
			(lines[i].scale << 32) >> 32;
		code[i] = line;

	}

	KubLingRaw* retVal = new KubLingRaw(code, totalCode, rawLabels, totalLabels, heap->GetHighest(), totalOffset);

	delete[] lines;
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
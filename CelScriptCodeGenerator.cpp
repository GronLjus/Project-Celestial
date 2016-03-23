#include "stdafx.h"
#include "CelScriptCodeGenerator.h"

using namespace CrossHandlers;
using namespace Logic;

int CelScriptCodeGenerator::handleVariable(CelestialSlicedList<symbol>* symbolTable, int varAdr, CelestialList<int>* varClear, CelestialList<CelestialList<unsigned char>*>* commands, unsigned int &codeCount)
{

	symbol sym = symbolTable->GetValue(varAdr);
	
	if (sym.name[0] == '?' && sym.address == 0)
	{

		sym.address = var;

	}
	else
	{

		sym.address = sym.address != 0 ? sym.address : holes->GetFirstNode() != nullptr ? holes->PopElement() : var;

	}

	if (var == sym.address)
	{

		var++;

	}

	sym.codeCount++;

	if (sym.codeCount == sym.symCount && sym.gC)
	{

		varClear->PushElement(sym.address);

	}

	if (sym.waitFlag)
	{

		sym.waitFlag = false;
		CelestialList<unsigned char>* addLine = new CelestialList<unsigned char>();
		addLine->AddElement((unsigned char)0);
		addLine->AddElement((unsigned char)0);
		addLine->AddElement((unsigned char)0);
		addLine->AddElement((unsigned char)0);
		addLine->AddElement(opCode_WTFRVR);
		addLine->AddElement(sym.address >> 0);
		addLine->AddElement(sym.address >> 8);
		addLine->AddElement(sym.address >> 16);
		addLine->AddElement(sym.address >> 24);
		commands->AddElement(addLine);
		codeCount++;

	}

	symbolTable->Add(sym, varAdr);
	return sym.address;

}

CelestialList<CelestialList<unsigned char>*>* CelScriptCodeGenerator::generateCode(CelestialTreeNode<syntax>* ast, CelestialSlicedList<symbol>* symbolTable,unsigned int &codeCount)
{

	CelestialTreeNode<syntax>* node = ast;
	CelestialList<unsigned char>* line = new CelestialList<unsigned char>();
	CelestialList<CelestialList<unsigned char>*>* commands = new CelestialList<CelestialList<unsigned char>*>();
	CelestialListNode<CelestialTreeNode<syntax>*>* branch = node->GetLeafs()->GetFirstNode();
	unsigned char opCode = std::stoi(node->GetNodeObject().val);
	bool apuConstOp = opCode == opcode_SUM2CONST || opCode == opcode_SUB2CONST || opCode == opcode_MUL2CONST || opCode == opcode_DIV2CONST || opCode == opcode_NUMEQUAL2CONST || opCode == opcode_NUMGRT2CONST || opCode == opcode_NEGATECONST;
	CelestialList<int>* varClear = new CelestialList<int>();

	line->AddElement(opRetVar >> 0);
	line->AddElement(opRetVar >> 8);
	line->AddElement(opRetVar >> 16);
	line->AddElement(opRetVar >> 24);

	if (opRetVar != 0)
	{

		opRetVar = 0;

	}

	if (node->GetNodeObject().type == SyntaxType_FLOW)
	{

		bool postedLine = false;
		int bodLine = 0;
		bool isIfBlock = opCode == FlowType_BRANCH;
		bool isLoopBlock = opCode == FlowType_LOOP;
		unsigned int startCheck = 0;
		CelestialList<CelestialList<unsigned char>*>* generatedLines = nullptr;
		int cons = 0;

		while (branch != nullptr)
		{


			if (std::stoi(branch->GetNodeObject()->GetNodeObject().val) == FlowType_COND)
			{

				line->AddElement(opcode_JMPINVVAR);
				CelestialListNode<CelestialTreeNode<syntax>*>* conLine = branch->GetNodeObject()->GetLeafs()->GetFirstNode();
				startCheck = codeCount;

				while (conLine != nullptr)
				{

					int adr = 0;

					if (conLine->GetNodeObject()->GetNodeObject().type == SyntaxType_OPERATORVAR ||
						conLine->GetNodeObject()->GetNodeObject().type == SyntaxType_OPERATORCONST ||
						conLine->GetNodeObject()->GetNodeObject().type == SyntaxType_OPERATORCONSTPASS ||
						conLine->GetNodeObject()->GetNodeObject().type == SyntaxType_OPERATORNA)
					{

						adr = holes->GetFirstNode() != nullptr ? holes->PopElement() : var;

						if (var == adr)
						{

							var++;

						}

						opRetVar = adr;
						generatedLines = generateCode(conLine->GetNodeObject(), symbolTable, codeCount);
						varClear->PushElement(adr);

					}
					else if (conLine->GetNodeObject()->GetNodeObject().type == SyntaxType_VAR)
					{

						int varAdr = std::stoi(conLine->GetNodeObject()->GetNodeObject().val);
						adr = handleVariable(symbolTable, varAdr, varClear,commands,codeCount);

					}

					if (cons == 0 && branch->GetNodeObject()->GetLeafs()->GetCount() == 1 || cons == 1 && branch->GetNodeObject()->GetLeafs()->GetCount() > 1)
					{


						line->AddElement(adr >> 0);
						line->AddElement(adr >> 8);
						line->AddElement(adr >> 16);
						line->AddElement(adr >> 24);

					}
					else if (cons == 0)
					{

						startCheck = codeCount;

					}

					if (generatedLines != nullptr && cons != 2)
					{

						commands->AddElement(generatedLines);
						delete generatedLines;
						generatedLines = nullptr;

					}

					cons++;
					conLine = conLine->GetNext();

				}

				codeCount++;
				commands->AddElement(line);
				postedLine = true;

			}
			else if (std::stoi(branch->GetNodeObject()->GetNodeObject().val) == FlowType_BODY)
			{

				CelestialListNode<CelestialTreeNode<syntax>*>* bodyLines = branch->GetNodeObject()->GetLeafs()->GetFirstNode();

				while (bodyLines != nullptr)
				{

					CelestialList<CelestialList<unsigned char>*>* genLines = generateCode(bodyLines->GetNodeObject(), symbolTable, codeCount);
					commands->AddElement(genLines);
					delete genLines;
					bodyLines = bodyLines->GetNext();

				}

				bodLine = codeCount;

				if (isLoopBlock)
				{

					bodLine++;

					if (cons == 3 && generatedLines != nullptr)
					{

						commands->AddElement(generatedLines);
						delete generatedLines;
						generatedLines = nullptr;

					}

					CelestialList<unsigned char>* finishLine = new CelestialList<unsigned char>();
					commands->AddElement(finishLine);
					finishLine->AddElement((unsigned char)0);
					finishLine->AddElement((unsigned char)0);
					finishLine->AddElement((unsigned char)0);
					finishLine->AddElement((unsigned char)0);
					codeCount++;
					finishLine->AddElement(opcode_JMPNOW);

					finishLine->AddElement(startCheck >> 0);
					finishLine->AddElement(startCheck >> 8);
					finishLine->AddElement(startCheck >> 16);
					finishLine->AddElement(startCheck >> 24);

				}

			}
			else if (std::stoi(branch->GetNodeObject()->GetNodeObject().val) == FlowType_BRANCH)
			{

				if (isIfBlock)
				{

					CelestialList<unsigned char>* finishLine = new CelestialList<unsigned char>();
					commands->AddElement(finishLine);
					finishLine->AddElement((unsigned char)0);
					finishLine->AddElement((unsigned char)0);
					finishLine->AddElement((unsigned char)0);
					finishLine->AddElement((unsigned char)0);
					codeCount++;

					bodLine++;
					finishLine->AddElement(opcode_JMPNOW);
					CelestialList<CelestialList<unsigned char>*>* generatedLines = generateCode(branch->GetNodeObject(), symbolTable, codeCount);

					finishLine->AddElement(codeCount >> 0);
					finishLine->AddElement(codeCount >> 8);
					finishLine->AddElement(codeCount >> 16);
					finishLine->AddElement(codeCount >> 24);
					commands->AddElement(generatedLines);
					delete generatedLines;

				}
			}

			branch = branch->GetNext();

		}

		if (!postedLine)
		{

			delete line;

		}
		else
		{

			line->AddElement(bodLine >> 0);
			line->AddElement(bodLine >> 8);
			line->AddElement(bodLine >> 16);
			line->AddElement(bodLine >> 24);

		}
	}
	else
	{

		line->AddElement(opCode);
		unsigned int varAdr = 0;
		float apuConst1 = 0;
		float apuConst2 = 0;
		bool apuFloat = false;

		bool apuConst1In = false;
		bool setOpVar = false;

		while (branch != nullptr)
		{

			syntax syn = branch->GetNodeObject()->GetNodeObject();

			if (syn.type == SyntaxType_VAR)
			{

				varAdr = std::stoi(syn.val);
				int adr = handleVariable(symbolTable, varAdr, varClear, commands, codeCount);
				line->AddElement(adr >> 0);
				line->AddElement(adr >> 8);
				line->AddElement(adr >> 16);
				line->AddElement(adr >> 24);

			}
			else if (syn.type == SyntaxType_CONST || syn.type == SyntaxType_CONSTPASS || syn.type == SyntaxType_OPERATORCONST || syn.type == SyntaxType_OPERATORCONSTPASS)
			{

				CelestialList<unsigned char>* addLine = line;
				std::string constToAdd = syn.val;
				SyntaxType toDo = syn.type;
				toDo = toDo == SyntaxType_OPERATORCONST ? SyntaxType_CONST : toDo == SyntaxType_OPERATORCONSTPASS ? SyntaxType_CONSTPASS : toDo;

				if (syn.type == SyntaxType_OPERATORCONST || syn.type == SyntaxType_OPERATORCONSTPASS)
				{

					CelestialList<CelestialList<unsigned char>*>* subCommands = generateCode(branch->GetNodeObject(), symbolTable, codeCount);
					constToAdd = opRetConst;
					subCommands->KillList();
					delete subCommands;

				}

				int tempVar = 0;

				if (toDo != SyntaxType_CONSTPASS)
				{

					tempVar = holes->GetFirstNode() != nullptr ? holes->PopElement() : var;

					if (var == tempVar)
					{

						var++;

					}

					line->AddElement(tempVar >> 0);
					line->AddElement(tempVar >> 8);
					line->AddElement(tempVar >> 16);
					line->AddElement(tempVar >> 24);

					addLine = new CelestialList<unsigned char>();
					addLine->AddElement((unsigned char)0);
					addLine->AddElement((unsigned char)0);
					addLine->AddElement((unsigned char)0);
					addLine->AddElement((unsigned char)0);

					addLine->AddElement((unsigned char)opcode_SETCONST);//Set operator

					addLine->AddElement(tempVar >> 0);
					addLine->AddElement(tempVar >> 8);
					addLine->AddElement(tempVar >> 16);
					addLine->AddElement(tempVar >> 24);

				}

				if (constToAdd[0] == 's')
				{

					addLine->AddElement((constToAdd.length() - 1) >> 0);
					addLine->AddElement((constToAdd.length() - 1) >> 8);
					addLine->AddElement((constToAdd.length() - 1) >> 16);
					addLine->AddElement((constToAdd.length() - 1) >> 24);

					for (int i = 1; i < constToAdd.length(); i++)
					{

						addLine->AddElement(constToAdd[i]);

					}
				}
				else if (constToAdd[0] == 'f')
				{

					float value = std::stof(std::string(&(constToAdd.c_str()[1])));
					unsigned char bytes[sizeof(float)];
					memcpy(bytes, &value, sizeof(float));

					for (unsigned char i = 0; i < sizeof(float); i++)
					{
						
						addLine->AddElement(bytes[i]);

					}
					
					if (apuConstOp)
					{

						apuFloat = true;

						if (apuConst1In)
						{

							apuConst2 = value;

						}
						else
						{

							apuConst1 = value;
							apuConst1In = true;

						}
					}
				}
				else
				{

					int val = std::stoi(std::string(&(constToAdd.c_str()[1])));
					addLine->AddElement(val >> 0);
					addLine->AddElement(val >> 8);
					addLine->AddElement(val >> 16);
					addLine->AddElement(val >> 24);

					if (apuConstOp)
					{

						if (apuConst1In)
						{

							apuConst2 = val;

						}
						else
						{

							apuConst1 = val;
							apuConst1In = true;

						}
					}
				}

				if (toDo != SyntaxType_CONSTPASS)
				{

					commands->AddElement(addLine);
					codeCount++;

				}

				if (tempVar > 0)
				{

					varClear->PushElement(tempVar);

				}
			}
			else if (syn.type == SyntaxType_OPERATORNA)
			{

				CelestialList<CelestialList<unsigned char>*>* subCommands = generateCode(branch->GetNodeObject(), symbolTable, codeCount);
				commands->AddElement(subCommands);
				delete subCommands;

			}
			else if (syn.type == SyntaxType_OPERATORVAR)
			{

				int opRet = symbolTable->GetValue(varAdr).address;
				setOpVar = opCode == opcode_SETVAR;

				if (opCode != opcode_SETVAR)
				{

					opRet = holes->GetFirstNode() != nullptr ? holes->PopElement() : var;

					if (var == opRet)
					{

						var++;

					}

					line->AddElement(opRet >> 0);
					line->AddElement(opRet >> 8);
					line->AddElement(opRet >> 16);
					line->AddElement(opRet >> 24); 
					varClear->PushElement(opRet);

				}
				else
				{

					//handleVariable(symbolTable, varAdr, varClear, commands, codeCount);
					symbol tempSym = symbolTable->GetValue(varAdr);
					tempSym.waitFlag = true;
					symbolTable->Add(tempSym, varAdr);

				}

				opRetVar = opRet;
				CelestialList<CelestialList<unsigned char>*>* subCommands = generateCode(branch->GetNodeObject(), symbolTable, codeCount);
				commands->AddElement(subCommands);

				if (opCode != opcode_SETVAR)
				{

					CelestialList<unsigned char>* addLine = new CelestialList<unsigned char>();
					addLine->AddElement((unsigned char)0);
					addLine->AddElement((unsigned char)0);
					addLine->AddElement((unsigned char)0);
					addLine->AddElement((unsigned char)0);
					addLine->AddElement(opCode_WTFRVR);
					addLine->AddElement(opRet >> 0);
					addLine->AddElement(opRet >> 8);
					addLine->AddElement(opRet >> 16);
					addLine->AddElement(opRet >> 24);
					commands->AddElement(addLine);
					codeCount++;

				}

				delete subCommands;
				
			}

			branch = branch->GetNext();

		}

		if (!setOpVar && !apuConstOp)
		{

			codeCount++;
			commands->AddElement(line);

		}
		else
		{

			delete line;

		}

		if (opCode == opcode_SUM2CONST)
		{

			opRetConst = apuFloat ? 'f' + std::to_string(apuConst1 + apuConst2) :
				'n' + std::to_string((int)(apuConst1 + apuConst2));

		}
		else if (opCode == opcode_SUB2CONST)
		{

			opRetConst = apuFloat ? 'f' + std::to_string(apuConst1 - apuConst2) :
				'n' + std::to_string((int)(apuConst1 - apuConst2));

		}
		else if (opCode == opcode_MUL2CONST)
		{

			opRetConst = apuFloat ? 'f' + std::to_string(apuConst1 * apuConst2) :
				'n' + std::to_string((int)(apuConst1 * apuConst2));

		}
		else if (opCode == opcode_DIV2CONST)
		{

			opRetConst = apuFloat ? 'f' + std::to_string(apuConst1 / apuConst2) :
				'n' + std::to_string((int)(apuConst1 / apuConst2));

		}
		else if (opCode == opcode_NUMEQUAL2CONST)
		{

			opRetConst = 'n' + std::to_string(apuConst1 == apuConst2 ? 1 : 0);

		}
		else if (opCode == opcode_NUMGRT2CONST)
		{

			opRetConst = 'n' + std::to_string(apuConst1 > apuConst2 ? 1 : 0);

		}
		else if (opCode == opcode_NEGATECONST)
		{

			opRetConst = apuFloat ? 'f' + std::to_string(-apuConst1) :
				'n' + std::to_string((int)(-apuConst1));

		}
	}

	while (varClear->GetFirstNode() != nullptr)
	{

		int varRemove = varClear->PopElement();

		if (varRemove == var - 1)
		{

			var--;

		}
		else
		{

			holes->PushElement(varRemove);

		}
	}

	delete varClear;
	return commands;

}

CelScriptCodeGenerator::CelScriptCodeGenerator(Keyword* keywords, int keyWordsSize)
{

	this->keywords = keywords;
	this->keyWordsSize = keyWordsSize;

	holes = new CelestialList<int>;

}

CelestialList<CelestialList<unsigned char>*>* CelScriptCodeGenerator::GenerateCode(AST ast)
{

	CelestialListNode<CelestialTree<syntax>*>* trees = ast.trees->GetFirstNode();
	CelestialList<CelestialList<unsigned char>*>* byteCode = new CelestialList<CelestialList<unsigned char>*>();
	var = 1;
	unsigned int codeCount = 1;
	CelestialList<unsigned char>* initBlock = new CelestialList<unsigned char>();
	unsigned char initSize[4]{var >> 0, var >> 8, var >> 16, var >> 24};
	byteCode->AddElement(initBlock);

	while (trees != nullptr)
	{

		syntax syn = trees->GetNodeObject()->GetRoot()->GetNodeObject();


		opRetVar = 0;
		CelestialList<CelestialList<unsigned char>*>* commands = generateCode(trees->GetNodeObject()->GetRoot(), ast.symbolTable, codeCount);

		if (syn.type == SyntaxType_FLOW && std::stoi(syn.val) == FlowType_INIT)
		{

			initSize[0] = (codeCount) >> 0;
			initSize[1] = (codeCount) >> 8;
			initSize[2] = (codeCount) >> 16;
			initSize[3] = (codeCount) >> 24;

		}

		if (commands != nullptr)
		{

			byteCode->AddElement(commands);
			delete commands;

		}

		trees = trees->GetNext();

	}

	initBlock->AddElement(initSize[0]);
	initBlock->AddElement(initSize[1]);
	initBlock->AddElement(initSize[2]);
	initBlock->AddElement(initSize[3]);
	return byteCode;

}

CelScriptCodeGenerator::~CelScriptCodeGenerator()
{

	delete holes;

}
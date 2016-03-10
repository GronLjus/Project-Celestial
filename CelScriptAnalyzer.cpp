#include "stdafx.h"
#include "CelScriptAnalyzer.h"

using namespace Logic;
using namespace CrossHandlers;
using namespace std;

CelestialTreeNode<syntax>* CelScriptAnalyzer::analyzeFlow(CelestialTreeNode<Token>* node, CelestialTreeNode<syntax>* parent, CompileError &err)
{

	int fl = -1;

	for (int i = 0; i < flowSize && fl == -1; i++)
	{

		fl = strcmp(flowOps[i].keyword.c_str(), node->GetNodeObject().val.c_str()) == 0 ? i : -1;

	}

	syntax newSyn;
	newSyn.val = to_string(flowOps[fl].type);
	newSyn.type = SyntaxType_FLOW;
	CelestialTreeNode<syntax>* newtree = new CelestialTreeNode<syntax>(newSyn, parent);
	CelestialList<CelestialTreeNode<Token>*>* leafs = node->GetLeafs();

	
	if (fl == -1)
	{

		err.errorType = ScriptError_UNKNOWNKEYWORD;
		err.line = node->GetNodeObject().line;

	}
	else if (leafs != nullptr && err.errorType == ScriptError_OK)
	{

		if (flowOps[fl].op == FlowOperator_INIT && !empty)
		{

			err.errorType = ScriptError_ALREADYINIT;
			err.line = node->GetNodeObject().line;

		}
		else
		{

			if (flowOps[fl].op == FlowOperator_INIT)
			{

				inInit = true;

			}

			empty = false;
			FlowType* paramTypes = new FlowType[flowOps[fl].params];
			int pa = 0;
			CelestialListNode<CelestialTreeNode<Token>*>* parameter = leafs->GetFirstNode();

			while (parameter != nullptr && err.errorType == ScriptError_OK && pa < flowOps[fl].params)
			{

				Token param = parameter->GetNodeObject()->GetNodeObject();

				if (param.type == TokenType_COND)
				{

					paramTypes[pa] = FlowType_COND;
					syntax bodSyn;
					bodSyn.val = to_string(FlowType_COND);
					bodSyn.type = SyntaxType_FLOW;
					CelestialTreeNode<syntax>* bodNode = new CelestialTreeNode<syntax>(bodSyn, newtree);

					if (parameter->GetNodeObject()->GetLeafs() != nullptr)
					{

						VarType e = VarType_NA;
						ParamType p;
						CelestialListNode<CelestialTreeNode<Token>*>* cond = parameter->GetNodeObject()->GetLeafs()->GetFirstNode();
						int condA = 0;

						while (cond != nullptr && err.errorType == ScriptError_OK)
						{

							if (cond->GetNodeObject()->GetNodeObject().type == TokenType_KEYWORD)
							{

								CelestialTreeNode<syntax>* condition = analyzeOperator(cond->GetNodeObject(), bodNode, err, e, p);

								if (condition != nullptr)
								{

									bodNode->AddLeaf(condition);

								}
							}
							else if (cond->GetNodeObject()->GetNodeObject().type == TokenType_VAR)
							{

								Token tok = cond->GetNodeObject()->GetNodeObject();
								int foundSymbol = -1;

								for (int i = 0; i < symbols && foundSymbol == -1; i++)
								{

									foundSymbol = strcmp(symbolTable->GetValue(i).name.c_str(), tok.val.c_str()) == 0 ? i : -1;

								}

								if (foundSymbol == -1)
								{

									err.errorType = ScriptError_VARUNIN;
									err.line = tok.line;

								}
								else
								{

									symbol nSym = symbolTable->GetValue(foundSymbol);
									e = nSym.type;
									nSym.symCount++;
									symbolTable->Add(nSym, foundSymbol);
									syntax newSyn;
									newSyn.type = SyntaxType_VAR;
									newSyn.val = to_string(foundSymbol);
									bodNode->AddLeaf(new CelestialTreeNode<syntax>(newSyn, newtree));

								}
							}

							if (err.errorType == ScriptError_OK)
							{

								if (e != VarType_NUMBER && ((condA == 0 && parameter->GetNodeObject()->GetLeafs()->GetCount() == 1) || (condA == 1 && parameter->GetNodeObject()->GetLeafs()->GetCount() > 1)))
								{

									err.errorType = ScriptError_INCOMPATIBLE;
									err.line = param.line;

								}

								condA++;
								cond = cond->GetNext();

							}
						}

						if (err.errorType == ScriptError_OK)
						{

							if (condA > flowOps[fl].maxParamArgs[pa])
							{

								err.errorType = ScriptError_TOMANYPARAM;
								err.line = param.line;

							}
							else if (condA < flowOps[fl].minParamArgs[pa])
							{

								err.errorType = ScriptError_TOFEWPARAM;
								err.line = param.line;

							}
							else if (condA > 0)
							{

								newtree->AddLeaf(bodNode);

							}
						}

						if (condA == 0 || err.errorType != ScriptError_OK)
						{

							delete bodNode;

						}
					}
					else
					{

						if (flowOps[fl].minParamArgs[pa] > 0)
						{

							err.errorType = ScriptError_TOFEWPARAM;
							err.line = param.line;

						}

						delete bodNode;

					}
				}
				else if (param.type == TokenType_BODY)
				{

					paramTypes[pa] = FlowType_BODY;
					VarType e;
					ParamType p;
					syntax bodSyn;
					bodSyn.val = to_string(FlowType_BODY);
					bodSyn.type = SyntaxType_FLOW;
					CelestialTreeNode<syntax>* bodNode = new CelestialTreeNode<syntax>(bodSyn, newtree);
					newtree->AddLeaf(bodNode);

					if (parameter->GetNodeObject()->GetLeafs() == nullptr)
					{

						err.errorType = ScriptError_NOBODY;
						err.line = param.line;

					}
					else
					{

						CelestialListNode<CelestialTreeNode<Token>*>* body = parameter->GetNodeObject()->GetLeafs()->GetFirstNode();

						while (body != nullptr && err.errorType == ScriptError_OK)
						{

							CelestialTreeNode<syntax>* analysedBody = nullptr;

							if (body->GetNodeObject()->GetNodeObject().type == TokenType_KEYWORD)
							{

								CelestialTreeNode<syntax>* analysedBod = analyzeOperator(body->GetNodeObject(), bodNode, err, e, p);

								if (analysedBod != nullptr)
								{

									bodNode->AddLeaf(analysedBod);

								}
							}
							else if (body->GetNodeObject()->GetNodeObject().type == TokenType_FLOW)
							{

								analysedBody = analyzeFlow(body->GetNodeObject(), bodNode, err);

							}

							if (analysedBody != nullptr)
							{

								bodNode->AddLeaf(analysedBody);

							}

							body = body->GetNext();

						}
					}
				}
				else if (param.type == TokenType_FLOW)
				{

					paramTypes[pa] = FlowType_BRANCH;
					CelestialTreeNode<syntax>* analysedFlow = analyzeFlow(parameter->GetNodeObject(), newtree, err);

					if (analysedFlow != nullptr)
					{

						newtree->AddLeaf(analysedFlow);

					}
				}
				else
				{

					err.errorType = ScriptError_UNEXPECTEDKEYWORD;
					err.line = node->GetNodeObject().line;

				}

				pa++;
				parameter = parameter->GetNext();

			}

			if (err.errorType == ScriptError_OK)
			{

				bool isCorr = true;
				int offset = 0;

				for (int i = 0; i < pa && isCorr; i++)
				{

					isCorr = paramTypes[i] == flowOps[fl].parameters[offset + i];

					if (!isCorr && flowOps[fl].optionalParams[i])
					{

						offset++;
						isCorr = true;

					}
				}

				if (!isCorr)
				{

					err.errorType = ScriptError_INCOMPATIBLE;
					err.line = node->GetNodeObject().line;

				}
			}

			delete[] paramTypes;
			inInit = false;

		}
	}

	return newtree;

}

CelScriptAnalyzer::operatorParams CelScriptAnalyzer::analyzeOperatorParams(CompileError &err, OperatorTypes op, CelestialList<CelestialTreeNode<Token>*>* leafs, CelestialTreeNode<syntax>* newtree)
{
	
	operatorParams retParams;
	retParams.param = 0;
	retParams.opVar = 0;
	retParams.hasEnum = false;
	retParams.sigType = new VarType[leafs->GetCount()];
	retParams.sigSource = new ParamType[leafs->GetCount()];
	retParams.sigAddrr = new int[leafs->GetCount()];

	for (int i = 0; i < leafs->GetCount(); i++)
	{

		retParams.sigAddrr[i] = -1;

	}

	CelestialListNode<CelestialTreeNode<Token>*>* parameter = leafs->GetFirstNode();

	while (parameter != nullptr && err.errorType == ScriptError_OK)
	{

		bool isEnum = false;
		Token val = parameter->GetNodeObject()->GetNodeObject();

		if (val.type == TokenType_KEYWORD)
		{

			int enumPlace = -1;
			int opPlace = -1;

			for (int i = 0; i < operators[op].enumAmount && enumPlace == -1; i++)
			{

				enumPlace = strcmp(operators[op].enums[i].c_str(), val.val.c_str()) == 0 ? i : -1;

			}

			for (int i = 0; i < operatorSize && opPlace == -1 && enumPlace == -1; i++)
			{

				opPlace = strcmp(operators[i].keyword.c_str(), val.val.c_str()) == 0 ? i : -1;

			}

			if (enumPlace == -1 && opPlace == -1)
			{

				err.errorType = ScriptError_UNKNOWNKEYWORD;
				err.line = val.line;

			}
			else if ((enumPlace != -1 && retParams.param > 0))
			{

				err.errorType = ScriptError_INCOMPATIBLE;
				err.line = val.line;

			}
			else if (enumPlace != -1)
			{

				retParams.opVar = enumPlace;
				retParams.hasEnum = true;
				isEnum = true;

			}
			else
			{

				CelestialTreeNode<syntax>* analyzedNode = analyzeOperator(parameter->GetNodeObject(), newtree, err, retParams.sigType[retParams.param], retParams.sigSource[retParams.param]);

				if (err.errorType == ScriptError_OK)
				{

					newtree->AddLeaf(analyzedNode);

				}
				else
				{
								
					delete analyzedNode;

				}
			}
		}
		else if (val.type == TokenType_VAR || val.type == TokenType_PARAM)
		{

			int foundSymbol = -1;

			for (int i = 0; i < symbols && foundSymbol == -1; i++)
			{

				foundSymbol = strcmp(symbolTable->GetValue(i).name.c_str(), val.val.c_str()) == 0 ? i : -1;

			}

			if (foundSymbol == -1)
			{

				VarType type = VarType_NA;

				if (val.type == TokenType_PARAM)
				{

					bool isRP = false;

					for (unsigned int i = 1; i < val.val.length() && !isRP; i++)
					{

						isRP = !(val.val[i] == '0' ||
							val.val[i] == '1' ||
							val.val[i] == '2' ||
							val.val[i] == '3' ||
							val.val[i] == '4' ||
							val.val[i] == '5' ||
							val.val[i] == '6' ||
							val.val[i] == '7' ||
							val.val[i] == '8' ||
							val.val[i] == '9') && 
							!(i == 1 && val.val[i] == 's');

					}

					if (isRP)
					{

						bool foundRP = false;
						VarType rpType = VarType_NA;

						for (unsigned int i = 0; i < rPSize && !foundRP; i++)
						{

							foundRP = strcmp(rParams[i].name.c_str(), val.val.c_str()) == 0;
							rpType = rParams[i].type;

						}

						if (!foundRP)
						{

							err.errorType = ScriptError_UNKNOWNSYSTEMPARAM;
							err.line = val.line;

						}
						else
						{

							type = rpType;

						}
					}
					else
					{

						type = val.val[1] == 's' ? VarType_STRING : VarType_NUMBER;

					}
				}

				symbol nSym;
				nSym.address = 0;
				nSym.name = val.val;
				nSym.type = type;
				nSym.symCount = 1;
				nSym.codeCount = 0;
				nSym.gC = !inInit && val.type != TokenType_PARAM;
				nSym.waitFlag = false;
				foundSymbol = symbolTable->Add(nSym);
				symbols++;

			}
			else
			{

				symbol nSym = symbolTable->GetValue(foundSymbol);
				nSym.symCount++;
				symbolTable->Add(nSym, foundSymbol);

			}

			retParams.sigAddrr[retParams.param] = foundSymbol;
			retParams.sigType[retParams.param] = symbolTable->GetValue(foundSymbol).type;
			retParams.sigSource[retParams.param] = ParamType_VAR;

			syntax newSyn;
			newSyn.type = SyntaxType_VAR;
			newSyn.val = to_string(foundSymbol);
			newtree->AddLeaf(new CelestialTreeNode<syntax>(newSyn, newtree));

		}
		else if (val.type == TokenType_WORD)
		{

			syntax newSyn;
			newSyn.type = SyntaxType_CONST;
			newSyn.val = val.val;

			VarType type = newSyn.val[0] == 's' ? VarType_STRING : 
				newSyn.val[0] == 'f' ? VarType_FLOAT : VarType_NUMBER;

			newtree->AddLeaf(new CelestialTreeNode<syntax>(newSyn, newtree));
			retParams.sigAddrr[retParams.param] = -1;
			retParams.sigType[retParams.param] = type;
			retParams.sigSource[retParams.param] = ParamType_CONST;

		}
		else
		{

			err.errorType = ScriptError_INCOMPATIBLE;
			err.line = val.line;

		}

		parameter = parameter->GetNext();

		if (!isEnum)
		{

			retParams.param++;

		}
	}

	return retParams;

}

CelScriptAnalyzer::operatorParams CelScriptAnalyzer::getOperatorVariation(CompileError &err, unsigned int line, OperatorTypes op, operatorParams ops)
{

	if (!ops.hasEnum && err.errorType == ScriptError_OK)
	{

		bool foundSig = true;

		do
		{

			unsigned char tempCount = 0;
			foundSig = ops.param >= operators[op].minParams[ops.opVar] && ops.param <= operators[op].params[ops.opVar];

			for (unsigned int i = 0; i < ops.param && foundSig; i++)
			{

				if (operators[op].amountParOperators[ops.opVar] > 0 && 
					operators[op].parOperator[ops.opVar][i] > 0 && 
					operators[op].parRepeatsMax[ops.opVar][i] > 0)
				{

					bool foundRepeating = true;
					unsigned char repeated = 0;
					
					while (foundRepeating && tempCount < ops.param)
					{

						foundRepeating = !(((ops.sigType[tempCount] != operators[op].paramsyntax[ops.opVar][i] && operators[op].paramsyntax[ops.opVar][i] != VarType_NA) ||
							(ops.sigSource[tempCount] != operators[op].paramTypes[ops.opVar][i] && operators[op].paramTypes[ops.opVar][i] != ParamType_NA)));

						if (foundRepeating)
						{

							tempCount++;
							repeated++;

						}
					}

					foundSig = repeated >= operators[op].parRepeatsMin[ops.opVar][i] && repeated <= operators[op].parRepeatsMax[ops.opVar][i];

				}
				else
				{

					foundSig = !(((ops.sigType[tempCount] != operators[op].paramsyntax[ops.opVar][i] && operators[op].paramsyntax[ops.opVar][i] != VarType_NA) ||
						(ops.sigSource[tempCount] != operators[op].paramTypes[ops.opVar][i] && operators[op].paramTypes[ops.opVar][i] != ParamType_NA)));
					
					if (foundSig || !operators[op].optionalPar[ops.opVar][i])
					{

						tempCount++;

					}
				}
			}

			if (!foundSig)
			{

				ops.opVar++;

			}

		} while (!foundSig && ops.opVar < operators[op].enumAmount);

		if (!foundSig)
		{

			err.errorType = ScriptError_BADSIG;
			err.line = line;

		}
	}

	return ops;

}

CelestialTreeNode<syntax>* CelScriptAnalyzer::analyzeSyntaxNode(CompileError &err, CelestialTreeNode<syntax>* synTree)
{

	syntax syn = synTree->GetNodeObject();

	if (syn.type == SyntaxType_VAR)
	{

		int foundSymbol = stoi(syn.val);
		symbol nSym = symbolTable->GetValue(foundSymbol);
		nSym.symCount++;
		symbolTable->Add(nSym, foundSymbol);

	}

	CelestialTreeNode<syntax>* newNode = new CelestialTreeNode<syntax>(syn, nullptr);

	if (synTree->GetLeafs() != nullptr)
	{

		CelestialListNode<CelestialTreeNode<syntax>*>* leaf = synTree->GetLeafs()->GetFirstNode();

		while (leaf != nullptr)
		{

			CelestialTreeNode<syntax>* newNode2 = analyzeSyntaxNode(err, leaf->GetNodeObject());
			newNode2->SetParent(newNode);
			newNode->AddLeaf(newNode2);
			leaf = leaf->GetNext();

		}
	}

	return newNode;

}

CelScriptAnalyzer::operatorParams CelScriptAnalyzer::expandTree(CompileError &err, unsigned int line, OperatorTypes op, operatorParams ops, CelestialTreeNode<syntax>* newtree)
{

	if (operators[op].amountParOperators[ops.opVar] > 0)
	{

		unsigned int nrOfNewChildren = 0;
		OperatorTypes lastOp = OperatorTypes_NA;

		for (unsigned int i = 0; i < ops.param; i++)
		{

			unsigned int iMod = i < operators[op].params[ops.opVar] ? i : operators[op].params[ops.opVar] - 1;

			if (!operators[op].parOperatorAppend[ops.opVar][iMod])
			{

				nrOfNewChildren++;

			}
		}

		CelestialTreeNode<syntax>** newChildren = new CelestialTreeNode<syntax>*[nrOfNewChildren];
		operatorParams* newOpTable = new operatorParams[nrOfNewChildren];
		OperatorTypes* newOps = new OperatorTypes[nrOfNewChildren];

		unsigned int totalNrNewChildren = nrOfNewChildren;
		nrOfNewChildren = 0;
		CelestialListNode<CelestialTreeNode<syntax>*>* parameter = newtree->GetLeafs()->GetFirstNode();

		for (unsigned int i = 0;i < ops.param && parameter != nullptr && err.errorType == ScriptError_OK; i++)
		{

			unsigned int iMod = i < operators[op].params[ops.opVar] ? i : operators[op].params[ops.opVar] - 1;

			if (!operators[op].parOperatorAppend[ops.opVar][iMod])
			{

				if (operators[op].parOperator[ops.opVar][iMod] > 0)
				{

					syntax newSyn;
					newOps[nrOfNewChildren] = operators[op].parOperators[ops.opVar][operators[op].parOperator[ops.opVar][iMod] - 1];
					newSyn.type = SyntaxType_OPERATORNA;
					newSyn.val = operators[newOps[nrOfNewChildren]].keyword;
					newChildren[nrOfNewChildren] = new CelestialTreeNode<syntax>(newSyn, newtree);
					newChildren[nrOfNewChildren]->AddLeaf(parameter->GetNodeObject());
					parameter->SetNodeObject(newChildren[nrOfNewChildren]);
					newOpTable[nrOfNewChildren].param = 1;
					ops.sigAddrr[i] = -1;
					CelestialListNode<CelestialTreeNode<syntax>*>* kParameter = newtree->GetLeafs()->GetFirstNode();

					for (unsigned int k = 0; k < ops.param; k++)
					{

						unsigned int kMod = k < operators[op].params[ops.opVar] ? k : operators[op].params[ops.opVar] - 1;

						if (operators[op].parOperatorAppend[ops.opVar][kMod])
						{

							CelestialTreeNode<syntax>* syn = analyzeSyntaxNode(err, kParameter->GetNodeObject());
							syn->SetParent(newChildren[nrOfNewChildren]);
							newChildren[nrOfNewChildren]->AddLeaf(syn);
							newOpTable[nrOfNewChildren].param++;

						}

						kParameter = kParameter->GetNext();

					}


					newOpTable[nrOfNewChildren].sigType = new VarType[newOpTable[nrOfNewChildren].param];
					newOpTable[nrOfNewChildren].sigSource = new ParamType[newOpTable[nrOfNewChildren].param];
					newOpTable[nrOfNewChildren].sigAddrr = new int[newOpTable[nrOfNewChildren].param];

					newOpTable[nrOfNewChildren].opVar = 0;
					newOpTable[nrOfNewChildren].hasEnum = false;
					newOpTable[nrOfNewChildren].param = 0;

					newOpTable[nrOfNewChildren].sigAddrr[newOpTable[nrOfNewChildren].param] = ops.sigAddrr[i];
					newOpTable[nrOfNewChildren].sigSource[newOpTable[nrOfNewChildren].param] = ops.sigSource[i];
					newOpTable[nrOfNewChildren].sigType[newOpTable[nrOfNewChildren].param] = ops.sigType[i];
					newOpTable[nrOfNewChildren].param = 1;

					for (unsigned int k = 0; k < ops.param; k++)
					{

						unsigned int kMod = k < operators[op].params[ops.opVar] ? k : operators[op].params[ops.opVar] - 1;

						if (operators[op].parOperatorAppend[ops.opVar][kMod])
						{

							newOpTable[nrOfNewChildren].sigAddrr[newOpTable[nrOfNewChildren].param] = ops.sigAddrr[k];
							newOpTable[nrOfNewChildren].sigSource[newOpTable[nrOfNewChildren].param] = ops.sigSource[k];
							newOpTable[nrOfNewChildren].sigType[newOpTable[nrOfNewChildren].param] = ops.sigType[k];
							newOpTable[nrOfNewChildren].param++;

						}
					}

					if (err.errorType == ScriptError_OK)
					{

						newOpTable[nrOfNewChildren] = getOperatorVariation(err, line, newOps[nrOfNewChildren], newOpTable[nrOfNewChildren]);

					}

					if (err.errorType == ScriptError_OK)
					{

						newOpTable[nrOfNewChildren] = expandTree(err, line, newOps[nrOfNewChildren], newOpTable[nrOfNewChildren], newChildren[nrOfNewChildren]);

					}

					if (err.errorType == ScriptError_OK)
					{

						finalizeNode(err, line, newOps[nrOfNewChildren], newOpTable[nrOfNewChildren], newChildren[nrOfNewChildren], ops.sigType[i], ops.sigSource[i]);

					}

					nrOfNewChildren++;

				}
			}

			parameter = parameter->GetNext();

		}

		delete[] newOps;
		delete[] newOpTable;
		delete[] newChildren;

		if (err.errorType == ScriptError_OK)
		{
		
			ops.opVar = 0;
			ops = getOperatorVariation(err, line, op, ops);

		}
	}

	return ops;

}

CelScriptAnalyzer::operatorParams CelScriptAnalyzer::getTypeCheck(CompileError &err, unsigned int line, OperatorTypes op, operatorParams ops)
{

	if (err.errorType == ScriptError_OK)
	{

		bool typeCheck = true;
		unsigned char par = 0;

		for (int i = 0; i < operators[op].params[ops.opVar] && typeCheck && par < ops.param; i++)
		{

			if (operators[op].parRepeatsMax[ops.opVar][i] > 0)
			{

				unsigned char repeat = 0;

				while (par < ops.param && repeat < operators[op].parRepeatsMax[ops.opVar][i] && typeCheck)
				{

					typeCheck = ops.sigType[par] == operators[op].paramsyntax[ops.opVar][i] || 
						operators[op].paramsyntax[ops.opVar][i] == VarType_NA;

					if ((!typeCheck && !operators[op].optionalPar[ops.opVar][i]) || typeCheck)
					{

						par++;
						repeat++;

					}
				}

				typeCheck = repeat >= operators[op].parRepeatsMin[ops.opVar][i] && repeat <= operators[op].parRepeatsMax[ops.opVar][i];

			}
			else
			{
			
				typeCheck = ops.sigType[par] == operators[op].paramsyntax[ops.opVar][i] ||
					operators[op].paramsyntax[ops.opVar][i] == VarType_NA;
					

				if (typeCheck)
				{

					par++;

				}
				else if (operators[op].optionalPar[ops.opVar][i])
				{

					typeCheck = true;

				}
			}
		}

		if (!typeCheck)
		{

			err.errorType = ScriptError_BADSIG;
			err.line = line;

		}
	}

	return ops;

}

CelScriptAnalyzer::operatorParams CelScriptAnalyzer::makeTreeConst(CompileError &err, OperatorTypes op, operatorParams ops, CelestialTreeNode<syntax>* newtree)
{

	if (err.errorType == ScriptError_OK)
	{

		CelestialListNode<CelestialTreeNode<syntax>*>* node = newtree->GetLeafs()->GetFirstNode();
		int p = 0;

		while (node != nullptr)
		{

			syntax synHere = node->GetNodeObject()->GetNodeObject();

			if (ops.sigSource[p] == ParamType_CONST && operators[op].paramTypes[ops.opVar][p] == ParamType_CONST)
			{

				if (synHere.type == SyntaxType_CONST)
				{

					synHere.type = SyntaxType_CONSTPASS;

				}
				else if (synHere.type == SyntaxType_OPERATORCONST)
				{

					synHere.type = SyntaxType_OPERATORCONSTPASS;

				}

				node->GetNodeObject()->SetNodeObject(synHere);

			}

			p++;
			node = node->GetNext();

		}
	}

	return ops;

}

CelScriptAnalyzer::operatorParams CelScriptAnalyzer::checkVarInit(CompileError &err, unsigned int line, OperatorTypes op, operatorParams ops)
{

	if (err.errorType == ScriptError_OK)
	{

		bool unInut = false;

		for (int i = 0; i < operators[op].params[ops.opVar] && !unInut && i < ops.param; i++)
		{

			unInut = ops.sigType[i] == VarType_NA && operators[op].paramsyntax[ops.opVar][i] != VarType_NA;

		}

		if (unInut)
		{

			err.errorType = ScriptError_VARUNIN;
			err.line = line;

		}
		else if (operators[op].writeParam[ops.opVar] > 0)
		{

			if (ops.sigType[operators[op].readParam[ops.opVar] - 1] == VarType_NA)
			{

				err.errorType = ScriptError_VARUNIN;
				err.line = line;

			}
			else if (ops.sigType[operators[op].writeParam[ops.opVar] - 1] == VarType_NA)
			{

				symbol sym = symbolTable->GetValue(ops.sigAddrr[operators[op].writeParam[ops.opVar] - 1]);
				sym.type = ops.sigType[operators[op].readParam[ops.opVar] - 1];
				symbolTable->Add(sym, ops.sigAddrr[operators[op].writeParam[ops.opVar] - 1]);

			}
			else if (ops.sigType[operators[op].writeParam[ops.opVar] - 1] != ops.sigType[operators[op].readParam[ops.opVar] - 1])
			{

				err.errorType = ScriptError_INCOMPATIBLE;
				err.line = line;

			}
		}

		if (err.errorType == ScriptError_OK)
		{

			if (ops.param < operators[op].minParams[ops.opVar])
			{

				err.errorType = ScriptError_TOFEWPARAM;
				err.line = line;

			}
		}
	}

	return ops;

}

void CelScriptAnalyzer::finalizeNode(CompileError &err, unsigned int line, OperatorTypes op, operatorParams ops, CelestialTreeNode<syntax>* node, VarType &returnType, ParamType& returnSource)
{

	ops = getTypeCheck(err, line, op, ops);
	ops = makeTreeConst(err, op, ops, node);
	ops = checkVarInit(err, line, op, ops);

	if (err.errorType == ScriptError_OK)
	{

		syntax rootSyn = node->GetNodeObject();
		rootSyn.val = to_string(operators[op].byteCodes[ops.opVar]);

		if (operators[op].returnType[ops.opVar] == ParamType_CONST)
		{

			rootSyn.type = SyntaxType_OPERATORCONST;

		}
		else if (operators[op].returnType[ops.opVar] == ParamType_VAR)
		{

			rootSyn.type = SyntaxType_OPERATORVAR;

		}

		node->SetNodeObject(rootSyn);
		returnType = operators[op].returns[ops.opVar];
		returnSource = operators[op].returnType[ops.opVar];

	}

	delete[] ops.sigType;
	delete[] ops.sigSource;
	delete[] ops.sigAddrr;

}

CelestialTreeNode<syntax>* CelScriptAnalyzer::analyzeOperator(CelestialTreeNode<Token>* node, CelestialTreeNode<syntax>* parent, CompileError &err, VarType &returnType, ParamType& returnSource)
{

	empty = false;
	syntax newSyn;
	newSyn.val = node->GetNodeObject().val;
	newSyn.type = SyntaxType_OPERATORNA;
	CelestialTreeNode<syntax>* newtree = new CelestialTreeNode<syntax>(newSyn, parent);

	OperatorTypes op = OperatorTypes_NA;

	for (int i = 0; i < operatorSize && op == OperatorTypes_NA; i++)
	{

		op = strcmp(operators[i].keyword.c_str(), node->GetNodeObject().val.c_str()) == 0 ? OperatorTypes(i) : OperatorTypes_NA;

	}

	if (op != OperatorTypes_NA)
	{

		CelestialList<CelestialTreeNode<Token>*>* leafs = node->GetLeafs();

		if (leafs != nullptr)
		{

			operatorParams ops = analyzeOperatorParams(err, op, leafs, newtree);

			if (err.errorType == ScriptError_OK)
			{

				ops = getOperatorVariation(err, node->GetNodeObject().line, op, ops);

			}
			if (err.errorType == ScriptError_OK)
			{

				ops = expandTree(err, node->GetNodeObject().line, op, ops, newtree);

			}

			if (err.errorType == ScriptError_OK)
			{

				finalizeNode(err, node->GetNodeObject().line, op, ops, newtree, returnType, returnSource);

			}
		}
		else
		{

			err.errorType = ScriptError_TOFEWPARAM;
			err.line = node->GetNodeObject().line;

		}
	}
	else
	{

		err.errorType = ScriptError_HUH;
		err.line = node->GetNodeObject().line;

	}
	
	return newtree;

}

CelScriptAnalyzer::CelScriptAnalyzer(Keyword* keywords, int keyWordsSize, Operator* operators, int operatorSize, FlowController* flowOps, int flowSize, RuntimeParameter* rParams, int rPSize)
{

	this->keywords = keywords;
	this->keyWordsSize = keyWordsSize;

	this->operators = operators;
	this->operatorSize = operatorSize;

	this->flowOps = flowOps;
	this->flowSize = flowSize;

	this->rParams = rParams;
	this->rPSize = rPSize;

	symbolTable = new CelestialSlicedList<symbol>(100);

}

AST CelScriptAnalyzer::AnalyzeTrees(CelestialList<CelestialTree<Token>*>* parseTrees, CompileError &err)
{

	AST ast;
	symbolTable->Reset();
	symbols = 0;

	for (unsigned int i = 0; i < rPSize; i++)
	{
		symbol rPSym = symbol();
		rPSym.address = 0;
		rPSym.gC = false;
		rPSym.name = rParams[i].name;
		rPSym.type = rParams[i].type;
		rPSym.symCount = 0;
		rPSym.codeCount = 0;
		rPSym.waitFlag = false;
		symbolTable->Add(rPSym);
		symbols++;

	}

	ast.trees = new CelestialList<CelestialTree<syntax>*>();
	CelestialListNode<CelestialTree<Token>*>* tree = parseTrees->GetFirstNode();
	inInit = false;
	empty = true;

	while (tree != nullptr && err.errorType == ScriptError_OK)
	{

		VarType e;
		ParamType p;
		CelestialTreeNode<syntax>* rootNode = nullptr;

		if (tree->GetNodeObject()->GetRoot()->GetNodeObject().type == TokenType_KEYWORD)
		{

			rootNode = analyzeOperator(tree->GetNodeObject()->GetRoot(), nullptr, err, e, p);

		}
		else if (tree->GetNodeObject()->GetRoot()->GetNodeObject().type == TokenType_FLOW)
		{

			 rootNode = analyzeFlow(tree->GetNodeObject()->GetRoot(), nullptr, err);

		}

		if (rootNode != nullptr)
		{

			ast.trees->AddElement(new CelestialTree<syntax>(rootNode));

		}

		ast.symbolTable = symbolTable;
		ast.symbols = symbols;
		tree = tree->GetNext();

	}

	return ast;

}

CelScriptAnalyzer::~CelScriptAnalyzer()
{

	delete symbolTable;

}
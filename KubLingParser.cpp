#include "stdafx.h"
#include "KubLingParser.h"

using namespace Logic;
using namespace CrossHandlers;

KubLingParser::KubLingParser(Keyword* keywords, int keyWordsSize, FlowController* flowOps, int flowSize)
{

	this->flowOps = flowOps;
	this->flowSize = flowSize;
	this->keywords = keywords;
	this->keyWordsSize = keyWordsSize;
	
}

int KubLingParser::addChildrenTooTree(CelestialListNode<Token>* &tokenNode, CompileError &err, CelestialTreeNode<Token>* parent)
{

	int retVal = 0;
	tokenNode = tokenNode->GetNext();

	while (tokenNode != nullptr && retVal == 0 && err.errorType == ScriptError_OK)
	{

		CelestialTreeNode<Token>* newNode = nullptr;

		if (tokenNode->GetNodeObject().type == TokenType_KEYWORD)
		{

			int found = -1;

			for (int i = 0; i < keyWordsSize && found == -1; i++)
			{

				found = 0 == std::strcmp(tokenNode->GetNodeObject().val.c_str(), keywords[i].keyword.c_str()) ? i : -1;

			}

			if (found == -1)
			{

				err.errorType = ScriptError_UNKNOWNKEYWORD;
				err.line = tokenNode->GetNodeObject().line;

			}
			else
			{

				if (keywords[found].type == KeyWordType_OPERATOR)
				{

					newNode = new CelestialTreeNode<Token>(tokenNode->GetNodeObject(), parent);
					retVal = addChildrenTooTree(tokenNode, err, newNode);

				}
				else if (keywords[found].type == KeyWordType_FLOW)
				{

					newNode = handleFlowNode(tokenNode, err, parent);

				}
				else if (keywords[found].type == KeyWordType_ENUM)
				{

					newNode = new CelestialTreeNode<Token>(tokenNode->GetNodeObject(), parent);

				}
			}
		}
		else if (tokenNode->GetNodeObject().type == TokenType_FLOW)
		{

			newNode = handleFlowNode(tokenNode, err, parent);
			int dbg = 0;
			parent->AddLeaf(newNode);
			return 0;

		}
		else if (tokenNode->GetNodeObject().type == TokenType_BRACKET)
		{

			if (tokenNode->GetNodeObject().val[0] == '(')
			{

				newNode = new CelestialTreeNode<Token>(tokenNode->GetNodeObject(), parent);
				retVal = addChildrenTooTree(tokenNode, err, newNode);

				if (retVal == 1)
				{

					retVal = 0;

				}
			}
			else
			{

				return 1;

			}
			
		}
		else if (tokenNode->GetNodeObject().type == TokenType_SEPERATOR)
		{

			if (tokenNode->GetNodeObject().val == ";")
			{

				return 2;

			}
			else
			{

				return 0;

			}
		}
		else if (tokenNode->GetNodeObject().type == TokenType_WORD || 
			tokenNode->GetNodeObject().type == TokenType_VAR || 
			tokenNode->GetNodeObject().type == TokenType_PARAM)
		{

			newNode = new CelestialTreeNode<Token>(tokenNode->GetNodeObject(), parent);

		}
		else if (tokenNode->GetNodeObject().type == TokenType_BODY)
		{

			if (tokenNode->GetNodeObject().val[0] == '{')
			{

				Token bodStartLine = tokenNode->GetNodeObject();
				newNode = new CelestialTreeNode<Token>(tokenNode->GetNodeObject(), parent);

				while (tokenNode->GetNext() != nullptr && tokenNode->GetNext()->GetNodeObject().val[0] != '}')
				{

					addChildrenTooTree(tokenNode, err, newNode);

					if (err.errorType == ScriptError_OK)
					{

						orderTree(newNode->GetLeafs()->GetLastNode()->GetNodeObject(), err);
						pruneBrackets(newNode->GetLeafs()->GetLastNode()->GetNodeObject());

					}

					if (tokenNode->GetNext() == nullptr)
					{

						err.errorType = ScriptError_UNCLOSEDBODY;
						err.line = bodStartLine.line;
						
					}
				}

				parent->AddLeaf(newNode);
				return 0;

			}
			else
			{

				err.errorType = ScriptError_UNEXPECTEDKEYWORD;
				err.line = tokenNode->GetNodeObject().line;

			}
		}
		else if (tokenNode->GetNodeObject().type == TokenType_COND)
		{

			newNode = new CelestialTreeNode<Token>(tokenNode->GetNodeObject(), parent);

			while (tokenNode->GetNext()->GetNodeObject().type != TokenType_COND)
			{

				addChildrenTooTree(tokenNode, err, newNode);

				if (err.errorType == ScriptError_OK)
				{

					orderTree(newNode->GetLeafs()->GetLastNode()->GetNodeObject(), err);
					pruneBrackets(newNode->GetLeafs()->GetLastNode()->GetNodeObject());

				}
			}

			parent->AddLeaf(newNode);
			return 0;

		}

		parent->AddLeaf(newNode);

		if (tokenNode != nullptr && retVal == 0)
		{

			tokenNode = tokenNode->GetNext();

		}
	}

	return retVal;

}

CelestialTreeNode<Token>* KubLingParser::handleFlowNode(CelestialListNode<Token>* &tokenNode, CompileError &err, CelestialTreeNode<Token>* parent)
{

	CelestialTreeNode<Token>* newNode = new CelestialTreeNode<Token>(tokenNode->GetNodeObject(), parent);
	int fl = -1;

	for (int i = 0; i < flowSize && fl == -1; i++)
	{

		fl = 0 == std::strcmp(tokenNode->GetNodeObject().val.c_str(), flowOps[i].keyword.c_str()) ? i : -1;

	}

	FlowController fc = flowOps[fl];
	bool wrongFlow = false;

	if (fc.op == FlowOperator_IF || fc.op == FlowOperator_ELSE)
	{

		if (fc.op == FlowOperator_ELSE)
		{

			wrongFlow = true;

			if (parent->GetNodeObject().type == TokenType_FLOW)
			{

				fl = -1;

				for (int i = 0; i < flowSize && fl == -1; i++)
				{

					fl = 0 == std::strcmp(parent->GetNodeObject().val.c_str(), flowOps[i].keyword.c_str()) ? i : -1;

				}

				wrongFlow = !(flowOps[fl].op == FlowOperator_ELSE || flowOps[fl].op == FlowOperator_IF);

			}

			if (wrongFlow)
			{

				err.errorType = ScriptError_MISSINGFLOW;
				err.line = tokenNode->GetNodeObject().line;

			}
		}
	}

	if (!wrongFlow)
	{

		int pCount = 0;
		bool breakFlow = false;

		while (!breakFlow && err.errorType == ScriptError_OK && tokenNode->GetNext() != nullptr)
		{

			if (tokenNode->GetNext()->GetNodeObject().type == TokenType_FLOW)
			{

				tokenNode = tokenNode->GetNext();
				newNode->AddLeaf(handleFlowNode(tokenNode, err, newNode));

			}
			else
			{

				addChildrenTooTree(tokenNode, err, newNode);
				tokenNode = tokenNode->GetNext();

			}

			breakFlow = err.errorType != ScriptError_OK ||tokenNode->GetNext() == nullptr;

			if (!breakFlow)
			{

				breakFlow = !(tokenNode->GetNext()->GetNodeObject().val[0] == '{' || tokenNode->GetNext()->GetNodeObject().type == TokenType_COND);

				if ((tokenNode->GetNext()->GetNodeObject().type == TokenType_FLOW && err.errorType == ScriptError_OK) && (fc.op == FlowOperator_IF || fc.op == FlowOperator_ELSE))
				{

					int fl2 = -1;

					for (int i = 0; i < flowSize && fl2 == -1; i++)
					{

						fl2 = 0 == std::strcmp(tokenNode->GetNext()->GetNodeObject().val.c_str(), flowOps[i].keyword.c_str()) ? i : -1;

					}

					breakFlow = flowOps[fl2].op != FlowOperator_ELSE;

				}
			}
		}
	}

	return newNode;

}

CelestialTreeNode<Token>* KubLingParser::orderTree(CelestialTreeNode<Token>* treeRoot, CompileError &err )
{

	if (treeRoot->GetLeafs() == nullptr)
	{

		return treeRoot;

	}

	CelestialListNode<CelestialTreeNode<Token>*>* branch = treeRoot->GetLeafs()->GetFirstNode();
	int rootKeyWord = -1;

	for (int i = 0; i < keyWordsSize && rootKeyWord == -1; i++)
	{

		rootKeyWord = std::strcmp(treeRoot->GetNodeObject().val.c_str(), keywords[i].keyword.c_str()) == 0 ? i : -1;

	}

	while (branch != nullptr && err.errorType == ScriptError_OK)
	{

		Token tok = branch->GetNodeObject()->GetNodeObject();
		CelestialTreeNode<Token>* newBranch = orderTree(branch->GetNodeObject(), err);

		if (err.errorType != ScriptError_OK)
		{

			return nullptr;

		}

		newBranch->SetParent(treeRoot);
		branch->SetNodeObject(newBranch);

		if (tok.type == TokenType_KEYWORD && rootKeyWord != -1)
		{


			if (keywords[rootKeyWord].priority > 0 && branch->GetNodeObject()->GetLeafs() != nullptr)
			{

				CelestialListNode<CelestialTreeNode<Token>*>* subBranch = branch->GetNodeObject()->GetLeafs()->GetFirstNode();
				int keyWord2 = -1;

				for (int i = 0; i < keyWordsSize && keyWord2 == -1; i++)
				{

					keyWord2 = std::strcmp(tok.val.c_str(), keywords[i].keyword.c_str()) == 0 ? i : -1;

				}

				if (keyWord2 > 0 && 0 < keywords[keyWord2].priority && keywords[keyWord2].priority >= keywords[rootKeyWord].priority)
				{

					CelestialTreeNode<Token>* b = branch->GetNodeObject();

					if (subBranch != nullptr)
					{
					
						CelestialTreeNode<Token>* sB = subBranch->GetNodeObject();
						sB->SetParent(treeRoot);
						branch->SetNodeObject(sB);

						treeRoot = orderTree(treeRoot, err);
						treeRoot->SetParent(b);
						subBranch->SetNodeObject(treeRoot);
						return b;

					}
					else
					{

						err.errorType = ScriptError_HUH;
						err.line = branch->GetNodeObject()->GetNodeObject().line;
						return nullptr;

					}
				}
			}
		}

		branch = branch->GetNext();

	}

	return treeRoot;

}

void KubLingParser::pruneBrackets(CelestialTreeNode<Token>* node)
{

	if (node->GetLeafs() != nullptr)
	{

		CelestialListNode<CelestialTreeNode<Token>*>* branch = node->GetLeafs()->GetFirstNode();

		while (branch != nullptr)
		{

			if (branch->GetNodeObject()->GetNodeObject().type == TokenType_BRACKET)
			{

				CelestialTreeNode<Token>* bracketNode = branch->GetNodeObject();

				if (branch->GetNodeObject()->GetLeafs() != nullptr)
				{

					branch->SetNodeObject(branch->GetNodeObject()->GetLeafs()->GetFirstNode()->GetNodeObject());

				}

				bracketNode->RemoveLeafs();
				delete bracketNode;

			}
			
			pruneBrackets(branch->GetNodeObject());
			branch = branch->GetNext();

		}
	}
}

CelestialList<CelestialTree<Token>*>* KubLingParser::ParseTokens(CelestialList<Token>* tokens, CompileError &err)
{

	CelestialListNode<Token>* node = tokens->GetFirstNode();
	CelestialList<CelestialTree<Token>*>* parseTreeList = new CelestialList<CelestialTree<Token>*>();

	while (node != nullptr && err.errorType == ScriptError_OK)
	{

		//Identify Keyword
		int found = -1;

		for (int i = 0; i < keyWordsSize && found == -1; i++)
		{

			std::string keyWords = keywords[i].keyword;
			found = 0 == std::strcmp(node->GetNodeObject().val.c_str(), keywords[i].keyword.c_str()) ? i : -1;

		}

		if (found == -1)
		{

			err.errorType = ScriptError_UNKNOWNKEYWORD;
			err.line = node->GetNodeObject().line;
			node = nullptr;
		
		}
		else
		{

			if (keywords[found].type == KeyWordType_OPERATOR)
			{

				CelestialTreeNode<Token>* genNode = new CelestialTreeNode<Token>(node->GetNodeObject(), nullptr);
				addChildrenTooTree(node, err, genNode);
				parseTreeList->AddElement(new CelestialTree<Token>(genNode));

				if (err.errorType == ScriptError_OK)
				{

					orderTree(genNode, err);
					pruneBrackets(genNode);

				}
			}
			else if (keywords[found].type == KeyWordType_FLOW)
			{

				CelestialTreeNode<Token>* genNode = handleFlowNode(node,err,nullptr);
				parseTreeList->AddElement(new CelestialTree<Token>(genNode));

			}

			if (node != nullptr)
			{

				node = node->GetNext();

			}
		}
	}

	return parseTreeList;

}

KubLingParser::~KubLingParser()
{


}
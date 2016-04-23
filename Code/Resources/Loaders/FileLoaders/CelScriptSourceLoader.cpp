#include "stdafx.h"
#include "CelScriptSourceLoader.h"
#include "CelestialList.h"
#include <fstream>
#include <iostream>

using namespace Resources;
using namespace std;

CelScriptSourceLoader::CelScriptSourceLoader()
{

	extens = 1;
	extensions = new string[extens];
	extensions[0] = "celsrc";
	celCompiler = new Logic::CelScriptCompiler();

}

string* CelScriptSourceLoader::Extension() const
{

	return extensions;

}

int CelScriptSourceLoader::GetNrExtensions() const
{

	return extens;

}

void CelScriptSourceLoader::Init(Graphics::CardHandler* &card, TextContainer* outText)
{

	celCompiler->Init(outText);

}

void CelScriptSourceLoader::SaveSource(string path, CelScriptSource* source)
{

	ofstream fileOut;
	fileOut.open(path, ios::out);

	if (fileOut.is_open())
	{

		int count = 0;

		while (fileOut.good() && count < source->GetLines())
		{

			fileOut << source->GetLine(count);
			count++;

		}

		fileOut.close();

	}
}

CelestialList<string*>* CelScriptSourceLoader::putFileIntoList(string path)
{
	ifstream fileIn;
	fileIn.open(path, ios::in);
	CelestialList<string*>* retVal = new CelestialList<string*>();

	if (fileIn.is_open())
	{

		int count = 0;

		while (fileIn.good())
		{

			string line;
			getline(fileIn, line);
			int offset = 0;
			string newLine = "";

			for (int i = 0; i < line.length(); i++)
			{
				if (line[i] != '\t')
				{

					newLine = newLine + line[i];
					offset++;

				}
			}
			
			if (newLine[0] == '#')
			{

				CelestialList<string*>* includeList = putFileIntoList(newLine.substr(1));
				includeList->AddElement(retVal);
				delete retVal;
				retVal = includeList;

			}
			else
			{

				count++;
				string* fullLine = new string[2];
				fullLine[0] = newLine;
				fullLine[1] = path;
				retVal->AddElement(fullLine);

			}
		}

		fileIn.close();

	}

	return retVal;

}

CelScriptSource* CelScriptSourceLoader::LoadSource(string path)
{

	CelScriptSource* retVal = nullptr;

	CelestialList<string*>* input = putFileIntoList(path);

	string** src = new string*[input->GetCount()];
	CelestialListNode<string*>* lne = input->GetFirstNode();
	int currentLine = 0;

	while (lne != nullptr)
	{

		src[currentLine] = lne->GetNodeObject();
		currentLine++;
		lne = lne->GetNext();

	}

	retVal = new CelScriptSource(input->GetCount(), src);
	delete input;
	return retVal;

}

CelScriptCompiled* CelScriptSourceLoader::Load(string path)
{

	CelScriptSource* retVal = LoadSource(path);

	Logic::CompileError compileErr;
	compileErr.errorType = Logic::ScriptError_OK;
	CelScriptCompiled* retComp = celCompiler->CompileSource(retVal,compileErr);
	delete retVal;
	return retComp;

}

CelScriptSourceLoader::~CelScriptSourceLoader()
{

	delete[] extensions;
	delete celCompiler;

}
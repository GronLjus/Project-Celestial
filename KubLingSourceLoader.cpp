#include "stdafx.h"
#include "KubLingSourceLoader.h"
#include "CelestialList.h"
#include <fstream>
#include <iostream>

using namespace Resources;
using namespace std;

KubLingSourceLoader::KubLingSourceLoader()
{

	extens = 1;
	extensions = new string[extens];
	extensions[0] = "celsrc";
	celCompiler = new Logic::KubLingCompiler();

}

string* KubLingSourceLoader::Extension() const
{

	return extensions;

}

int KubLingSourceLoader::GetNrExtensions() const
{

	return extens;

}

void KubLingSourceLoader::Init(Graphics::CardHandler* &card, TextContainer* outText)
{

	celCompiler->Init(outText);

}

void KubLingSourceLoader::SaveSource(string path, KubLingSource* source)
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

CelestialList<string*>* KubLingSourceLoader::putFileIntoList(string path)
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

KubLingSource* KubLingSourceLoader::LoadSource(string path)
{

	KubLingSource* retVal = nullptr;

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

	retVal = new KubLingSource(input->GetCount(), src, path.substr(0,path.find_last_of('.')));
	delete input;
	return retVal;

}

KubLingCompiled* KubLingSourceLoader::Load(string path)
{

	KubLingSource* retVal = LoadSource(path);

	Logic::CompileError compileErr;
	compileErr.errorType = Logic::ScriptError_OK;
	KubLingCompiled* retComp = celCompiler->CompileSource(retVal,compileErr);
	delete retVal;
	return retComp;

}

KubLingSourceLoader::~KubLingSourceLoader()
{

	delete[] extensions;
	delete celCompiler;

}
#include "../../../../stdafx.h"
#include "TRNFileLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;
using namespace Resources;

TRNFileLoader::TRNFileLoader()
{

	nrExtensions = 1;
	extensions = new string[nrExtensions];
	extensions[0] = "trn";
	mLoader = nullptr;

}

std::string* TRNFileLoader::Extension() const
{

	return extensions;

}

int TRNFileLoader::GetNrExtensions() const
{

	return nrExtensions;

}

void TRNFileLoader::Init(Graphics::CardHandler* &card, TextContainer* outText)
{

	if(mLoader != nullptr)
	{

		delete mLoader;

	}

	mLoader = new MTLFileLoader();
	mLoader->Init(card, outText);

}

TerrainObject* TRNFileLoader::Load(std::string text)
{

	ifstream fileIn;
	fileIn.open(text,ios::in);
	string line;
	string mapFile;
	string mtlFile;
	int width;
	int height;
	int depth;

	while(fileIn.good())
	{

		getline(fileIn,line);
		string first = getWord(0,line);

		if(first == "mtl")
		{

			mtlFile = getWord(1,line);

		}
		else if(first == "hMap")
		{

			mapFile = getWord(1,line);

		}
		else if(first == "w")
		{

			width = convertStringToInt(getWord(1,line));

		}
		else if(first == "h")
		{

			height = convertStringToInt(getWord(1,line));

		}
		else if(first == "d")
		{

			depth = convertStringToInt(getWord(1,line));

		}
	}

	int mSize = 0;
	MeshObject::Material** materials = mLoader->LoadMaterial(mtlFile,mSize);
	TerrainObject* retVal = nullptr;

	if(mSize >= 1)
	{
	
		retVal = new TerrainObject(mapFile,10,4,width,height,depth,0,0,0,materials[0]);
	
	}

	if(materials != nullptr)
	{

		delete[] materials;

	}

	return retVal;

}

int TRNFileLoader::convertStringToInt(string what) const
{

	std::istringstream iss(what);
    int x;
	iss >>  x;
    return x;
 
}

int TRNFileLoader::getWords(string which) const
{
	
	int spaces = 0;

	for(UINT i=0;i<which.length();i++)
	{

		if(which[i] == ' ' && i <which.length()-1)
		{

			spaces++;

		}
	}

	return spaces+1;

}

string TRNFileLoader::getWord(int what, string which) const
{

	string word = "";
	int spaces = 0;

	for(UINT i=0;i<which.length() && spaces <= what;i++)
	{

		if(which[i] == ' ' || which[i] == '_')
		{

			spaces++;

		}
		else if(spaces == what)
		{

			word += which[i];

		}
	}

	return word;

}

TRNFileLoader::~TRNFileLoader()
{

	delete mLoader;
	delete[] extensions;

}
#include "stdafx.h"
#include "MTLFileLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;
using namespace Resources;


MeshObject::Material** MTLFileLoader::LoadMaterial(std::string file,int &size)
{

	size = -1;
	MeshObject::Material** retVal = nullptr;
	string materialDiffFile;
	string materialAmbFile;
	string materialNormFile;
	UCHAR* materialDiffColours = new UCHAR[3];
	UCHAR* materialAmbColours = new UCHAR[3];

	ifstream fileIn;
	fileIn.open(file,ios::in);
	string line;

	while(fileIn.good())
	{

		getline(fileIn,line);

		string first = getWord(0,line);

		if(first == "newmtl")
		{

			if (size >= 0)
			{

				createMaterial(retVal[size], materialDiffFile, materialAmbFile, materialNormFile, materialDiffColours, materialAmbColours);

			}

			size++;
			MeshObject::Material** temp = new MeshObject::Material*[size+1];

			if (size > 0)
			{

				for (int i = 0; i<size-1; i++)
				{

					temp[i] = retVal[i];

				}

				delete[] retVal;
				
			}

			retVal = temp;
			retVal[size] = new MeshObject::Material(getWord(1,line));

			materialDiffFile = "*Colour";
			materialAmbFile = "*Colour";
			materialNormFile = "*Colour";

			materialDiffColours[0] = (UCHAR)(256 * 0.8f);
			materialDiffColours[1] = (UCHAR)(256 * 0.8f);
			materialDiffColours[2] = (UCHAR)(256 * 0.8f);

			materialAmbColours[0] = (UCHAR)(256 * 0.2f);
			materialAmbColours[1] = (UCHAR)(256 * 0.2f);
			materialAmbColours[2] = (UCHAR)(256 * 0.2f);

		}

		if(first == "illum" && size >= 0)
		{

			retVal[size]->SetIllum((byte)convertStringToFloat(getWord(1,line)));

		}

		if(first == "Kd" && size >= 0)
		{

			materialDiffColours[0] = (UCHAR)(256 * convertStringToFloat(getWord(1, line)));
			materialDiffColours[1] = (UCHAR)(256 * convertStringToFloat(getWord(2, line)));
			materialDiffColours[2] = (UCHAR)(256 * convertStringToFloat(getWord(3, line)));

		}
		
		if(first == "Ka" && size >= 0)
		{


			materialAmbColours[0] = (UCHAR)(256 * convertStringToFloat(getWord(1, line)));
			materialAmbColours[1] = (UCHAR)(256 * convertStringToFloat(getWord(2, line)));
			materialAmbColours[2] = (UCHAR)(256 * convertStringToFloat(getWord(3, line)));

		}

		if(first == "Tf" && size >= 0)
		{

			float* temp = new float[3];

			for(int i=0;i< 3;i++)
			{
			
				temp[i] = convertStringToFloat(getWord(i+1,line));

			}

			retVal[size]->SetFilter(temp);
			delete[] temp;

		}

		if(first == "map" && size >= 0)
		{

			if(getWord(1,line) == "Kd")
			{

				materialDiffFile = getWord(2, line);

			}
			else if(getWord(1,line) == "Ka")
			{

				materialAmbFile = getWord(2, line);

			}
			else if(getWord(1,line) == "Kn")
			{

				materialNormFile = getWord(2, line);

			}
		}

		if((first == "d" ||first == "Tr") && size >= 0 )
		{

			retVal[size]->SetAlpha(convertStringToFloat(getWord(1,line)));

		}

		if(first == "Ni"  && size >= 0)
		{

			retVal[size]->SetLightRefraction(convertStringToFloat(getWord(1,line)));

		}
	}

	if (size >= 0 && retVal != nullptr)
	{

		createMaterial(retVal[size], materialDiffFile, materialAmbFile, materialNormFile, materialDiffColours, materialAmbColours);

	}

	delete[] materialDiffColours;
	delete[] materialAmbColours;

	size++;
	fileIn.close();
	return retVal;

}

void MTLFileLoader::createMaterial(MeshObject::Material* obj, string dFile, string aFile, string nFile, UCHAR* diff, UCHAR* amb)
{

	if (dFile[0] != '*')
	{

		obj->SetDiffuse(textureLoader->LoadTextureFromFile(dFile));

	}
	else
	{

		int col[3] = { (int)diff[0], (int)diff[1], (int)diff[2] };
		obj->SetDiffuse(textureLoader->LoadTextureFromColour(col));

	}

	if (aFile[0] != '*')
	{

		obj->SetAmbient(textureLoader->LoadTextureFromFile(aFile));

	}
	else
	{

		int col[3] = { (int)amb[0], (int)amb[1], (int)amb[2] };
		obj->SetAmbient(textureLoader->LoadTextureFromColour(col));

	}

	if (nFile[0] != '*')
	{

		obj->SetNormal(textureLoader->LoadTextureFromFile(nFile));

	}
}

float MTLFileLoader::convertStringToFloat(string what) const
{
	std::istringstream iss(what);
    float x;
	iss >>  x;
    return x;
 
}

int MTLFileLoader::getWords(string which) const
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

string MTLFileLoader::getWord(int what, string which) const
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

void MTLFileLoader::Init(Graphics::CardHandler* &card, TextContainer* outText)
{

	this->card = card;
	extens = 1;
	extensions = new string[extens];
	extensions[0] = "mtl";
	textureLoader = new TextureHandler();
	textureLoader->Init(card);

}

std::string* MTLFileLoader::Extension() const
{

	return extensions;

}

int MTLFileLoader::GetNrExtensions() const
{

	return extens;

}

MTLFileLoader::~MTLFileLoader()
{

	delete[] extensions;
	delete textureLoader;

}
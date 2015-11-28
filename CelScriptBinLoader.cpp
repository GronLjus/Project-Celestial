#include "stdafx.h"
#include "CelScriptBinLoader.h"
#include "CelestialList.h"
#include <fstream>
#include <iostream>
#include "RunTimeOperations.h"

using namespace Resources;
using namespace std;

CelScriptBinLoader::CelScriptBinLoader()
{

	extens = 1;
	extensions = new string[extens];
	extensions[0] = "celbin";

}

string* CelScriptBinLoader::Extension() const
{

	return extensions;

}

int CelScriptBinLoader::GetNrExtensions() const
{

	return extens;

}

void CelScriptBinLoader::Init(Graphics::CardHandler* &card, TextContainer* outText)
{

}

void CelScriptBinLoader::SaveBinary(std::string path, CelScriptCompiled* binary)
{

	ofstream fileOut;
	fileOut.open(path, ios::out | ios::binary);
	int savedBytes = 0;
	int count = 0;

	if (fileOut.good())
	{

		fileOut << binary->GetMaxParams(false);
		fileOut << binary->GetMaxParams(true);

		for (int i = 0; i < binary->GetMaxParams(false); i++)
		{

			fileOut << binary->GetAdr(i, false);

		}


		for (int i = 0; i < binary->GetMaxParams(true); i++)
		{

			fileOut << binary->GetAdr(i, true);

		}

		for (int i = 0; i < Logic::RunTimeParams_NA; i++)
		{

			fileOut << binary->GetAdr(Logic::RunTimeParams(i));

		}
	}
	
	while (fileOut.good() && savedBytes < binary->GetCodeSize())
	{

		int commands;
		unsigned char* command = binary->GetCode(commands, count);
		fileOut << commands;
		fileOut.write((char*)command, commands);
		savedBytes += commands;
		count++;

	}

	fileOut.flush();
	fileOut.close();

}

CelScriptCompiled* CelScriptBinLoader::Load(std::string path)
{

	ifstream fileIn;
	fileIn.open(path, ios::in | ios::binary | ios::ate);
	CelScriptCompiled* retVal = nullptr;

	if (fileIn.is_open())
	{
		
		streampos size = fileIn.tellg();
		char* memblock = new char[size];
		fileIn.seekg(0, ios::beg);
		fileIn.read(memblock, size);
		fileIn.close();

		if (size > 0)
		{

			int readbytes = 0;
			unsigned int numParams = memblock[readbytes] | ((int)memblock[readbytes + 1] << 8) | ((int)memblock[readbytes + 2] << 16) | ((int)memblock[readbytes + 3] << 24);
			readbytes += sizeof(int);
			unsigned int strParams = memblock[readbytes] | ((int)memblock[readbytes + 1] << 8) | ((int)memblock[readbytes + 2] << 16) | ((int)memblock[readbytes + 3] << 24);
			readbytes += sizeof(int);
			retVal = new CelScriptCompiled(numParams, strParams);

			for (int i = 0; i < numParams; i++)
			{

				retVal->AddParamAdr(i, 
					memblock[readbytes] | ((int)memblock[readbytes + 1] << 8) | ((int)memblock[readbytes + 2] << 16) | ((int)memblock[readbytes + 3] << 24), 
					false);
				readbytes += sizeof(int);

			}

			for (int i = 0; i < strParams; i++)
			{

				retVal->AddParamAdr(i,
					memblock[readbytes] | ((int)memblock[readbytes + 1] << 8) | ((int)memblock[readbytes + 2] << 16) | ((int)memblock[readbytes + 3] << 24),
					true);
				readbytes += sizeof(int);

			}

			for (int i = 0; i < Logic::RunTimeParams_NA; i++)
			{

				retVal->AddSystemParamAdr(Logic::RunTimeParams(i),
					memblock[readbytes] | ((int)memblock[readbytes + 1] << 8) | ((int)memblock[readbytes + 2] << 16) | ((int)memblock[readbytes + 3] << 24));
				readbytes += sizeof(int);

			}

			while (readbytes < size)
			{

				int commandSize = memblock[readbytes] | ((int)memblock[readbytes + 1] << 8) | ((int)memblock[readbytes + 2] << 16) | ((int)memblock[readbytes+3] << 24);
				readbytes += 4;
				unsigned char* command = new unsigned char[commandSize];
				memcpy(command, &(memblock[readbytes]), commandSize);
				retVal->AddCommand(command, commandSize);
				readbytes +=commandSize;

			}
		}

		delete[] memblock;

	}

	return retVal;

}

CelScriptBinLoader::~CelScriptBinLoader()
{

	delete[] extensions;

}
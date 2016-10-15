#include "stdafx.h"
#include "KubLingBinLoader.h"
#include "CelestialList.h"
#include <fstream>
#include <iostream>
#include "RunTimeOperations.h"

using namespace Resources;
using namespace std;

KubLingBinLoader::KubLingBinLoader()
{

	extens = 1;
	extensions = new string[extens];
	extensions[0] = "celbin";

}

string* KubLingBinLoader::Extension() const
{

	return extensions;

}

int KubLingBinLoader::GetNrExtensions() const
{

	return extens;

}

void KubLingBinLoader::Init(Graphics::CardHandler* &card, TextContainer* outText)
{

}

void KubLingBinLoader::SaveBinary(std::string path, KubLingCompiled* binary)
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

KubLingCompiled* KubLingBinLoader::Load(std::string path)
{

	ifstream fileIn;
	fileIn.open(path, ios::in | ios::binary | ios::ate);
	KubLingCompiled* retVal = nullptr;

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
			unsigned int fltParams = memblock[readbytes] | ((int)memblock[readbytes + 1] << 8) | ((int)memblock[readbytes + 2] << 16) | ((int)memblock[readbytes + 3] << 24);
			readbytes += sizeof(int);
			retVal = new KubLingCompiled(numParams, strParams, fltParams,path);

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

KubLingBinLoader::~KubLingBinLoader()
{

	delete[] extensions;

}
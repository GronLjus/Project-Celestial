#include "stdafx.h"
#include "CLSVLoader.h"
#include <stdio.h>

using namespace std;
using namespace Resources;

CLSVLoader::CLSVLoader()
{

	extens = 1;
	extensions = new string[extens];
	extensions[0] = "clsv";
	blockSize = 1048576;

}

void createFolder(string folderPath)
{

	std::size_t folderPathPlace = folderPath.find_first_of("/\\");


	string folder = folderPath.substr(0, folderPathPlace);
	std::wstring stemp = std::wstring(folder.begin(), folder.end());
	CreateDirectory(stemp.c_str(), nullptr);

	if (folderPathPlace == folderPath.npos)
	{

		return;

	}

	createFolder(folderPath.substr(folderPathPlace + 1));

}

void CLSVLoader::Save(string path, char* data, unsigned int size)
{


	std::size_t folderPathPlace = path.find_last_of("/\\");
	string folderPath = path.substr(0, folderPathPlace);

	createFolder(folderPath);

	FILE* outFile;

	unsigned int error = fopen_s(&outFile, path.c_str(), "wb");
	char header[]{0x0c, 0xe1, 0x5a};
	fwrite(header, 1, 3, outFile);
	unsigned int offset = 0;

	while (offset < size)
	{

		unsigned int write = min(blockSize, size - offset);

		fwrite(&data[offset], 1, write, outFile);
		offset += write;

	}

	fclose(outFile);

}

char* CLSVLoader::Load(string path, unsigned int &size)
{

	FILE* inFile;
	unsigned int error = fopen_s(&inFile, path.c_str(), "rb");
	unsigned int offset = 0;

	fseek(inFile, 0, SEEK_END);
	size = ftell(inFile);

	if (size <= 3)
	{

		return nullptr;

	}

	rewind(inFile);
	unsigned char header[]{ 0x0c, 0xe1, 0x5a };
	fread(header, 1, 3, inFile);

	if (header[0] != 0x0c ||
		header[1] != 0xe1 ||
		header[2] != 0x5a)
	{

		return nullptr;

	}

	size -= 3;
	char* data = new char[size];
	offset = 0;

	while (offset < size)
	{

		unsigned int read = min(blockSize, size - offset);

		fread(&data[offset], 1, read, inFile);
		offset += read;

	}

	fclose(inFile);
	return data;

}


string* CLSVLoader::Extension() const
{

	return extensions;

}

int CLSVLoader::GetNrExtensions() const
{

	return extens;

}

void CLSVLoader::Init(Graphics::CardHandler* &card, TextContainer* outText)
{

	this->outText = outText;

}

CLSVLoader::~CLSVLoader() 
{

	delete[] extensions;
}
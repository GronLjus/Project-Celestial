#include "stdafx.h"
#include "CLMSHFileLoader.h"
#include <stdio.h>
#include <fstream>

using namespace std;
using namespace CelestialMath;
using namespace Resources;

CLMSHFileLoader::CLMSHFileLoader()
{

	extens = 1;
	extensions = new string[extens];
	extensions[0] = "clmsh";

}

char CLMSHFileLoader::getByte(unsigned int offset)
{

	return fileBuffer[offset];

}

float CLMSHFileLoader::getFloat(unsigned int offset)
{

	float retVal;
	memcpy(&retVal, &(fileBuffer[offset]), 4);
	return retVal;

}

unsigned int CLMSHFileLoader::getUInt(unsigned int offset)
{

	unsigned int retVal;
	memcpy(&retVal, &(fileBuffer[offset]), 4);
	return retVal;

}

CelMesh* CLMSHFileLoader::Load(string text)
{
	CelMesh* retVal = nullptr;
	ifstream fileStream(text,ios::ate | ios::binary | ios::in);
	streampos end = fileStream.tellg();
	fileStream.seekg(0, ios::beg);
	streampos beg = fileStream.tellg();
	fileBuffer = new char[end - beg];
	fileStream.read(fileBuffer, end - beg);
	fileStream.close();

	if (end - beg > 28)
	{

		char first = getByte(0);
		char second = getByte(1);

		if (first == 0x0c && (unsigned char)second == 0xe1)
		{

			Vector3 scale;
			scale.x = getFloat(2);
			scale.y = getFloat(6);
			scale.z = getFloat(10);
			unsigned int vertices = getUInt(14);
			unsigned int indices = getUInt(18);
			unsigned int hearderOffset = 22;

			if (vertices > 0 && indices > 0)
			{

				CelMesh::Vertex* verts = new CelMesh::Vertex[vertices];
				unsigned int* indexBuff = new unsigned int[indices];
				memcpy(verts, &(fileBuffer[hearderOffset]), vertices * sizeof(CelMesh::Vertex));
				hearderOffset += vertices * sizeof(CelMesh::Vertex);
				memcpy(indexBuff, &(fileBuffer[hearderOffset]), indices * sizeof(unsigned int));

				retVal = new CelMesh(indices, indexBuff, vertices, verts, false, scale);

			}

			/*for (unsigned int v = 0; v < vertices; v++)
			{


				verts[v].pos.x = getFloat(hearderOffset);
				hearderOffset += 4;
				verts[v].pos.y = getFloat(hearderOffset);
				hearderOffset += 4;
				verts[v].pos.z = getFloat(hearderOffset);
				hearderOffset += 4;

				verts[v].uv.x = getFloat(hearderOffset);
				hearderOffset += 4;
				verts[v].uv.y = getFloat(hearderOffset);
				hearderOffset += 4;

				verts[v].norm.x = getFloat(hearderOffset);
				hearderOffset += 4;
				verts[v].norm.y = getFloat(hearderOffset);
				hearderOffset += 4;
				verts[v].norm.z = getFloat(hearderOffset);
				hearderOffset += 4;

			}*/
		}
	}

	delete[] fileBuffer;
	return retVal;

}

string* CLMSHFileLoader::Extension() const
{

	return extensions;

}

int CLMSHFileLoader::GetNrExtensions() const
{

	return extens;

}

void CLMSHFileLoader::Init(Graphics::CardHandler* &card, TextContainer* outText)
{

	this->outText = outText;

}

CLMSHFileLoader::~CLMSHFileLoader()
{

	delete[] extensions;

}
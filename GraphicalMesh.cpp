#include "stdafx.h"
#include "GraphicalMesh.h"

using namespace Graphics;
using namespace CrossHandlers;

GraphicalMesh::GraphicalMesh()
{

	this->AmbTexture = nullptr;
	this->DiffTexture = nullptr;
	this->NormTexture = nullptr;
	this->meshIndLength = 0;
	this->meshIndStart = 0;
	type = MeshType_SOLID;

}

GraphicalMesh::GraphicalMesh(DXTextureResource* AmbTexture, DXTextureResource* DiffTexture, DXTextureResource* NormTexture, unsigned int meshIndStart, unsigned int meshIndLength, MeshType type)
{

	this->AmbTexture = AmbTexture;
	this->DiffTexture = DiffTexture;
	this->NormTexture = NormTexture;
	this->meshIndLength = meshIndLength;
	this->meshIndStart = meshIndStart;
	this->type = type;

}

MeshType GraphicalMesh::GetMeshType() const
{

	return type;

}

DXTextureResource* GraphicalMesh::GetAmbientTexture() const
{

	return AmbTexture;

}

DXTextureResource* GraphicalMesh::GetDiffuseTexture() const
{

	return DiffTexture;

}

DXTextureResource* GraphicalMesh::GetNormalTexture() const
{

	return NormTexture;

}

unsigned int GraphicalMesh::GetIndexStart() const
{

	return meshIndStart;

}

unsigned int GraphicalMesh::GetIndexLength() const
{

	return meshIndLength;

}

GraphicalMesh::~GraphicalMesh()
{

}
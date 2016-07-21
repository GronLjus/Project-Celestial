#include "stdafx.h"
#include "PlaneLoader.h"
#include "ResourceHandler.h"
#include <cmath>
using namespace Resources;

CelMesh* PlaneLoader::LoadShape()
{

	CelMesh::Vertex* vertices = new CelMesh::Vertex[4];
	
	vertices[0] = CelMesh::Vertex();
	vertices[0].pos = Vector3(-0.5f,0.5f, 0.5f);
	vertices[0].uv = Vector2(0.0f, 1.0f);
	vertices[0].norm = Vector3(0.0f, 1.0f, 0.0);

	vertices[1].pos = Vector3(0.5f, 0.5f, 0.5f);
	vertices[1].uv = Vector2(1.0f, 1.0f);
	vertices[1].norm = Vector3(0.0f, 1.0f, 0.0);

	vertices[2].pos = Vector3(0.5f, -0.5f, -0.5f);
	vertices[2].uv = Vector2(1.0f, 0.0f);
	vertices[2].norm = Vector3(0.0f, 1.0f, 0.0);

	vertices[3].pos = Vector3(-0.5f, -0.5f, -0.5f);
	vertices[3].uv = Vector2(0.0f, 0.0f);
	vertices[3].norm = Vector3(0.0f, 1.0f, 0.0);
	
	unsigned int* index = new unsigned int[6]{ 0,1,2, 2, 3, 0 };

	return new CelMesh(6, index,4, vertices,false, Vector3(1.0f,0.001f,1.0f));//The object to load data into
}

PlaneLoader::~PlaneLoader()
{

}
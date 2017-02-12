#include "stdafx.h"
#include "CelMesh.h"

using namespace Resources;
using namespace CelestialMath;
using namespace CrossHandlers;

CelMesh::CelMesh(unsigned int indices, unsigned int* indexBuffer, unsigned int vertices, Vertex* vertexBuffer, bool wireframe, CelestialMath::Vector3 size)
{

	this->indexBuffer = indexBuffer;
	this->indices = indices;
	this->vertexBuffer = vertexBuffer;
	this->vertices = vertices;
	this->wireFrame = wireframe;
	this->size = size;

	box = new BoundingBox(size.x, size.y, size.z, 0, 0, 0);
	float p1 = (size.x/2)*(size.x / 2) + (size.y / 2)*(size.y / 2) + (size.z / 2)*(size.z / 2);
	sphere = new BoundingSphere(0, 0, 0, p1);

}

IBounding* CelMesh::GetBoundingObjectCopy(Shape shape) const
{

	if (shape == Shape_BOX)
	{

		return box->GetCopy();

	}
	else
	{

		return sphere->GetCopy();

	}
}

unsigned char* CelMesh::Update(CrossHandlers::Message* mess)
{

	return nullptr;

}

unsigned int* CelMesh::GetIndexBuffer(unsigned int &indices) const
{

	indices = this->indices;
	return indexBuffer;

}

CelMesh::Vertex* CelMesh::GetVertexBuffer(unsigned int &vertices) const
{

	vertices = this->vertices;
	return vertexBuffer;

}

bool CelMesh::IsWireframe() const
{

	return wireFrame;

}

Vector3 CelMesh::GetScale() const
{

	return size;

}

CelMesh::~CelMesh()
{

	delete[] vertexBuffer;
	delete[] indexBuffer;

	delete box;
	delete sphere;

}
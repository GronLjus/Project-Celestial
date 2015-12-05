#include "stdafx.h"
#include "DrawingBoard.h"

using namespace Entities;
using namespace CrossHandlers;
using namespace Resources;
using namespace CelestialMath;

DrawingBoard::DrawingBoard()
{

	vertexBuffer = new BufferObject();
	vertexBuffer->SetStep(sizeof(Vertex));
	indexBuffer = new BufferObject();
	indexBuffer->SetStep(sizeof(unsigned int));

}

void DrawingBoard::addObjectToVertexBuffer(Resources::MeshObject* mesh)
{

	Vertex* vertices = (Vertex*)vertexBuffer->GetTempBuffer();
	unsigned int start = vertexBuffer->GetTempAmount();
	unsigned int end = start + mesh->GetVertices();

	if (vertices == nullptr)
	{

		vertices = new Vertex[mesh->GetVertices()];

	}
	else
	{

		Vertex* newVertices = new Vertex[vertexBuffer->GetTempAmount() + mesh->GetVertices()];

		for (unsigned int i = 0; i < vertexBuffer->GetTempAmount(); i++)
		{

			newVertices[i] = vertices[i];

		}

		delete[] vertices;
		vertices = newVertices;
		vertexBuffer->SetTemp(vertices);

	}

	vertexBuffer->IncreaseTemp(end);

	for (unsigned int i = start; i < end; i++)
	{
		Vector3 pos;
		Vector2 tex;
		Vector3 norm;
		Resources::MeshObject::Vertex* temp = mesh->GetVertex(start - i);

		if (temp->getVM() >= 3)
		{

			pos = Vector3(temp->getV()[0], temp->getV()[1], temp->getV()[2]);

		}

		if (temp->getVTM() >= 2)
		{

			tex = Vector2((temp->getVT())[0], 1 - (temp->getVT())[1]);

		}

		if (temp->getVNM() >= 3)
		{

			norm = Vector3((temp->getVN())[0], (temp->getVN())[1], (temp->getVN())[2]);

		}

		vertices[i] = Vertex(pos, tex, norm);

	}
}

void DrawingBoard::addObjectToIndexBuffer(Resources::MeshObject* mesh)
{

	unsigned int* indices = (unsigned int*)indexBuffer->GetTempBuffer();
	unsigned int start = indexBuffer->GetTempAmount();

	if (indices == nullptr)
	{

		indices = new unsigned int[mesh->GetTotalIndices() * 2];

	}
	else
	{

		unsigned int* newIndices = new unsigned int[indexBuffer->GetTempAmount() + mesh->GetTotalIndices() * 2];

		for (unsigned int i = 0; i < indexBuffer->GetTempAmount(); i++)
		{

			newIndices[i] = indices[i];

		}

		delete[] indices;
		indices = newIndices;
		vertexBuffer->SetTemp(indices);

	}


	unsigned int totalIndices = start;

	Resources::MeshObject::Face* face = mesh->GetFirstFace();

	while (face != nullptr)//Go through each face in the mesh
	{

		for (int k = 0; k<face->getSize(); k++)//Go through each index in the face
		{

			indices[totalIndices] = face->GetIndexAt(k);
			totalIndices++;

			if (face->GetAdjIndexAt(k) == -1)
			{

				indices[totalIndices] = face->GetIndexAt(k);

			}
			else
			{

				indices[totalIndices] = face->GetAdjIndexAt(k);

			}

			totalIndices++;

		}

		face = face->getNext();

	}

	indexBuffer->IncreaseTemp(totalIndices);

}

void DrawingBoard::AddMesh(MeshObject* mesh)
{

	addObjectToVertexBuffer(mesh);
	addObjectToIndexBuffer(mesh);
	
}

BufferObject* DrawingBoard::GetVertexBuffers() const
{

	return vertexBuffer;

}

BufferObject* DrawingBoard::GetIndexBuffers() const
{

	return indexBuffer;

}

DrawingBoard::~DrawingBoard()
{

	delete vertexBuffer;
	delete indexBuffer;

}
#include "stdafx.h"
#include "DrawingBoard.h"

using namespace Entities;
using namespace CrossHandlers;
using namespace Resources;
using namespace CelestialMath;

DrawingBoard::DrawingBoard()
{

	vertexBuffer = new BufferObject2<BufferVertex>(100);
	indexBuffer = new BufferObject2<unsigned int>(2048);
	instanceBuffer = new BufferObject2<Instance>(256);

}
unsigned int DrawingBoard::AddInstance(GameObject* object)
{

	Matrix invTranW = MatrixTranspose(object->GetInverseTransformation());
	Matrix oW = object->GetLastTransformation();
	Matrix w = object->GetMatrix();
	
	return instanceBuffer->Add(Instance(w, invTranW, oW));

}

unsigned int DrawingBoard::addObjectToVertexBuffer(Resources::MeshObject* mesh)
{

	unsigned int offset = vertexBuffer->GetBufferSize();

	for (unsigned int i = 0; i < mesh->GetVertices(); i++)
	{
		Vector3 pos;
		Vector2 tex;
		Vector3 norm;
		Resources::MeshObject::Vertex* temp = mesh->GetVertex(i);

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


		vertexBuffer->Add(BufferVertex(pos, tex, norm));

	}

	return offset;

}

void DrawingBoard::addObjectToIndexBuffer(Resources::MeshObject* mesh, unsigned int offset)
{

	Resources::MeshObject::Face* face = mesh->GetFirstFace();

	while (face != nullptr)//Go through each face in the mesh
	{

		for (int k = 0; k<face->getSize(); k++)//Go through each index in the face
		{

			indexBuffer->Add(offset + face->GetIndexAt(k));

			if (face->GetAdjIndexAt(k) == -1)
			{

				indexBuffer->Add(offset + face->GetIndexAt(k));

			}
			else
			{

				indexBuffer->Add(offset + face->GetAdjIndexAt(k));

			}
		}

		face = face->getNext();

	}
}

void DrawingBoard::AddMesh(MeshObject* mesh)
{

	addObjectToIndexBuffer(mesh, addObjectToVertexBuffer(mesh));
	
}

BufferObject2<BufferVertex>* DrawingBoard::GetVertexBuffers() const
{

	return vertexBuffer;

}

BufferObject2<unsigned int>* DrawingBoard::GetIndexBuffers() const
{

	return indexBuffer;

}

DrawingBoard::~DrawingBoard()
{

	delete vertexBuffer;
	delete indexBuffer;
	delete instanceBuffer;

}
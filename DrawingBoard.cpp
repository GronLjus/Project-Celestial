#include "stdafx.h"
#include "DrawingBoard.h"

using namespace Entities;
using namespace CrossHandlers;
using namespace Resources;
using namespace CelestialMath;
using namespace Graphics;

DrawingBoard::DrawingBoard(unsigned char maxFlips, unsigned int maxInstances)
{

	this->maxInstances = maxInstances;
	this->maxFlips = maxFlips;
	bufflip = 0;
	vertexBuffer = new BufferObject2<BufferVertex>(100);
	indexBuffer = new BufferObject2<unsigned int>(2048);
	instanceBuffer = new BufferObject2<Instance>(256);
	meshInstances = new CelestialSlicedList<BufferObject2<Instance>*>(32, nullptr);

	meshDictionary = new CelestialSlicedList<unsigned int>(32);
	hasInstance = new CelestialSlicedList<bool>(32,false);
	meshInstance = new CelestialStack<unsigned int>(false);

	meshes = 32;
	meshesArr = new GraphicalMesh[meshes];

}

void DrawingBoard::StartAddingInstances()
{

	instanceBuffer->Reset();

}

GraphicalMesh* DrawingBoard::GetMeshes() const
{

	return meshesArr;

}

void  DrawingBoard::AddInstance(Matrix objectMatrix, Matrix objectMatrixInvTr, unsigned int meshId)
{

	if (!(hasInstance->GetValue(meshId)))
	{

		meshInstance->PushElement(meshId);
		hasInstance->Add(true, meshId);

	}

	BufferObject2<Instance>* instBuff = meshInstances->GetValue(meshDictionary->GetValue(meshId));
	instBuff->Add(Instance(objectMatrix, objectMatrixInvTr, objectMatrix));

}

void DrawingBoard::AddInstance(GameObject* object)
{

	if (!object->IsFlipBuffered(bufflip))
	{

		Matrix invTranW = MatrixTranspose(object->GetInvTrnMatrix());
		Matrix oW = MatrixTranspose(object->GetLastTransformation());
		Matrix w = MatrixTranspose(object->GetMatrix());

		if (!(hasInstance->GetValue(object->GetMeshId())))
		{

			meshInstance->PushElement(object->GetMeshId());
			hasInstance->Add(true, object->GetMeshId());

		}

		BufferObject2<Instance>* instBuff = meshInstances->GetValue(meshDictionary->GetValue(object->GetMeshId()));
		instBuff->Add(Instance(w, invTranW, oW));

	}
}

void DrawingBoard::FinalizeInstances(ViewObject* onView)
{

	bufflip++;
	bufflip %= maxFlips;
	unsigned int totalInst = 0;
	unsigned int inBuff = 0;
	unsigned int start = 0;

	while (meshInstance->GetCount() > 0)
	{

		unsigned int meshVal = meshInstance->PopElement();
		unsigned int localMesh = meshDictionary->GetValue(meshVal);
		BufferObject2<Instance>* instBuff = meshInstances->GetValue(localMesh);
		unsigned int objectsToAdd = instBuff->GetBufferSize();

		while (start + objectsToAdd >= maxInstances)
		{

			onView->AddInstanceFragment(localMesh, start, maxInstances-start, inBuff);
			objectsToAdd -= maxInstances - start;
			start = 0;
			inBuff++;

		}

		onView->AddInstanceFragment(localMesh, start, objectsToAdd, inBuff);
		start += objectsToAdd;

		instanceBuffer->Add(instBuff);
		instBuff->Reset();
		hasInstance->Add(false, meshVal);

	}
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

			if (!mesh->IsWireFrame())
			{

				if (face->GetAdjIndexAt(k) == -1)
				{

					indexBuffer->Add(offset + face->GetIndexAt(k));

				}
				else
				{

					indexBuffer->Add(offset + face->GetAdjIndexAt(k));

				}
			}
		}

		face = face->getNext();

	}
}

unsigned int DrawingBoard::AddMesh(MeshObject* mesh)
{

	unsigned int indexStart = indexBuffer->GetBufferSize();
	addObjectToIndexBuffer(mesh, addObjectToVertexBuffer(mesh));
	meshDictionary->Add(meshInstances->GetFirstEmpty(), ((BaseObject*)mesh)->GetId());

	unsigned int localMesh = meshDictionary->GetValue(((BaseObject*)mesh)->GetId());

	if (localMesh >= meshes)
	{

		meshes += 32;
		GraphicalMesh* newMeshes = new GraphicalMesh[meshes];

		for (unsigned int i = 0; i < meshes - 32; i++)
		{

			newMeshes[i] = meshesArr[i];

		}

		GraphicalMesh* oldMesh = meshesArr;
		meshesArr = newMeshes;
		delete[] oldMesh;

	}

	DXTextureResource* amb = nullptr;
	DXTextureResource* diff = nullptr;
	DXTextureResource* norm = nullptr;

	MeshObject::Material** mats = mesh->getMaterials();

	if (mats != nullptr)
	{

		amb = mats[0]->GetAmbient() == nullptr ? nullptr : mesh->getMaterials()[0]->GetAmbient()->GetDXT();
		diff = mats[0]->GetDiffuse() == nullptr ? nullptr : mesh->getMaterials()[0]->GetDiffuse()->GetDXT();
		norm = mats[0]->GetNormal() == nullptr ? nullptr : mesh->getMaterials()[0]->GetNormal()->GetDXT();

	}

	GraphicalMesh meshRep(amb,
		diff,
		norm,
		indexStart,
		indexBuffer->GetBufferSize()-indexStart,
		mesh->IsWireFrame() ? MeshType_WF : MeshType_SOLID);

	meshesArr[localMesh] = meshRep;

	if (meshInstances->GetValue(localMesh) == nullptr)
	{

		meshInstances->Add(new BufferObject2<Instance>(32));

	}
	else
	{

		meshInstances->GetValue(localMesh)->Reset();

	}

	return localMesh;

}

BufferObject2<BufferVertex>* DrawingBoard::GetVertexBuffers() const
{

	return vertexBuffer;

}

BufferObject2<Instance>* DrawingBoard::GetInstanceBuffer() const
{
	
	return instanceBuffer;

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

	meshInstances->KillList();
	delete meshInstances;

	delete meshDictionary;
	delete hasInstance;
	delete meshInstance;

	delete[] meshesArr;

}
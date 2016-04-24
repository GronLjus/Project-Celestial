#include "../../../../stdafx.h"
#include "GridLoader.h"
using namespace Resources;

MeshObject* GridLoader::LoadGrid(unsigned int cells, float gridSize) const
{

	MeshObject* mesh = new MeshObject(0);
	unsigned int totalCells = (cells+1)*(cells+1);
	MeshObject::Vertex** vertices = new MeshObject::Vertex*[totalCells];
	unsigned int globalVertex = 0;
	
	for (unsigned int z = 0; z <= cells; z++)
	{

		for (unsigned int x = 0; x <= cells; x++)
		{

			float u = ((float)x) / (float)cells;
			float v = ((float)z) / (float)cells;

			vertices[globalVertex] = new MeshObject::Vertex(3, 2, 3);
			vertices[globalVertex]->SetVertix(0, (u * 2 - 1) * (gridSize * 0.5f));
			vertices[globalVertex]->SetVertix(1, 0.0f);
			vertices[globalVertex]->SetVertix(2, (v * 2 - 1) * (gridSize * 0.5f));

			vertices[globalVertex]->SetTextureVertix(0, u);
			vertices[globalVertex]->SetTextureVertix(1, v);

			vertices[globalVertex]->SetVertixNormal(0, 0.0f);
			vertices[globalVertex]->SetVertixNormal(1, 1.0f);
			vertices[globalVertex]->SetVertixNormal(2, 0.0f);

			if (x < cells)
			{

				unsigned int* indexes = new unsigned int[2]{ globalVertex,globalVertex + 1 };
				MeshObject::Face* face = new MeshObject::Face(2, 0, 0);
				face->SetIndex((int*)indexes);
				mesh->AddFace(face);

			}
			
			if(z < cells)
			{
				unsigned int* indexes = new unsigned int[2]{ globalVertex ,globalVertex + cells + 1 };
				MeshObject::Face* face = new MeshObject::Face(2, 0, 0);
				face->SetIndex((int*)indexes);
				mesh->AddFace(face);

			}

			globalVertex++;

		}
	}

	delete[] mesh->AddVertices(vertices, totalCells);
	delete[] vertices;
	mesh->SetWireFrame(true);
	return mesh;

}

MeshObject* GridLoader::LoadShape(MeshObject::Material* mat)
{

	return LoadGrid(32,32.0f);

}

GridLoader::~GridLoader()
{

}
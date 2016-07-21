#include "stdafx.h"
#include "GridLoader.h"
using namespace Resources;

CelMesh* GridLoader::LoadGrid(unsigned int cells, float gridSize) const
{

	unsigned int totalCells = (cells+1)*(cells+1);
	CelMesh::Vertex* vertices = new CelMesh::Vertex[totalCells];
	unsigned int globalVertex = 0;
	unsigned int globalIndex = 0;
	unsigned int* indices = new unsigned int[4 * cells * cells + cells * 4];
	
	for (unsigned int z = 0; z <= cells; z++)
	{

		for (unsigned int x = 0; x <= cells; x++)
		{

			float u = ((float)x) / (float)cells;
			float v = ((float)z) / (float)cells;

			vertices[globalVertex] = CelMesh::Vertex();
			vertices[globalVertex].pos = CelestialMath::Vector3(
				(u * 2 - 1) * (0.5f*gridSize),
				0.0f,
				(v * 2 - 1) * (0.5f*gridSize));
			vertices[globalVertex].uv = CelestialMath::Vector2(u, v);
			vertices[globalVertex].norm = CelestialMath::Vector3(0.0f, 1.0f, 0.0f);

			if (x < cells)
			{

				indices[globalIndex] = globalVertex;
				globalIndex++;
				indices[globalIndex] = globalVertex + 1;
				globalIndex++;

			}

			if (z < cells)
			{

				indices[globalIndex] = globalVertex;
				globalIndex++;
				indices[globalIndex] = globalVertex+cells+1;
				globalIndex++;

			}

			globalVertex++;

		}
	}

	return new CelMesh(globalIndex, indices, totalCells, vertices, true, CelestialMath::Vector3(gridSize,1.0f, gridSize));

}

CelMesh* GridLoader::LoadShape()
{

	return LoadGrid(32,32.0f);

}

GridLoader::~GridLoader()
{

}
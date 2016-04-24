#include "../../../../stdafx.h"
#include "TerrainMesh.h"

using namespace Resources;
using namespace CrossHandlers;

TerrainMesh::TerrainMesh(int sub,int levels,BoxedBool* isAlive)
{

	this->sub = sub;
	this->levels = levels;
	willKillMyself = false;
	isBuffered = false;
	isAlive->SetValue(true);
	this->isAlive = isAlive;
	locks = 0;

	indices = nullptr;
	vertices = nullptr;
	vBuffers = nullptr;
	iBuffers = nullptr;

	vAmounts = nullptr;
	iAmounts = nullptr;

	if(levels > 0)
	{

		indices = new UINT*[levels];
		vertices = new MeshObject::Vertex**[levels];
		vBuffers = new ID3D10Buffer*[levels];
		iBuffers = new ID3D10Buffer*[levels];

		vAmounts = new int[levels];
		iAmounts = new int[levels];

		for(int i=0;i<levels;i++)
		{

			indices[i] = nullptr;
			vertices[i] = nullptr;
			vBuffers[i] = nullptr;
			iBuffers[i] = nullptr;

		}
	}
}

int TerrainMesh::GetSub() const
{

	return sub;

}

void TerrainMesh::AddVertices(MeshObject::Vertex** verts,int amount,int level)
{

	vertices[level] = verts;
	vAmounts[level] = amount;

}

void TerrainMesh::AddIndices(UINT* inds,int amount,int level)
{

	indices[level] = inds;
	iAmounts[level] = amount;

}
		
int TerrainMesh::GetLevels() const
{

	return levels;

}

UINT** TerrainMesh::GetIndices(int* &amounts)
{

	amounts = iAmounts;
	return indices;

}

MeshObject::Vertex*** TerrainMesh::GetVertices(int* &amounts)
{

	amounts = vAmounts;
	return vertices;

}

void TerrainMesh::SetVertexBuffer(ID3D10Buffer* buffer,int level)
{

	isBuffered = true;
	vBuffers[level] = buffer;

}

void TerrainMesh::SetIndexBuffer(ID3D10Buffer* buffer,int level)
{
	
	isBuffered = true;
	iBuffers[level] = buffer;

}

ID3D10Buffer** TerrainMesh::GetIndexBuffers()
{

	return iBuffers;

}

ID3D10Buffer** TerrainMesh::GetVertexBuffers()
{

	return vBuffers;

}

int TerrainMesh::GetIndexAmount(int level) const
{

	return iAmounts[level];

}

bool TerrainMesh::IsBuffered() const
{

	return isBuffered;

}

void TerrainMesh::Lock()
{

	locks++;

	if(locks == 0)
	{

		int dbg = 0;

	}
}

void TerrainMesh::Unlock()
{

	locks--;

	if(locks < 0)
	{

		int dbg = 0;
		//locks = 0;

	}

	if(locks <= 0 && willKillMyself)
	{

		isAlive->SetValue(false);
		delete this;

	}
}

void TerrainMesh::FlagForDeletion()
{

	willKillMyself = true;
	
	if(locks > 0)
	{

		int dbg = 0;

	}

	if(locks <= 0)
	{
		
		isAlive->SetValue(false);
		delete this;

	}
}
		
TerrainMesh::~TerrainMesh()
{

	for(int i=0;i<levels;i++)
	{

		if(vertices[i] != nullptr)
		{

			for(int k=0;k<vAmounts[i];k++)
			{

				delete vertices[i][k];

			}

			delete[] vertices[i];

		}

		if(indices[i] != nullptr)
		{

			delete[] indices[i];

		}

		if(iBuffers[i] != nullptr)
		{

			iBuffers[i]->Release();

		}

		if(vBuffers[i] != nullptr)
		{

			vBuffers[i]->Release();

		}
	}

	delete[] vertices;
	delete[] indices;
	delete[] iBuffers;
	delete[] vBuffers;

	if(vAmounts != nullptr)
	{

		delete[] vAmounts;

	}

	if(iAmounts != nullptr)
	{

		delete[] iAmounts;

	}
}
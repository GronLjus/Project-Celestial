#include "stdafx.h"
#include "CelestialBufferHandler.h"
#include "CelestialMath.h"
using namespace Graphics;
using namespace CrossHandlers;
using namespace Resources;
using namespace CelestialMath;
using namespace Entities;

CelestialBufferHandler::CelestialBufferHandler(ID3D11Device* card, unsigned int flips, unsigned int maxInstances)
{

	this->card = card;

	maxLights = 100;
	this->maxInstances = maxInstances;
	strides = new UINT[BufferTypes_COUNT];
	strides[BufferTypes_VERTEX] = sizeof(BufferVertex);
	strides[BufferTypes_INSTANCE] = sizeof(Instance);
	vertices = nullptr;
	indices = nullptr;

	instantFlip = 0;
	instantFlips = flips;

	instances = new ID3D11Buffer**[instantFlips];
	instanceChain = new unsigned int[instantFlips];

	for (unsigned int i = 0; i < flips; i++)
	{
		
		instanceChain[i] = 1;
		instances[i] = new ID3D11Buffer*[instanceChain[i]];

		for (unsigned int k = 0; k < instanceChain[i]; k++ )
		{

			instances[i][k] = nullptr;

		}

	}

	maxVertices = 1000;

}

HRESULT CelestialBufferHandler::InitVertexLayout(void* signature,SIZE_T size)
{

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"MTRANSFORM",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1},  
		{"MTRANSFORM",  1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1}, 
		{"MTRANSFORM",  2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1}, 
		{"MTRANSFORM",  3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"INVTRANMTRANSFORM",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},  
		{"INVTRANMTRANSFORM",  1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1}, 
		{"INVTRANMTRANSFORM",  2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1}, 
		{"INVTRANMTRANSFORM",  3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},  
		{ "OMTRANSFORM", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{"OMTRANSFORM",  1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1}, 
		{"OMTRANSFORM",  2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1}, 
		{"OMTRANSFORM",  3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1}
	};

	UINT numElements = 15;
	return card->CreateInputLayout(layout,numElements,signature,size,&vertexLayout);

}

HRESULT CelestialBufferHandler::InitTerrainVertexLayout(void* signature,SIZE_T size)
{

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT numElements = 3;
	terrainStride = sizeof(BufferVertex);
	return card->CreateInputLayout(layout,numElements,signature,size,&terrainLayout);

}

HRESULT CelestialBufferHandler::InitParticleVertexLayout(void* signature,SIZE_T size)
{

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "SIZE", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TIME", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "PSTIME", 0, DXGI_FORMAT_R32_SINT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "STIME", 0, DXGI_FORMAT_R32_SINT,1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{"TRANSFORMATION",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},  
		{"TRANSFORMATION",  1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1}, 
		{"TRANSFORMATION",  2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1}, 
		{"TRANSFORMATION",  3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"FACTOR",  0, DXGI_FORMAT_R32G32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},  
		{"COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1}, 
		{"ETIME",  0, DXGI_FORMAT_R32_SINT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"GLOW",  0, DXGI_FORMAT_R32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"GFADE",  0, DXGI_FORMAT_R32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1}
	};

	UINT numElements = 15;
	return card->CreateInputLayout(layout,numElements,signature,size,&particleLayout);

}

HRESULT CelestialBufferHandler::InitLightVertexLayout(void* signature,SIZE_T size)
{

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "RADIUS", 0, DXGI_FORMAT_R32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "ANGLE", 0, DXGI_FORMAT_R32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "DIFFUSE", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "SPECULAR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "FORWARD", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "RIGHTWARD", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "SIZE", 0, DXGI_FORMAT_R32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT numElements = 8;
	lightStride = sizeof(Light);
	return card->CreateInputLayout(layout,numElements,signature,size,&lightLayout);

}

void CelestialBufferHandler::UpdateMeshBuffers(DrawingBoard* db, ID3D11DeviceContext* context)
{
	
	if (db->GetIndexBuffers() == nullptr || db->GetVertexBuffers() == nullptr)
	{

		return;

	}

	if (vertices == nullptr)
	{

		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = strides[BufferTypes_VERTEX] * db->GetVertexBuffers()->GetBufferSize(); //total size of buffer in bytes
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = db->GetVertexBuffers()->GetBuffer();
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;
		HRESULT hr = card->CreateBuffer(&bd, &initData, &vertices);

		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(unsigned int) * db->GetIndexBuffers()->GetBufferSize(); //total size of buffer in bytes
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;

		initData.pSysMem = db->GetIndexBuffers()->GetBuffer();
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;
		hr = card->CreateBuffer(&bd, &initData, &indices);

	}
	else
	{

		BufferVertex* mapped1 = nullptr;
		D3D11_MAPPED_SUBRESOURCE mapped = D3D11_MAPPED_SUBRESOURCE();
		context->Map(vertices, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
		memcpy(mapped.pData, db->GetVertexBuffers()->GetBuffer(), sizeof(BufferVertex)*db->GetVertexBuffers()->GetBufferSize());
		context->Unmap(vertices, 0);

		context->Map(indices, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
		memcpy(mapped.pData, db->GetIndexBuffers()->GetBuffer(), sizeof(unsigned int)*db->GetIndexBuffers()->GetBufferSize());
		context->Unmap(indices, 0);

	}
}

void CelestialBufferHandler::UpdateInstanceBuffer(DrawingBoard* db, ID3D11DeviceContext* context, unsigned int flip, unsigned int chain)
{

	if (chain >= instanceChain[flip])
	{

		ID3D11Buffer** newInstances = new ID3D11Buffer*[chain+1];

		for (unsigned int i = 0; i < instanceChain[flip]; i++)
		{

			newInstances[i] = instances[flip][i];

		}

		for (unsigned i = instanceChain[flip]; i <= chain; i++)
		{

			newInstances[i] = nullptr;

		}

		delete[] instances[flip];
		instances[flip] = newInstances;
		instanceChain[flip] = chain + 1;

	}

	if (instances[flip][chain] == nullptr)
	{

		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = strides[BufferTypes_INSTANCE] * maxInstances; //total size of buffer in bytes
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		HRESULT hr = card->CreateBuffer(&bd, nullptr, &instances[flip][chain]);

		int dbg = 0;

	}

	unsigned int totalSize = db->GetInstanceBuffer()->GetBufferSize() - (chain * maxInstances);
	unsigned int toAdd = totalSize >= maxInstances ? maxInstances : totalSize;

	D3D11_MAPPED_SUBRESOURCE mapped = D3D11_MAPPED_SUBRESOURCE();
	context->Map(instances[flip][chain], 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	memcpy(mapped.pData, &(db->GetInstanceBuffer()->GetBuffer()[chain * maxInstances]), toAdd * sizeof(Instance));
	context->Unmap(instances[flip][chain], 0);
	totalSize -= toAdd;

	if (totalSize > 0)
	{

		UpdateInstanceBuffer(db, context, flip, chain + 1);

	}
}

ID3D11Buffer* CelestialBufferHandler::GetVertexBuffer() const
{

	return vertices;

}

ID3D11Buffer* CelestialBufferHandler::GetIndexBuffer() const
{

	return indices;

}

ID3D11Buffer* CelestialBufferHandler::GetInstanceBuffer(unsigned int flip, unsigned int chain) const
{

	return instances[flip][chain];

}

UINT CelestialBufferHandler::GetStride(BufferTypes buffer) const
{

	return strides[buffer];

}

UINT CelestialBufferHandler::GetTerrainStride() const
{

	return terrainStride;

}

UINT CelestialBufferHandler::GetLightStride() const
{

	return lightStride;

}

ID3D11InputLayout* CelestialBufferHandler::GetVertexLayout() const
{

	return vertexLayout;

}

ID3D11InputLayout* CelestialBufferHandler::GetParticleLayout() const
{

	return particleLayout;

}

ID3D11InputLayout* CelestialBufferHandler::GetTerrainLayout() const
{

	return terrainLayout;

}

ID3D11InputLayout* CelestialBufferHandler::GetLightLayout() const
{

	return lightLayout;

}

void CelestialBufferHandler::Release()
{

	vertexLayout->Release();
	particleLayout->Release();
	terrainLayout->Release();

	if (vertices != nullptr)
	{

		vertices->Release();

	}

	if (indices != nullptr)
	{

		indices->Release();

	}

	for (unsigned int i = 0; i < instantFlips; i++)
	{

		for (unsigned int k = 0; k < instanceChain[i]; k++)
		{

			if (instances[i][k] != nullptr)
			{

				instances[i][k]->Release();
				instances[i][k] = nullptr;

			}
		}

		delete[] instances[i];
		instanceChain[i] = 0;

	}
	//lightLayout->Release();

}

CelestialBufferHandler::~CelestialBufferHandler()
{

	delete[] instanceChain;
	delete[] instances;
	delete[] strides;

}
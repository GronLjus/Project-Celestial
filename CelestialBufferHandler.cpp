#include "stdafx.h"
#include "CelestialBufferHandler.h"
#include "CelestialMath.h"
using namespace Graphics;
using namespace CrossHandlers;
using namespace Resources;
using namespace CelestialMath;
using namespace Entities;

CelestialBufferHandler::CelestialBufferHandler(ID3D11Device* card, unsigned int flips)
{

	this->card = card;

	maxLights = 100;
	maxInstances = 200;//Gives us a buffersize of 200*2^7=25600 bytes
	strides = new UINT[BufferTypes_COUNT];
	strides[BufferTypes_VERTEX] = sizeof(BufferVertex);
	strides[BufferTypes_INSTANCE] = sizeof(Instance);
	vertices = nullptr;
	indices = nullptr;

	instantFlip = 0;
	instantFlips = flips;

	instances = new ID3D11Buffer*[instantFlips];

	for (unsigned int i = 0; i < flips; i++)
	{
		
		instances[i] = nullptr;

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

void CelestialBufferHandler::UpdateInstanceBuffer(DrawingBoard* db, ID3D11DeviceContext* context, unsigned int flip)
{

	if (instances[flip] == nullptr)
	{

		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = strides[BufferTypes_INSTANCE] * db->GetInstanceBuffer()->GetBufferSize(); //total size of buffer in bytes
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = db->GetInstanceBuffer()->GetBuffer();
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;
		HRESULT hr = card->CreateBuffer(&bd, &initData, &instances[flip]);

	}
	else
	{

		D3D11_MAPPED_SUBRESOURCE mapped = D3D11_MAPPED_SUBRESOURCE();
		context->Map(instances[flip], 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
		memcpy(mapped.pData, db->GetInstanceBuffer()->GetBuffer(), sizeof(Instance)*db->GetInstanceBuffer()->GetBufferSize());
		context->Unmap(instances[flip], 0);

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

ID3D11Buffer* CelestialBufferHandler::GetInstanceBuffer(unsigned int flip) const
{

	return instances[flip];

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

		if (instances[i] != nullptr)
		{

			instances[i]->Release();

		}
	}

	//lightLayout->Release();

}

CelestialBufferHandler::~CelestialBufferHandler()
{

	delete[] strides;
	delete[] instances;

}
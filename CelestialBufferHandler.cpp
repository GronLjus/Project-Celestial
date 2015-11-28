#include "stdafx.h"
#include "CelestialBufferHandler.h"
#include "CelestialMath.h"
using namespace Graphics;
using namespace CrossHandlers;
using namespace Resources;
using namespace CelestialMath;

CelestialBufferHandler::CelestialBufferHandler(ID3D10Device1* card)
{

	this->card = card;

	maxLights = 100;
	maxInstances = 200;//Gives us a buffersize of 200*2^7=25600 bytes
	strides = new UINT[BufferTypes_COUNT];
	strides[BufferTypes_VERTEX] = sizeof(Vertex);
	strides[BufferTypes_INSTANCE] = sizeof(Transformation);
	maxVertices = 1000;

}

HRESULT CelestialBufferHandler::InitVertexLayout(void* signature,SIZE_T size)
{

	D3D10_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{"MTRANSFORM",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D10_INPUT_PER_INSTANCE_DATA, 1},  
		{"MTRANSFORM",  1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_INSTANCE_DATA, 1}, 
		{"MTRANSFORM",  2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_INSTANCE_DATA, 1}, 
		{"MTRANSFORM",  3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_INSTANCE_DATA, 1},
		{"INVTRANMTRANSFORM",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_INSTANCE_DATA, 1},  
		{"INVTRANMTRANSFORM",  1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_INSTANCE_DATA, 1}, 
		{"INVTRANMTRANSFORM",  2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_INSTANCE_DATA, 1}, 
		{"INVTRANMTRANSFORM",  3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_INSTANCE_DATA, 1},  
		{"OMTRANSFORM",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D10_INPUT_PER_INSTANCE_DATA, 1},  
		{"OMTRANSFORM",  1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_INSTANCE_DATA, 1}, 
		{"OMTRANSFORM",  2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_INSTANCE_DATA, 1}, 
		{"OMTRANSFORM",  3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_INSTANCE_DATA, 1}
	};

	UINT numElements = 15;
	return card->CreateInputLayout(layout,numElements,signature,size,&vertexLayout);

}

HRESULT CelestialBufferHandler::InitTerrainVertexLayout(void* signature,SIZE_T size)
{

	D3D10_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT numElements = 3;
	terrainStride = sizeof(Vertex);
	return card->CreateInputLayout(layout,numElements,signature,size,&terrainLayout);

}

HRESULT CelestialBufferHandler::InitParticleVertexLayout(void* signature,SIZE_T size)
{

	D3D10_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "SIZE", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TIME", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "PSTIME", 0, DXGI_FORMAT_R32_SINT,0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "STIME", 0, DXGI_FORMAT_R32_SINT,1, 0, D3D10_INPUT_PER_INSTANCE_DATA, 1 },
		{"TRANSFORMATION",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_INSTANCE_DATA, 1},  
		{"TRANSFORMATION",  1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_INSTANCE_DATA, 1}, 
		{"TRANSFORMATION",  2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_INSTANCE_DATA, 1}, 
		{"TRANSFORMATION",  3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_INSTANCE_DATA, 1},
		{"FACTOR",  0, DXGI_FORMAT_R32G32_FLOAT, 1, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_INSTANCE_DATA, 1},  
		{"COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_INSTANCE_DATA, 1}, 
		{"ETIME",  0, DXGI_FORMAT_R32_SINT, 1, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_INSTANCE_DATA, 1},
		{"GLOW",  0, DXGI_FORMAT_R32_FLOAT, 1, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_INSTANCE_DATA, 1},
		{"GFADE",  0, DXGI_FORMAT_R32_FLOAT, 1, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_INSTANCE_DATA, 1}
	};

	UINT numElements = 15;
	return card->CreateInputLayout(layout,numElements,signature,size,&particleLayout);

}

HRESULT CelestialBufferHandler::InitLightVertexLayout(void* signature,SIZE_T size)
{

	D3D10_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "RADIUS", 0, DXGI_FORMAT_R32_FLOAT,0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "ANGLE", 0, DXGI_FORMAT_R32_FLOAT,0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "DIFFUSE", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "SPECULAR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "FORWARD", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "RIGHTWARD", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "SIZE", 0, DXGI_FORMAT_R32_FLOAT,0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT numElements = 8;
	lightStride = sizeof(Light);
	return card->CreateInputLayout(layout,numElements,signature,size,&lightLayout);

}

HRESULT CelestialBufferHandler::InitMesh(Resources::MeshObject* mesh)
{
	
	HRESULT hr = S_OK;

	if(mesh->GetBuffer(BufferTypes_VERTEX,0) == nullptr)//Fill the vertex buffer
	{

		Vertex* tempVerts = new Vertex[mesh->GetVertices()];//Use this array as the value for the buffer

		for(int i=0;i<mesh->GetVertices();i++)//Go through each vertexlevel in the mesh
		{

			Vector3 pos;
			Vector2 tex;
			Vector3 norm;
			Resources::MeshObject::Vertex* temp = mesh->GetVertex(i);
					
			if(temp->getVM() >= 3)
			{

				pos = Vector3(temp->getV()[0],temp->getV()[1],temp->getV()[2]);

			}
						
			if(temp->getVTM() >= 2)
			{

				tex = Vector2((temp->getVT())[0],1-(temp->getVT())[1]);

			}
						
			if(temp->getVNM() >= 3)
			{

				norm = Vector3((temp->getVN())[0],(temp->getVN())[1],(temp->getVN())[2]);

			}
						
			tempVerts[i] = Vertex(pos,tex,norm);
			
		}
		
		D3D10_BUFFER_DESC bd;
		bd.Usage = D3D10_USAGE_DEFAULT;
		bd.ByteWidth = strides[BufferTypes_VERTEX] * mesh->GetVertices(); //total size of buffer in bytes
		bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		D3D10_SUBRESOURCE_DATA initData;
		initData.pSysMem = tempVerts;
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;

		DXBufferObject* tempDXBuffer = new DXBufferObject();
		ID3D10Buffer* tempBuffer;
		hr = card->CreateBuffer( &bd, &initData, &tempBuffer);
		delete[] tempVerts;

		tempDXBuffer->SetSize(mesh->GetVertices());
		tempDXBuffer->SetBuffer(tempBuffer);

		BufferObject* vBuffer = new BufferObject();
		vBuffer->AddDxBuffer(tempDXBuffer);
		mesh->SetBuffer(BufferTypes_VERTEX,vBuffer,0);

	}	
	
	if(mesh->GetBuffer(BufferTypes_INDEX,0) == nullptr)//Fill the indexbuffer
	{

		int totalIndices = 0;
		int material = -1;
			
		Resources::MeshObject::Face* face = mesh->GetFirstFace();
		UINT* ind = new UINT[mesh->GetTotalIndices()*2];

		while(face != nullptr)//Go through each face in the mesh
		{
			
			if(face->GetMaterial() != material)
			{

				material = face->GetMaterial();
				mesh->getMaterials()[material]->SetStartingIndex(totalIndices);
				mesh->getMaterials()[material]->ResetCount();

			}
			
			for(int k = 0;k<face->getSize();k++)//Go through each index in the face
			{

				ind[totalIndices] = face->GetIndexAt(k);
				totalIndices++;

				if(face->GetAdjIndexAt(k) == -1)
				{
								
					ind[totalIndices] = face->GetIndexAt(k);

				}
				else
				{
							
					ind[totalIndices] = face->GetAdjIndexAt(k);

				}

				totalIndices++;

			}
		
			mesh->getMaterials()[material]->IncrementCount(face->getSize()*2);
			face = face->getNext();

		}
				
		D3D10_BUFFER_DESC bufferDesc;
		bufferDesc.Usage           = D3D10_USAGE_DEFAULT;
		bufferDesc.ByteWidth       = sizeof( unsigned int ) * totalIndices;
		bufferDesc.BindFlags       = D3D10_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags  = 0;
		bufferDesc.MiscFlags       = 0;
		D3D10_SUBRESOURCE_DATA initIndData;
		initIndData.pSysMem = ind;
		initIndData.SysMemPitch = 0;
		initIndData.SysMemSlicePitch = 0;
		
		ID3D10Buffer* tempBuffer;
		hr = card->CreateBuffer( &bufferDesc, &initIndData, &tempBuffer);

		DXBufferObject* tempDX = new DXBufferObject();

		tempDX->SetBuffer(tempBuffer);
		tempDX->SetSize(totalIndices);
		tempDX->SetCompany(mesh->GetBuffer(BufferTypes_VERTEX,0)->GetFirstDXBuffer());

		delete[] ind;

		BufferObject* iBuffer = new BufferObject();
		iBuffer->AddDxBuffer(tempDX);
		mesh->SetBuffer(BufferTypes_INDEX,iBuffer,0);

	}

	return hr;

}

HRESULT CelestialBufferHandler::InitParticleEmitter(IParticleEmitterBufferable* emitter)
{

	HRESULT hr = S_OK;

	if (emitter->GetBuffer(BufferTypes_VERTEX, 0) == nullptr)
	{

		int vertices = emitter->GetNrOfParticles();
		int levels = (vertices / maxVertices) + 1;
		int totalVertices = 0;
		BufferObject* buffer = new BufferObject();

		for (int i = 0; i < levels; i++)
		{

			int vertOnThisLevel = maxVertices;

			if (i == levels - 1)
			{

				vertOnThisLevel = vertices - ((vertices / maxVertices)*maxVertices);

			}

			Particle* particles = new Particle[vertOnThisLevel];

			for (int k = 0; k < vertOnThisLevel; k++)
			{

				Resources::Particle particle = emitter->GetParticle(totalVertices);
				particles[k] = Particle(particle.GetPosition(),
					particle.GetVelocity(),
					particle.GetSize(),
					Vector3(particle.GetLoopTime(), particle.GetFadeTime(), particle.GetTimeToLive()),
					particle.GetStartTime());
				totalVertices++;

			}

			D3D10_BUFFER_DESC bd;
			bd.Usage = D3D10_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(Particle)* vertOnThisLevel;//total size of buffer in bytes
			bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;
			D3D10_SUBRESOURCE_DATA initData;
			initData.pSysMem = particles;
			initData.SysMemPitch = 0;
			initData.SysMemSlicePitch = 0;

			ID3D10Buffer* bufferObject;
			hr = card->CreateBuffer(&bd, &initData, &bufferObject);
			delete[] particles;

			DXBufferObject* tempBuffer = new DXBufferObject();
			tempBuffer->SetSize(vertOnThisLevel);
			tempBuffer->SetBuffer(bufferObject);
			buffer->AddDxBuffer(tempBuffer);

		}

		emitter->SetBuffer(buffer, BufferTypes_VERTEX, 0);
		emitter->SetVertexStride(sizeof(Particle));
		emitter->SetInstanceStride(sizeof(ParticleInstance));

	}

	return hr;

}

void CelestialBufferHandler::InitTerrain(Resources::TerrainMesh* object)
{

	if(!object->IsBuffered())
	{

		int* iAmounts;
		int* vAmounts;
		UINT** indices = object->GetIndices(iAmounts);
		Resources::MeshObject::Vertex*** vertices = object->GetVertices(vAmounts);

		for(int k=0;k<object->GetLevels();k++)
		{

			if(indices[k] != nullptr)
			{

				ID3D10Buffer* buffer = nullptr;
				D3D10_BUFFER_DESC bufferDesc;
				bufferDesc.Usage           = D3D10_USAGE_DEFAULT;
				bufferDesc.ByteWidth       = sizeof( unsigned int ) * iAmounts[k];
				bufferDesc.BindFlags       = D3D10_BIND_INDEX_BUFFER;
				bufferDesc.CPUAccessFlags  = 0;
				bufferDesc.MiscFlags       = 0;
				D3D10_SUBRESOURCE_DATA initIndData;
				initIndData.pSysMem = indices[k];
				initIndData.SysMemPitch = 0;
				initIndData.SysMemSlicePitch = 0;
				HRESULT hr = card->CreateBuffer( &bufferDesc, &initIndData, &buffer);
				object->SetIndexBuffer(buffer,k);

			}

			if(vAmounts[k] > 0)
			{
			
				Vertex* verts = new Vertex[vAmounts[k]];
				ID3D10Buffer* buffer = nullptr;

				for(int i=0;i<vAmounts[k];i++)
				{
					Vector3 pos;
					Vector2 tex;
					Vector3 nor;
					Resources::MeshObject::Vertex* temp = vertices[k][i];
					
					if(temp->getVM() >= 3)
					{

						pos = Vector3(temp->getV()[0],temp->getV()[1],temp->getV()[2]);

					}
						
					if(temp->getVTM() >= 2)
					{

						tex = Vector2((temp->getVT())[0],1-(temp->getVT())[1]);

					}
					
					if(temp->getVNM() >= 3)
					{

						nor = Vector3(temp->getVN()[0],temp->getVN()[1],temp->getVN()[2]);

					}

					verts[i] = Vertex(pos,tex,nor);

				}

				D3D10_BUFFER_DESC bd;
				bd.Usage = D3D10_USAGE_DEFAULT;
				bd.ByteWidth = terrainStride * vAmounts[k];//total size of buffer in bytes
				bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
				bd.CPUAccessFlags = 0;
				bd.MiscFlags = 0;
				D3D10_SUBRESOURCE_DATA initData;
				initData.pSysMem = verts;
				initData.SysMemPitch = 0;
				initData.SysMemSlicePitch = 0;
				HRESULT hr = card->CreateBuffer( &bd, &initData, &buffer );
				delete[] verts;
				object->SetVertexBuffer(buffer,k);

			}
		}
	}
}

HRESULT CelestialBufferHandler::moveThroughBuffer(DXBufferObject* buffer,BufferObject* buff,int sizeO,int max)
{

	HRESULT hr = S_OK;
	void** mapped = buffer->GetMappedPointer();
	int size = buffer->GetSize();

	if(mapped == nullptr || size == max)//The buffer is either full or closed
	{

		
		if(size == maxInstances-1 || buffer->GetBuffer() == nullptr)//The buffer is full or we lack an internal buffer
		{

			if(buffer->GetNext() == nullptr || buffer->GetBuffer() == nullptr)//We are at the last buffer or we lack an internal buffer
			{
			
				//Create an internal buffer
				ID3D10Buffer* dBuffer = nullptr;
				D3D10_BUFFER_DESC ibd;
				ibd.Usage = D3D10_USAGE_DYNAMIC;
				ibd.ByteWidth = sizeO * max;//total size of buffer in bytes
				ibd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
				ibd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
				ibd.MiscFlags = 0;
				hr = card->CreateBuffer( &ibd, nullptr, &dBuffer);

				if(buffer->GetBuffer() != nullptr)//We are at the last buffer, create a new one
				{
					
					buffer->UnMap();
					buffer = new DXBufferObject();
					buff->AddDxBuffer(buffer);

				}
				
				buffer->SetBuffer(dBuffer);

			}
			else//Move to the next buffer
			{

				buffer = buffer->GetNext();
				
			}
		}
		else if(mapped == nullptr)//The buffer is closed, start from the beginning
		{

			buffer = buff->GetFirstDXBuffer();

		}
		
		mapped = nullptr;
		buffer->GetBuffer()->Map(D3D10_MAP_WRITE_DISCARD,0,(void**)&mapped);
		buffer->Map(mapped);
		buff->SetActiveDxBuffer(buffer);//Make sure we start from this buffer next time
		buffer->SetSize(0);

	}

	return hr;

}

HRESULT CelestialBufferHandler::AddInstance(IParticleEmitterBufferable* emitter,ParticleEmitterInstance* instance,int flip)
{

	HRESULT hr = S_OK;
	BufferObject* buff = emitter->GetBuffer(BufferTypes_INSTANCE,flip);

	if(buff == nullptr)
	{

		buff = new BufferObject();
		buff->AddDxBuffer(new DXBufferObject());
		emitter->SetBuffer(buff,BufferTypes_INSTANCE,flip);

	}

	DXBufferObject* buffer = buff->GetActiveDXBuffer();

	hr = moveThroughBuffer(buffer,buff,sizeof(ParticleInstance),maxInstances);

	ParticleInstance* instArray = (ParticleInstance*)buffer->GetMappedPointer();
	int size = buffer->GetSize();
	instArray[size] = ParticleInstance(instance->GetStartTime(),
										instance->GetTransformation(),
										Vector2(instance->GetVelocityFactor(),instance->GetSizeFactor()),
										instance->GetColour(),
										instance->GetEndTime(),
										instance->GetGlow(),
										instance->GetFade());

	size++;
	buffer->SetSize(size);
	return hr;

}

void CelestialBufferHandler::AddInstance(BufferObject* buffer, ResourceObject* container)
{

	DXBufferObject* dxBuffer = buffer->GetActiveDXBuffer();
	HRESULT hr;

	if(dxBuffer == nullptr)
	{

		buffer->AddDxBuffer(new DXBufferObject());
		dxBuffer = buffer->GetActiveDXBuffer();

	}

	hr = moveThroughBuffer(dxBuffer,buffer,sizeof(Transformation),maxInstances);
	
	Transformation* trans = (Transformation*)dxBuffer->GetMappedPointer();
	int size = dxBuffer->GetSize();

	Matrix invTranW = MatrixTranspose(container->GetInverseTransformation());
	Matrix oW = container->GetLastTransformation();
	Matrix w = container->PopTransformation();
	trans[size] = Transformation(w,invTranW,oW);
	dxBuffer->SetSize(size+1);

}

void CelestialBufferHandler::AddInstance(BufferObject* buffer,ILight* light)
{

	DXBufferObject* buff = buffer->GetActiveDXBuffer();

	if(buff == nullptr)
	{

		buff = new DXBufferObject();
		buffer->AddDxBuffer(buff);

	}
	
	HRESULT hr = moveThroughBuffer(buff,buffer,lightStride,maxLights);
	Light* mapped = (Light*)buff->GetMappedPointer();

	int size = buff->GetSize();
	Vector4 diff = light->GetDiff();
	Vector4 amb = light->GetSpec();
	Vector4 d = diff;
	Vector4 a = amb;
	float si = light->GetSize();

	mapped[size] = Light(light->GetPos(),light->GetIntensity(),light->GetAngle(),d,a,light->GetDirection(),light->GetSide(),si);
	buff->SetSize(size+1);

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

ID3D10InputLayout* CelestialBufferHandler::GetVertexLayout() const
{

	return vertexLayout;

}

ID3D10InputLayout* CelestialBufferHandler::GetParticleLayout() const
{

	return particleLayout;

}

ID3D10InputLayout* CelestialBufferHandler::GetTerrainLayout() const
{

	return terrainLayout;

}

ID3D10InputLayout* CelestialBufferHandler::GetLightLayout() const
{

	return lightLayout;

}

void CelestialBufferHandler::Release()
{

	vertexLayout->Release();
	particleLayout->Release();
	terrainLayout->Release();
	//lightLayout->Release();

}

CelestialBufferHandler::~CelestialBufferHandler()
{

	delete[] strides;

}
#pragma once
#include "../../Resources/Loaders/TerrainObjects/TerrainMesh.h"
#include "../../Resources/Objects/Lights/ILight.h"
#include "../../Resources/Objects/Container/ResourceObject.h"
#include "../../Resources/Objects/Particles/IParticleEmitterBufferable.h"
#include "../../Resources/Objects/Particles/ParticleEmitterInstance.h"

namespace Graphics
{

	///<summary>This interface abstacts the usage of bufferhandlers</summary>
	class IBufferHandler
	{

		public:
			///<summary>Initiates the vertexlayout this bufferHandler should use, must be called before any other method</summary>
			///<param name='signature'>[in]A pointer to the first shader in the pipeline</param>
			///<param name='size'>[in]The size of the shader</param>
			///<returns>Any error-codes</returns>
			virtual HRESULT InitVertexLayout(void* signature,SIZE_T size) = 0;
			///<summary>Initiates the vertexlayout this bufferHandler should use for terrains, must be called before any other method</summary>
			///<param name='signature'>[in]A pointer to the first shader in the pipeline</param>
			///<param name='size'>[in]The size of the shader</param>
			///<returns>Any error-codes</returns>
			virtual HRESULT InitTerrainVertexLayout(void* signature,SIZE_T size) = 0;
			///<summary>Initiates the vertexlayout this bufferHandler should use for particles, must be called before any other method</summary>
			///<param name='signature'>[in]A pointer to the first shader in the pipeline</param>
			///<param name='size'>[in]The size of the shader</param>
			///<returns>Any error-codes</returns>
			virtual HRESULT InitParticleVertexLayout(void* signature,SIZE_T size) = 0;
			///<summary>Initiates the vertexlayout this bufferHandler should use for lights, must be called before any other method</summary>
			///<param name='signature'>[in]A pointer to the first shader in the pipeline</param>
			///<param name='size'>[in]The size of the shader</param>
			///<returns>Any error-codes</returns>
			virtual HRESULT InitLightVertexLayout(void* signature,SIZE_T size) = 0;

			///<summary>Initiates buffers from a mesh, must be able to be called for the same object twice</summary>
			///<param name='mesh'>[in]A pointer to the meshobject to initiate</param>
			///<returns>Any error-codes</returns>
			virtual HRESULT InitMesh(Resources::MeshObject* mesh) = 0;
			///<summary>Gets the vertexbuffer for a terrains underobject</summary>
			///<param name='object'>[in]A pointer to the terrainMesh to initilize, must be called before the terrain is drawed</param>
			virtual void InitTerrain(Resources::TerrainMesh* object) = 0;
			///<summary>Initiates the vertexbuffers for an emitter, must be able to be called for the same object twice</summary>
			///<param name='emitter'>[in]A pointer to the Particleemitter to initiate</param>
			///<returns>Any error-codes</returns>
			virtual HRESULT InitParticleEmitter(Resources::IParticleEmitterBufferable* emitter) = 0;

			///<summary>Adds an instance to an emitter</summary>
			///<param name='emitter'>[in]A pointer to the emitter to load instancs to</param>
			///<param name='instance'>[in]A pointer to the instance</param>
			///<param name='flip'>[in]The flip to use</param>
			///<returns>Any error-codes</returns>
			virtual HRESULT AddInstance(Resources::IParticleEmitterBufferable* emitter, Resources::ParticleEmitterInstance* instance, int flip) = 0;
			///<summary>Adds a new instance to the instance buffer</summary>
			///<param name='buffer'>[in]A pointer to the buffer to add instances to</param>
			///<param name='container'>[in]A pointer to the container with the instancedata to add</param>
			virtual void AddInstance(CrossHandlers::BufferObject* buffer,Resources::ResourceObject* container) = 0;
			///<summary>Adds the light to a buffer</summary>
			///<param name='object'>[in]A pointer to the bufferobject to load the light into</param>
			///<param name='light'>[in]A pointer to the light to load values from</param>
			///<param name='shadows'>[in]A pointer to the resourceobject containing the shadows</param>
			virtual void AddInstance(CrossHandlers::BufferObject* buffer, Resources::ILight* light) = 0;

			///<summary>Gets the strides for a certain mesh and buffertype</summary>
			///<param name='mesh'>[in]A pointer to the meshobject to load strides from</param>
			///<param name='buffer'>[in]The type of buffer strides should be loaded for</param>
			///<returns>The size of a stride in the chosen buffer</returns>
			virtual UINT GetStride(CrossHandlers::BufferTypes buffer) const = 0;
			///<summary>Gets the stride of the lights</summary>
			///<returns>The stride of the light-vertex in bytes</returns>
			virtual UINT GetLightStride() const = 0;
			///<summary>Gets the terrain's stride</summary>
			///<returns>The stride of the terrain-vertex in bytes</returns>
			virtual UINT GetTerrainStride() const = 0;

			///<summary>Gets the layout for the vertexbuffer</summary>
			///<returns>A pointer to the layout used by this bufferhandler</returns>
			virtual ID3D10InputLayout* GetVertexLayout() const = 0;
			///<summary>Gets the layout for the particle system</summary>
			///<returns>A pointer to the layout used by this bufferhandler</returns>
			virtual ID3D10InputLayout* GetParticleLayout() const = 0;
			///<summary>Gets the layout for the lights</summary>
			///<returns>A pointer to the layout used by this bufferhandler</returns>
			virtual ID3D10InputLayout* GetLightLayout() const = 0;
			///<summary>Gets the vertexlayout the terrain uses</summary>
			///<returns>A pointer to the layout of the terrain-vertices</returns>
			virtual ID3D10InputLayout* GetTerrainLayout() const = 0;
		
			///<summary>Releases the data used by this handler, must be called at the end of the objects life</summary>
			virtual void Release() = 0;
			virtual ~IBufferHandler(){}

			///<summary>This struct contains the data needed to represent a vertex with only a position</summary>
			struct PosVertex
			{
				///<summary>The vertexes' position</summary>
				CelestialMath::Vector3 pos;

				///<param name='p'>[in]The position of the vertex</param>
				PosVertex(CelestialMath::Vector3 p) : pos(p){}
				PosVertex(){}

			};
	};
}
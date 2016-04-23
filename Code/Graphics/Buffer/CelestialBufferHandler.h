#pragma once
#include "DrawingBoard.h"
#include "TerrainMesh.h"
#include "ILight.h"
#include "ResourceObject.h"

using namespace CelestialMath;

namespace Graphics
{

	///<summary>This class encapsulates the buffers needed to draw to CelestialShader</summary>
	class CelestialBufferHandler
	{

		public:
			///<param name='card'>[in]The device to create buffers from</param>
			///<param name='meshDictionary'>[in]The meshDictionary to use as id-reference</param>
			CelestialBufferHandler(ID3D11Device* card,unsigned int flips,unsigned int maxInstances);

			void UpdateMeshBuffers(Entities::DrawingBoard* db, ID3D11DeviceContext* context);
			void UpdateInstanceBuffer(Entities::DrawingBoard* db, ID3D11DeviceContext* context, unsigned int flip, unsigned int chain);

			ID3D11Buffer* GetVertexBuffer() const;
			ID3D11Buffer* GetIndexBuffer() const;
			ID3D11Buffer* GetInstanceBuffer(unsigned int flip, unsigned int chain) const;

			HRESULT InitVertexLayout(void* signature,SIZE_T size);
			HRESULT InitTerrainVertexLayout(void* signature,SIZE_T size);
			HRESULT InitParticleVertexLayout(void* signature,SIZE_T size);
			HRESULT InitLightVertexLayout(void* signature,SIZE_T size);

			UINT GetStride(CrossHandlers::BufferTypes buffer) const;
			UINT GetTerrainStride() const;
			UINT GetLightStride() const;

			ID3D11InputLayout* GetVertexLayout() const;
			ID3D11InputLayout* GetParticleLayout() const;
			ID3D11InputLayout* GetTerrainLayout() const;
			ID3D11InputLayout* GetLightLayout() const;

			virtual void Release();
			~CelestialBufferHandler();

		private:

			///<summary>This struct contains the data needed to represent a matrix</summary>
			struct BufferMatrix
			{

				///<summary>The first row</summary>
				Vector4 a;
				///<summary>The second row</summary>
				Vector4 b;
				///<summary>The third row</summary>
				Vector4 c;
				///<summary>The fourth row</summary>
				Vector4 d;

				///<param name='x'>[in]The first row in the matrix</param>
				///<param name='y'>[in]The second row in the matrix</param>
				///<param name='z'>[in]The third row in the matrix</param>
				///<param name='w'>[in]The fourth row in the matrix</param>
				BufferMatrix( Vector4 x,Vector4 y,Vector4 z,Vector4 w ) : a(x), b(y),c(z),d(w) {}
				///<param name='m'>[in]The matrix to use</param>
				BufferMatrix(Matrix m):a(m._11,m._12,m._13,m._14),b(m._21,m._22,m._23,m._24),c(m._31,m._32,m._33,m._34),d(m._41,m._42,m._43,m._44){}
				BufferMatrix(){}
			};
		
			///<summary>This struct contains the data that represents a particle</summary>
			struct Particle
			{
				///<summary>The position of the particle</summary>
				Vector3 pos;
				///<summary>The velocity of the particle</summary>
				Vector3 vel;
				///<summary>The size of the particle</summary>
				Vector3 size;
				///<summary>The times of the particle</summary>
				Vector3 time;
				///<summary>The time this particle will start in milliseconds</summary>
				int startTime;
				///<param name='p'>[in]The position of the vector</param>
				///<param name='t'>[in]The texture of the vector</param>
				///<param name='n'>[in]The normal of the vector</param>
				///<param name='st'>[in]The time this particle will start</param>
				Particle( Vector3 p,Vector3 v,Vector3 s,Vector3 t,int st ) : pos(p), vel(v),size(s),time(t),startTime(st) {}
				Particle(){}
			};

			///<summary>This struct contains the data that represents an instance of a particlesystem</summary>
			struct ParticleInstance
			{
				///<summary>The time this instance started in milliseconds</summary>
				int startTime;
				///<summary>The first column of the transformation</summary>
				Vector4 wA;
				///<summary>The second column of the transformation</summary>
				Vector4 wB;
				///<summary>The third column of the transformation</summary>
				Vector4 wC;
				///<summary>The fourth column of the transformation</summary>
				Vector4 wD;
				
				///<summary>The factors of this instance</summary>
				Vector2 factors;
				///<summary>The position of the particle</summary>
				Vector4 colour;
				///<summary>When the instance will pass on, become no more, cease to be, expire and go to meet it's maker, become a LATE instance</summary>
				int endTime;
				///<summary>How much of the instances particles will have a glow</summary>
				float glow;
				///<summary>How much of the particles glow will be a fade from white to colour</summary>
				float fade;

				///<param name='st'>[in]The starttime of this instance</param>
				///<param name='w'>[in]The transformation matrix of the instance</param>
				///<param name='f'>[in]The size and velocity factors of this instance</param>
				///<param name='c'>[in]The colour of the particles in this instance</param>
				///<param name='et'>[in]The endtime of the instances</param>
				///<param name='g'>[in]The glowfactor of the instance</param>
				///<param name='fd'>[in]How much of the particles glow will be a fade from white to colour</param>
				ParticleInstance(int st,Matrix w,Vector2 f,Vector4 c,int et,float g,float fd)
				{

					startTime = st;
					wA = Vector4(w._11,w._21,w._31,w._41);
					wB = Vector4(w._12,w._22,w._32,w._42);
					wC = Vector4(w._13,w._23,w._33,w._43);
					wD = Vector4(w._14,w._24,w._34,w._44);
					factors = f;
					colour = c;
					endTime = et;
					glow = g;
					fade = fd;

				}
				ParticleInstance();
			};

			///<summary>This struct contains the data representing a light</summary>
			struct Light
			{
				///<summary>The position of the light</summary>
				Vector3 pos;
				///<summary>The radius of the light</summary>
				float radius;
				///<summary>The angle between the midpoint and lightside</summary>
				float angle;
				///<summary>The diffuse colour of the light</summary>
				Vector4 diffuse;
				///<summary>The specular colour of the light</summary>
				Vector4 specular;
				///<summary>The vector of the lights forward direction</summary>
				Vector3 forward;
				///<summary>The vector of the lights rightward direction</summary>
				Vector3 right;
				///<summary>The size of the light</summary>
				float size;

				///<param name='r'>[in]The radius of the light</param>
				///<param name='ra'>[in]The diffuse colour of the light</param>
				///<param name='a'>[in]The angle between the midpoint and lightside</param>
				///<param name='d'>[in]The specular colour of the light</param>
				///<param name='s'>[in]The vector of the lights forward direction</param>
				///<param name='r'>[in]The vector of the lights rightward direction</param>
				///<param name='si'>[in]The size of the light</param>
				Light(Vector3 p,float ra,float a,Vector4 d,Vector4 s,Vector3 f,Vector3 r,float si) : pos(p),radius(ra),angle(a),diffuse(d),specular(s),forward(f),right(r),size(si){}

			};


			///<summary> A double buffer containing all strides for buffers</summary>
			UINT* strides;
		
			///<summary>A pointer to the device used by this handler</summary>
			ID3D11Device* card;
			///<summary>A pointer to the vertexlayout this buffer uses</summary>
			ID3D11InputLayout* vertexLayout;
			///<summary>A pointer to the particlelayout this buffer uses</summary>
			ID3D11InputLayout* particleLayout;
			///<summary>A pointer to the vertexlayout this buffer uses for terrains</summary>
			ID3D11InputLayout* terrainLayout;
			///<summary>A pointer to the vertexlayout this buffer uses for lights</summary>
			ID3D11InputLayout* lightLayout;
			///<summary>The size of the vertex used for terrains</summary>
			UINT terrainStride;
			///<summary>The size of the vertex used for lights</summary>
			UINT lightStride;

			///<summary>The maximum number of vertices in a buffer</summary>
			int maxVertices;
			///<summary>The maximum number of lights in a buffer</summary>
			int maxLights;

			unsigned int maxInstances;
			ID3D11Buffer* vertices;
			ID3D11Buffer* indices;
			ID3D11Buffer*** instances;
			unsigned int* instanceChain;
			unsigned int instantFlip;
			unsigned int instantFlips;

	};
}
#pragma once
#include "IBufferHandler.h"

using namespace CelestialMath;

namespace Graphics
{

	///<summary>This class encapsulates the buffers needed to draw to CelestialShader</summary>
	class CelestialBufferHandler : public IBufferHandler
	{

		public:
			///<param name='card'>[in]The device to create buffers from</param>
			///<param name='meshDictionary'>[in]The meshDictionary to use as id-reference</param>
			CelestialBufferHandler(ID3D10Device1* card);

			virtual HRESULT InitVertexLayout(void* signature,SIZE_T size);
			virtual HRESULT InitTerrainVertexLayout(void* signature,SIZE_T size);
			virtual HRESULT InitParticleVertexLayout(void* signature,SIZE_T size);
			virtual HRESULT InitLightVertexLayout(void* signature,SIZE_T size);

			virtual HRESULT InitMesh(Resources::MeshObject* mesh);
			virtual HRESULT InitParticleEmitter(Resources::IParticleEmitterBufferable* emitter);
			virtual void InitTerrain(Resources::TerrainMesh* object);

			virtual HRESULT AddInstance(Resources::IParticleEmitterBufferable* emitter, Resources::ParticleEmitterInstance* instance, int flip);
			virtual void AddInstance(CrossHandlers::BufferObject* buffer, Resources::ResourceObject* container);
			virtual void AddInstance(CrossHandlers::BufferObject* buffer, Resources::ILight* light);

			virtual UINT GetStride(CrossHandlers::BufferTypes buffer) const;
			virtual UINT GetTerrainStride() const;
			virtual UINT GetLightStride() const;

			virtual ID3D10InputLayout* GetVertexLayout() const;
			virtual ID3D10InputLayout* GetParticleLayout() const;
			virtual ID3D10InputLayout* GetTerrainLayout() const;
			virtual ID3D10InputLayout* GetLightLayout() const;

			virtual void Release();
			~CelestialBufferHandler();

		private:
			///<summary>This struct contains all data needed to represents a vertex</summary>
			struct Vertex
			{

				///<summary>The position of the vertex</summary>
				Vector3 pos;
				///<summary>The UV-coordinates of the vertex</summary>
				Vector2 texture;
				///<summary>The normal of the vertex</summary>
				Vector3 normal;
			
				///<param name='p'>[in]The position of the vector</param>
				///<param name='t'>[in]The texture of the vector</param>
				///<param name='n'>[in]The normal of the vector</param>
				Vertex( Vector3 p,Vector2 t,Vector3 n ) : pos(p), texture(t),normal(n) {}
				Vertex(){}
			};

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
		
			///<summary>This struct contains the data needed to represent an instances transformation</summary>
			struct Transformation
			{
				///<summary>The first column of the world-matrix</summary>
				Vector4 wA;
				///<summary>The second column of the world-matrix</summary>
				Vector4 wB;
				///<summary>The third column of the world-matrix</summary>
				Vector4 wC;
				///<summary>The fourth column of the world-matrix</summary>
				Vector4 wD;

				///<summary>The first column of the inverted transposed world-matrix</summary>
				Vector4 iWA;
				///<summary>The second column of the inverted transposed world-matrix</summary>
				Vector4 iWB;
				///<summary>The third column of the inverted transposed world-matrix</summary>
				Vector4 iWC;
				///<summary>The fourth column of the inverted transposed world-matrix</summary>
				Vector4 iWD;
			
				///<summary>The first column of the old world-matrix</summary>
				Vector4 oWA;
				///<summary>The second column of the old world-matrix</summary>
				Vector4 oWB;
				///<summary>The third column of the old world-matrix</summary>
				Vector4 oWC;
				///<summary>The fourth column of the old world-matrix</summary>
				Vector4 oWD;

				///<param name='w'>[in]The world-matrix of the transformation</param>
				///<param name='iWT'>[in]The inverted transposed world-matrix of the transformation</param>
				///<param name='w'>[in]The last world-matrix of the transformation</param>
				Transformation(Matrix w,Matrix iWT,Matrix oWT)
				{

					wA = Vector4(w._11,w._21,w._31,w._41);
					wB = Vector4(w._12,w._22,w._32,w._42);
					wC = Vector4(w._13,w._23,w._33,w._43);
					wD = Vector4(w._14,w._24,w._34,w._44);
				
					iWA = Vector4(iWT._11,iWT._21,iWT._31,iWT._41);
					iWB = Vector4(iWT._12,iWT._22,iWT._32,iWT._42);
					iWC = Vector4(iWT._13,iWT._23,iWT._33,iWT._43);
					iWD = Vector4(iWT._14,iWT._24,iWT._34,iWT._44);
				
					oWA = Vector4(oWT._11,oWT._21,oWT._31,oWT._41);
					oWB = Vector4(oWT._12,oWT._22,oWT._32,oWT._42);
					oWC = Vector4(oWT._13,oWT._23,oWT._33,oWT._43);
					oWD = Vector4(oWT._14,oWT._24,oWT._34,oWT._44);

				}

				Transformation(){}

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

			///<summary>Moves through the list of buffers and gets the size and mapped array</summary>
			///<param name='buffer'>[in]A pointer to the buffer to peruse</param>
			///<param name='buff'>[in]A pointer to the bufferobject</param>
			///<param name='sizeO'>[in]The size of the struct to use</param>
			///<param name='max'>[in]The maximum amount of elements</param>
			///<returns>Any error-codes</returns>
			HRESULT moveThroughBuffer(CrossHandlers::DXBufferObject* buffer,CrossHandlers::BufferObject* buff,int sizeO,int max);
			///<summary>The maximum number of instances in a instancebuffer</summary>
			int maxInstances;

			///<summary> A double buffer containing all strides for buffers</summary>
			UINT* strides;
		
			///<summary>A pointer to the device used by this handler</summary>
			ID3D10Device1* card;
			///<summary>A pointer to the vertexlayout this buffer uses</summary>
			ID3D10InputLayout* vertexLayout;
			///<summary>A pointer to the particlelayout this buffer uses</summary>
			ID3D10InputLayout* particleLayout;
			///<summary>A pointer to the vertexlayout this buffer uses for terrains</summary>
			ID3D10InputLayout* terrainLayout;
			///<summary>A pointer to the vertexlayout this buffer uses for lights</summary>
			ID3D10InputLayout* lightLayout;
			///<summary>The size of the vertex used for terrains</summary>
			UINT terrainStride;
			///<summary>The size of the vertex used for lights</summary>
			UINT lightStride;

			///<summary>The maximum number of vertices in a buffer</summary>
			int maxVertices;
			///<summary>The maximum number of lights in a buffer</summary>
			int maxLights;
	};
}
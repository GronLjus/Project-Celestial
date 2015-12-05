#pragma once
#include "BufferObject.h"
#include "MeshObject.h"

namespace Entities
{

	class DrawingBoard
	{
	
		public:
			DrawingBoard();
			void AddMesh(Resources::MeshObject* mesh);
			CrossHandlers::BufferObject* GetVertexBuffers() const;
			CrossHandlers::BufferObject* GetIndexBuffers() const;
			~DrawingBoard();

		private:
			///<summary>This struct contains all data needed to represents a vertex</summary>
			struct Vertex
			{

				///<summary>The position of the vertex</summary>
				CelestialMath::Vector3 pos;
				///<summary>The UV-coordinates of the vertex</summary>
				CelestialMath::Vector2 texture;
				///<summary>The normal of the vertex</summary>
				CelestialMath::Vector3 normal;

				///<param name='p'>[in]The position of the vector</param>
				///<param name='t'>[in]The texture of the vector</param>
				///<param name='n'>[in]The normal of the vector</param>
				Vertex(CelestialMath::Vector3 p, CelestialMath::Vector2 t, CelestialMath::Vector3 n) : pos(p), texture(t), normal(n) {}
				Vertex(){}
			};

			void addObjectToVertexBuffer(Resources::MeshObject* mesh);
			void addObjectToIndexBuffer(Resources::MeshObject* mesh);
			CrossHandlers::BufferObject* vertexBuffer;
			CrossHandlers::BufferObject* indexBuffer;

	};
}
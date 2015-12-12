#pragma once
#include "BufferPoints.h"
#include "BufferObj.h"
#include "MeshObject.h"
#include "GameObject.h"

namespace Entities
{

	class DrawingBoard
	{
	
		public:
			DrawingBoard();
			void AddMesh(Resources::MeshObject* mesh);
			unsigned int AddInstance(Resources::GameObject* object);
			CrossHandlers::BufferObject2<CrossHandlers::BufferVertex>* GetVertexBuffers() const;
			CrossHandlers::BufferObject2<CrossHandlers::Instance>* GetInstanceBuffer() const;
			CrossHandlers::BufferObject2<unsigned int>* GetIndexBuffers() const;
			~DrawingBoard();

		private:
			

			unsigned int addObjectToVertexBuffer(Resources::MeshObject* mesh);
			void addObjectToIndexBuffer(Resources::MeshObject* mesh, unsigned int offset);
			CrossHandlers::BufferObject2<CrossHandlers::BufferVertex>* vertexBuffer;
			CrossHandlers::BufferObject2<unsigned int>* indexBuffer;
			CrossHandlers::BufferObject2<CrossHandlers::Instance>* instanceBuffer;

	};
}
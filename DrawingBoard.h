#pragma once
#include "BufferPoints.h"
#include "BufferObj.h"
#include "MeshObject.h"
#include "GameObject.h"
#include "CelestialSlicedList.h"
#include "CelestialStack.h"
#include "ViewObject.h"

namespace Entities
{

	class DrawingBoard
	{
	
		public:
			DrawingBoard();
			void AddMesh(Resources::MeshObject* mesh);
			void StartAddingInstances();
			unsigned int AddInstance(Resources::GameObject* object);
			void FinalizeInstances(Entities::ViewObject* onView);
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
			CrossHandlers::CelestialSlicedList<CrossHandlers::BufferObject2<CrossHandlers::Instance>*>* meshInstances;
			CrossHandlers::CelestialSlicedList<unsigned int>* meshDictionary;
			CrossHandlers::CelestialSlicedList<bool>* hasInstance;
			CrossHandlers::CelestialStack<unsigned int>* meshInstance;

	};
}
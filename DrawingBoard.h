#pragma once
#include "BufferPoints.h"
#include "BufferObj.h"
#include "MeshObject.h"
#include "GameObject.h"
#include "CelestialSlicedList.h"
#include "CelestialStack.h"
#include "ViewObject.h"
#include "CelMesh.h"
#include "GraphicalMesh.h"

namespace Entities
{

	class DrawingBoard
	{
	
		public:
			DrawingBoard(unsigned char maxFlips, unsigned int maxInstances);
			unsigned int AddMesh(Resources::CelMesh* mesh);
			Graphics::GraphicalMesh* GetMeshes() const;
			void StartAddingInstances();
			void AddInstance(Resources::GameObject* object);
			void AddInstance(CelestialMath::Matrix objectMatrix, CelestialMath::Matrix objectMatrixInvTr, unsigned int meshId);
			void FinalizeInstances(Entities::ViewObject* onView);
			CrossHandlers::BufferObject2<CrossHandlers::BufferVertex>* GetVertexBuffers() const;
			CrossHandlers::BufferObject2<CrossHandlers::Instance>* GetInstanceBuffer() const;
			CrossHandlers::BufferObject2<unsigned int>* GetIndexBuffers() const;
			~DrawingBoard();

		private:
			
			unsigned int maxInstances;
			unsigned char maxFlips;
			unsigned char bufflip;
			unsigned int meshes;
			Graphics::GraphicalMesh* meshesArr;
			unsigned int addObjectToVertexBuffer(Resources::CelMesh* mesh);
			void addObjectToIndexBuffer(Resources::CelMesh* mesh, unsigned int offset);
			CrossHandlers::BufferObject2<CrossHandlers::BufferVertex>* vertexBuffer;
			CrossHandlers::BufferObject2<unsigned int>* indexBuffer;
			CrossHandlers::BufferObject2<CrossHandlers::Instance>* instanceBuffer;
			CrossHandlers::CelestialSlicedList<CrossHandlers::BufferObject2<CrossHandlers::Instance>*>* meshInstances;
			CrossHandlers::CelestialSlicedList<unsigned int>* meshDictionary;
			CrossHandlers::CelestialSlicedList<bool>* hasInstance;
			CrossHandlers::CelestialStack<unsigned int>* meshInstance;

	};
}
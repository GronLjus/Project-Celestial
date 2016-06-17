#pragma once
#include "CelestialSlicedList.h"
#include "BoundingBox.h"
#include "CelestialMath.h"
#include "DrawingBoard.h"

namespace CrossHandlers
{
	class ObjectTree
	{

		public:
			ObjectTree(unsigned int cells, unsigned int minCells, CelestialMath::Vector2 position, unsigned int mesh);
			void AddObject(Resources::GameObject* obj);
			unsigned int AddInstance(Entities::ViewObject* view, Entities::DrawingBoard* board);
			unsigned int GetClosestObject(CelestialMath::Vector3 origin, CelestialMath::Vector3 unitDirection, float &smallestDistance) const;
			unsigned int* GetCollidedObject(Resources::GameObject* obj, unsigned int &objects) const; 
			unsigned int* GetCollidedObject(Resources::GameObject* obj, unsigned int* alreadyCollided, unsigned int aca, unsigned int &objects) const;

			bool RemoveObject(unsigned int id);
			BoundingBox* GetBox() const;
			unsigned int GetObjects() const;
			~ObjectTree();

		private:
			void expandCollidedArray();
			unsigned int objectAmountMax;
			unsigned int size;
			unsigned int gridMeshId;
			CelestialMath::Matrix gridMatrix;
			CelestialMath::Matrix gridInvTrMatrix;
			BoundingBox* box;
			CelestialSlicedList<Resources::GameObject*>* objects;
			ObjectTree** subTrees;

			unsigned int* collidedObjects;
			unsigned int collidedObjectAmounts;
	};
}
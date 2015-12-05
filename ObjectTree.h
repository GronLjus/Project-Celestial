#pragma once
#include "GameObject.h"
#include "CelestialSlicedList.h"
#include "BoundingBox.h"
#include "CelestialMath.h"

namespace CrossHandlers
{
	class ObjectTree
	{

		public:
			ObjectTree(unsigned int cells, unsigned int minCells, unsigned int cellSize, CelestialMath::Vector2 position);
			void AddObject(Resources::GameObject* obj);
			BoundingBox* GetBox() const;
			~ObjectTree();

		private:
			unsigned int size;
			BoundingBox* box;
			CelestialSlicedList<Resources::GameObject*>* objects;
			ObjectTree** subTrees;

	};
}
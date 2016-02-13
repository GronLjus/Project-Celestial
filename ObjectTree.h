#pragma once
#include "GameObject.h"
#include "CelestialSlicedList.h"
#include "BoundingBox.h"
#include "CelestialMath.h"
#include "DrawingBoard.h"

namespace CrossHandlers
{
	class ObjectTree
	{

		public:
			ObjectTree(unsigned int cells, unsigned int minCells, CelestialMath::Vector2 position);
			void AddObject(Resources::GameObject* obj);
			unsigned int AddInstance(Entities::ViewObject* view, Entities::DrawingBoard* board);
			unsigned int GetClosestObject(CelestialMath::Vector3 origin, CelestialMath::Vector3 unitDirection,float &smallestDistance) const;
			BoundingBox* GetBox() const;
			unsigned int GetObjects() const;
			~ObjectTree();

		private:
			unsigned int objectAmountMax;
			unsigned int size;
			BoundingBox* box;
			CelestialSlicedList<Resources::GameObject*>* objects;
			ObjectTree** subTrees;

	};
}
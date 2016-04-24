#pragma once
#include "../../Resources/Objects/GameObject.h"
#include "CelestialSlicedList.h"
#include "../Bounding/Shapes/BoundingBox.h"
#include "../../CelestialMath/CelestialMath.h"
#include "../../Entities/Objects/DrawingBoard.h"

namespace CrossHandlers
{
	class ObjectTree
	{

		public:
			ObjectTree(unsigned int cells, unsigned int minCells, CelestialMath::Vector2 position, unsigned int mesh);
			void AddObject(Resources::GameObject* obj);
			unsigned int AddInstance(Entities::ViewObject* view, Entities::DrawingBoard* board);
			unsigned int GetClosestObject(CelestialMath::Vector3 origin, CelestialMath::Vector3 unitDirection,float &smallestDistance) const;
			bool RemoveObject(unsigned int id);
			BoundingBox* GetBox() const;
			unsigned int GetObjects() const;
			~ObjectTree();

		private:
			unsigned int objectAmountMax;
			unsigned int size;
			unsigned int gridMeshId;
			CelestialMath::Matrix gridMatrix;
			CelestialMath::Matrix gridInvTrMatrix;
			BoundingBox* box;
			CelestialSlicedList<Resources::GameObject*>* objects;
			ObjectTree** subTrees;

	};
}
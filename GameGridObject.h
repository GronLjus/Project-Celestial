#pragma once
#include "GameObject.h"

namespace Resources
{

	class GameGridObject : public GameObject
	{

	public:
		GameGridObject(CrossHandlers::BoundingBox* baseBox, CrossHandlers::BoundingSphere* baseSphere, 
			unsigned int meshId, std::string name);
		GameGridObject();

		virtual GameObjectType GetType() const;

		void SizeGrid(float nodeWidth);

		void PopulateGrid(unsigned int node, unsigned int x, unsigned int y);

		unsigned int GetNodeWidth() const;
		unsigned int GetNodeHeigth() const;

		unsigned int GetNode(unsigned int x, unsigned int y);
		unsigned int* GetNodes() const;

		CrossHandlers::Intersection ContainsPoint(CelestialMath::Vector3 point);


		//Serializable Interface
		virtual char* Serialize(unsigned int &size);
		virtual char* Unserialize(char* data);

		virtual ~GameGridObject();

	private:
		unsigned int gridWidth;
		unsigned int gridHeight;

		unsigned int* nodes;

		void sizeGrid();

	};
}
#pragma once
#include "IShapeLoader.h"

namespace Resources
{

	///<summary>This class loads a plane</summary>
	class GridLoader : public IShapeLoader
	{

	public:

		MeshObject* LoadGrid(unsigned int cells, float gridSize) const;
		virtual MeshObject* LoadShape(MeshObject::Material* mat);
		virtual ~GridLoader();

	};
}
#pragma once
#include "IShapeLoader.h"

namespace Resources
{

	///<summary>This class loads a plane</summary>
	class GridLoader : public IShapeLoader
	{

	public:

		CelMesh* LoadGrid(unsigned int cells, float gridSize) const;
		virtual CelMesh* LoadShape();
		virtual ~GridLoader();

	};
}
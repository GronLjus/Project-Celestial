#pragma once
#include "IShapeLoader.h"

namespace Resources
{

	///<summary>This class loads a plane</summary>
	class PlaneLoader: public IShapeLoader
	{

		public:

			virtual CelMesh* LoadShape();
			virtual ~PlaneLoader();

	};
}
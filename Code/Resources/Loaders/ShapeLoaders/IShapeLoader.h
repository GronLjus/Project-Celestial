#pragma once
#include "../../Objects/MeshObject/MeshObject.h"

namespace Resources
{

	///<summary>This interfaces provides the basic functionality for all shapeloaders</summary>
	class IShapeLoader
	{

		public:
			///<summary>Loads the shape into a meshobject</summary>
			///<param name='mat'>[in]A pointer to the material this shape should use</param>
			///<returns>A pointer to the meshobject created by the loader</returns>
			virtual MeshObject* LoadShape(MeshObject::Material* mat) = 0;
			virtual ~IShapeLoader(){}

	};
}
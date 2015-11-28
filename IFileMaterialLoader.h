#pragma once

#include "IFileLoader.h"

namespace Resources
{

	class IFileMaterialLoader : public IFileLoader
	{
		
		public:
			///<summary>Loads materials from a file</summary>
			///<param name='text'>[in]The name of the file containing the material</param>
			///<param name='size'>[out]The size of the array</param>
			///<returns>A pointer to the array material loaded or nullptr</returns>
			virtual MeshObject::Material** LoadMaterial(std::string file,int &size) = 0;

			virtual ~IFileMaterialLoader(){}
	};
}
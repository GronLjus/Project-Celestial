#pragma once

#include "IFileLoader.h"
#include "CelMesh.h"
#include <string>

namespace Resources
{

	///<summary>This interfaces virtualizes the loading of files</summary>
	class IFileMeshLoader : public IFileLoader
	{
		public:

			///<summary>Loads a mesh</summary>
			///<param name='text'>[in]The name of the file containing the mesh</param>
			///<returns>A meshobject containing the mesh-data</returns>
			virtual CelMesh* Load(std::string text) = 0;
			virtual ~IFileMeshLoader(){}

	};
}
#pragma once
#include "IFileLoader.h"
#include "TerrainObject.h"

namespace Resources
{

	///<summary>This interface provides the funcionality required to load terrain from a file</summary>
	class IFileTerrainLoader:public IFileLoader
	{

		public:
			///<summary>Loads a terrain</summary>
			///<param name='text'>[in]The name of the file containing the terrain-data</param>
			///<returns>A pointer to the terrainobject containing the loaded data</returns>
			virtual TerrainObject* Load(std::string text) = 0;
			virtual ~IFileTerrainLoader(){}

	};
}
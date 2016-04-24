#pragma once

#include "IContainable.h"
#include "../../../CrossHandler-Tools/Container/CelestialSlicedList.h"

namespace Resources
{

	///<summary>This class handles the containment of meshes</summary>
	class ResourceContainer
	{

		public:
			ResourceContainer();

			///<summary>Adds a mesh to the internal list</summary>
			///<param name='object'>[in]A pointer to the object to add</param>
			///<returns>The id of the object</returns>
			int AddObject(IContainable* object);

			///<summary>Deletes a mesh from the internal list</summary>
			///<param name='object'>[in]The id to remove</param>
			void DeleteObject(int id);

			///<summary>Gets a object</summary>
			///<param name='id'>[in]The id to get</param>
			///<returns>A pointer to the object with selected id</returns>
			IContainable* GetDataObject(int id);

			~ResourceContainer();

		private:

			///<summary>The list of all objects</summary>
			CrossHandlers::CelestialSlicedList<IContainable*>* theList;
	};
}
#pragma once

#include "../CrossHandler-Tools/Container/CelestialList.h"
#include "../Resources/Objects/Container/ResourceObject.h"

namespace Logic
{

	///<summary>This class contains all the data in a scene</summary>
	class LogicScene
	{

		public:
			LogicScene();
			///<summary>Clears the scene</summary>
			void ClearScene();
			///<summary>Adds an object to the scene</summary>
			///<param val='mesh'>[in]A pointer to the object</param>
			void AddObject(Resources::ResourceObject* object);

			///<summary>Gets the objects in the scene</summary>
			///<returns>A pointer to the list of objects in the scene</returns>
			CrossHandlers::CelestialList<Resources::ResourceObject*>* GetObjects();

			~LogicScene();
		private:
			///<summary>The objects in the scene</summary>
			CrossHandlers::CelestialList<Resources::ResourceObject*>* objects;
	};
}
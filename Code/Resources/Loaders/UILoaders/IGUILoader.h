#pragma once 

#include "../FileLoaders/Interfaces/IFileGUILoader.h"

namespace Resources
{

	class IGUILoader : public IFileGUILoader
	{

		public:
			///<summary>Creates a new object</summary>
			///<param val='hor'>[in]How the object should snap horizontally</param>
			///<param val='ver'>[in]How the object should stap vertically</param>
			///<returns>A pointer to the newly created object</returns>
			virtual GUIObject* Create(GUISnap hor, GUISnap ver, std::string file) = 0;
			virtual ~IGUILoader(){}

	};
}
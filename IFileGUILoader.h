#pragma once

#include "IFileLoader.h"
#include "GUIObject.h"

namespace Resources
{

	///<summary>This interfaces virtualizes the loading of UIObjects from files</summary>
	class IFileGUILoader : public IFileLoader
	{

		public:
			///<summary>Loads a GUIObject</summary>
			///<param name='text'>[in]The name of the file containing the object</param>
			///<returns>A GUIobject with the loaded data</returns>
			virtual GUIObject* Load(std::string text) = 0;
			virtual ~IFileGUILoader(){}

	};
}
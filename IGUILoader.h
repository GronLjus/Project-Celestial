#pragma once 

#include "IFileGUILoader.h"
#include "CelestialMath.h"

namespace Resources
{

	class IGUILoader : public IFileGUILoader
	{

		public:
			///<summary>Creates a new object</summary>
			///<param val='hor'>[in]How the object should snap horizontally</param>
			///<param val='ver'>[in]How the object should stap vertically</param>
			///<returns>A pointer to the newly created object</returns>
			virtual GUIObject* Create(GUISnap hor, GUISnap ver) = 0;
			virtual ~IGUILoader(){}

	};
}
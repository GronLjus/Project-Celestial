#pragma once
#include "BaseObject.h"

namespace Resources
{

	class ScriptableObject : public BaseObject
	{

		public:
			ScriptableObject();
			virtual void Update(CrossHandlers::Message* mess);
			unsigned int GetRightClickScript() const;
			unsigned int GetLeftClickScript() const;
			unsigned int GetMiddleClickScript() const;
			virtual ~ScriptableObject(){}

		private:
			unsigned int rightClickScript;
			unsigned int leftClickScript;
			unsigned int middleClickScript;

	};
}

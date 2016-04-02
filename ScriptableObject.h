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

			unsigned int GetRightDragScript() const;
			unsigned int GetLeftDragScript() const;
			unsigned int GetMiddleDragScript() const;

			virtual ~ScriptableObject(){}

		private:
			unsigned int rightClickScript;
			unsigned int leftClickScript;
			unsigned int middleClickScript;

			unsigned int rightDragScript;
			unsigned int leftDragScript;
			unsigned int middleDragScript;

	};
}

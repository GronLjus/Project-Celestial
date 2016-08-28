#pragma once
#include "SerializableObject.h"
#include "BaseObject.h"
#include "Dictionary.h"

namespace Resources
{

	class ScriptableObject : public SerializableObject, public BaseObject
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

			unsigned int GetWheelScript() const;

			unsigned int GetUpdownScript() const;

			unsigned int GetTravelNodeScript() const;

			bool TranslateScripts(CrossHandlers::Dictionary* dict);

			//Serializable Interface
			virtual char* Serialize(unsigned int &size);
			virtual char* Unserialize(char* data);

			virtual ~ScriptableObject(){}

		private:

			unsigned int serial;
			unsigned int rightClickScript;
			unsigned int leftClickScript;
			unsigned int middleClickScript;

			unsigned int rightDragScript;
			unsigned int leftDragScript;
			unsigned int middleDragScript;

			unsigned int wheelScript;

			unsigned int upDownScript;

			unsigned int travelArrivedNodeScript;

	};
}

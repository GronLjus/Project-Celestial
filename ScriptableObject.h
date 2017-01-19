#pragma once
#include "SerializableObject.h"
#include "BaseObject.h"
#include "Dictionary.h"
#include "KeyCodes.h"

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
			unsigned int GetSplitNodeScript() const;

			unsigned int GetKeyScript(Input::CelestialKeyCategories cat, unsigned char code) const;

			bool TranslateScripts(CrossHandlers::Dictionary* dict);

			//Serializable Interface
			virtual char* Serialize(unsigned int &size);
			virtual char* Unserialize(char* data);

			virtual ~ScriptableObject();

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
			unsigned int splitNodeScript;

			struct trigger
			{

				Input::CelestialKeyCategories cat;
				unsigned char code;
				unsigned int script;

			};

			void addTrigger(trigger trigg);

			trigger* triggers;
			unsigned int currentTriggers;
			unsigned int maxTriggers;
	};
}

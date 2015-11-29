#pragma once
#include "BaseObject.h"

namespace Resources
{

	enum TriggerType{ TriggerType_KEYTRIGGER, TriggerType_NA };

	class KeyTrigger : public BaseObject
	{
		public:
			unsigned int scriptToRun;
			unsigned int keyCode;
			bool charTrigg;

	};
}
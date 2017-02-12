#pragma once
#include "BaseObject.h"

namespace Resources
{

	enum TriggerType{ TriggerType_KEYTRIGGER, TriggerType_NA };

	class KeyTrigger : public BaseObject
	{
	public:
		virtual unsigned char* Update(CrossHandlers::Message* mess) { return nullptr; }
			unsigned int scriptToRun;
			unsigned int keyCode;
			bool charTrigg;

	};
}
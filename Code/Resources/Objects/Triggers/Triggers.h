#pragma once
#include "../BaseObject.h"

namespace Resources
{

	enum TriggerType{ TriggerType_KEYTRIGGER, TriggerType_NA };

	class KeyTrigger : public BaseObject
	{
	public:
		virtual void Update(CrossHandlers::Message* mess){}
			unsigned int scriptToRun;
			unsigned int keyCode;
			bool charTrigg;

	};
}
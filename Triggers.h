#pragma once

namespace Resources
{

	enum TriggerType{ TriggerType_KEYTRIGGER, TriggerType_NA };

	struct KeyTrigger
	{
		unsigned int scriptToRun;
		unsigned int keyCode;
		bool charTrigg;

	};
}
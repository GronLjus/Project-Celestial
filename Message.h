#pragma once
#include <string>

namespace CrossHandlers
{

	enum MessageSource{ MessageSource_RESOURCES, MessageSource_GUIENTITIES, MessageSource_CELSCRIPT, MessageSource_GRAPHICS, MessageSource_INPUT, MessageSource_MASTER, MessageSource_NA };
	enum MessageType{ MessageType_INPUT, MessageType_SCRIPT, MessageType_RESOURCES, MessageType_GUIENTITIES, MessageType_GRAPHICS, MessageType_EVENT, MessageType_SYSTEM, MessageType_NA };

	enum InputMess{ InputMess_CHARDOWN, InputMess_NONCHARDOWN, InputMess_MOUSEMOVE, InputMess_TOGGLESCRIPTTRIGGER_KEYPRESS, InputMess_TOGGLESCRIPTTRIGGER_MOUSEMOVE, InputMess_NA };
	enum ScriptMess{ ScriptMess_RUN, ScriptMess_RUNFROM, ScriptMess_ADDPARNUM, ScriptMess_ADDPARASTR, ScriptMess_RESUME, ScriptMess_ADD, ScriptMess_CATCHUP, ScriptMess_NA };
	enum ResourceMess{
		ResourceMess_ADDOBJ, ResourceMess_LOADMESH, ResourceMess_LOADLIGHT, ResourceMess_LOADCHARKEYTRIGGER, ResourceMess_LOADKEYTRIGGER, ResourceMess_LOADSCRIPT,
		ResourceMess_LOADGUI, ResourceMess_ADDGUITRIGGER, ResourceMess_SIZEGUI, ResourceMess_RESNAPGUI, ResourceMess_POSGUI, ResourceMess_ADDGUITOGUI, ResourceMess_NA
	};
	enum EventMess{ EventMess_OBJECTADDED, EventMess_SCRIPTADDED, EventMess_GUIOBJECTADDED, EventMess_WAITFORFRAME, EventMess_NA };
	enum GUIMess{ GUIMess_POST, GUIMess_APPEND, GUIMess_CLEAR, GUIMess_ERASE, GUIMess_NA };
	enum GraphicMess{ GraphicMess_GETSCREEN, GraphicMess_NA};

	enum SystemMess{SystemMess_GETSCREENRES,SystemMesss_NA};

	struct Message
	{

		public:
			Message(){}
			MessageSource source;
			MessageSource destination;
			MessageType type;
			bool read;
			unsigned int senderId;
			unsigned int returnParam;
			unsigned int timeSent;
			unsigned int mess;
			unsigned int param1;
			unsigned int param2;
			unsigned int param3;
			std::string stringParam;

	};
}
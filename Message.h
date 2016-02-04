#pragma once
#include <string>

namespace CrossHandlers
{

	enum MessageSource{ MessageSource_RESOURCES, MessageSource_ENTITIES, MessageSource_GUIENTITIES, MessageSource_CELSCRIPT, MessageSource_GRAPHICS, MessageSource_INPUT, MessageSource_MASTER, MessageSource_OBJECT, MessageSource_NA };
	enum MessageType{ MessageType_INPUT, MessageType_SCRIPT, MessageType_RESOURCES, MessageType_ENTITIES, MessageType_GUIENTITIES, MessageType_GRAPHICS, MessageType_EVENT, MessageType_SYSTEM, MessageType_OBJECT, MessageType_NA };

	enum InputMess{ InputMess_CHARDOWN, InputMess_NONCHARDOWN, InputMess_MOUSEMOVE, InputMess_TOGGLESCRIPTTRIGGER_KEYPRESS, InputMess_TOGGLESCRIPTTRIGGER_MOUSEMOVE, InputMess_NA };
	enum ScriptMess{ ScriptMess_RUN, ScriptMess_RUNFROM, ScriptMess_ADDPARNUM, ScriptMess_ADDPARASTR, ScriptMess_RESUME, ScriptMess_CATCHUP, ScriptMess_NA };
	enum ResourceMess{
		ResourceMess_LOADCAMERA, ResourceMess_LOADOBJECT,
		ResourceMess_LOADGAMEBOARD, ResourceMess_LOADMESH, ResourceMess_LOADLIGHT, ResourceMess_LOADCHARKEYTRIGGER, ResourceMess_LOADKEYTRIGGER, ResourceMess_LOADSCRIPT,
		ResourceMess_LOADGUI, ResourceMess_ADDGUITRIGGER, ResourceMess_SIZEGUI, ResourceMess_RESNAPGUI, ResourceMess_POSGUI, ResourceMess_ADDGUITOGUI, ResourceMess_NA
	};
	enum EventMess{ EventMess_OBJECTADDED, EventMess_SCRIPTADDED, EventMess_GUIOBJECTADDED, EventMess_WAITFORFRAME, EventMess_NA };
	enum GUIMess{ GUIMess_POST, GUIMess_APPEND, GUIMess_CLEAR, GUIMess_ERASE, GUIMess_NA };
	enum GraphicMess{ GraphicMess_GETSCREEN, GraphicMess_SETGAMEBOARD, GraphicMess_SETCAMERA, GraphicMess_UPDATEGAMEBOARDBUFFERS, GraphicMess_NA };

	enum GameBoardMess{ GameBoardMess_ADDMESH, GameBoardMess_SETCAM, GameBoardMess_ADDOBJECT, GameBoardMess_SETGAMEBOARD, GameBoardMess_NA };
	enum SystemMess{SystemMess_GETSCREENRES,SystemMesss_NA};

	enum ObjectMess{ ObjectMess_MOVE, ObjectMess_NA};

	struct Message
	{

		public:
			Message() :params(nullptr)
			{ 
				numParams = 128; 
				params = new char[numParams]; 
				for (unsigned char i = 0; i < numParams; i++)
				{ 
					params[i] = 0; 
				} 
			}
			Message(const Message& mess) : Message(){ 
				source = mess.source; 
				destination = mess.destination; 
				type = mess.type; 
				read = mess.read;
				senderId = mess.senderId;
				returnParam = mess.returnParam; 
				timeSent = mess.timeSent; 
				this->mess = mess.mess;
				SetParams(mess.params, 0, mess.numParams-1); 
			}
			MessageSource source;
			MessageSource destination;
			MessageType type;
			bool read;
			unsigned int senderId;
			unsigned int returnParam;
			unsigned int timeSent;
			unsigned int mess;
			char* params = nullptr;
			unsigned int numParams;
			void SetParams(const char* newParams, unsigned int offset, unsigned int length)
			{
				if (offset + length >= numParams)
				{

					numParams += 128;
					char* newPar = new char[numParams]; 
					for (unsigned int i = 0; i < 128; i++)
					{ 
						newPar[numParams - 1 - i] = 0; 
					}

					memcpy(newPar, params, numParams - 128);
					delete[] params;
					params = newPar;

				}

				memcpy(&params[offset], newParams, length);

			}
			~Message(){ 
				delete[] params;
				params = nullptr;
			}

	};
}
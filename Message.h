#pragma once
#include <string>

namespace CrossHandlers
{

	enum MessageSource{ MessageSource_RESOURCES, MessageSource_ENTITIES, MessageSource_GUIENTITIES, MessageSource_CELSCRIPT, MessageSource_GRAPHICS, MessageSource_INPUT, MessageSource_MASTER, MessageSource_OBJECT, MessageSource_NA };
	enum MessageType{ MessageType_INPUT, MessageType_SCRIPT, MessageType_RESOURCES, MessageType_ENTITIES, MessageType_GUIENTITIES, MessageType_GRAPHICS, MessageType_EVENT, MessageType_SYSTEM, MessageType_OBJECT, MessageType_NA };

	enum InputMess{ InputMess_MOUSEMOVE,InputMess_MOUSEDWN, InputMess_MOUSEUP, InputMess_MOUSEWHEEL, InputMess_KEYDWN, InputMess_KEYUP, InputMess_CHAR, InputMess_NA };
	enum ScriptMess{ ScriptMess_RUN, ScriptMess_RUNFROM, ScriptMess_ADDPARNUM, ScriptMess_ADDPARASTR, ScriptMess_ADDPARFLOAT, ScriptMess_RESUME, ScriptMess_CATCHUP, ScriptMess_NA };
	enum ResourceMess{
		ResourceMess_LOADCAMERA, ResourceMess_LOADOBJECT,
		ResourceMess_LOADGAMEBOARD, ResourceMess_LOADMESH, ResourceMess_LOADLIGHT, ResourceMess_LOADCHARKEYTRIGGER, ResourceMess_LOADKEYTRIGGER, ResourceMess_LOADSCRIPT,
		ResourceMess_LOADGUI, ResourceMess_UNLOADOBJECT, ResourceMess_NA
	};
	enum EventMess{ EventMess_OBJECTADDED, EventMess_SCRIPTADDED, EventMess_GUIOBJECTADDED, EventMess_LNKDBG, EventMess_WAITFORFRAME, EventMess_NA };
	enum GUIMess{
		GUIMess_ADDSCREENTARGET, 
		GUIMess_CLICKOBJECT, GUIMess_MOVEMOUSE, GUIMess_STARTDRAGGING, GUIMess_DRAGOBJECT, GUIMess_STOPDRAGGING, GUIMess_WHEELOBJECT,
		GUIMess_POST, GUIMess_APPEND, GUIMess_CLEAR, GUIMess_ERASE, GUIMess_FOCUS, 
		GUIMess_HANDLECHAR, GUIMess_HANDLEKEY, GUIMess_NA };
	enum GraphicMess{ GraphicMess_GETSCREEN, GraphicMess_SETGAMEBOARD, GraphicMess_SETUI, GraphicMess_SETCAMERA, GraphicMess_UPDATEGAMEBOARDBUFFERS,
		GraphicMess_SETCONTENTBRUSH, GraphicMess_SETBORDERBRUSH, GraphicMess_NA
	};

	enum GameBoardMess{ GameBoardMess_ADDMESH, GameBoardMess_SETBOARDOBJECT, GameBoardMess_SETCAM, GameBoardMess_ADDOBJECT, GameBoardMess_SETGAMEBOARD, 
		GameBoardMess_CLICKOBJECT, GameBoardMess_STARTDRAGGING, GameBoardMess_DRAGOBJECT, GameBoardMess_STOPDRAGGING, GameBoardMess_WHEELDRAGGING,
		GameBoardMess_ORBITOBJECT, GameBoardMess_NA };
	enum SystemMess{SystemMess_GETSCREENRES,SystemMesss_NA};

	enum ObjectMess{
		ObjectMess_MOVE, ObjectMess_SCALE, ObjectMess_POS, ObjectMess_SIZE, ObjectMess_POINT,
		ObjectMess_ORBIT, ObjectMess_ROTATE,
		ObjectMess_HIDE, ObjectMess_SHOW,
		ObjectMess_SETHVRSCRPT, ObjectMess_SETENTRSCRPT, ObjectMess_SETEXTSCRPT, 
		ObjectMess_SETLCSCRPT, ObjectMess_SETRCSCRPT, ObjectMess_SETMCSCRPT,
		ObjectMess_SETLDSCRPT, ObjectMess_SETRDSCRPT, ObjectMess_SETMDSCRPT,
		ObjectMess_SETWHEELSCRIPT,
		ObjectMess_SETSNAP, ObjectMess_SETTEXT, ObjectMess_APPENDTEXT, ObjectMess_APPENDTEXTLINE,
		ObjectMess_HANDLECHAR, ObjectMess_HANDLEKEY,
		ObjectMess_STEPFRAME, ObjectMess_REMOVECHILD, ObjectMess_REMOVE,
		ObjectMess_LOCKMOUSE, ObjectMess_LOCKKEYS, ObjectMess_LOCKINPUT,
		ObjectMess_INCREMENTLAYER, ObjectMess_DECREMENTLAYER, ObjectMess_SETLAYER, ObjectMess_NA
	};

	struct Message
	{

		public:
			Message() :params(nullptr)
			{ 
				numParams = 128; 
				params = new unsigned char[numParams];
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
			unsigned char* params = nullptr;
			unsigned int numParams;
			void SetParams(const unsigned char* newParams, unsigned int offset, unsigned int length)
			{
				if (offset + length >= numParams)
				{

					numParams += 128;
					unsigned char* newPar = new unsigned char[numParams];
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
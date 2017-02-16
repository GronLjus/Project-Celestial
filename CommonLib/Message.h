#pragma once
#include <string>

namespace CrossHandlers
{

	enum MessageSource{ MessageSource_RESOURCES, MessageSource_ENTITIES, MessageSource_GUIENTITIES, MessageSource_CELSCRIPT, MessageSource_GRAPHICS, MessageSource_INPUT, MessageSource_SYSTEM, MessageSource_MASTER, MessageSource_OBJECT, MessageSource_TASKS, MessageSource_NA };
	enum MessageType{ MessageType_INPUT, MessageType_SCRIPT, MessageType_RESOURCES, MessageType_ENTITIES, MessageType_GUIENTITIES, MessageType_GRAPHICS, MessageType_EVENT, MessageType_SYSTEM, MessageType_OBJECT, MessageType_TASKING, MessageType_NA };

	enum InputMess{ InputMess_MOUSEMOVE,InputMess_MOUSEDWN, InputMess_MOUSEUP, InputMess_MOUSEWHEEL, InputMess_KEYDWN, InputMess_KEYUP, InputMess_CHAR, InputMess_NA };
	enum ScriptMess{ ScriptMess_RUN, ScriptMess_SETCODE, ScriptMess_ADDPARNUM, ScriptMess_ADDPARASTR, ScriptMess_ADDPARFLOAT, ScriptMess_RESUME, ScriptMess_CATCHUP, ScriptMess_NA };
	enum ResourceMess{
		ResourceMess_LOADCAMERA, ResourceMess_LOADOBJECT,
		ResourceMess_ADDTASKPAR,ResourceMess_LOADTASK,
		ResourceMess_LOADCOPYOBJECT, ResourceMess_LOADCOPYOBJECTAT,
		ResourceMess_LOADGAMEBOARD, ResourceMess_LOADMESH, ResourceMess_LOADLIGHT, ResourceMess_LOADCHARKEYTRIGGER, ResourceMess_LOADKEYTRIGGER, ResourceMess_LOADSCRIPT,
		ResourceMess_LOADGUI, ResourceMess_UNLOADOBJECT,
		ResourceMess_CLEARBOARD, 
		ResourceMess_SAVEBOARD, ResourceMess_LOADBOARD,
		ResourceMess_NA
	};
	enum EventMess{ EventMess_OBJECTADDED, EventMess_SCRIPTADDED, EventMess_GUIOBJECTADDED, 
		EventMess_LNKDBG, EventMess_WAITFORFRAME,
		EventMess_NA };
	enum GUIMess{
		GUIMess_SETUI, GUIMess_ADDCHILD,
		GUIMess_POST, GUIMess_APPEND, GUIMess_CLEAR, GUIMess_ERASE, GUIMess_FOCUS, 
		GUIMess_HANDLECHAR, GUIMess_HANDLEKEY, GUIMess_NA };
	enum GraphicMess{ GraphicMess_GETSCREEN, 
		GraphicMess_SETGAMEBOARD, GraphicMess_SETUI, GraphicMess_SETCAMERA, 
		GraphicMess_UPDATEGAMEBOARDBUFFERS,
		GraphicMess_SETCONTENTBRUSH, GraphicMess_SETBORDERBRUSH,
		GraphicMess_RESETIMAGE, GraphicMess_NA
	};

	enum GameBoardMess{ GameBoardMess_SETBOARDOBJECT, GameBoardMess_SETCAM, 
		GameBoardMess_SETGAMEBOARD,GameBoardMess_SETTRACKING, GameBoardMess_SETUI,
		GameBoardMess_ADDOBJECT, GameBoardMess_ADDMESH,  
		GameBoardMess_ADDROUTEGROUP,
		GameBoardMess_CLICKOBJECT, GameBoardMess_WHEELOBJECT, 
		GameBoardMess_STARTDRAGGING, GameBoardMess_DRAGOBJECT, GameBoardMess_STOPDRAGGING,
		GameBoardMess_MOUSEMOVE, GameBoardMess_MOUSEUD,
		GameBoardMess_HANDLEKEY,
		GameBoardMess_CLEARTRACK,
		GameBoardMess_LOCKTRACK,
		GameBoardMess_HOOKTRACK, GameBoardMess_UNHOOKTRACK,
		GameBoardMess_ORBITOBJECT, 
		GameBoardMess_POPULATEGRID, GameBoardMess_LOCKONGRID,
		GameBoardMess_GETROUTE,GameBoardMess_ADDROUTE, GameBoardMess_SPAWN, GameBoardMess_TRAVEL,
		GameBoardMess_SPLITOBJECT, GameBoardMess_GLUEOBJECT,
		GameBoardMess_GETPARENT,
		GameBoardMess_ROUTEOBJECT,
		GameBoardMess_NA };
	enum SystemMess{SystemMess_GETSCREENRES, 
		SystemMess_SETCURSOR, SystemMess_HIDECURSOR, SystemMess_SHOWCURSOR, SystemMess_MOVECURSOR,
		SystemMess_PAUSE, SystemMess_SKIP,
		SystemMesss_NA};

	enum ObjectMess{
		ObjectMess_MOVE, ObjectMess_POS, ObjectMess_PROPEL,
		ObjectMess_SCALE, ObjectMess_SIZE, ObjectMess_POINT,
		ObjectMess_COPY,
		ObjectMess_ORBIT, ObjectMess_ROTATE,
		ObjectMess_HIDE, ObjectMess_SHOW,
		ObjectMess_SETHVRSCRPT, ObjectMess_SETENTRSCRPT, ObjectMess_SETEXTSCRPT, 
		ObjectMess_SETLCSCRPT, ObjectMess_SETRCSCRPT, ObjectMess_SETMCSCRPT,
		ObjectMess_SETLDSCRPT, ObjectMess_SETRDSCRPT, ObjectMess_SETMDSCRPT,
		ObjectMess_SETWHLSCRPT, ObjectMess_SETUPDWNSCRPT, ObjectMess_ADDKEYSCRPT,
		ObjectMess_SETTRVLNDESCRPT, ObjectMess_SETSPLTSCRPT,
		ObjectMess_SETSCRPTTRGT,
		ObjectMess_SETSNAP, ObjectMess_SETTEXT, ObjectMess_APPENDTEXT, ObjectMess_APPENDTEXTLINE,
		ObjectMess_HANDLECHAR, ObjectMess_HANDLEKEY,
		ObjectMess_STEPFRAME, ObjectMess_REMOVECHILD, ObjectMess_REMOVE,
		ObjectMess_LOCKMOUSE, ObjectMess_LOCKKEYS, ObjectMess_LOCKINPUT,
		ObjectMess_INCREMENTLAYER, ObjectMess_DECREMENTLAYER, ObjectMess_SETLAYER,
		ObjectMess_SAVESCOPE,
		ObjectMess_SETCOLLFILTER,
		ObjectMess_TRANSLATE,
		ObjectMess_NA
	};

	enum TaskMess{ TaskMess_ADDTASK, TaskMess_REMOVETASK,
		TaskMess_SETTIME, TaskMess_PAUSECLOCK, TaskMess_NA};
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
				
				killWhenDone = false;

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

			Message(const Message* mess) : Message() {
				source = mess->source;
				destination = mess->destination;
				type = mess->type;
				read = mess->read;
				senderId = mess->senderId;
				returnParam = mess->returnParam;
				timeSent = mess->timeSent;
				this->mess = mess->mess;
				SetParams(mess->params, 0, mess->numParams - 1);
			}

			MessageSource source;
			MessageSource destination;
			MessageType type;
			bool read;
			bool killWhenDone;
			unsigned int senderId;
			unsigned int returnMess;
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
			void SetParam(const unsigned int par, unsigned int offset)
			{
				if (offset + 4 >= numParams)
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

				memcpy(&params[offset], &par, 4);

			}
			~Message(){ 
				delete[] params;
				params = nullptr;
			}

	};
}
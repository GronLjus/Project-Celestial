#include "stdafx.h"
#include "CelScriptRunTimeHandler.h"
#include "GUIObject.h"
#include "CelScriptCompiled.h"
#include "CrossScriptMemoryObject.h"
#include "GUILayout.h"
#include <thread>

using namespace Logic;
using namespace Resources;
using namespace CrossHandlers;

#pragma region operators
RunTimeError SetVarValOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int var1 = params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24);
	unsigned int var2 = params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24);

	rtc->memory->CopyVariable(var1 - 1, var2 - 1);
	rtc->varWaiting->Add(false, var1 - 1);
	return RunTimeError_OK;

}

unsigned char* loadString(unsigned int var, unsigned int mId, RunTimeCommons* rtc, unsigned int &length)
{

	unsigned int s = 4;
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	length = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	rtc->currentStringSize = length;

	while (length > rtc->stringLoadSize)
	{

		rtc->stringLoadSize += 100;
		delete[] rtc->stringLoader;
		rtc->stringLoader = new unsigned char[rtc->stringLoadSize];

	}

	rtc->memory->ReadVariable(var - 1, 4, length, rtc->stringLoader);
	return rtc->stringLoader;

}

RunTimeError sendMessageOut(Message& mess, RunTimeCommons* rtc)
{
	rtc->outMessageBuffer[rtc->outCurrentMessage].destination = mess.destination;
	rtc->outMessageBuffer[rtc->outCurrentMessage].mess = mess.mess;
	rtc->outMessageBuffer[rtc->outCurrentMessage].returnParam = mess.returnParam;
	rtc->outMessageBuffer[rtc->outCurrentMessage].SetParams(mess.params, 0, mess.numParams-1);
	rtc->outMessageBuffer[rtc->outCurrentMessage].timeSent = mess.timeSent;
	rtc->outMessageBuffer[rtc->outCurrentMessage].type = mess.type;
	rtc->outMessageBuffer[rtc->outCurrentMessage].read = false;
	rtc->mQueue->PushMessage(&rtc->outMessageBuffer[rtc->outCurrentMessage]);
	rtc->outCurrentMessage++;
	rtc->outCurrentMessage %= rtc->outMessages;
	return !(rtc->outMessageBuffer[rtc->outCurrentMessage].read) ? RunTimeError_MSGFULL : RunTimeError_OK;

}

RunTimeError SetConstValOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 5)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int var = params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24);
	rtc->memory->AddVariable(var - 1, &params[4], paramSize - 4);
	rtc->varWaiting->Add(false, var - 1);
	return RunTimeError_OK;

}

RunTimeError LoadGameBoardOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	unsigned char hasMore = paramSize == 8 ? 1 : 0;

	Message mess;
	mess.returnParam = returnVar;
	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_LOADGAMEBOARD;
	mess.SetParams(rtc->intLoader, 0, 4);
	mess.SetParams(&hasMore, 4, 1);
	rtc->varWaiting->Add(true, returnVar-1);

	if (paramSize == 8)
	{

		unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
		rtc->memory->ReadVariable(var2 - 1, rtc->intLoader, s);
		mess.SetParams(rtc->intLoader, 5, 4);

	}

	return sendMessageOut(mess, rtc);

}

RunTimeError UnLoadOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);

	Message mess;
	mess.returnParam = returnVar;
	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_UNLOADOBJECT;
	mess.SetParams(rtc->intLoader, 0, 4);

	return sendMessageOut(mess, rtc);

}

RunTimeError LoadCameraOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	Message mess;
	mess.returnParam = returnVar;
	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_LOADCAMERA;
	rtc->varWaiting->Add(true, returnVar - 1);
	return sendMessageOut(mess, rtc);

}

RunTimeError LoadObjectOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	unsigned int meshId = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));

	Message mess;
	mess.returnParam = returnVar;
	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_LOADOBJECT;
	mess.SetParams(rtc->intLoader, 0, 4);
	rtc->varWaiting->Add(true, returnVar - 1);

	return sendMessageOut(mess, rtc);

}

RunTimeError LoadObjectCopyOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	unsigned int meshId = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));

	Message mess;
	mess.returnParam = returnVar;
	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_LOADCOPYOBJECT;
	mess.SetParams(rtc->intLoader, 0, 4);
	rtc->varWaiting->Add(true, returnVar - 1);

	return sendMessageOut(mess, rtc);

}

RunTimeError LoadMeshOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int stringChar;
	unsigned char* dir = loadString(var, mId, rtc, stringChar);

	Message mess;
	mess.returnParam = returnVar;
	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_LOADMESH;
	mess.SetParams(dir, 0, stringChar);
	rtc->varWaiting->Add(true, returnVar-1);

	return sendMessageOut(mess, rtc);

}

RunTimeError LoadLightOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	unsigned int s = 4;

	if (paramSize < 12)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int var1 = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	unsigned int var3 = (params[8] | ((int)params[9] << 8) | ((int)params[10] << 16) | ((int)params[11] << 24));
	Message mess;
	rtc->memory->ReadVariable(var1 - 1, rtc->intLoader, s);
	mess.SetParams(rtc->intLoader, 0, 4);
	rtc->memory->ReadVariable(var2 - 1, rtc->intLoader, s);
	mess.SetParams(rtc->intLoader, 4, 4);
	rtc->memory->ReadVariable(var3 - 1, rtc->intLoader, s);
	mess.SetParams(rtc->intLoader, 8, 4);

	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_LOADLIGHT;
	mess.returnParam = returnVar;
	rtc->varWaiting->Add(true, returnVar-1);
	return sendMessageOut(mess, rtc);

}

RunTimeError LoadScriptOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int length;
	unsigned char* path = loadString(var, mId, rtc, length);
	Message mess;

	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_LOADSCRIPT;
	mess.returnParam = returnVar;
	mess.SetParams(path, 0, length);

	rtc->varWaiting->Add(true, returnVar-1);
	return sendMessageOut(mess, rtc);

}

RunTimeError LoadKeyTriggerOperatorChar(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int length;
	unsigned char* tempString = loadString((params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24)), mId, rtc, length);
	Message mess;
	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_LOADCHARKEYTRIGGER;
	mess.SetParams(params, 0, 4);
	mess.SetParams(tempString, 4, length);
	return sendMessageOut(mess, rtc);

}

RunTimeError LoadKeyTriggerOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	Message mess;
	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_LOADKEYTRIGGER;
	mess.SetParams(params, 0, 8);
	return sendMessageOut(mess, rtc);

}

RunTimeError LoadTextBoxOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	Message mess;
	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_LOADGUI;
	mess.returnParam = returnVar;
	int enu = GUIObjects_TEXTBOX;
	unsigned unsigned char tempBuff[]{enu >> 0, enu >> 8, enu >> 16, enu >>24};
	mess.SetParams(tempBuff, 0, 4);
	mess.SetParams((unsigned char*)"", 4, 1);
	rtc->varWaiting->Add(true, returnVar - 1);
	return sendMessageOut(mess, rtc);

}

RunTimeError LoadPanelOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	Message mess;
	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_LOADGUI;
	int enu = GUIObjects_LAYOUT;
	mess.returnParam = returnVar;
	unsigned char tempBuff[]{enu >> 0, enu >> 8, enu >> 16, enu >> 24};
	mess.SetParams(tempBuff, 0, 4);
	mess.SetParams((unsigned char*)"", 4, 1);
	rtc->varWaiting->Add(true, returnVar - 1);
	return sendMessageOut(mess, rtc);

}

RunTimeError LoadImageOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	Message mess;
	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_LOADGUI;
	int enu = GUIObjects_IMAGE;
	mess.returnParam = returnVar;
	unsigned char tempBuff[]{enu >> 0, enu >> 8, enu >> 16, enu >> 24};
	mess.SetParams(tempBuff, 0, 4);
	unsigned int length;
	unsigned char* tempString = loadString((params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24)), mId, rtc, length);
	mess.SetParams(tempString, 4, length);
	rtc->varWaiting->Add(true, returnVar - 1);
	return sendMessageOut(mess, rtc);

}

RunTimeError AddObjectOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var1 = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var1 - 1, rtc->intLoader, s);

	Message mess;
	mess.destination = MessageSource_ENTITIES;
	mess.type = MessageType_ENTITIES;
	mess.mess = GameBoardMess_ADDOBJECT;
	mess.SetParams(rtc->intLoader, 0, 4);
	return sendMessageOut(mess, rtc);

}

RunTimeError SetBorderOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 16)
	{

		return RunTimeError_BADPARAMS;

	}

	Message mess;
	mess.destination = MessageSource_GRAPHICS;
	mess.type = MessageType_GRAPHICS;
	mess.mess = GraphicMess_SETBORDERBRUSH;

	unsigned int s = 4;
	unsigned int obj = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(obj - 1, rtc->intLoader, s);
	mess.SetParams(rtc->intLoader, 0, 4);

	unsigned int xVar = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(xVar - 1, rtc->intLoader, s);
	mess.SetParams(rtc->intLoader, 4, 8);

	unsigned int yVar = (params[8] | ((int)params[9] << 8) | ((int)params[10] << 16) | ((int)params[11] << 24));
	rtc->memory->ReadVariable(yVar - 1, rtc->intLoader, s);
	mess.SetParams(rtc->intLoader, 8, 12);

	unsigned int zVar = (params[12] | ((int)params[13] << 8) | ((int)params[14] << 16) | ((int)params[15] << 24));
	rtc->memory->ReadVariable(zVar - 1, rtc->intLoader, s);
	mess.SetParams(rtc->intLoader, 12, 16);

	return sendMessageOut(mess, rtc);

}

RunTimeError SetContentOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 16)
	{

		return RunTimeError_BADPARAMS;

	}

	Message mess;
	mess.destination = MessageSource_GRAPHICS;
	mess.type = MessageType_GRAPHICS;
	mess.mess = GraphicMess_SETCONTENTBRUSH;

	unsigned int s = 4;
	unsigned int obj = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(obj - 1, rtc->intLoader, s);
	mess.SetParams(rtc->intLoader, 0, 4);

	unsigned int xVar = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(xVar - 1, rtc->intLoader, s);
	mess.SetParams(rtc->intLoader, 4, 8);

	unsigned int yVar = (params[8] | ((int)params[9] << 8) | ((int)params[10] << 16) | ((int)params[11] << 24));
	rtc->memory->ReadVariable(yVar - 1, rtc->intLoader, s);
	mess.SetParams(rtc->intLoader, 8, 12);

	unsigned int zVar = (params[12] | ((int)params[13] << 8) | ((int)params[14] << 16) | ((int)params[15] << 24));
	rtc->memory->ReadVariable(zVar - 1, rtc->intLoader, s);
	mess.SetParams(rtc->intLoader, 12, 16);

	return sendMessageOut(mess, rtc);

}

RunTimeError AddMeshOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var1 = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var1 - 1, rtc->intLoader, s);

	Message mess;
	mess.destination = MessageSource_ENTITIES;
	mess.type = MessageType_ENTITIES;
	mess.mess = GameBoardMess_ADDMESH;
	mess.SetParams(rtc->intLoader, 0, 4);
	return sendMessageOut(mess, rtc);

}

RunTimeError GetScreenWidthOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	Message mess;
	mess.returnParam = returnVar;
	mess.destination = MessageSource_GRAPHICS;
	mess.type = MessageType_GRAPHICS;
	mess.mess = GraphicMess_GETSCREEN;
	unsigned char tempBuff[]{0 >> 0, 0 >> 8, 0 >> 16, 0 >> 24};
	mess.SetParams(tempBuff, 0, 4);
	rtc->varWaiting->Add(true, returnVar-1);
	return sendMessageOut(mess, rtc);

}

RunTimeError GetScreenHeightOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	Message mess;
	mess.returnParam = returnVar;
	mess.destination = MessageSource_GRAPHICS;
	mess.type = MessageType_GRAPHICS;
	mess.mess = GraphicMess_GETSCREEN;
	unsigned char tempBuff[]{0 >> 0, 0 >> 8, 0 >> 16, 0 >> 24};
	mess.SetParams(tempBuff, 0, 4);
	rtc->varWaiting->Add(true, returnVar-1);
	return sendMessageOut(mess, rtc);

}

RunTimeError ReSnapOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 12)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	unsigned int objectToMod = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	BaseObject* object = rtc->gameObjects->GetValue(objectToMod);

	unsigned int hV = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	unsigned int snapEnum = (params[8] | ((int)params[9] << 8) | ((int)params[10] << 16) | ((int)params[11] << 24));
	
	rtc->memory->ReadVariable(hV - 1, rtc->intLoader, s);
	hV = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	
	rtc->memory->ReadVariable(snapEnum - 1, rtc->intLoader, s);
	snapEnum = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));


	unsigned char tempBuff[]{(unsigned char)hV, (unsigned char)snapEnum};
	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_SETSNAP;
	mess.SetParams(tempBuff, 0, 2);
	object->Update(&mess);

	return RunTimeError_OK;

}

RunTimeError PosOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 12)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	unsigned int objectToMod = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	BaseObject* object = rtc->gameObjects->GetValue(objectToMod);

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_POS;

	unsigned int xVar = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(xVar - 1, rtc->intLoader, s);
	mess.SetParams(rtc->intLoader, 0, 4);

	unsigned int yVar = (params[8] | ((int)params[9] << 8) | ((int)params[10] << 16) | ((int)params[11] << 24));
	rtc->memory->ReadVariable(yVar - 1, rtc->intLoader, s);
	mess.SetParams(rtc->intLoader, 4, 4);

	if (paramSize == 16)
	{

		unsigned int zVar = (params[12] | ((int)params[13] << 8) | ((int)params[14] << 16) | ((int)params[15] << 24));
		rtc->memory->ReadVariable(zVar - 1, rtc->intLoader, s);
		mess.SetParams(rtc->intLoader, 8, 4);

	}

	object->Update(&mess);

	return RunTimeError_OK;

}

RunTimeError GlueObjectOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 20)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	unsigned int objectToMod = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	PositionableObject* parent = (PositionableObject*)rtc->gameObjects->GetValue(objectToMod);

	var = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	objectToMod = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));

	PositionableObject* child = (PositionableObject*)rtc->gameObjects->GetValue(objectToMod);
	CelestialMath::Vector3 pos;

	unsigned int xVar = (params[8] | ((int)params[9] << 8) | ((int)params[10] << 16) | ((int)params[11] << 24));
	rtc->memory->ReadVariable(xVar - 1, rtc->intLoader, s);
	memcpy(&pos.x, rtc->intLoader, 4);

	unsigned int yVar = (params[12] | ((int)params[13] << 8) | ((int)params[14] << 16) | ((int)params[15] << 24));
	rtc->memory->ReadVariable(yVar - 1, rtc->intLoader, s);
	memcpy(&pos.y, rtc->intLoader, 4);

	unsigned int zVar = (params[16] | ((int)params[17] << 8) | ((int)params[18] << 16) | ((int)params[19] << 24));
	rtc->memory->ReadVariable(zVar - 1, rtc->intLoader, s);
	memcpy(&pos.z, rtc->intLoader, 4);

	parent->AddSubObject(child, pos);
	return RunTimeError_OK;

}

RunTimeError MoveOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 12)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	unsigned int objectToMod = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	BaseObject* object = rtc->gameObjects->GetValue(objectToMod);

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_MOVE;

	unsigned int xVar = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(xVar - 1, rtc->intLoader, s);
	mess.SetParams(rtc->intLoader, 0, 4);

	unsigned int yVar = (params[8] | ((int)params[9] << 8) | ((int)params[10] << 16) | ((int)params[11] << 24));
	rtc->memory->ReadVariable(yVar - 1, rtc->intLoader, s);
	mess.SetParams(rtc->intLoader, 4, 4);

	if (paramSize == 16)
	{

		unsigned int zVar = (params[12] | ((int)params[13] << 8) | ((int)params[14] << 16) | ((int)params[15] << 24));
		rtc->memory->ReadVariable(zVar - 1, rtc->intLoader, s);
		mess.SetParams(rtc->intLoader, 8, 4);

	}

	object->Update(&mess);

	return RunTimeError_OK;

}

RunTimeError RotateOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 16)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	unsigned int objectToMod = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	BaseObject* object = rtc->gameObjects->GetValue(objectToMod);

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_ROTATE;

	unsigned int xVar = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(xVar - 1, rtc->intLoader, s);
	mess.SetParams(rtc->intLoader, 0, 4);

	unsigned int yVar = (params[8] | ((int)params[9] << 8) | ((int)params[10] << 16) | ((int)params[11] << 24));
	rtc->memory->ReadVariable(yVar - 1, rtc->intLoader, s);
	mess.SetParams(rtc->intLoader, 4, 4);

	unsigned int zVar = (params[12] | ((int)params[13] << 8) | ((int)params[14] << 16) | ((int)params[15] << 24));
	rtc->memory->ReadVariable(zVar - 1, rtc->intLoader, s);
	mess.SetParams(rtc->intLoader, 8, 4);

	object->Update(&mess);

	return RunTimeError_OK;

}

RunTimeError OrbitOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	Message mess;
	mess.destination = MessageSource_ENTITIES;
	mess.type = MessageType_ENTITIES;
	mess.mess = GameBoardMess_ORBITOBJECT;

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	mess.SetParams(rtc->intLoader, 0, 4);

	unsigned int fVar = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(fVar - 1, rtc->intLoader, s);
	mess.SetParams(rtc->intLoader, 4, 4);

	return sendMessageOut(mess, rtc);

}

RunTimeError PropelOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}
	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	unsigned int objectToMod = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	BaseObject* object = rtc->gameObjects->GetValue(objectToMod);

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_PROPEL;

	unsigned int fVar = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(fVar - 1, rtc->intLoader, s);
	mess.SetParams(rtc->intLoader, 0, 4);
	object->Update(&mess);

	return RunTimeError_OK;

}

RunTimeError IgnoreMouseOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	unsigned int objectToMod = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	BaseObject* object = rtc->gameObjects->GetValue(objectToMod);

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_LOCKMOUSE;
	mess.params[0] = 0;
	object->Update(&mess);

	return RunTimeError_OK;

}

RunTimeError IgnoreKeyboardOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	unsigned int objectToMod = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	BaseObject* object = rtc->gameObjects->GetValue(objectToMod);

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_LOCKKEYS;
	mess.params[0] = 0;
	object->Update(&mess);

	return RunTimeError_OK;

}

RunTimeError IgnoreInputOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	unsigned int objectToMod = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	BaseObject* object = rtc->gameObjects->GetValue(objectToMod);

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_LOCKINPUT;
	mess.params[0] = 0;
	object->Update(&mess);

	return RunTimeError_OK;

}

RunTimeError AcceptMouseOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	unsigned int objectToMod = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	BaseObject* object = rtc->gameObjects->GetValue(objectToMod);

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_LOCKMOUSE;
	mess.params[0] = 1;
	object->Update(&mess);

	return RunTimeError_OK;

}

RunTimeError AcceptKeyboardOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	unsigned int objectToMod = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	BaseObject* object = rtc->gameObjects->GetValue(objectToMod);

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_LOCKKEYS;
	mess.params[0] = 1;
	object->Update(&mess);

	return RunTimeError_OK;

}

RunTimeError AcceptInputOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	unsigned int objectToMod = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	BaseObject* object = rtc->gameObjects->GetValue(objectToMod);

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_LOCKINPUT;
	mess.params[0] = 1;
	object->Update(&mess);

	return RunTimeError_OK;

}

RunTimeError IncrementLayerOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	unsigned int objectToMod = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	BaseObject* object = rtc->gameObjects->GetValue(objectToMod);

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_INCREMENTLAYER;
	object->Update(&mess);

	return RunTimeError_OK;

}

RunTimeError DecrementLayerOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	unsigned int objectToMod = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	BaseObject* object = rtc->gameObjects->GetValue(objectToMod);

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_DECREMENTLAYER;
	object->Update(&mess);

	return RunTimeError_OK;

}

RunTimeError SetLayerOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	unsigned int objectToMod = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	BaseObject* object = rtc->gameObjects->GetValue(objectToMod);

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_LOCKINPUT;

	unsigned int layer = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(layer - 1, rtc->intLoader, s);
	mess.SetParams(rtc->intLoader, 0, 4);
	object->Update(&mess);
	return RunTimeError_OK;

}

RunTimeError SizeOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 12)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	unsigned int objectToMod = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	BaseObject* object = rtc->gameObjects->GetValue(objectToMod);

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_SIZE;

	unsigned int xVar = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(xVar - 1, rtc->intLoader, s);
	mess.SetParams(rtc->intLoader, 0, 4);

	unsigned int yVar = (params[8] | ((int)params[9] << 8) | ((int)params[10] << 16) | ((int)params[11] << 24));
	rtc->memory->ReadVariable(yVar - 1, rtc->intLoader, s);
	mess.SetParams(rtc->intLoader, 4, 4);

	if (paramSize == 16)
	{

		unsigned int zVar = (params[12] | ((int)params[13] << 8) | ((int)params[14] << 16) | ((int)params[15] << 24));
		rtc->memory->ReadVariable(zVar - 1, rtc->intLoader, s);
		mess.SetParams(rtc->intLoader, 8, 4);

	}

	object->Update(&mess);

	return RunTimeError_OK;

}

RunTimeError ScaleOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 12)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	unsigned int objectToMod = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	BaseObject* object = rtc->gameObjects->GetValue(objectToMod);

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_SCALE;

	unsigned int xVar = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(xVar - 1, rtc->intLoader, s);
	mess.SetParams(rtc->intLoader, 0, 4);

	unsigned int yVar = (params[8] | ((int)params[9] << 8) | ((int)params[10] << 16) | ((int)params[11] << 24));
	rtc->memory->ReadVariable(yVar - 1, rtc->intLoader, s);
	mess.SetParams(rtc->intLoader, 4, 4);

	if (paramSize == 16)
	{

		unsigned int zVar = (params[12] | ((int)params[13] << 8) | ((int)params[14] << 16) | ((int)params[15] << 24));
		rtc->memory->ReadVariable(zVar - 1, rtc->intLoader, s);
		mess.SetParams(rtc->intLoader, 8, 4);

	}

	object->Update(&mess);

	return RunTimeError_OK;

}

RunTimeError Add2DOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}
	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	unsigned int objectToMod = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	GUIObject* object = (GUIObject*)rtc->gameObjects->GetValue(objectToMod);

	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(var2 - 1, rtc->intLoader, s);
	objectToMod = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	GUIObject* child = (GUIObject*)rtc->gameObjects->GetValue(objectToMod);

	if (object->GetType() == GUIObjects_LAYOUT)
	{

		((GUILayout*)object)->AddChild(child);
		
	}

	return RunTimeError_OK;

}

RunTimeError RemoveOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	unsigned int objectToMod = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	BaseObject* object = rtc->gameObjects->GetValue(objectToMod);


	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_REMOVE;
	object->Update(&mess);
	return RunTimeError_OK;

}

RunTimeError SetTextOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	unsigned int objectToMod = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	BaseObject* object = rtc->gameObjects->GetValue(objectToMod);
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	unsigned int length;
	unsigned char* message = loadString(var2, mId, rtc, length);

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_SETTEXT;
	mess.SetParams(message, 0, length);
	object->Update(&mess);
	return RunTimeError_OK;

}

RunTimeError AppendTextOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	unsigned int objectToMod = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	BaseObject* object = rtc->gameObjects->GetValue(objectToMod);
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	unsigned int length;
	unsigned char* message = loadString(var2, mId, rtc, length);

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_APPENDTEXT;
	mess.SetParams(message, 0, length);
	object->Update(&mess);
	return RunTimeError_OK;

}

RunTimeError AddTextLineOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	unsigned int objectToMod = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	BaseObject* object = rtc->gameObjects->GetValue(objectToMod);
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	unsigned int length;
	unsigned char* message = loadString(var2, mId, rtc, length);

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_APPENDTEXTLINE;
	mess.SetParams(message, 0, length);
	object->Update(&mess);
	return RunTimeError_OK;

}

RunTimeError HideOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	unsigned int objectToMod = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	BaseObject* object = rtc->gameObjects->GetValue(objectToMod);

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_HIDE;
	object->Update(&mess);
	return RunTimeError_OK;

}

RunTimeError HideCursorOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	Message mess;
	mess.destination = MessageSource_SYSTEM;
	mess.type = MessageType_SYSTEM;
	mess.mess = SystemMess_HIDECURSOR;
	return sendMessageOut(mess, rtc);

}

RunTimeError ShowCursorOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	Message mess;
	mess.destination = MessageSource_SYSTEM;
	mess.type = MessageType_SYSTEM;
	mess.mess = SystemMess_SHOWCURSOR;
	return sendMessageOut(mess, rtc);

}

RunTimeError ShowOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	unsigned int objectToMod = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	BaseObject* object = rtc->gameObjects->GetValue(objectToMod);

	Message mess;
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_SHOW;
	object->Update(&mess);
	return RunTimeError_OK;

}

RunTimeError PostStrOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int length;
	unsigned char* message = loadString(var, mId, rtc, length);

	Message mess;
	mess.destination = MessageSource_GUIENTITIES;
	mess.type = MessageType_GUIENTITIES;
	mess.mess = GUIMess_POST;
	mess.SetParams(message, 0, length);
	return sendMessageOut(mess, rtc);

}

RunTimeError PostNumOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	int val = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	Message mess;
	mess.destination = MessageSource_GUIENTITIES;
	mess.type = MessageType_GUIENTITIES;
	mess.mess = GUIMess_POST;
	std::string stringParam = std::to_string(val);
	///TODO: Remove magic number
	mess.SetParams((unsigned char*)stringParam.c_str(), 0, stringParam.length());
	return sendMessageOut(mess, rtc);

}

RunTimeError PostFloatOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	float val = 0.0f;
	memcpy(&val, rtc->intLoader, 4);

	Message mess;
	mess.destination = MessageSource_GUIENTITIES;
	mess.type = MessageType_GUIENTITIES;
	mess.mess = GUIMess_POST;
	std::string stringParam = std::to_string(val);
	mess.SetParams((unsigned char*)stringParam.c_str(), 0, stringParam.length());
	return sendMessageOut(mess, rtc);

}

RunTimeError CastFloatOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	int val = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	float valF = val;
	unsigned char bytes[sizeof(float)];
	memcpy(bytes, &valF, sizeof(float));
	rtc->memory->AddVariable(returnVar - 1, bytes, s);
	rtc->varWaiting->Add(false, returnVar - 1);
	return RunTimeError_OK;

}

RunTimeError NegateConstOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	return RunTimeError_OK;

}

RunTimeError NegateVarOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	int val = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	val *= -1;

	rtc->intLoader[0] = val >> 0;
	rtc->intLoader[1] = val >> 8;
	rtc->intLoader[2] = val >> 16;
	rtc->intLoader[3] = val >> 24;
	rtc->memory->AddVariable(returnVar - 1, rtc->intLoader, s);
	rtc->varWaiting->Add(false, returnVar - 1);
	return RunTimeError_OK;

}

RunTimeError NegateFloatOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	float val = 0.0f;
	memcpy(&val, rtc->intLoader, 4);
	val *= -1;

	unsigned char bytes[sizeof(float)];
	memcpy(bytes, &val, sizeof(float));

	rtc->memory->AddVariable(returnVar - 1, bytes, s);
	rtc->varWaiting->Add(false, returnVar - 1);
	return RunTimeError_OK;

}

RunTimeError SumConst2Operator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	return RunTimeError_OK;

}

RunTimeError SumVarOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	int val = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	rtc->memory->ReadVariable(var2 - 1, rtc->intLoader, s);
	val += (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));

	rtc->intLoader[0] = val >> 0;
	rtc->intLoader[1] = val >> 8;
	rtc->intLoader[2] = val >> 16;
	rtc->intLoader[3] = val >> 24;
	rtc->memory->AddVariable(returnVar - 1, rtc->intLoader, s);
	rtc->varWaiting->Add(false, returnVar - 1);
	return RunTimeError_OK;

}

RunTimeError ParentOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	unsigned int val = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	unsigned int parent = rtc->gameObjects->GetValue(val)->GetParentId();

	rtc->intLoader[0] = parent >> 0;
	rtc->intLoader[1] = parent >> 8;
	rtc->intLoader[2] = parent >> 16;
	rtc->intLoader[3] = parent >> 24;
	rtc->memory->AddVariable(returnVar - 1, rtc->intLoader, s);
	rtc->varWaiting->Add(false, returnVar - 1);
	return RunTimeError_OK;

}

RunTimeError SumFloatOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	float val = 0.0f;
	memcpy(&val, rtc->intLoader, 4);
	rtc->memory->ReadVariable(var2 - 1, rtc->intLoader, s);
	float val2 = 0.0f;
	memcpy(&val2, rtc->intLoader, 4);
	val += val2;

	unsigned char bytes[sizeof(float)];
	memcpy(bytes, &val, sizeof(float));

	rtc->memory->AddVariable(returnVar - 1, bytes, s);
	rtc->varWaiting->Add(false, returnVar - 1);
	return RunTimeError_OK;

}

RunTimeError SubConst2Operator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	return RunTimeError_OK;

}

RunTimeError SubVarOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	int val = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	rtc->memory->ReadVariable(var2 - 1, rtc->intLoader, s);
	val -= (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));

	rtc->intLoader[0] = val >> 0;
	rtc->intLoader[1] = val >> 8;
	rtc->intLoader[2] = val >> 16;
	rtc->intLoader[3] = val >> 24;
	rtc->memory->AddVariable(returnVar - 1, rtc->intLoader, s);
	rtc->varWaiting->Add(false, returnVar - 1);
	return RunTimeError_OK;

}

RunTimeError SubFloatOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}
	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	float val = 0.0f;
	memcpy(&val, rtc->intLoader, 4);
	rtc->memory->ReadVariable(var2 - 1, rtc->intLoader, s);
	float val2 = 0.0f;
	memcpy(&val2, rtc->intLoader, 4);
	val -= val2;

	unsigned char bytes[sizeof(float)];
	memcpy(bytes, &val, sizeof(float));

	rtc->memory->AddVariable(returnVar - 1, bytes, s);
	rtc->varWaiting->Add(false, returnVar - 1);
	return RunTimeError_OK;

}

RunTimeError MulConst2Operator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	return RunTimeError_OK;

}

RunTimeError MulVarOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	int val = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	rtc->memory->ReadVariable(var2 - 1, rtc->intLoader, s);
	val *= (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));

	rtc->intLoader[0] = val >> 0;
	rtc->intLoader[1] = val >> 8;
	rtc->intLoader[2] = val >> 16;
	rtc->intLoader[3] = val >> 24;
	rtc->memory->AddVariable(returnVar - 1, rtc->intLoader, s);
	rtc->varWaiting->Add(false, returnVar - 1);
	return RunTimeError_OK;

}

RunTimeError MulFloatOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}
	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	float val = 0.0f;
	memcpy(&val, rtc->intLoader, 4);
	rtc->memory->ReadVariable(var2 - 1, rtc->intLoader, s);
	float val2 = 0.0f;
	memcpy(&val2, rtc->intLoader, 4);
	val *= val2;

	unsigned char bytes[sizeof(float)];
	memcpy(bytes, &val, sizeof(float));

	rtc->memory->AddVariable(returnVar - 1, bytes, s);
	rtc->varWaiting->Add(false, returnVar - 1);
	return RunTimeError_OK;

}

RunTimeError DivConst2Operator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	return RunTimeError_OK;

}

RunTimeError DivVarOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	int val = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	rtc->memory->ReadVariable(var2 - 1, rtc->intLoader, s);
	val /= (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));

	rtc->intLoader[0] = val >> 0;
	rtc->intLoader[1] = val >> 8;
	rtc->intLoader[2] = val >> 16;
	rtc->intLoader[3] = val >> 24;
	rtc->memory->AddVariable(returnVar - 1, rtc->intLoader, s);
	rtc->varWaiting->Add(false, returnVar - 1);
	return RunTimeError_OK;

}

RunTimeError DivFloatOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	float val = 0.0f;
	memcpy(&val, rtc->intLoader, 4);
	rtc->memory->ReadVariable(var2 - 1, rtc->intLoader, s);
	float val2 = 0.0f;
	memcpy(&val2, rtc->intLoader, 4);
	val /= val2;

	unsigned char bytes[sizeof(float)];
	memcpy(bytes, &val, sizeof(float));

	rtc->memory->AddVariable(returnVar - 1, bytes, s);
	rtc->varWaiting->Add(false, returnVar - 1);
	return RunTimeError_OK;

}

RunTimeError NumEqlConst2Operator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	return RunTimeError_OK;

}

RunTimeError NumEqlVarOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	int val = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	rtc->memory->ReadVariable(var2 - 1, rtc->intLoader, s);
	val = val == (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24)) ? 1 : 0;

	rtc->intLoader[0] = val >> 0;
	rtc->intLoader[1] = val >> 8;
	rtc->intLoader[2] = val >> 16;
	rtc->intLoader[3] = val >> 24;
	rtc->memory->AddVariable(returnVar - 1, rtc->intLoader, s);
	rtc->varWaiting->Add(false, returnVar - 1);
	return RunTimeError_OK;

}

RunTimeError FloatEqlVarOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}
	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	float val = 0.0f;
	memcpy(&val, rtc->intLoader, 4);
	rtc->memory->ReadVariable(var2 - 1, rtc->intLoader, s);
	float val2 = 0.0f;
	memcpy(&val2, rtc->intLoader, 4);
	unsigned int eql = val == val2 ? 1 : 0;
	rtc->intLoader[0] = eql >> 0;
	rtc->intLoader[1] = eql >> 8;
	rtc->intLoader[2] = eql >> 16;
	rtc->intLoader[3] = eql >> 24;
	rtc->memory->AddVariable(returnVar - 1, rtc->intLoader, s);
	rtc->varWaiting->Add(false, returnVar - 1);
	return RunTimeError_OK;

}

RunTimeError StrEqlConst2Operator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	return RunTimeError_OK;

}

RunTimeError StrEqlVarOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int length;
	std::string varVal1 = std::string((char*)loadString(var, mId, rtc, length));
	var = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	std::string varVal2 = std::string((char*)loadString(var, mId, rtc, length));


	int val = varVal1 == varVal2 ? 1 : 0;
	rtc->intLoader[0] = val >> 0;
	rtc->intLoader[1] = val >> 8;
	rtc->intLoader[2] = val >> 16;
	rtc->intLoader[3] = val >> 24;
	unsigned int s = 4;
	rtc->memory->AddVariable(returnVar - 1, rtc->intLoader, s);
	rtc->varWaiting->Add(false, returnVar - 1);
	return RunTimeError_OK;

}

RunTimeError NumGrtConst2Operator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	return RunTimeError_OK;

}

RunTimeError NumGrtFloaOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}
	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	float val = 0.0f;
	memcpy(&val, rtc->intLoader, 4);
	rtc->memory->ReadVariable(var2 - 1, rtc->intLoader, s);
	float val2 = 0.0f;
	memcpy(&val2, rtc->intLoader, 4);
	unsigned int eql = val > val2 ? 1 : 0;
	rtc->intLoader[0] = eql >> 0;
	rtc->intLoader[1] = eql >> 8;
	rtc->intLoader[2] = eql >> 16;
	rtc->intLoader[3] = eql >> 24;
	rtc->memory->AddVariable(returnVar - 1, rtc->intLoader, s);
	rtc->varWaiting->Add(false, returnVar - 1);
	return RunTimeError_OK;

}

RunTimeError NumGrtVarOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	int val = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	rtc->memory->ReadVariable(var2 - 1, rtc->intLoader, s);
	val = val > (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24)) ? 1 : 0;

	rtc->intLoader[0] = val >> 0;
	rtc->intLoader[1] = val >> 8;
	rtc->intLoader[2] = val >> 16;
	rtc->intLoader[3] = val >> 24;
	rtc->memory->AddVariable(returnVar - 1, rtc->intLoader, s);
	rtc->varWaiting->Add(false, returnVar - 1);
	return RunTimeError_OK;

}

RunTimeError RunScriptOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);

	rtc->status = scriptStatus_RUNOTHERSCRIPT;
	Message mess;

	mess.destination = MessageSource_CELSCRIPT;
	mess.type = MessageType_SCRIPT;
	mess.mess = ScriptMess_RUNFROM;
	mess.SetParams(rtc->intLoader, 0, 4);
	unsigned char tempBuff[]{rtc->stack >> 0, rtc->stack >> 8, rtc->stack >> 16, rtc->stack >> 24};
	mess.SetParams(tempBuff, 4, 4);
	return sendMessageOut(mess, rtc);

}

RunTimeError SetGameBoardOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);

	Message mess;
	mess.destination = MessageSource_ENTITIES;
	mess.type = MessageType_ENTITIES;
	mess.mess = GameBoardMess_SETGAMEBOARD;
	mess.SetParams(rtc->intLoader, 0, 4);
	return sendMessageOut(mess, rtc);

}

RunTimeError SetCameraOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);

	Message mess;
	mess.destination = MessageSource_ENTITIES;
	mess.type = MessageType_ENTITIES;
	mess.mess = GameBoardMess_SETCAM;
	mess.SetParams(rtc->intLoader, 0, 4);
	return sendMessageOut(mess, rtc);

}

RunTimeError SetLeftClickOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	unsigned int objectToMod = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	BaseObject* object = rtc->gameObjects->GetValue(objectToMod);

	Message mess;
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(var2 - 1, rtc->intLoader, s);
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_SETLCSCRPT;
	mess.SetParams(rtc->intLoader, 0, 4);
	object->Update(&mess);
	return RunTimeError_OK;

}

RunTimeError SetMouseCursor(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);

	Message mess;
	mess.destination = MessageSource_SYSTEM;
	mess.type = MessageType_SYSTEM;
	mess.mess = SystemMess_SETCURSOR;
	mess.SetParams(rtc->intLoader, 0, 4);
	return sendMessageOut(mess, rtc);

}

RunTimeError SetMiddleClickOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	unsigned int objectToMod = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	BaseObject* object = rtc->gameObjects->GetValue(objectToMod);

	Message mess;
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(var2 - 1, rtc->intLoader, s);
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_SETMCSCRPT;
	mess.SetParams(rtc->intLoader, 0, 4);
	object->Update(&mess);
	return RunTimeError_OK;

}

RunTimeError SetRightClickOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	unsigned int objectToMod = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	BaseObject* object = rtc->gameObjects->GetValue(objectToMod);

	Message mess;
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(var2 - 1, rtc->intLoader, s);
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_SETRCSCRPT;
	mess.SetParams(rtc->intLoader, 0, 4);
	object->Update(&mess);
	return RunTimeError_OK;

}

RunTimeError LinkTargetOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	unsigned int objectToMod = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	BaseObject* object = rtc->gameObjects->GetValue(objectToMod);

	Message mess;
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(var2 - 1, rtc->intLoader, s);
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_SETSCRPTTRGT;
	mess.SetParams(rtc->intLoader, 0, 4);
	object->Update(&mess);
	return RunTimeError_OK;

}

RunTimeError SetLeftDragOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	unsigned int objectToMod = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	BaseObject* object = rtc->gameObjects->GetValue(objectToMod);

	Message mess;
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(var2 - 1, rtc->intLoader, s);
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_SETLDSCRPT;
	mess.SetParams(rtc->intLoader, 0, 4);
	object->Update(&mess);
	return RunTimeError_OK;

}

RunTimeError SetMiddleDragOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	unsigned int objectToMod = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	BaseObject* object = rtc->gameObjects->GetValue(objectToMod);

	Message mess;
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(var2 - 1, rtc->intLoader, s);
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_SETMDSCRPT;
	mess.SetParams(rtc->intLoader, 0, 4);
	object->Update(&mess);
	return RunTimeError_OK;

}

RunTimeError SetMouseWheelOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	unsigned int objectToMod = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	BaseObject* object = rtc->gameObjects->GetValue(objectToMod);

	Message mess;
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(var2 - 1, rtc->intLoader, s);
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_SETWHLSCRPT;
	mess.SetParams(rtc->intLoader, 0, 4);
	object->Update(&mess);
	return RunTimeError_OK;

}

RunTimeError SetMouseUpDownOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	unsigned int objectToMod = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	BaseObject* object = rtc->gameObjects->GetValue(objectToMod);

	Message mess;
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(var2 - 1, rtc->intLoader, s);
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_SETUPDWNSCRPT;
	mess.SetParams(rtc->intLoader, 0, 4);
	object->Update(&mess);
	return RunTimeError_OK;

}

RunTimeError SetRightDragOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	unsigned int objectToMod = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	BaseObject* object = rtc->gameObjects->GetValue(objectToMod);

	Message mess;
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(var2 - 1, rtc->intLoader, s);
	mess.destination = MessageSource_OBJECT;
	mess.type = MessageType_OBJECT;
	mess.mess = ObjectMess_SETRDSCRPT;
	mess.SetParams(rtc->intLoader, 0, 4);
	object->Update(&mess);
	return RunTimeError_OK;

}

RunTimeError SetScriptParNumOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));

	Message mess;
	mess.destination = MessageSource_CELSCRIPT;
	mess.type = MessageType_SCRIPT;
	mess.mess = ScriptMess_ADDPARNUM;
	rtc->memory->ReadVariable(var2 - 1, rtc->intLoader, s);
	mess.SetParams(rtc->intLoader, 0, 4);
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	mess.SetParams(rtc->intLoader, 4, 4);
	return sendMessageOut(mess, rtc);

}

RunTimeError SetScriptParStrOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	unsigned int length;
	unsigned char* tempString = loadString(var, mId, rtc, length);

	Message mess;
	mess.destination = MessageSource_CELSCRIPT;
	mess.type = MessageType_SCRIPT;
	mess.mess = ScriptMess_ADDPARASTR;
	rtc->memory->ReadVariable(var2 - 1, rtc->intLoader, s);
	mess.SetParams(rtc->intLoader, 0, 4);
	mess.SetParams(tempString, 4, length);
	return sendMessageOut(mess, rtc);

}

RunTimeError SetScriptParFloatOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));

	Message mess;
	mess.destination = MessageSource_CELSCRIPT;
	mess.type = MessageType_SCRIPT;
	mess.mess = ScriptMess_ADDPARFLOAT;
	rtc->memory->ReadVariable(var2 - 1, rtc->intLoader, s);
	mess.SetParams(rtc->intLoader, 0, 4);
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	mess.SetParams(rtc->intLoader, 4, 4);
	return sendMessageOut(mess, rtc);

}

RunTimeError SetUIOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);

	Message mess;
	mess.destination = MessageSource_GUIENTITIES;
	mess.type = MessageType_GUIENTITIES;
	mess.mess = GUIMess_SETUI;
	mess.SetParams(rtc->intLoader, 0, 4);
	return sendMessageOut(mess, rtc);

}

RunTimeError TrackObjectOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);

	Message mess;
	mess.destination = MessageSource_ENTITIES;
	mess.type = MessageType_ENTITIES;
	mess.mess = GameBoardMess_SETTRACKING;
	mess.SetParams(rtc->intLoader, 0, 4);
	return sendMessageOut(mess, rtc);

}

RunTimeError ClearTrackingOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	Message mess;
	mess.destination = MessageSource_ENTITIES;
	mess.type = MessageType_ENTITIES;
	mess.mess = GameBoardMess_CLEARTRACK;
	return sendMessageOut(mess, rtc);

}

RunTimeError HockTrackingOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	Message mess;
	mess.destination = MessageSource_ENTITIES;
	mess.type = MessageType_ENTITIES;
	mess.mess = GameBoardMess_HOOKTRACK;
	return sendMessageOut(mess, rtc);

}

RunTimeError FocusUIOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);

	Message mess;
	mess.destination = MessageSource_GUIENTITIES;
	mess.type = MessageType_GUIENTITIES;
	mess.mess = GUIMess_FOCUS;
	mess.SetParams(rtc->intLoader, 0, 4);
	return sendMessageOut(mess, rtc);

}

RunTimeError ExportConstOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 5)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int length;
	unsigned char* bytes = loadString(var, mId, rtc, length);
	std::string name = std::string((char*)bytes, length);

	((CrossScriptMemoryObject*)rtc->gameObjects->GetValue(rtc->crossScriptObject))->SetMem(name, &params[4], paramSize - 4);
	return RunTimeError_OK;

}

RunTimeError ExportVarOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int length;
	unsigned char* bytes = loadString(var, mId, rtc, length);
	std::string name = std::string((char*)bytes, length);
	unsigned int var2 = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));

	unsigned int s = rtc->memory->GetVarLength(var2 - 1);

	while (s > rtc->stringLoadSize)
	{

		rtc->stringLoadSize += 100;
		delete[] rtc->stringLoader;
		rtc->stringLoader = new unsigned char[rtc->stringLoadSize];

	}

	rtc->memory->ReadVariable(var2 - 1, rtc->stringLoader, s);

	((CrossScriptMemoryObject*)rtc->gameObjects->GetValue(rtc->crossScriptObject))->SetMem(name, rtc->stringLoader, s);
	return RunTimeError_OK;

}

RunTimeError LinkDBGOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var1 = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var1 - 1, rtc->intLoader, s);

	Message mess;
	mess.destination = MessageSource_MASTER;
	mess.type = MessageType_SYSTEM;
	mess.mess = EventMess_LNKDBG;
	mess.SetParams(rtc->intLoader, 0, 4);
	return sendMessageOut(mess, rtc);

}

RunTimeError ImportOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int length;
	unsigned char* bytes = loadString(var, mId, rtc, length);
	std::string name = std::string((char*)bytes, length);

	unsigned int byteLength = 0;
	unsigned char* vals = ((CrossScriptMemoryObject*)rtc->gameObjects->GetValue(rtc->crossScriptObject))->GetMem(name, byteLength);

	if (vals == nullptr)
	{
		return RunTimeError_BADPARAMS;

	}

	rtc->memory->AddVariable(returnVar - 1, vals, byteLength);
	rtc->varWaiting->Add(false, returnVar - 1);

	return RunTimeError_OK;

}

RunTimeError JumpInv(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int location = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	int val = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));

	if (val == 0)
	{

		rtc->counter = location - 1;

	}

	return RunTimeError_OK;

}

RunTimeError JumpNow(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	rtc->counter = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24)) - 1;
	return RunTimeError_OK;

}

RunTimeError WaitForVar(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	return rtc->varWaiting->GetValue(var-1) ? RunTimeError_WAITINGFORWAR : RunTimeError_OK;

}
#pragma endregion

CelScriptRuntimeHandler::CelScriptRuntimeHandler(MessageQueue* mQueue, CelestialSlicedList<BaseObject*>* gameObjects, unsigned int crossScriptObject)
{

	this->crossScriptObject = crossScriptObject;
	maxOutMessages = 128;
	abort = false;
	this->mQueue = mQueue;

	this->operators = new ExecuteFunction[opcode_NA];

	operators[opcode_SETCONST] = SetConstValOperator;
	operators[opcode_SETVAR] = SetVarValOperator;

	operators[opcode_LOADLIGHT] = LoadLightOperator;
	operators[opcode_LOADMESH] = LoadMeshOperator;
	operators[opcode_LOADSCRIPT] = LoadScriptOperator;
	operators[opcode_LOADTXTBX] = LoadTextBoxOperator;
	operators[opcode_LOADPANEL] = LoadPanelOperator;
	operators[opcode_LOADIMAGE] = LoadImageOperator;
	operators[opcode_LOADGMBRD] = LoadGameBoardOperator;
	operators[opcode_LOADCAM] = LoadCameraOperator;
	operators[opcode_LOADOBJCT] = LoadObjectOperator;
	operators[opcode_LOADCPY] = LoadObjectCopyOperator;

	operators[opcode_UNLOAD] = UnLoadOperator;

	operators[opcode_ADDOBJECT] = AddObjectOperator;
	operators[opcode_ADDMESH] = AddMeshOperator;

	operators[opcode_RMVE] = RemoveOperator;

	operators[opcode_SETGMEBRD] = SetGameBoardOperator;
	operators[opcode_SETCMRA] = SetCameraOperator;

	operators[opcode_SETLCLK] = SetLeftClickOperator;
	operators[opcode_SETMCLK] = SetMiddleClickOperator;
	operators[opcode_SETRCLK] = SetRightClickOperator;
	operators[opcode_SETLDRG] = SetLeftDragOperator;
	operators[opcode_SETMDRG] = SetMiddleDragOperator;
	operators[opcode_SETRDRG] = SetRightDragOperator;
	operators[opcode_SETMWHL] = SetMouseWheelOperator;
	operators[opcode_SETUD] = SetMouseUpDownOperator;

	operators[opcode_SETUI] = SetUIOperator;
	operators[opcode_SETCRS] = SetMouseCursor;
	operators[opcode_FCSUI] = FocusUIOperator;

	operators[opcode_GETSCRNY] = GetScreenHeightOperator;
	operators[opcode_PRNT] = ParentOperator;

	operators[opcode_RESNAP] = ReSnapOperator;
	operators[opcode_POS] = PosOperator;
	operators[opcode_MVE] = MoveOperator;
	operators[opcode_PRPL] =PropelOperator;
	operators[opcode_RTE] = RotateOperator;
	operators[opcode_ORB] = OrbitOperator;
	operators[opcode_SIZE] = SizeOperator;
	operators[opcode_SCL] = ScaleOperator;
	operators[opcode_2DADDCHLD] = Add2DOperator;

	operators[opcode_POSTSTR] = PostStrOperator;
	operators[opcode_POSTNMBR] = PostNumOperator;
	operators[opcode_POSTFLOAT] = PostFloatOperator;

	operators[opcode_IGNRMS] = IgnoreMouseOperator; 
	operators[opcode_IGNRKY] = IgnoreKeyboardOperator; 
	operators[opcode_IGNRIN] = IgnoreInputOperator;

	operators[opcode_ACPTMS] = AcceptMouseOperator;
	operators[opcode_ACPTKY] = AcceptKeyboardOperator;
	operators[opcode_ACPTIN] = AcceptInputOperator;

	operators[opcode_INCRMNTLYR] = IncrementLayerOperator;
	operators[opcode_DCRMNLYR] = DecrementLayerOperator;
	operators[opcode_STLYR] = SetLayerOperator;

	operators[opcode_NEGATECONST] = NegateConstOperator;
	operators[opcode_NEGATEVAR] = NegateVarOperator;
	operators[opcode_NEGATEFLOAT] = NegateFloatOperator;

	operators[opcode_SUM2CONST] = SumConst2Operator;
	operators[opcode_SUMFLOAT] = SumFloatOperator;
	operators[opcode_SUMVAR] = SumVarOperator;

	operators[opcode_SUB2CONST] = SubConst2Operator;
	operators[opcode_SUBFLOAT] = SubFloatOperator;
	operators[opcode_SUBVAR] = SubVarOperator;

	operators[opcode_MUL2CONST] = MulConst2Operator;
	operators[opcode_MULFLOAT] = MulFloatOperator;
	operators[opcode_MULVAR] = MulVarOperator;

	operators[opcode_DIV2CONST] = DivConst2Operator;
	operators[opcode_DIVFLOAT] = DivFloatOperator;
	operators[opcode_DIVVAR] = DivVarOperator;

	operators[opcode_CASTFLOAT] = CastFloatOperator;

	operators[opcode_NUMEQUAL2CONST] = NumEqlConst2Operator;
	operators[opcode_NUMEQUALFLOAT] = FloatEqlVarOperator;
	operators[opcode_NUMEQUALVAR] = NumEqlVarOperator;


	operators[opcode_STREQUAL2CONST] = StrEqlConst2Operator;
	operators[opcode_STREQUALVAR] = StrEqlVarOperator;

	operators[opcode_NUMGRT2CONST] = NumGrtConst2Operator;
	operators[opcode_NUMGRTFLOAT] = NumGrtFloaOperator;
	operators[opcode_NUMGRTVAR] = NumGrtVarOperator;

	operators[opcode_RNSCRPT] = RunScriptOperator;
	operators[opcode_STSCRPTPRMNMBR] = SetScriptParNumOperator;
	operators[opcode_STSCRPTPRMSTR] = SetScriptParStrOperator;

	operators[opcode_EXPRTCNST] = ExportConstOperator;
	operators[opcode_EXPRTVAR] = ExportVarOperator;
	operators[opcode_IMPRT] = ImportOperator;

	operators[opcode_HDE] = HideOperator;
	operators[opcode_SHW] = ShowOperator;
	operators[opcode_HDECRS] = HideCursorOperator;
	operators[opcode_SHWCRS] = ShowCursorOperator;

	operators[opcode_LNKDBG] = LinkDBGOperator;
	operators[opcode_LNKTRGT] = LinkTargetOperator;


	operators[opcode_STXT] = SetTextOperator;
	operators[opcode_ADDTXT] = AppendTextOperator;
	operators[opcode_ADDLNE] = AddTextLineOperator;
	operators[opcode_STCNTNT] = SetContentOperator;
	operators[opcode_STBRDR] = SetBorderOperator;

	operators[opcode_TRCK] = TrackObjectOperator;
	operators[opcode_CRLTRCK] = ClearTrackingOperator;
	operators[opcode_HCKTRCK] = HockTrackingOperator;
	operators[opcode_GLUOBJCT] = GlueObjectOperator;

	operators[opcode_JMPINVVAR] = JumpInv;
	operators[opcode_JMPNOW] = JumpNow;

	operators[opCode_WTFRVR] = WaitForVar;

	rtc = new CelestialSlicedList<RunTimeCommons*>(20,nullptr);
	this->gameObjects = gameObjects;
	scriptStarts = new CelestialSlicedList<unsigned int>(20, 0);

	scriptFloatParams = new CelestialSlicedList<unsigned int>(20, 0);
	scriptNumParams = new CelestialSlicedList<unsigned int>(20, 0);
	scriptStrParams = new CelestialSlicedList<unsigned int>(20, 0);

	scriptParents = new CelestialSlicedList<unsigned int>(20, 0);
	scriptInited = new CelestialSlicedList<bool>(20);
	scriptIds = 0;

}

unsigned int CelScriptRuntimeHandler::GetGlobalId(unsigned int localId) const
{

	RunTimeCommons* rt = rtc->GetValue(localId);

	if (rt != nullptr)
	{

		return rt->boundObject;

	}

	return 0;
}

RunTimeError CelScriptRuntimeHandler::initScript(Resources::CelScriptCompiled* script, unsigned int id)
{
	RunTimeError er = RunTimeError_OK;

	if (script->GetScriptId() == 0)
	{

		scriptIds++;
		script->SetScriptId(scriptIds);

		RunTimeCommons* newRtc = new RunTimeCommons();
		newRtc->boundObject = id;
		newRtc->memory = new MemoryPool(128);
		newRtc->mQueue = mQueue;
		newRtc->varWaiting = new CelestialSlicedList<bool>(100, false);
		newRtc->outMessageBuffer = new Message[maxOutMessages];
		newRtc->outMessages = maxOutMessages;
		newRtc->gameObjects = gameObjects;
		newRtc->crossScriptObject = crossScriptObject;

		for (int i = 0; i < maxOutMessages; i++)
		{

			newRtc->outMessageBuffer[i].read = true;
			newRtc->outMessageBuffer[i].source = MessageSource_CELSCRIPT;
			newRtc->outMessageBuffer[i].senderId = scriptIds - 1;
		}

		int size;
		unsigned char* command = script->GetCode(size, 0);
		unsigned int scriptStart = command[0] | ((int)command[1] << 8) | ((int)command[2] << 16) | ((int)command[3] << 24);
		scriptStarts->Add(scriptStart);
		scriptInited->Add(scriptStart == 1, scriptIds - 1);
		newRtc->counter = 1;

		rtc->Add(newRtc, scriptIds - 1);

		if (!(scriptInited->GetValue(scriptIds - 1)))
		{

			er = commonScripts(scriptStart, newRtc, script);

		}
	}

	return er;

}

RunTimeError CelScriptRuntimeHandler::AddScriptFloatParam(int scriptId, unsigned char* value)
{

	CelScriptCompiled* script = (CelScriptCompiled*)gameObjects->GetValue(scriptId);
	RunTimeError er = initScript(script, scriptId);

	unsigned int par = scriptFloatParams->GetValue(script->GetScriptId() - 1);
	unsigned int parPlace = script->GetAdr(par, 'f');
	scriptFloatParams->Add(par + 1, script->GetScriptId() - 1);

	if (parPlace != 0)
	{

		rtc->GetValue(script->GetScriptId() - 1)->memory->AddVariable(parPlace - 1, value, 4);

	}

	return er;
}

RunTimeError CelScriptRuntimeHandler::AddScriptNumParam(int scriptId, unsigned char* value)
{

	CelScriptCompiled* script = (CelScriptCompiled*)gameObjects->GetValue(scriptId); 
	RunTimeError er = initScript(script, scriptId);

	unsigned int par = scriptNumParams->GetValue(script->GetScriptId() - 1);
	unsigned int parPlace = script->GetAdr(par, 'n');
	scriptNumParams->Add(par + 1, script->GetScriptId() - 1);

	if (parPlace != 0)
	{

		rtc->GetValue(script->GetScriptId() - 1)->memory->AddVariable(parPlace - 1, value, 4);

	}

	return er;

}

RunTimeError CelScriptRuntimeHandler::AddScriptStrParam(int scriptId, std::string value)
{

	CelScriptCompiled* script = (CelScriptCompiled*)gameObjects->GetValue(scriptId);
	RunTimeError er = initScript(script, scriptId);

	unsigned int par = scriptStrParams->GetValue(script->GetScriptId() - 1);
	unsigned int parPlace = script->GetAdr(par, 's');
	scriptStrParams->Add(par + 1, script->GetScriptId() - 1);

	if (parPlace != 0)
	{
		unsigned char* charArr = new unsigned char[value.length() + 4];
		charArr[0] = value.length() >> 0;
		charArr[1] = value.length() >> 8;
		charArr[2] = value.length() >> 16;
		charArr[3] = value.length() >> 24;

		for (unsigned int i = 4; i < 4 + value.length(); i++)
		{

			charArr[i] = value[i - 4];

		}

		rtc->GetValue(script->GetScriptId() - 1)->memory->AddVariable(parPlace - 1, charArr, value.length() + 4);
		delete[] charArr;
	}

	return er;

}

RunTimeError CelScriptRuntimeHandler::commonScripts(unsigned int end, RunTimeCommons* thisRtc, CelScriptCompiled* script)
{

	RunTimeError er = RunTimeError_OK;
	thisRtc->status = scriptStatus_RUNNING;
	
	for (thisRtc->counter; 
		thisRtc->counter < end && er == RunTimeError_OK && !abort; 
		thisRtc->counter++)
	{

		int size;
		unsigned char* command = script->GetCode(size, thisRtc->counter);
		unsigned int retVal = command[0] | ((int)command[1] << 8) | ((int)command[2] << 16) | ((int)command[3] << 24);
		er = operators[command[4]](retVal, &command[5], size - 5, script->GetScriptId()-1, thisRtc);

		if (thisRtc->status == scriptStatus_RUNOTHERSCRIPT)
		{

			er = RunTimeError_HALT;

		}

		if (thisRtc->pause)
		{

			thisRtc->status = scriptStatus_PAUSED;

			while (thisRtc->pause)
			{

				std::this_thread::yield();

			}

			thisRtc->status = scriptStatus_PRIMED;

		}
	}

	if (er == RunTimeError_MSGFULL || er == RunTimeError_WAITINGFORWAR)
	{
		
		thisRtc->counter -= er == RunTimeError_MSGFULL ? 0 : 1;
		thisRtc->status = scriptStatus_PRIMED;

	}
	else if (er == RunTimeError_OK)
	{

		scriptInited->Add(true, script->GetScriptId()-1);
		thisRtc->counter = scriptStarts->GetValue(script->GetScriptId()-1);
		thisRtc->status = scriptStatus_PRIMED;

	}

	return er;

}

RunTimeError CelScriptRuntimeHandler::RunScript(int id,unsigned int stackId, unsigned int eTime)
{

	CelScriptCompiled* script = (CelScriptCompiled*)gameObjects->GetValue(id);
	RunTimeError er = initScript(script, id);

	if (er != RunTimeError_OK)
	{

		return er;

	}

	RunTimeCommons* thisRtc = rtc->GetValue(script->GetScriptId()-1);
	thisRtc->stack = stackId;

	if (script != nullptr && thisRtc != nullptr)
	{


		if (thisRtc->status == scriptStatus_PRIMED)
		{

			unsigned int parPlace = script->GetAdr(RunTimeParams_ETIME);

			if (parPlace != 0)
			{

				unsigned char charArr[4]{eTime >> 0, eTime >> 8, eTime >> 16, eTime >> 24 };
				rtc->GetValue(script->GetScriptId() - 1)->memory->AddVariable(parPlace - 1, charArr, 4);

			}

			scriptNumParams->Add(0, script->GetScriptId() - 1);
			scriptStrParams->Add(0, script->GetScriptId() - 1);
			scriptFloatParams->Add(0, script->GetScriptId() - 1);

		}

		RunTimeError er = commonScripts(script->GetCodeSize(), thisRtc, script);
		return abort ? RunTimeError_ABORT : er;
	}

	return RunTimeError_OHNO;

}

void CelScriptRuntimeHandler::SetWaitingScriptVar(unsigned int scriptId, unsigned int scriptVar, int value)
{

	RunTimeCommons* thisRtc = rtc->GetValue(scriptId);
	CelScriptCompiled* script = (CelScriptCompiled*)gameObjects->GetValue(thisRtc->boundObject);
	thisRtc->pause = true;
	unsigned char commandVal[8];

	commandVal[0] = scriptVar >> 0;
	commandVal[1] = scriptVar >> 8;
	commandVal[2] = scriptVar >> 16;
	commandVal[3] = scriptVar >> 24;
	commandVal[4] = value >> 0;
	commandVal[5] = value >> 8;
	commandVal[6] = value >> 16;
	commandVal[7] = value >> 24;

	while (thisRtc->status == scriptStatus_RUNNING)
	{

		std::this_thread::yield();

	}

	SetConstValOperator(0, commandVal, 8, scriptId, thisRtc);
	thisRtc->pause = false;
	
}

void CelScriptRuntimeHandler::KillExecutions()
{

	abort = true;

}

CelScriptRuntimeHandler::~CelScriptRuntimeHandler()
{

	rtc->KillList();
	delete rtc;
	delete[] operators;
	delete scriptStarts;
	delete scriptInited;

	delete scriptNumParams;
	delete scriptStrParams;
	delete scriptFloatParams;

	delete scriptParents;

}
#include "stdafx.h"
#include "CelScriptRunTimeHandler.h"
#include "GUIObject.h"
#include "CelScriptCompiled.h"
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

	if (length > rtc->stringLoadSize)
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
	rtc->outMessageBuffer[rtc->outCurrentMessage].SetParams(mess.params, 0, mess.numParams);
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

	Message mess;
	mess.returnParam = returnVar;
	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_LOADGAMEBOARD;
	mess.SetParams(rtc->intLoader, 0, 4);
	rtc->varWaiting->Add(true, returnVar-1);
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
	int enu = GUIObjects_TEXTBOX;
	unsigned unsigned char tempBuff[]{enu >> 0, enu >> 8, enu >> 16, enu >>24};
	mess.SetParams(tempBuff, 0, 4);
	return sendMessageOut(mess, rtc);

}

RunTimeError LoadPanelOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	Message mess;
	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_LOADGUI;
	int enu = GUIObjects_LAYOUT;
	unsigned char tempBuff[]{enu >> 0, enu >> 8, enu >> 16, enu >> 24};
	mess.SetParams(tempBuff, 0, 4);
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

	Message mess;
	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_RESNAPGUI;
	mess.SetParams(params, 0, 12);

	return sendMessageOut(mess, rtc);

}

RunTimeError Pos2DOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 12)
	{

		return RunTimeError_BADPARAMS;

	}

	Message mess;
	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_POSGUI;
	mess.SetParams(params, 0, 12);
	return sendMessageOut(mess, rtc);

}

RunTimeError Size2DOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 12)
	{

		return RunTimeError_BADPARAMS;

	}

	Message mess;
	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_SIZEGUI;
	mess.SetParams(params, 0, 12);
	return sendMessageOut(mess, rtc);

}

RunTimeError Add2DOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	Message mess;
	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_ADDGUITOGUI;
	mess.SetParams(params, 0, 8);
	return sendMessageOut(mess, rtc);

}

RunTimeError AddKeyTriggerOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8)
	{

		return RunTimeError_BADPARAMS;

	}

	Message mess;
	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_ADDGUITRIGGER;
	mess.mess = GraphicMess_GETSCREEN;
	unsigned char tempBuff[]{0 >> 0, 0 >> 8, 0 >> 16, 0 >> 24};
	mess.SetParams(tempBuff, 0, 4);
	mess.SetParams(params, 4, 8);
	return sendMessageOut(mess, rtc);

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
	///TODO: Remove magic number
	unsigned char tempBuff[]{1 >> 0, 1 >> 8, 1 >> 16, 1 >> 24};
	mess.SetParams(tempBuff, 0, 4);
	mess.SetParams(message, 4, length);
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
	unsigned char tempBuff[]{1 >> 0, 1 >> 8, 1 >> 16, 1 >> 24};
	mess.SetParams(tempBuff, 0, 4);
	mess.SetParams((unsigned char*)stringParam.c_str(), 4, stringParam.length());
	return sendMessageOut(mess, rtc);

}

RunTimeError PostNumConstOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 4)
	{

		return RunTimeError_BADPARAMS;

	}

	int val = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	Message mess;
	mess.destination = MessageSource_GUIENTITIES;
	mess.type = MessageType_GUIENTITIES;
	mess.mess = GUIMess_POST;
	std::string stringParam = std::to_string(val);
	///TODO: Remove magic number
	unsigned char tempBuff[]{1 >> 0, 1 >> 8, 1 >> 16, 1 >> 24};
	mess.SetParams(tempBuff, 0, 4);
	mess.SetParams((unsigned char*)stringParam.c_str(), 4, stringParam.length());
	return sendMessageOut(mess, rtc);

}

RunTimeError SumConst2Operator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	return RunTimeError_OK;

}

RunTimeError SumConst1Operator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	int val = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24)) + (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->intLoader[0] = val >> 0;
	rtc->intLoader[1] = val >> 8;
	rtc->intLoader[2] = val >> 16;
	rtc->intLoader[3] = val >> 24;
	rtc->memory->AddVariable(returnVar - 1, rtc->intLoader, s);
	rtc->varWaiting->Add(false, returnVar - 1);
	return RunTimeError_OK;

}

RunTimeError Sum1ConstOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	int val = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24)) + (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->intLoader[0] = val >> 0;
	rtc->intLoader[1] = val >> 8;
	rtc->intLoader[2] = val >> 16;
	rtc->intLoader[3] = val >> 24;
	rtc->memory->AddVariable(returnVar - 1, rtc->intLoader, s);
	rtc->varWaiting->Add(false, returnVar - 1);
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

RunTimeError SubConst2Operator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	return RunTimeError_OK;

}

RunTimeError SubConst1Operator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	int val = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24)) - (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->intLoader[0] = val >> 0;
	rtc->intLoader[1] = val >> 8;
	rtc->intLoader[2] = val >> 16;
	rtc->intLoader[3] = val >> 24;
	rtc->memory->AddVariable(returnVar - 1, rtc->intLoader, s);
	rtc->varWaiting->Add(false, returnVar - 1);
	return RunTimeError_OK;

}

RunTimeError Sub1ConstOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	int val = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24)) - (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	rtc->intLoader[0] = val >> 0;
	rtc->intLoader[1] = val >> 8;
	rtc->intLoader[2] = val >> 16;
	rtc->intLoader[3] = val >> 24;
	rtc->memory->AddVariable(returnVar - 1, rtc->intLoader, s);
	rtc->varWaiting->Add(false, returnVar - 1);
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

RunTimeError MulConst2Operator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	return RunTimeError_OK;

}

RunTimeError MulConst1Operator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	int val = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24)) * (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->intLoader[0] = val >> 0;
	rtc->intLoader[1] = val >> 8;
	rtc->intLoader[2] = val >> 16;
	rtc->intLoader[3] = val >> 24;
	rtc->memory->AddVariable(returnVar - 1, rtc->intLoader, s);
	rtc->varWaiting->Add(false, returnVar - 1);
	return RunTimeError_OK;

}

RunTimeError Mul1ConstOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	int val = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24))*(rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	rtc->intLoader[0] = val >> 0;
	rtc->intLoader[1] = val >> 8;
	rtc->intLoader[2] = val >> 16;
	rtc->intLoader[3] = val >> 24;
	rtc->memory->AddVariable(returnVar - 1, rtc->intLoader, s);
	rtc->varWaiting->Add(false, returnVar - 1);
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

RunTimeError DivConst2Operator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	return RunTimeError_OK;

}

RunTimeError DivConst1Operator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	int val = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24)) / (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->intLoader[0] = val >> 0;
	rtc->intLoader[1] = val >> 8;
	rtc->intLoader[2] = val >> 16;
	rtc->intLoader[3] = val >> 24;
	rtc->memory->AddVariable(returnVar - 1, rtc->intLoader, s);
	rtc->varWaiting->Add(false, returnVar - 1);
	return RunTimeError_OK;

}

RunTimeError Div1ConstOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	int val = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24)) / (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24));
	rtc->intLoader[0] = val >> 0;
	rtc->intLoader[1] = val >> 8;
	rtc->intLoader[2] = val >> 16;
	rtc->intLoader[3] = val >> 24;
	rtc->memory->AddVariable(returnVar - 1, rtc->intLoader, s);
	rtc->varWaiting->Add(false, returnVar - 1);
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

RunTimeError NumEqlConst2Operator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	return RunTimeError_OK;

}

RunTimeError NumEqlConst1Operator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	int val = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24)) == (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24)) ? 1 : 0;
	rtc->intLoader[0] = val >> 0;
	rtc->intLoader[1] = val >> 8;
	rtc->intLoader[2] = val >> 16;
	rtc->intLoader[3] = val >> 24;
	rtc->memory->AddVariable(returnVar - 1, rtc->intLoader, s);
	rtc->varWaiting->Add(false, returnVar - 1);
	return RunTimeError_OK;

}

RunTimeError NumEql1ConstOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	int val = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24)) == (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24)) ? 1 : 0;
	rtc->intLoader[0] = val >> 0;
	rtc->intLoader[1] = val >> 8;
	rtc->intLoader[2] = val >> 16;
	rtc->intLoader[3] = val >> 24;
	rtc->memory->AddVariable(returnVar - 1, rtc->intLoader, s);
	rtc->varWaiting->Add(false, returnVar - 1);
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

RunTimeError StrEqlConst2Operator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	return RunTimeError_OK;

}

RunTimeError StrEqlConst1Operator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	unsigned int length;
	std::string varVal = std::string((char*)loadString(var, mId, rtc, length));

	std::string constVal = std::string((char)(&(params[8])), paramSize - 7);

	int val = varVal == constVal ? 1 : 0;
	rtc->intLoader[0] = val >> 0;
	rtc->intLoader[1] = val >> 8;
	rtc->intLoader[2] = val >> 16;
	rtc->intLoader[3] = val >> 24;
	unsigned int s = 4;
	rtc->memory->AddVariable(returnVar - 1, rtc->intLoader, s);
	rtc->varWaiting->Add(false, returnVar - 1);
	return RunTimeError_OK;

}

RunTimeError StrEql1ConstOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int length = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	std::string constVal = std::string((char*)(&(params[4])), length - 4);
	length += 4;

	unsigned int var = (params[length] | ((int)params[length + 1] << 8) | ((int)params[length + 2] << 16) | ((int)params[length + 3] << 24));
	unsigned int stringlength;
	std::string varVal = std::string((char*)loadString(var, mId, rtc, stringlength));
	int val = varVal == constVal ? 1 : 0;

	rtc->intLoader[0] = val >> 0;
	rtc->intLoader[1] = val >> 8;
	rtc->intLoader[2] = val >> 16;
	rtc->intLoader[3] = val >> 24;
	unsigned int s = 4;
	rtc->memory->AddVariable(returnVar - 1, rtc->intLoader, s);
	rtc->varWaiting->Add(false, returnVar - 1);
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

RunTimeError NumGrtConst1Operator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	int val = (rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24)) >(params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24)) ? 1 : 0;
	rtc->intLoader[0] = val >> 0;
	rtc->intLoader[1] = val >> 8;
	rtc->intLoader[2] = val >> 16;
	rtc->intLoader[3] = val >> 24;
	rtc->memory->AddVariable(returnVar - 1, rtc->intLoader, s);
	rtc->varWaiting->Add(false, returnVar - 1);
	return RunTimeError_OK;

}

RunTimeError NumGrt1ConstOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
{

	if (paramSize < 8 || returnVar == 0)
	{

		return RunTimeError_BADPARAMS;

	}

	unsigned int s = 4;
	unsigned int var = (params[4] | ((int)params[5] << 8) | ((int)params[6] << 16) | ((int)params[7] << 24));
	rtc->memory->ReadVariable(var - 1, rtc->intLoader, s);
	int val = (params[0] | ((int)params[1] << 8) | ((int)params[2] << 16) | ((int)params[3] << 24)) >(rtc->intLoader[0] | ((int)rtc->intLoader[1] << 8) | ((int)rtc->intLoader[2] << 16) | ((int)rtc->intLoader[3] << 24)) ? 1 : 0;
	rtc->intLoader[0] = val >> 0;
	rtc->intLoader[1] = val >> 8;
	rtc->intLoader[2] = val >> 16;
	rtc->intLoader[3] = val >> 24;
	rtc->memory->AddVariable(returnVar - 1, rtc->intLoader, s);
	rtc->varWaiting->Add(false, returnVar - 1);
	return RunTimeError_OK;

}

RunTimeError NumGrtVarOperator(unsigned int returnVar, unsigned char* params, unsigned int paramSize, unsigned int mId, RunTimeCommons* rtc)
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

CelScriptRuntimeHandler::CelScriptRuntimeHandler(MessageQueue* mQueue, CelestialSlicedList<BaseObject*>* gameObjects)
{

	maxOutMessages = 128;
	abort = false;
	this->mQueue = mQueue;

	this->operators = new ExecuteFunction[opcode_NA];

	operators[opcode_SETCONST] = SetConstValOperator;
	operators[opcode_SETVAR] = SetVarValOperator;

	operators[opcode_LOADLIGHT] = LoadLightOperator;
	operators[opcode_LOADMESH] = LoadMeshOperator;
	operators[opcode_LOADSCRIPT] = LoadScriptOperator;
	operators[opcode_LOADKYTRGR] = LoadKeyTriggerOperator;
	operators[opcode_LOADCHRKYTRGR] = LoadKeyTriggerOperatorChar;
	operators[opcode_LOADTXTBX] = LoadTextBoxOperator;
	operators[opcode_LOADPANEL] = LoadPanelOperator;
	operators[opcode_LOADGMBRD] = LoadGameBoardOperator;
	operators[opcode_LOADCAM] = LoadCameraOperator;
	operators[opcode_LOADOBJCT] = LoadObjectOperator;

	operators[opcode_ADDOBJECT] = AddObjectOperator;
	operators[opcode_ADDMESH] = AddMeshOperator;
	operators[opcode_SETGMEBRD] = SetGameBoardOperator;
	operators[opcode_SETCMRA] = SetCameraOperator;
	operators[opcode_SETLCLK] = SetLeftClickOperator;

	operators[opcode_GETSCRNX] = GetScreenWidthOperator;
	operators[opcode_GETSCRNY] = GetScreenHeightOperator;

	operators[opcode_RESNAP] = ReSnapOperator;
	operators[opcode_2DPOS] = Pos2DOperator;
	operators[opcode_2DSIZE] = Size2DOperator;
	operators[opcode_2DADDASNODE] = Add2DOperator;
	operators[opcode_2DADDKYTRGR] = AddKeyTriggerOperator;

	operators[opcode_POSTSTR] = PostStrOperator;
	operators[opcode_POSTNMBR] = PostNumOperator;
	operators[opcode_POSTNMBRCONST] = PostNumConstOperator;

	operators[opcode_SUM2CONST] = SumConst2Operator;
	operators[opcode_SUM1CONST] = Sum1ConstOperator;
	operators[opcode_SUMCONST1] = SumConst1Operator;
	operators[opcode_SUMVAR] = SumVarOperator;

	operators[opcode_SUB2CONST] = SubConst2Operator;
	operators[opcode_SUB1CONST] = Sub1ConstOperator;
	operators[opcode_SUBCONST1] = SubConst1Operator;
	operators[opcode_SUBVAR] = SubVarOperator;

	operators[opcode_MUL2CONST] = MulConst2Operator;
	operators[opcode_MUL1CONST] = Mul1ConstOperator;
	operators[opcode_MULCONST1] = MulConst1Operator;
	operators[opcode_MULVAR] = MulVarOperator;

	operators[opcode_DIV2CONST] = DivConst2Operator;
	operators[opcode_DIV1CONST] = Div1ConstOperator;
	operators[opcode_DIVCONST1] = DivConst1Operator;
	operators[opcode_DIVVAR] = DivVarOperator;

	operators[opcode_NUMEQUAL2CONST] = NumEqlConst2Operator;
	operators[opcode_NUMEQUAL1CONST] = NumEql1ConstOperator;
	operators[opcode_NUMEQUALCONST1] = NumEqlConst1Operator;
	operators[opcode_NUMEQUALVAR] = NumEqlVarOperator;

	operators[opcode_STREQUAL2CONST] = StrEqlConst2Operator;
	operators[opcode_STREQUAL1CONST] = StrEql1ConstOperator;
	operators[opcode_STREQUALCONST1] = StrEqlConst1Operator;
	operators[opcode_STREQUALVAR] = StrEqlVarOperator;

	operators[opcode_NUMGRT2CONST] = NumGrtConst2Operator;
	operators[opcode_NUMGRT1CONST] = NumGrt1ConstOperator;
	operators[opcode_NUMGRTCONST1] = NumGrtConst1Operator;
	operators[opcode_NUMGRTVAR] = NumGrtVarOperator;

	operators[opcode_RNSCRPT] = RunScriptOperator;
	operators[opcode_STSCRPTPRMNMBR] = SetScriptParNumOperator;
	operators[opcode_STSCRPTPRMSTR] = SetScriptParStrOperator;

	operators[opcode_JMPINVVAR] = JumpInv;
	operators[opcode_JMPNOW] = JumpNow;

	operators[opCode_WTFRVR] = WaitForVar;

	rtc = new CelestialSlicedList<RunTimeCommons*>(20,nullptr);
	this->gameObjects = gameObjects;
	scriptStarts = new CelestialSlicedList<unsigned int>(20, 0);
	scriptNumParams = new CelestialSlicedList<unsigned int>(20, 0);
	scriptStrParams = new CelestialSlicedList<unsigned int>(20, 0);
	scriptParents = new CelestialSlicedList<unsigned int>(20, 0);
	scriptInited = new CelestialSlicedList<bool>(20);
	scriptIds = 0;

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

RunTimeError CelScriptRuntimeHandler::AddScriptParam(int scriptId, int value)
{

	CelScriptCompiled* script = (CelScriptCompiled*)gameObjects->GetValue(scriptId); 
	RunTimeError er = initScript(script, scriptId);

	unsigned int par = scriptNumParams->GetValue(script->GetScriptId() - 1);
	unsigned int parPlace = script->GetAdr(par, false);
	scriptNumParams->Add(par + 1, script->GetScriptId() - 1);
	unsigned char charArr[4]{value >> 0, value >> 8, value >> 16, value >> 24 };
	rtc->GetValue(script->GetScriptId() - 1)->memory->AddVariable(parPlace - 1, charArr, 4);
	return er;

}

RunTimeError CelScriptRuntimeHandler::AddScriptParam(int scriptId, std::string value)
{

	CelScriptCompiled* script = (CelScriptCompiled*)gameObjects->GetValue(scriptId);
	RunTimeError er = initScript(script, scriptId);

	unsigned int par = scriptStrParams->GetValue(script->GetScriptId()-1);
	unsigned int parPlace = script->GetAdr(par, true);
	scriptStrParams->Add(par + 1, script->GetScriptId() - 1);
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

			scriptNumParams->Add(0, (thisRtc->intLoader[0] | ((int)thisRtc->intLoader[1] << 8) | ((int)thisRtc->intLoader[2] << 16) | ((int)thisRtc->intLoader[3] << 24)));
			scriptStrParams->Add(0, (thisRtc->intLoader[0] | ((int)thisRtc->intLoader[1] << 8) | ((int)thisRtc->intLoader[2] << 16) | ((int)thisRtc->intLoader[3] << 24)));
			er = RunTimeError_HALT;
			thisRtc->counter++;

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
	delete scriptParents;

}
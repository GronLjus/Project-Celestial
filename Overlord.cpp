#include "stdafx.h"
#include "Overlord.h"
#include "CelestialMath.h"
#include <iostream>
#include <sstream>
#include <thread>

#include "KubLingMachine.h"

using namespace Entities;
using namespace Logic;
using namespace Graphics;
using namespace Input;
using namespace Resources;
using namespace CrossHandlers;
using namespace CelestialMath;
using namespace Tasking;
using namespace std;

//Global variables for use in the threads
int gFPS;
int lFPS;

int gFPS2;
int lFPS2;

unsigned int gTime = 0;
unsigned int gTimeLast = 0;
unsigned int lTime = 0;
unsigned int lTimeLast = 0;

Overlord::Overlord(void)
{

	unsigned int flip = 2;
	die = false;
	okToDraw = false;
	gBH = new GameBoardHandler();
	gH = new GraphicHandler(flip);
	rH = new ResourceHandler(flip);
	iH = new InputHandler();
	klH = new KubLingHandler();
	guiH = new GUIEntityHandler();
	tH = new TaskHandler();

	messageHandlers = new IHandleMessages*[MessageSource_NA];
	messageHandlers[MessageSource_RESOURCES] = rH;
	messageHandlers[MessageSource_ENTITIES] = gBH;
	messageHandlers[MessageSource_GUIENTITIES] = guiH;
	messageHandlers[MessageSource_CELSCRIPT] = klH;
	messageHandlers[MessageSource_GRAPHICS] = gH;
	messageHandlers[MessageSource_INPUT] = iH;
	messageHandlers[MessageSource_TASKS] = tH;
	messageHandlers[MessageSource_MASTER] = nullptr;
	messageHandlers[MessageSource_OBJECT] = nullptr;

	dbgOut = new TextContainer();
	inQueue = new MessageQueue();
	outQueue = new MessageQueue();

}

HRESULT Overlord::Init(HWND hwnd)
{

	unsigned int maxInstances = 512;
	dbgOut->AddTextLine("Initiating CelestialEngine");
	CoInitialize(nullptr);
	this->hWnd = hwnd;
	gQ = GraphicQuality();
	dS = DrawingStyle();

	gQ.blurSamples = 4;
	dS.enlighten = true;
	gQ.fullscreen = false;
	dS.motionBlur = true;
	gQ.resolutionX = 1024;
	gQ.resolutionY = 768;
	gQ.shadows.blockerSamples = 4.0f;
	gQ.shadows.shadowCoding = ShadowCode_SSVOLUME;
	gQ.shadows.shadowSamples = 32.0f;
	gQ.shadows.shadowResolutionX = 1536;
	gQ.shadows.shadowResolutionY = 1024;
	gQ.shadows.shadowTyping = ShadowType_VOLUME;
	dS.useNormalMaps = true;
	dS.useParticle = true;
	dS.useSkybox = true;

	HRESULT res = gH->PreInit(hwnd, gQ, dS, rH->GetObjectContainer());
	tH->Init(rH->GetObjectContainer());

	if (res != S_OK)
	{

		return res;

	}

	CardHandler* tempCard = gH->GetCardHandler();
	rH->Init(tempCard, dbgOut, vectorUI2(gQ.resolutionX, gQ.resolutionY),32,32*1.0f,maxInstances);
	unsigned int rawCode = rH->AssembleRaws("Content\\Scripts");
	unsigned int heap = rH->GetHeapContainer();

	unsigned char kubInit[8];
	memcpy(&kubInit[0], &rawCode, 4);
	memcpy(&kubInit[4], &heap, 4);
	klH->Init(rH->GetObjectContainer());
	guiH->Init(rH->GetObjectContainer());


	Message mess;
	mess.source = MessageSource_NA;
	mess.type = MessageType_SCRIPT;
	mess.destination = MessageSource_CELSCRIPT;
	mess.source = MessageSource_CELSCRIPT;
	mess.SetParams(kubInit, 0, 8);
	mess.mess = ScriptMess_SETCODE;
	klH->HandleMessage(&mess);
	klH->Update(0);


	mess.source = MessageSource_NA;
	mess.type = MessageType_RESOURCES;
	mess.destination = MessageSource_RESOURCES;
	mess.source = MessageSource_CELSCRIPT;
	mess.SetParams((unsigned char*)"Content\\Scripts\\PreLoad", 0, 31);
	mess.mess = ResourceMess_LOADSCRIPT;
	rH->HandleMessage(&mess);
	rH->Update(0);
	Message* retMess = rH->GetMessages()->PopMessage();
	unsigned int scriptId = retMess->params[8] | ((int)retMess->params[9] << 8) | ((int)retMess->params[10] << 16) | ((int)retMess->params[11] << 24);

	if (scriptId != 0)
	{

		firstMessage.source = MessageSource_NA;
		firstMessage.destination = MessageSource_CELSCRIPT;
		firstMessage.type = MessageType_SCRIPT;
		firstMessage.mess = ScriptMess_RUN;
		firstMessage.SetParams(&(retMess->params[8]), 0, 4);
		klH->HandleMessage(&firstMessage);

		do
		{
		
			klH->Update(0);
			updateMessages(MessageSource_CELSCRIPT);
			rH->Update(0);
			updateMessages(MessageSource_RESOURCES);
			guiH->Update(0);
			updateMessages(MessageSource_GUIENTITIES);
			gH->Update(0);
			updateMessages(MessageSource_GRAPHICS);

		} while (!klH->AllStopped());
	}

	okToDraw = true;
	gBH->Init(rH->GetObjectContainer());
	iH->Init(rH->GetObjectContainer());

	dbgOut->AddTextLine("Initiating CelestialGraphics");
	res = gH->FullInit(dbgOut, maxInstances);

	dbgOut->AddTextLine("Engine Loaded!");
	dbgOut->AddTextLine("Loading root script");

	mess.SetParams((unsigned char*)"Content\\Scripts\\Root", 0, 28);
	rH->HandleMessage(&mess);
	rH->Update(0);
	retMess = rH->GetMessages()->PopMessage();
	scriptId = retMess->params[8] | ((int)retMess->params[9] << 8) | ((int)retMess->params[10] << 16) | ((int)retMess->params[11] << 24);

	if (scriptId != 0)
	{

		firstMessage.source = MessageSource_NA;
		firstMessage.destination = MessageSource_CELSCRIPT;
		firstMessage.type = MessageType_SCRIPT;
		firstMessage.mess = ScriptMess_RUN;
		firstMessage.SetParams(&(retMess->params[8]),0,4);
		klH->HandleMessage(&firstMessage);
		//klH->Update(0);

	}
	
	return res;

}

void Overlord::SendMsg(Message* msg)
{

	if (msg->type == MessageType_INPUT &&
		msg->mess == InputMess_KEYDWN &&
		msg->params[1] == CelestialSpecKeyCodes_F10)
	{

		gH->Toggle3D();

	}

	if(messageHandlers[msg->destination] != nullptr)
	{

		messageHandlers[msg->destination]->HandleMessage(msg);

	}
}

Message* Overlord::GetNextSystemMessage()
{
	Message* mess = outQueue->PopMessage();

	if (mess != nullptr && mess->type != MessageType_NA)
	{

		return mess;

	}

	return nullptr;

}

void Overlord::Kill()
{

	gH->Kill();
	klH->Kill();
	die = true;

}

void Overlord::processCommand(std::string* command, int nrParam)
{

	if (command[0] == "enable" || command[0] == "disable")
	{

		string addVal = command[0] == "enable" ? "1" : "0";

		string* newLine = new string[nrParam + 1];
		newLine[nrParam] = addVal;

		for (int i = 0; i < nrParam; i++)
		{

			newLine[i] = command[i];

		}

		nrParam++;
		delete[] command;
		command = newLine;

	}
	else if (command[0] == "load")
	{

		if (command[1] == "mesh")
		{

			if (command[2] != "plane")
			{


				string* newLine = new string[nrParam + 1];
				newLine[0] = command[0];
				newLine[1] = command[1];
				newLine[2] = "file";
				newLine[nrParam] = command[2];

				nrParam++;
				delete[] command;
				command = newLine;

			}
		}
	}
}

void Overlord::HandleDrawing(unsigned int time)
{

	if (okToDraw)
	{

		gH->Draw(time); 

		unsigned int time2 = clock();
		gTime += time2 - gTimeLast;
		gTimeLast = time2;

		if (gTime >= 1000)
		{

			gFPS = gFPS2;
			gTime = 0;
			gFPS2 = 0;

		}

		gFPS2++;

	}
}

void Overlord::updateMessages(MessageSource handler)
{

	if (messageHandlers[handler] != nullptr)
	{

		Message* currentMessage = messageHandlers[handler]->GetMessages()->PopMessage();

		while (currentMessage->type != MessageType_NA)
		{

			if (currentMessage->destination == MessageSource_SYSTEM)
			{

				outQueue->PushMessage(currentMessage);

			}
			else if (currentMessage->destination != MessageSource_NA &&currentMessage->destination != MessageSource_MASTER && messageHandlers[currentMessage->destination] != nullptr)
			{

				messageHandlers[currentMessage->destination]->HandleMessage(currentMessage);

			}
			else if (currentMessage->destination == MessageSource_MASTER)
			{

				inQueue->PushMessage(currentMessage);

				if (currentMessage->mess == EventMess_LNKDBG)
				{

					unsigned int param1 = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);
					GUITextBox* textBox = (GUITextBox*)rH->GetObjectContainer()->GetValue(param1);
					textBox->SetText(dbgOut);
					killdbg = false;
					currentMessage->read = true;

				}
			}

			currentMessage = messageHandlers[handler]->GetMessages()->PopMessage();

		}
	}
}

void Overlord::updateSystemVars(unsigned int time)
{

	unsigned char var[4];
	memcpy(var, &time, 4);
	klH->SetSystemVar(SystemMem_TIME, var, 4);

	memcpy(var, &lFPS, 4);
	klH->SetSystemVar(SystemMem_FPS1, var, 4);

	memcpy(var, &gFPS, 4);
	klH->SetSystemVar(SystemMem_FPS2, var, 4);

}

void Overlord::Update(unsigned int time)
{

	lTime += (time - lTimeLast);
	lTimeLast = time;

	if (lTime >= 1000)
	{

		lFPS = lFPS2;
		lTime = 0;
		lFPS2 = 0;

	}

	lFPS2++;

	if(gH->GetIsInited())
	{

		updateSystemVars(time);

		unsigned int start = clock();
		iH->Update(time);
		updateMessages(MessageSource_INPUT);
		unsigned int start1 = clock();
		gBH->UpdateMessages(time);
		updateMessages(MessageSource_ENTITIES);
		tH->UpdateMessages(time);
		tH->Update(time);
		updateMessages(MessageSource_TASKS);

		klH->Update(time);
		updateMessages(MessageSource_CELSCRIPT);
		unsigned int start3 = clock();
		rH->Update(time);
		updateMessages(MessageSource_RESOURCES);
		unsigned int start4 = clock();
		gBH->Update(time);
		updateMessages(MessageSource_ENTITIES);
		unsigned int start5 = clock();
		guiH->Update(time);
		updateMessages(MessageSource_GUIENTITIES);
		unsigned int start6 = clock();
		gH->Update(time);
		updateMessages(MessageSource_GRAPHICS);

		unsigned int stop = clock();
		unsigned int diff = stop - start;

	}
}

Overlord::~Overlord()
{
	
	gH->Release();
	okToDraw = false;

	delete inQueue;
	delete outQueue;
	delete[] messageHandlers;
	delete gBH;
	delete iH;
	delete gH;
	delete klH;
	delete guiH;
	delete rH;
	delete tH;

	if (killdbg)
	{

		delete dbgOut;

	}
}
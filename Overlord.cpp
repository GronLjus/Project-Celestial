#include "stdafx.h"
#include "Overlord.h"
#include "CelestialMath.h"
#include <iostream>
#include <sstream>
#include <thread>

using namespace Entities;
using namespace Logic;
using namespace Graphics;
using namespace Input;
using namespace Resources;
using namespace CrossHandlers;
using namespace CelestialMath;
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
	isDrawingBool = false;
	okToDraw = false;
	gBH = new GameBoardHandler();
	gH = new GraphicHandler(flip);
	rH = new ResourceHandler(flip);
	iH = new InputHandler();
	pH = new PlayerHandler();
	cH = new CelscriptHandler();
	guiH = new GUIEntityHandler();

	messageHandlers = new IHandleMessages*[MessageSource_NA];
	messageHandlers[MessageSource_RESOURCES] = rH;
	messageHandlers[MessageSource_ENTITIES] = gBH;
	messageHandlers[MessageSource_GUIENTITIES] = guiH;
	messageHandlers[MessageSource_CELSCRIPT] = cH;
	messageHandlers[MessageSource_GRAPHICS] = gH;
	messageHandlers[MessageSource_INPUT] = iH;
	messageHandlers[MessageSource_MASTER] = nullptr;
	messageHandlers[MessageSource_OBJECT] = nullptr;

	dbgOut = new TextContainer();
	inQueue = new MessageQueue();

}

HRESULT Overlord::Init(HWND hwnd)
{

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

	HRESULT res = gH->PreInit(hwnd, gQ, dS);

	if (res != S_OK)
	{

		return res;

	}

	CardHandler* tempCard = gH->GetCardHandler();
	rH->Init(tempCard, dbgOut, Vector2(gQ.resolutionX, gQ.resolutionY));
	guiH->Init(rH->GetObjectContainer());
	cH->Init(rH->GetObjectContainer(), rH->GetCrossScriptObject());
	gBH->Init(rH->GetObjectContainer());
	iH->Init(rH->GetObjectContainer());

	dbgOut->AddTextLine("Initiating CelestialGraphics");
	res = gH->FullInit(dbgOut, rH->GetObjectContainer());
	okToDraw = true;

	dbgOut->AddTextLine("Engine Loaded!");
	dbgOut->AddTextLine("Loading root script");

	Message mess;
	mess.source = MessageSource_NA;
	mess.type = MessageType_RESOURCES;
	mess.destination = MessageSource_RESOURCES;
	mess.source = MessageSource_CELSCRIPT;
	mess.SetParams((unsigned char*)"TestRoot.celsrc", 0, 15);
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
		firstMessage.SetParams(&(retMess->params[8]),0,4);
		cH->HandleMessage(&firstMessage);
		//cH->Update(0);

	}
	
	return res;

}

void Overlord::SendMsg(Message* msg)
{

	if(messageHandlers[msg->destination] != nullptr)
	{

		messageHandlers[msg->destination]->HandleMessage(msg);

	}
}

void Overlord::Kill()
{

	gH->Kill();
	cH->Kill();
	die = true;
	okToDraw = false;

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

	isDrawingBool = true;

	if (okToDraw)
	{

		isDrawing.lock();
		gH->Draw(); 
		isDrawing.unlock();

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
	
	isDrawingBool = false;

}

void Overlord::updateMessages(MessageSource handler)
{

	if (messageHandlers[handler] != nullptr)
	{

		Message* currentMessage = messageHandlers[handler]->GetMessages()->PopMessage();

		while (currentMessage->type != MessageType_NA)
		{

			if (currentMessage->destination != MessageSource_NA &&currentMessage->destination != MessageSource_MASTER && messageHandlers[currentMessage->destination] != nullptr)
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
					dbgOut = nullptr;
					currentMessage->read = true;

				}
			}

			currentMessage = messageHandlers[handler]->GetMessages()->PopMessage();

		}
	}
}

void Overlord::Update(unsigned int time)
{

	unsigned int time2 = clock();
	lTime += (time2 - lTimeLast);
	lTimeLast = time2;

	if (lTime >= 1000)
	{

		lFPS = lFPS2;
		lTime = 0;
		lFPS2 = 0;

	}

	lFPS2++;


	/*std::stringstream out;
	out << gFPS;
	dbgPnl->SetText(0, "DFPS: " + out.str());
	out.str("");
	out << lFPS;
	dbgPnl->SetText(1, "LFPS: " + out.str());*/

	if(gH->GetIsInited())
	{
		unsigned int start = clock();
		iH->Update(time);
		updateMessages(MessageSource_INPUT);
		unsigned int start1 = clock();
		gBH->UpdateMessages(time);
		updateMessages(MessageSource_ENTITIES);
		cH->Update(time);
		updateMessages(MessageSource_CELSCRIPT);
		unsigned int start2 = clock();
		pH->Update();
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
	
	isDrawing.lock();
	gH->Release();

	delete inQueue;
	delete[] messageHandlers;
	delete gBH;
	delete iH;
	delete gH;
	delete pH;
	delete cH;
	delete guiH;
	delete rH;

	if (dbgOut != nullptr)
	{

		delete dbgOut;

	}

	isDrawing.unlock();
	
}
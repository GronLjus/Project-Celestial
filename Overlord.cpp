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

	dbgIn = nullptr;
	dbgOut = nullptr;
	dbgPnl = nullptr;

	inQueue = new MessageQueue();

}

HRESULT Overlord::Init(HWND hwnd)
{

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

	Message mess;
	mess.source = MessageSource_NA;
	mess.destination = MessageSource_RESOURCES;
	mess.type = MessageType_RESOURCES;
	mess.mess = ResourceMess_LOADGUI;
	mess.param1 = GUIObjects_TEXTBOX;
	mess.param2 = GUISnap_LEFT;
	mess.param3 = GUISnap_TOP;
	Message mess2 = Message(mess);
	Message mess3 = Message(mess);
	Message mess4 = Message(mess);
	rH->HandleMessage(&mess);
	rH->HandleMessage(&mess2);
	rH->HandleMessage(&mess3);
	mess4.param1 = GUIObjects_LAYOUT;
	rH->HandleMessage(&mess4);
	rH->Update(0);

	dbgIn = (GUITextBox*)(rH->GetObjectContainer()->GetValue(rH->GetMessages()->PopMessage()->param1));
	dbgIn->SetPosition(Vector2(0, 0));
	dbgIn->SetSize(Vector2(1, 1));
	dbgIn->Toggle(true);

	int dbgOutId = rH->GetMessages()->PopMessage()->param1;
	dbgOut = (GUITextBox*)(rH->GetObjectContainer()->GetValue(dbgOutId));
	dbgOut->SetPosition(Vector2(0.0f, 0.0f));
	dbgOut->SetSize(Vector2(0.5f, 1.0f));
	dbgOut->Toggle(true);

	dbgPnl = (GUITextBox*)(rH->GetObjectContainer()->GetValue(rH->GetMessages()->PopMessage()->param1));
	dbgPnl->SetPosition(Vector2(0.75, 0.0f));
	dbgPnl->SetSize(Vector2(0.25f, 0.25f));
	dbgPnl->Toggle(true);

	Vector3* temp = new Vector3[3];
	temp[0] = Vector3(0.0f, 5.0f, -10.5f);
	temp[1] = Vector3(0.0f, 5.0f, 0.0f);
	temp[2] = Vector3(0.0f, 1.0f, 0.0f);
	cam = new Camera(temp[0], temp[1], temp[2], (float)(CELESTIAL_PI*0.25f), gQ.resolutionX, gQ.resolutionY, 500.0f);

	CardHandler* tempCard = gH->GetCardHandler();
	rH->Init(tempCard, dbgOut,Vector2(gQ.resolutionX,gQ.resolutionY));
	guiH->Init(rH->GetObjectContainer());
	cH->Init(rH->GetObjectContainer());
	gBH->Init(rH->GetObjectContainer());
	iH->Init(hwnd);
	pH->Init(nullptr, cam, iH);

	GUILayout* tempLayout = (GUILayout*)(rH->GetObjectContainer()->GetValue(rH->GetMessages()->PopMessage()->param1));
	tempLayout->SetPosition(Vector2(0, 0));
	tempLayout->SetSize(Vector2(1, 1));

	tempLayout->AddChild(dbgIn);
	tempLayout->AddChild(dbgOut);
	tempLayout->AddChild(dbgPnl);
	tempLayout->Toggle(true);
	gH->AddLayout(tempLayout);
	okToDraw = true;

	dbgIn->Toggle(false);
	dbgOut->Toggle(true);
	dbgPnl->Toggle(true);
	tempLayout->Toggle(true);

	dbgOut->AddText("Initiating CelestialEngine");

	dbgPnl->AddText("DFPS");
	dbgPnl->AddText("LFPS");

	dbgOut->AddText("Initiating CelestialGraphics");
	res = gH->FullInit(dbgOut, rH->GetObjectContainer());
	okToDraw = true;

	delete[] temp;

	dbgOut->AddText("Engine Loaded!");
	dbgOut->AddText("Loading root script");
	Message mess5 = Message(mess);
	mess5.source = MessageSource_CELSCRIPT;
	mess5.stringParam = "TestRoot.celsrc";
	mess5.mess = ResourceMess_LOADSCRIPT;
	rH->HandleMessage(&mess5);
	rH->Update(0);
	unsigned int scriptId = rH->GetMessages()->PopMessage()->param3;

	if (scriptId != 0)
	{

		firstMessage = Message(mess);
		firstMessage.destination = MessageSource_CELSCRIPT;
		firstMessage.type = MessageType_SCRIPT;
		firstMessage.mess = ScriptMess_RUN;
		firstMessage.param1 = scriptId;
		cH->HandleMessage(&firstMessage);
		//cH->Update(0);

	}
	if(res == S_OK)
	{

/*
		ResourceObject* tempObject = nullptr;
		meshes = 2;
		baseMeshes = new int[meshes];

		dbgOut->AddText("Loading Plane");
		baseMeshes[0] = rH->LoadBasicShape(Resources::Shape_PLANE, nullptr);
		tempObject = rH->GetResourceObject(baseMeshes[0], LightType_NA, BaseLightDesc(), -1);
		tempObject->SetPosition(Vector3(0, -1, 0));
		tempObject->SetScale(Vector3(60, 1, 60));
		scene->AddObject(tempObject);

		dbgOut->AddText("Loading Ball");
		baseMeshes[1] = rH->LoadMeshFromFile("Tennisboll.obj");
		tempObject = rH->GetResourceObject(baseMeshes[1], LightType_NA, BaseLightDesc(), -1);
		tempObject->SetPosition(Vector3(0, 5, 0));
		scene->AddObject(tempObject);

		BaseParticle basePart = BaseParticle();
		basePart.baseSize = Vector3(1, 1, 1);
		basePart.baseVelocity = 1.0f;
		basePart.size = 50;
		basePart.sizeFactor = 1;
		basePart.timeToLive = 2000;
		basePart.velFactor = 1;

		std::cout << "Loading lightparticle";
		rH->LoadParticleEffect(ParticleSystem_Light, basePart);

		BaseLightDesc baseLight = BaseLightDesc();
		baseLight.diff = Vector4(1, 1, 1, 1);
		baseLight.intensity = 150;
		baseLight.size = 3.2f;
		baseLight.spec = Vector4(0, 0, 0, 1);

		tempObject = rH->GetResourceObject(-1, LightType_POINT, baseLight, 0);
		tempObject->SetPosition(Vector3(0, 10, 0));
		scene->AddObject(tempObject);*/

		/*baseLight.diff = Vector4(1, 0, 0, 1);
		baseLight.intensity = 80;
		tempObject = rH->GetResourceObject(-1, LightType_POINT, baseLight, 0);
		tempObject->SetPosition(Vector3(10, 10, 0));
		scene->AddObject(tempObject);

		baseLight.diff = Vector4(0, 0, 1, 1);
		baseLight.intensity = 110;
		tempObject = rH->GetResourceObject(-1, LightType_POINT, baseLight, 0);
		tempObject->SetPosition(Vector3(-10, 11, 0));
		scene->AddObject(tempObject);

		baseLight.diff = Vector4(0, 1, 0, 1);
		baseLight.intensity = 90;
		tempObject = rH->GetResourceObject(-1, LightType_POINT, baseLight, 0);
		tempObject->SetPosition(Vector3(0, 9, 10));
		scene->AddObject(tempObject);

		baseLight.diff = Vector4(1, 1, 0, 1);
		baseLight.intensity = 125;
		tempObject = rH->GetResourceObject(-1, LightType_POINT, baseLight, 0);
		tempObject->SetPosition(Vector3(0, 12, -10));
		scene->AddObject(tempObject);*/

		/*basePart.size = 1;
		std::cout << "Loading Skybox";
		tempObject = rH->GetResourceObject(-1, LightType_NA, baseLight, rH->LoadParticleEffectFromFile(ParticleSystem_SkyBox, basePart, "SkyMapTest1.png"));
		scene->AddObject(tempObject);*/

	
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


	std::stringstream out;
	out << gFPS;
	dbgPnl->SetText(0, "DFPS: " + out.str());
	out.str("");
	out << lFPS;
	dbgPnl->SetText(1, "LFPS: " + out.str());

	if(gH->GetIsInited())
	{

		iH->Update(time);
		cH->Update(time);
		pH->Update();
		rH->Update(time);
		gBH->Update(time);
		guiH->Update(time);
		gH->Update(time);

		waitForMessages = false;
		handleMessages.notify_all();

	}
}

void Overlord::UpdateMessages(unsigned int time)
{

	while (!die && (waitForMessages))
	{

		//handleMessages.wait(unique_lock<mutex>());
		std::this_thread::yield();
	}

	for (int i = 0; i < MessageSource_NA && !die; i++)
	{

		if (messageHandlers[i] != nullptr)
		{

			Message* currentMessage = messageHandlers[i]->GetMessages()->PopMessage();

			while (currentMessage->type != MessageType_NA)
			{
				
				if (currentMessage->destination != MessageSource_NA &&currentMessage->destination != MessageSource_MASTER && messageHandlers[currentMessage->destination] != nullptr)
				{

					messageHandlers[currentMessage->destination]->HandleMessage(currentMessage);

				}
				else if (currentMessage->destination == MessageSource_MASTER)
				{

					inQueue->PushMessage(currentMessage);

				}

				currentMessage = messageHandlers[i]->GetMessages()->PopMessage();

			}
		}
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
	delete cam;
	delete gH;
	delete pH;
	delete cH;
	delete guiH;
	delete rH;
	isDrawing.unlock();
	
}
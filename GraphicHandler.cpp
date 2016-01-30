#include "StdAfx.h"
#include "GraphicHandler.h"
#include <time.h>
#include <sstream>

#include <stdlib.h>

#include <thread>

using namespace Graphics;
using namespace CrossHandlers;
using namespace Resources;
using namespace CelestialMath;
using namespace std;

GraphicHandler::GraphicHandler(unsigned int flips) : IHandleMessages(200,MessageSource_GRAPHICS)
{

	command = "";
	hasChosenWriting = false;
	isPreInited = false;
	isInited = false;
	canDraw = false;
	isDrawing = false;
	debug = true;
	guiLayouts = new CelestialList<Resources::GUILayout*>();
	cardHandler = new CardHandler(flips, true);
	debugCard = cardHandler;
	renderFlip = 0;
	nextFlip = 0;
	gameBoard = nullptr;
	cameraObject = nullptr;
	filter = MessageType_GRAPHICS;

}

CardHandler* GraphicHandler::GetCardHandler()
{

	return cardHandler;

}

HRESULT GraphicHandler::PreInit(HWND hwnd, GraphicQuality gQ, DrawingStyle dStyle)
{
	
	this->quality = gQ;
	HRESULT hr = cardHandler->Init(hwnd, gQ, dStyle);
	isPreInited = true;
	wf = false;
	return hr;

}

HRESULT GraphicHandler::FullInit(TextContainer* errorOut, CelestialSlicedList<BaseObject*>* gameObjects)
{

	HRESULT hr = cardHandler->InitShader(errorOut);
	isInited = true;
	debugCard->ToggleWireFrame(wf);
	debugCard->ToggleNormalSpikes(false);
	this->gameObjects = gameObjects;
	return hr;

}

void  GraphicHandler::AddLayout(Resources::GUILayout* layout)
{

	canDraw = false;//Pause rendering
	while (isDrawing){ Sleep(0); }//Wait till objects aren't being read
	guiLayouts->AddElement(layout);
	canDraw = true;

}

void GraphicHandler::ClearLayouts()
{

	canDraw = false;//Pause rendering
	while (isDrawing){ Sleep(0); }//Wait till objects aren't being read
	guiLayouts->Reset();
	canDraw = true;

}

void GraphicHandler::UpdateQuality(GraphicQuality gQ)
{

	canDraw = false;//Pause rendering
	while (isDrawing){ Sleep(0); }//Wait till objects aren't being read
	quality = gQ;
	cardHandler->UpdateQuality(gQ);
	canDraw = true;
}

void GraphicHandler::UpdateStyle(DrawingStyle dS)
{

	canDraw = false;//Pause rendering
	while (isDrawing){ Sleep(0); }//Wait till objects aren't being read
	dStyle = dS;
	cardHandler->UpdateStyle(dS);
	canDraw = true;

}

GraphicQuality GraphicHandler::GetQuality()
{

	return quality;

}

DrawingStyle GraphicHandler::GetStyle()
{

	return dStyle;

}

void GraphicHandler::Kill()
{

	isInited = false;//Stop all operations
	canDraw = false;//Stop rendering
	cardHandler->Kill();

}

void GraphicHandler::Update(unsigned int time)
{

	Message* currentMessage = inQueue->PopMessage();

	while (currentMessage->type != MessageType_NA)
	{

		if (currentMessage->mess == GraphicMess_GETSCREEN)
		{

			messageBuffer[this->currentMessage].timeSent = time;
			messageBuffer[this->currentMessage].destination = MessageSource_NA;
			messageBuffer[this->currentMessage].type = MessageType_NA;
			messageBuffer[this->currentMessage].mess = ScriptMess_RESUME;
			messageBuffer[this->currentMessage].param1 = currentMessage->senderId;
			messageBuffer[this->currentMessage].param2 = currentMessage->returnParam;
			messageBuffer[this->currentMessage].param3 = currentMessage->param1 == 0 ? this->quality.resolutionX : this->quality.resolutionY;
			messageBuffer[this->currentMessage].read = false;
			outQueue->PushMessage(&messageBuffer[this->currentMessage]);
			this->currentMessage = (this->currentMessage + 1) % outMessages;

		}
		else if (currentMessage->mess == GraphicMess_UPDATEGAMEBOARDBUFFERS && gameBoard != nullptr)
		{

			canDraw = false;//Pause rendering
			while (isDrawing){ this_thread::yield(); }//Wait until we aren't rendering
			cardHandler->UpdateMeshBuffers(gameBoard->GetDrawingBoard());
			canDraw = true;

		}
		else if (currentMessage->mess == GraphicMess_SETGAMEBOARD)
		{

			canDraw = false;//Pause rendering
			while (isDrawing){ this_thread::yield(); }//Wait until we aren't rendering
			this->gameBoard = (GameBoard*)gameObjects->GetValue(currentMessage->param1);
			cardHandler->UpdateMeshBuffers(gameBoard->GetDrawingBoard());
			canDraw = true;

		}
		else if (currentMessage->mess == GraphicMess_SETCAMERA)
		{

			canDraw = false;//Pause rendering
			while (isDrawing){ this_thread::yield(); }//Wait until we aren't rendering
			this->cameraObject = (CameraObject*)gameObjects->GetValue(currentMessage->param1);
			canDraw = true;

		}
		
		currentMessage->read = true;
		currentMessage = inQueue->PopMessage();

	}

	if (cameraObject != nullptr && gameBoard != nullptr)
	{

		unsigned int newFlip = cameraObject->GetFlip();
		cameraObject->IncrementFlipBuff();

		canDraw = false;//Pause rendering
		while (isDrawing){ this_thread::yield(); }//Wait until we aren't rendering
		cardHandler->UpdateInstanceBuffers(gameBoard->GetDrawingBoard(), newFlip);
		cardHandler->SetInstanceBuffers(renderFlip);
		nextFlip = newFlip;
		canDraw = true;

		while (cameraObject->GetFlip() == renderFlip && isInited)
		{

			this_thread::yield();

		}
	}
}

unsigned int GraphicHandler::GetRenderFlip() const
{

	return renderFlip;

}

void GraphicHandler::Draw()
{

	if (!canDraw)
	{

		this_thread::yield();
		return;

	}

	isDrawing = true;
	
	int sTime = clock();

	if (canDraw && isInited && !(cameraObject == nullptr || gameBoard == nullptr))
	{

		if (renderFlip != nextFlip)
		{

			renderFlip = nextFlip;

		}

		cardHandler->Draw(cameraObject->GetView(),gameBoard->GetDrawingBoard()->GetMeshes(), renderFlip);//Draw all meshes


	}
	else if (canDraw && isInited)
	{

		cardHandler->JustClear();

	}

	int dTime = clock();

	if (canDraw && isPreInited)
	{

		//Draw uis
		cardHandler->Draw(((CelestialList<Resources::GUIObject*>*)guiLayouts));

	}

	int eTime = clock();

	int tTime = eTime - sTime;
	int tdTime = eTime - dTime;
	int cTime = dTime - sTime;

	cardHandler->Present();//Show what we have drawn
	isDrawing = false;

}

bool GraphicHandler::GetIsInited() const
{

	return isInited;

}

void GraphicHandler::ToggleWireFrame(bool enabled)
{
	
	canDraw = false;
	wf = enabled;
	while(isDrawing){Sleep(0);}//Wait until we aren't rendering
	debugCard->ToggleWireFrame(wf);
	canDraw = true;

}

void GraphicHandler::ToggleGrid(bool enabled)
{

	canDraw = false;//Pause rendering
	while(isDrawing){Sleep(0);}//Wait until we aren't rendering
	debugCard->ToggleGridding(enabled);
	canDraw = true;

}
 
void GraphicHandler::ToggleNSpikes(bool enabled)
{

	canDraw = false;//Pause rendering
	while(isDrawing){Sleep(0);}//Wait until we aren't rendering
	debugCard->ToggleNormalSpikes(enabled);
	canDraw = true;

}

void GraphicHandler::ToggleGlobalBorders(bool val)
{
	
	canDraw = false;//Pause rendering
	while(isDrawing){Sleep(0);}//Wait until we aren't rendering
	debugCard->ToggleGlobalBorders(val);
	canDraw = true;

}

void GraphicHandler::ToggleDebug(bool enabled)
{

	debug = enabled;

}

void GraphicHandler::SetCommandText(std::string text)
{

	command = text;

}

void GraphicHandler::Release()
{
	
	isInited = false;
	canDraw = false;
	while(isReadingObjects > 0 || isDrawing){Sleep(0);}//Wait until objects aren't being read and we aren't rendering
	cardHandler->Release();

}

GraphicHandler::~GraphicHandler(void)
{

	isInited = false;
	while(isReadingObjects > 0 || isDrawing){Sleep(0);}//Wait until objects aren't being read and we aren't rendering
	
	delete guiLayouts;
	delete cardHandler;

}
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
	guiLayout = nullptr;
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

HRESULT GraphicHandler::PreInit(HWND hwnd, GraphicQuality gQ, DrawingStyle dStyle, CelestialSlicedList<BaseObject*>* gameObjects)
{
	
	this->quality = gQ;
	HRESULT hr = cardHandler->Init(hwnd, gQ, dStyle);
	isPreInited = true;
	canDraw = true;
	wf = false;
	this->gameObjects = gameObjects;
	return hr;

}

HRESULT GraphicHandler::FullInit(TextContainer* errorOut)
{

	HRESULT hr = cardHandler->InitShader(errorOut);
	isInited = true;
	debugCard->ToggleWireFrame(wf);
	debugCard->ToggleNormalSpikes(false);
	return hr;

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
			///TODO:REMOVE
			messageBuffer[this->currentMessage].timeSent = time;
			messageBuffer[this->currentMessage].destination = MessageSource_NA;
			messageBuffer[this->currentMessage].type = MessageType_NA;
			messageBuffer[this->currentMessage].mess = ScriptMess_RESUME;
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

			unsigned int param1 = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);
			canDraw = false;//Pause rendering
			while (isDrawing){ this_thread::yield(); }//Wait until we aren't rendering
			this->gameBoard = (GameBoard*)gameObjects->GetValue(param1);
			cardHandler->UpdateMeshBuffers(gameBoard->GetDrawingBoard());
			canDraw = true;

		}
		else if (currentMessage->mess == GraphicMess_SETCAMERA)
		{

			unsigned int param1 = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);
			canDraw = false;//Pause rendering
			while (isDrawing){ this_thread::yield(); }//Wait until we aren't rendering
			this->cameraObject = (CameraObject*)gameObjects->GetValue(param1);
			nextFlip = cameraObject->PeekNextFlip();
			renderFlip = cameraObject->PeekNextFlip();
			canDraw = true;

		}
		else if (currentMessage->mess == GraphicMess_SETUI)
		{

			unsigned int param1 = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);
			canDraw = false;//Pause rendering
			while (isDrawing){ this_thread::yield(); }//Wait until we aren't rendering
			this->guiLayout = (GUILayout*)gameObjects->GetValue(param1);
			canDraw = true;

		}
		else if (currentMessage->mess == GraphicMess_SETBORDERBRUSH)
		{

			unsigned int param1 = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);
			GUIObject* object = (GUIObject*)gameObjects->GetValue(param1);
			Vector3 color;
			memcpy(&color.x, &currentMessage->params[4], 4);
			memcpy(&color.y, &currentMessage->params[8], 4);
			memcpy(&color.z, &currentMessage->params[12], 4);
			cardHandler->SetBorderBrush(object, color);

		}
		else if (currentMessage->mess == GraphicMess_SETCONTENTBRUSH)
		{

			unsigned int param1 = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);
			GUIObject* object = (GUIObject*)gameObjects->GetValue(param1);
			Vector3 color;
			memcpy(&color.x, &currentMessage->params[4], 4);
			memcpy(&color.y, &currentMessage->params[8], 4);
			memcpy(&color.z, &currentMessage->params[12], 4);
			cardHandler->SetContentBrush(object, color);

		}
		
		currentMessage->read = true;
		currentMessage = inQueue->PopMessage();

	}

	if (cameraObject != nullptr && gameBoard != nullptr)
	{

		unsigned int newFlip = cameraObject->GetFlip();
		cardHandler->UpdateInstanceBuffers(gameBoard->GetDrawingBoard(), newFlip);
		nextFlip = newFlip;

		unsigned int waitTimeStart = clock();
		while (cameraObject->PeekNextFlip() == renderFlip && isInited)
		{

			this_thread::yield();

		}

		unsigned int waitTimeEnd = clock();
		unsigned int waitTimeDiff = waitTimeEnd - waitTimeStart;
		cameraObject->IncrementFlipBuff();
		canDraw = true;

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
			cardHandler->SetInstanceBuffers(renderFlip);

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
		cardHandler->Draw(guiLayout);

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
	
	delete cardHandler;

}
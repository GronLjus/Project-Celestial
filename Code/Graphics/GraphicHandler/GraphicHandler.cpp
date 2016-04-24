#include "../../../StdAfx.h"
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
	isDrawing = true;
	debug = true;
	guiLayout = 0;
	cardHandler = new CardHandler(flips, true);
	debugCard = cardHandler;
	renderFlip = 0;
	nextFlip = 0;
	gameBoard = nullptr;
	cameraObject = nullptr;
	filter = MessageType_GRAPHICS;
	stoppedDrawing = false;
	stopDrawing = false;

}

void GraphicHandler::pauseRendering()
{

	stopDrawing = true;

	while (!stoppedDrawing && isDrawing && isInited)
	{

		this_thread::yield();

	}
}

void GraphicHandler::resumeRendering()
{

	stopDrawing = false;

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

HRESULT GraphicHandler::FullInit(TextContainer* errorOut, unsigned int maxInstances)
{

	HRESULT hr = cardHandler->InitShader(errorOut, maxInstances);
	isInited = true;
	debugCard->ToggleWireFrame(wf);
	debugCard->ToggleNormalSpikes(false);
	return hr;

}

void GraphicHandler::UpdateQuality(GraphicQuality gQ)
{

	pauseRendering();
	quality = gQ;
	cardHandler->UpdateQuality(gQ);
	resumeRendering();
}

void GraphicHandler::UpdateStyle(DrawingStyle dS)
{

	pauseRendering();
	dStyle = dS;
	cardHandler->UpdateStyle(dS);
	resumeRendering();

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
	while (isDrawing){
		this_thread::yield();;
	}//Wait until objects aren't being read and we aren't rendering

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

			pauseRendering();
			cardHandler->UpdateMeshBuffers(gameBoard->GetDrawingBoard());
			resumeRendering();

		}
		else if (currentMessage->mess == GraphicMess_SETGAMEBOARD)
		{

			unsigned int param1 = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);

			pauseRendering();
			this->gameBoard = (GameBoard*)gameObjects->GetValue(param1);
			cardHandler->UpdateMeshBuffers(gameBoard->GetDrawingBoard());
			resumeRendering();

		}
		else if (currentMessage->mess == GraphicMess_SETCAMERA)
		{

			unsigned int param1 = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);

			pauseRendering();
			this->cameraObject = (CameraObject*)gameObjects->GetValue(param1);
			nextFlip = cameraObject->PeekNextFlip();
			renderFlip = cameraObject->PeekNextFlip();
			resumeRendering();

		}
		else if (currentMessage->mess == GraphicMess_SETUI)
		{

			unsigned int param1 = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);

			pauseRendering();
			GUILayout* temp = (GUILayout*)gameObjects->GetValue(param1);
			temp->Enable();

			if (guiLayout != nullptr)
			{
				guiLayout->Disable();

			}

			this->guiLayout = temp;
			guiLayout->Enable();
			resumeRendering();

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
		pauseRendering();
		cardHandler->UpdateInstanceBuffers(gameBoard->GetDrawingBoard(), newFlip);
		nextFlip = newFlip;
		cameraObject->IncrementFlipBuff();
		resumeRendering();
		canDraw = true;

	}
}

unsigned int GraphicHandler::GetRenderFlip() const
{

	return renderFlip;

}

void GraphicHandler::Draw(unsigned int time)
{

	while (stopDrawing && canDraw)
	{

		stoppedDrawing = true;
		this_thread::yield();

	}

	stoppedDrawing = false;

	if (!canDraw)
	{

		isDrawing = false;
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
		cardHandler->Draw(guiLayout, time);

	}

	int eTime = clock();

	int tTime = eTime - sTime;
	int tdTime = eTime - dTime;
	int cTime = dTime - sTime;

	cardHandler->Present();//Show what we have drawn

}

bool GraphicHandler::GetIsInited() const
{

	return isInited;

}

void GraphicHandler::ToggleWireFrame(bool enabled)
{
	
	wf = enabled;
	pauseRendering();
	debugCard->ToggleWireFrame(wf);
	resumeRendering();

}

void GraphicHandler::ToggleGrid(bool enabled)
{

	pauseRendering();
	debugCard->ToggleGridding(enabled);
	resumeRendering();

}
 
void GraphicHandler::ToggleNSpikes(bool enabled)
{

	pauseRendering();
	debugCard->ToggleNormalSpikes(enabled);
	resumeRendering();

}

void GraphicHandler::ToggleGlobalBorders(bool val)
{

	pauseRendering();
	debugCard->ToggleGlobalBorders(val);
	resumeRendering();

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
	while (isDrawing){
		this_thread::yield();;
	}//Wait until objects aren't being read and we aren't rendering
	cardHandler->Release();

}

GraphicHandler::~GraphicHandler(void)
{

	isInited = false;
	canDraw = false;
	while (isDrawing){
		this_thread::yield();;
	}//Wait until objects aren't being read and we aren't rendering
	delete cardHandler;

}
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

GraphicHandler::GraphicHandler(void) : IHandleMessages(200,MessageSource_GRAPHICS)
{

	command = "";
	hasChosenWriting = false;
	isPreInited = false;
	isInited = false;
	canDraw = false;
	isDrawing = false;
	debug = true;
	renderFlips = 2;
	cameras = new CameraFrame*[renderFlips];
	guiLayouts = new CelestialList<Resources::GUILayout*>();

	for (int i = 0; i < renderFlips; i++)
	{

		cameras[i] = nullptr;

	}

	cardHandler = new CardHandler(renderFlips, true);
	debugCard = cardHandler;
	inter = nullptr;
	readingFlip = 1;
	renderFlip = -1;
	graphicScene = new DrawScene();

	filter = MessageType_GRAPHICS;

}

CardHandler* GraphicHandler::GetCardHandler()
{

	return cardHandler;

}

void GraphicHandler::SetCamera(IGraphicCamera* camera)
{

	isInited = false;

	while(isReadingObjects > 0){Sleep(0);}//Pause until rendering is complete

	delete this->camera;
	this->camera = camera;
	isInited = true;

}

HRESULT GraphicHandler::PreInit(HWND hwnd, GraphicQuality gQ, DrawingStyle dStyle)
{
	
	this->quality = gQ;
	HRESULT hr = cardHandler->Init(hwnd, gQ, dStyle);
	isPreInited = true;
	wf = false;
	return hr;

}

HRESULT GraphicHandler::FullInit(IGraphicCamera* camera, TextContainer* errorOut, CelestialSlicedList<BaseObject*>* gameObjects)
{

	this->camera = camera;
	HRESULT hr = cardHandler->InitShader(errorOut);
	inter = cardHandler->GetIntermediator();
	isInited = true;
	debugCard->ToggleWireFrame(wf);
	debugCard->ToggleNormalSpikes(false);
	this->gameObjects = gameObjects;
	return hr;

}

void GraphicHandler::SetScene(Logic::LogicScene* scene)
{

	isInited = false;//Pause rendering while scene gets changed
	canDraw = false;

	while (isReadingObjects > 0 || isDrawing){ Sleep(0); }//Wait until object aren't being read

	logicScene = scene;
	graphicScene->ClearScene();

	CelestialListNode<ResourceObject*>* node = scene->GetObjects()->GetFirstNode();

	while (node != nullptr && node->GetNodeObject() != nullptr)
	{

		ResourceObject* object = node->GetNodeObject();

		if (object->GetLightEmitter() != nullptr)
		{

			graphicScene->AddLight(object->GetLightEmitter());

		}

		CelestialListNode<MeshObject*>* meshNode = graphicScene->GetMeshes()->GetFirstNode();
		bool meshExists = object->GetMesh() == nullptr;

		while (!meshExists && meshNode != nullptr)
		{

			meshExists = meshNode->GetNodeObject() == object->GetMesh();
			meshNode = meshNode->GetNext();

		}

		if (!meshExists)
		{

			graphicScene->AddMesh(object->GetMesh());
			inter->InitMesh(object->GetMesh(),renderFlips);

		}

		CelestialListNode<IParticleEmitter*>* particleNode = graphicScene->GetParticleSystem()->GetFirstNode();
		bool particleExists = object->GetParticleEmitter() == nullptr;

		while (!particleExists && particleNode != nullptr)
		{

			particleExists = particleNode->GetNodeObject() == object->GetParticleEmitter()->GetEmitter();
			particleNode = particleNode->GetNext();

		}

		if (!particleExists)
		{

			graphicScene->AddParticleSystem(object->GetParticleEmitter()->GetEmitter());
			inter->InitParticleSystem(object->GetParticleEmitter()->GetEmitter(), renderFlips);

		}
		
		node = node->GetNext();

	}

	isInited = true;//Start rendering again
	canDraw = true;

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
		else if (currentMessage->mess == GraphicMess_UPDATEGAMEBOARDBUFFERS)
		{

			canDraw = false;//Pause rendering
			while (isDrawing){ this_thread::yield(); }//Wait until we aren't rendering
			//Dostuff
			canDraw = true;

		}
		else if (currentMessage->mess == GraphicMess_SETGAMEBOARD)
		{

			canDraw = false;//Pause rendering
			while (isDrawing){ this_thread::yield(); }//Wait until we aren't rendering
			this->gameBoard = (GameBoard*)gameObjects->GetValue(currentMessage->param1);
			canDraw = true;

		}

		currentMessage->read = true;
		currentMessage = inQueue->PopMessage();

	}

	isReadingObjects = 1;

	if(isInited)
	{
		
		renderCount = 0;

		if(!hasChosenWriting)
		{

			writeFlip = (renderFlip+1)%renderFlips;
			hasChosenWriting = true;

		}

		if(writeFlip == readingFlip)
		{

			isReadingObjects = 0;
			return;

		}

		if (cameras[writeFlip] != nullptr)
		{

			delete cameras[writeFlip];

		}

		camera->Lock();
		cameras[writeFlip] = camera->GetFrame();
		camera->Unlock();

		
		//Check all objects against camera
		CelestialListNode<ResourceObject*>* obj = logicScene->GetObjects()->GetFirstNode();

		while (obj != nullptr && obj->GetNodeObject() != nullptr)
		{

			CelestialListNode<IBounding*>* meshVol = obj->GetNodeObject()->GetBoundingChain(ResourceCode_MESH)->GetFirstNode();
			CelestialListNode<IBounding*>* partVol = obj->GetNodeObject()->GetBoundingChain(ResourceCode_PARTICLES)->GetFirstNode();
			bool meshIn = obj->GetNodeObject()->GetMesh() != nullptr;
			bool partIn = obj->GetNodeObject()->GetParticleEmitter() != nullptr;

			while (meshVol != nullptr && meshIn)
			{

				meshIn = cameras[writeFlip]->CheckFrustum(meshVol->GetNodeObject());
				meshVol = meshVol->GetNext();

			}

			while (partVol != nullptr && partIn)
			{

				partIn = cameras[writeFlip]->CheckFrustum(partVol->GetNodeObject());
				partVol = partVol->GetNext();

			}

			if (meshIn)
			{

				inter->AddInstance(obj->GetNodeObject(), ResourceCode_MESH, writeFlip);

			}

			if (partIn && dStyle.useParticle)
			{

				inter->AddInstance(obj->GetNodeObject(), ResourceCode_PARTICLES, writeFlip);

			}

			obj = obj->GetNext();

		}

		renderFlip = writeFlip;
		hasChosenWriting = false;
		canDraw = true;

	}
	else
	{
		
		isReadingObjects = 0;
		Sleep(0);

	}
	
	isReadingObjects = 0;

}

void GraphicHandler::Draw()
{

	if (!canDraw)
	{

		Sleep(0);
		return;

	}

	isDrawing = true;
	
	int sTime = clock();

	if(canDraw && isInited && renderFlip >= 0)
	{

		int start = clock();
		readingFlip = renderFlip;
		float col = 0.0f;
		cardHandler->SetCamera(cameras[readingFlip]);
		cardHandler->Draw(graphicScene,readingFlip);//Draw all meshes


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
	
	for (int i = 0; i < renderFlips; i++)
	{

		if (cameras[i] != nullptr)
		{

			delete cameras[i];

		}

		cameras[i] = nullptr;

	}

	delete[] cameras;
	delete guiLayouts;
	delete graphicScene;
	delete cardHandler;

}
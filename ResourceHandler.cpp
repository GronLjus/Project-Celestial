#include "stdafx.h"
#include "ResourceHandler.h"
#include "GameBoard.h"
#include "CameraObject.h"
#include "CrossScriptMemoryObject.h"

using namespace Resources;
using namespace CrossHandlers;
using namespace CelestialMath;

ResourceHandler::ResourceHandler(unsigned int bufferFlips) : IHandleMessages(200, MessageSource_RESOURCES)
{

	loader = new ResourceLoader();
	gameObjects = new CelestialSlicedList<BaseObject*>(32, nullptr);
	
	crossScript = gameObjects->Add(new CrossScriptMemoryObject());
	filter = MessageType_RESOURCES;
	this->bufferFlips = bufferFlips;

}

unsigned int ResourceHandler::GetCrossScriptObject() const
{

	return crossScript;

}

///<summary>Initializes The handler and its underobjects</summary>
///<param name='card'>[in]The device to load from</param>
void ResourceHandler::Init(Graphics::CardHandler* &card, TextContainer* outText, Vector2 screen)
{

	loader->Init(card,outText);
	this->screen = screen;

}

CelestialSlicedList<BaseObject*>* ResourceHandler::GetObjectContainer() const
{

	return gameObjects;

}

GameObject* ResourceHandler::loadGameObject(unsigned int param1)
{

	BaseObject* mesh = gameObjects->GetValue(param1);
	BoundingBox* baseBox = nullptr;
	BoundingSphere* baseSphere = nullptr;
	unsigned int meshId = 0;

	if (mesh != nullptr)
	{

		meshId = mesh->GetId();
		MeshObject* meshObj = (MeshObject*)mesh;
		baseBox = (BoundingBox*)meshObj->GetBoundingObjectCopy(Shape_BOX);
		baseSphere = (BoundingSphere*)meshObj->GetBoundingObjectCopy(Shape_SPHERE);

	}

	GameObject* obj = new GameObject(baseBox, baseSphere, meshId);
	obj->SetId(gameObjects->GetFirstEmpty());
	gameObjects->Add(obj);
	return obj;

}

void ResourceHandler::Update(unsigned int time)
{

	Message* currentMessage = inQueue->PopMessage();

	while (currentMessage->type != MessageType_NA)
	{

		unsigned int outId = 0;

		if (currentMessage->mess == ResourceMess_LOADGAMEBOARD)
		{

			unsigned int param1 = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);
			GameBoard* bo = new GameBoard(param1);
			bo->SetId(gameObjects->GetFirstEmpty());
			outId = gameObjects->Add(bo);

			if (currentMessage->params[4] == 1)
			{

				GameObject* obj = loadGameObject(currentMessage->params[5] | ((int)currentMessage->params[6] << 8) | ((int)currentMessage->params[7] << 16) | ((int)currentMessage->params[8] << 24));
				bo->SetBoardObject(obj);

			}
		}
		else if (currentMessage->mess == ResourceMess_LOADOBJECT)
		{

			GameObject* obj = loadGameObject(currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24));
			outId = obj->GetId();

		}
		else if (currentMessage->mess == ResourceMess_LOADSCRIPT)
		{

			std::string stringParam((char*)currentMessage->params);
			CelScriptCompiled* bo = loader->LoadCLScript(stringParam);

			if (bo != nullptr)
			{

				bo->SetId(gameObjects->GetFirstEmpty());
				outId = gameObjects->Add(bo);

			}
		}
		else if (currentMessage->mess == ResourceMess_LOADCAMERA)
		{

			CameraObject* cam = new CameraObject(screen.x,screen.y,500.0f,bufferFlips);

			if (cam != nullptr)
			{

				cam->SetId(gameObjects->GetFirstEmpty());
				outId = gameObjects->Add(cam);

			}
		}
		else if (currentMessage->mess == ResourceMess_LOADLIGHT)
		{

			unsigned int param2 = currentMessage->params[4] | ((int)currentMessage->params[5] << 8) | ((int)currentMessage->params[6] << 16) | ((int)currentMessage->params[7] << 24);
			unsigned int param3 = currentMessage->params[8] | ((int)currentMessage->params[9] << 8) | ((int)currentMessage->params[10] << 16) | ((int)currentMessage->params[11] << 24);
			BaseLightDesc bld;
			bld.diff = CelestialMath::Vector4(((float)(currentMessage->params[0])) / 255.0f, ((float)(currentMessage->params[1] >> 8)) / 255.0f, ((float)(currentMessage->params[2] >> 16)) / 255.0f, ((float)(currentMessage->params[3] >> 24)) / 255.0f);
			bld.intensity = (float)param2;
			bld.size = (float)param3;
			ILight* light = loader->LoadLight(LightType_POINT, bld);
			light->SetId(gameObjects->GetFirstEmpty());
			outId = gameObjects->Add(light);

		}
		else if (currentMessage->mess == ResourceMess_LOADMESH)
		{

			std::string stringParam((char*)(&currentMessage->params[0]));
			BaseObject* bo = loader->LoadMeshFromFile(stringParam);
			bo->SetId(gameObjects->GetFirstEmpty());
			outId = gameObjects->Add(bo);

			messageBuffer[this->currentMessage].timeSent = time;
			messageBuffer[this->currentMessage].destination = MessageSource_ENTITIES;
			messageBuffer[this->currentMessage].type = MessageType_ENTITIES;
			messageBuffer[this->currentMessage].mess = GameBoardMess_ADDMESH;
			unsigned char tempBuff[]{outId >> 0, outId >> 8, outId >> 16, outId >> 24};
			messageBuffer[this->currentMessage].SetParams(tempBuff, 0, 4);
			messageBuffer[this->currentMessage].read = false;
			outQueue->PushMessage(&messageBuffer[this->currentMessage]);
			this->currentMessage = (this->currentMessage + 1) % outMessages;

		}
		else if (currentMessage->mess == ResourceMess_LOADGUI)
		{

			unsigned int param1 = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);
			std::string stringParam((char*)(&currentMessage->params[4]));
			GUIObject* obj = loader->LoadGUIObject(GUIObjects(param1), GUISnap_LEFT, GUISnap_TOP, stringParam);
			obj->ToggleVisibility(true);
			obj->SetId(gameObjects->GetFirstEmpty());
			outId = gameObjects->Add(obj);

		}
		else if (currentMessage->mess == ResourceMess_LOADKEYTRIGGER)
		{

			unsigned int param1 = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);
			unsigned int param2 = currentMessage->params[4] | ((int)currentMessage->params[5] << 8) | ((int)currentMessage->params[6] << 16) | ((int)currentMessage->params[7] << 24);

			KeyTrigger* kT = new KeyTrigger();
			kT->keyCode = param2;
			kT->scriptToRun = param1;
			kT->charTrigg = false;
			kT->SetId(gameObjects->GetFirstEmpty());
			outId = gameObjects->Add(kT);

		}
		else if (currentMessage->mess == ResourceMess_LOADCHARKEYTRIGGER)
		{

			unsigned int param1 = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);
			unsigned int param2 = currentMessage->params[4] | ((int)currentMessage->params[5] << 8) | ((int)currentMessage->params[6] << 16) | ((int)currentMessage->params[7] << 24);

			KeyTrigger* kT = new KeyTrigger();
			kT->keyCode = param2;
			kT->scriptToRun = param1;
			kT->charTrigg = true;
			kT->SetId(gameObjects->GetFirstEmpty());
			outId = gameObjects->Add(kT);

		}

		if (currentMessage->source == MessageSource_CELSCRIPT)
		{

			messageBuffer[this->currentMessage].timeSent = time;			
			messageBuffer[this->currentMessage].destination = MessageSource_CELSCRIPT;
			messageBuffer[this->currentMessage].type = MessageType_SCRIPT;
			messageBuffer[this->currentMessage].mess = ScriptMess_RESUME;
			unsigned char tempBuff[]{currentMessage->senderId >> 0, currentMessage->senderId >> 8, currentMessage->senderId >> 16, currentMessage->senderId >> 24,
				currentMessage->returnParam >> 0, currentMessage->returnParam >> 8, currentMessage->returnParam >> 16, currentMessage->returnParam >> 24,
				outId >> 0, outId >> 8, outId >> 16, outId >> 24
			};
			messageBuffer[this->currentMessage].SetParams(tempBuff, 0, 12);
			messageBuffer[this->currentMessage].read = false;
			outQueue->PushMessage(&messageBuffer[this->currentMessage]);
			this->currentMessage = (this->currentMessage + 1) % outMessages;

		}

		currentMessage->read = true;
		currentMessage = inQueue->PopMessage();

	}
}

ResourceHandler::~ResourceHandler()
{

	delete loader;
	gameObjects->KillList();
	delete gameObjects;

}
#include "stdafx.h"
#include "ResourceHandler.h"
#include "GameBoard.h"
#include "CameraObject.h"

using namespace Resources;
using namespace CrossHandlers;
using namespace CelestialMath;

ResourceHandler::ResourceHandler(unsigned int bufferFlips) : IHandleMessages(200, MessageSource_RESOURCES)
{

	loader = new ResourceLoader();
	gameObjects = new CelestialSlicedList<BaseObject*>(32, nullptr);
	filter = MessageType_RESOURCES;
	this->bufferFlips = bufferFlips;

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

void ResourceHandler::Update(unsigned int time)
{

	Message* currentMessage = inQueue->PopMessage();

	while (currentMessage->type != MessageType_NA)
	{

		unsigned int outId = 0;

		if (currentMessage->mess == ResourceMess_LOADGAMEBOARD)
		{

			GameBoard* bo = new GameBoard(currentMessage->param1);
			bo->SetId(gameObjects->GetFirstEmpty());
			outId = gameObjects->Add(bo);

		}
		else if (currentMessage->mess == ResourceMess_LOADOBJECT)
		{

			BaseObject* mesh = gameObjects->GetValue(currentMessage->param1);
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

			GameObject* obj = new GameObject(baseBox,baseSphere,meshId);
			obj->SetId(gameObjects->GetFirstEmpty());
			outId = gameObjects->Add(obj);

		}
		else if (currentMessage->mess == ResourceMess_LOADSCRIPT)
		{

			CelScriptCompiled* bo = loader->LoadCLScript(currentMessage->stringParam);

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

			BaseLightDesc bld;
			bld.diff = CelestialMath::Vector4(((float)(currentMessage->param1 >> 0)) / 255.0f, ((float)(currentMessage->param1 >> 8)) / 255.0f, ((float)(currentMessage->param1 >> 16)) / 255.0f, ((float)(currentMessage->param1 >> 24)) / 255.0f);
			bld.intensity = (float)currentMessage->param2;
			bld.size = (float)currentMessage->param3;
			ILight* light = loader->LoadLight(LightType_POINT, bld);
			light->SetId(gameObjects->GetFirstEmpty());
			outId = gameObjects->Add(light);

		}
		else if (currentMessage->mess == ResourceMess_LOADMESH)
		{

			BaseObject* bo = loader->LoadMeshFromFile(currentMessage->stringParam);
			bo->SetId(gameObjects->GetFirstEmpty());
			outId = gameObjects->Add(bo);

			messageBuffer[this->currentMessage].timeSent = time;
			messageBuffer[this->currentMessage].destination = MessageSource_ENTITIES;
			messageBuffer[this->currentMessage].type = MessageType_ENTITIES;
			messageBuffer[this->currentMessage].mess = GameBoardMess_ADDMESH;
			messageBuffer[this->currentMessage].param1 = outId;
			messageBuffer[this->currentMessage].read = false;
			outQueue->PushMessage(&messageBuffer[this->currentMessage]);
			this->currentMessage = (this->currentMessage + 1) % outMessages;

		}
		else if (currentMessage->mess == ResourceMess_LOADGUI)
		{

			GUIObject* obj = loader->LoadGUIObject(GUIObjects(currentMessage->param1), GUISnap_LEFT, GUISnap_TOP);
			obj->Toggle(true);
			obj->SetId(gameObjects->GetFirstEmpty());
			outId = gameObjects->Add(obj);

			messageBuffer[this->currentMessage].timeSent = time;
			messageBuffer[this->currentMessage].destination = MessageSource_CELSCRIPT;
			messageBuffer[this->currentMessage].type = MessageType_SCRIPT;
			messageBuffer[this->currentMessage].mess = EventMess_GUIOBJECTADDED;
			messageBuffer[this->currentMessage].param1 = outId;
			messageBuffer[this->currentMessage].read = false;
			outQueue->PushMessage(&messageBuffer[this->currentMessage]);
			this->currentMessage = (this->currentMessage + 1) % outMessages;

		}
		else if (currentMessage->mess == ResourceMess_LOADKEYTRIGGER)
		{

			KeyTrigger* kT = new KeyTrigger();
			kT->keyCode = currentMessage->param2;
			kT->scriptToRun = currentMessage->param1;
			kT->charTrigg = false;
			kT->SetId(gameObjects->GetFirstEmpty());
			outId = gameObjects->Add(kT);

		}
		else if (currentMessage->mess == ResourceMess_LOADCHARKEYTRIGGER)
		{

			KeyTrigger* kT = new KeyTrigger();
			kT->keyCode = currentMessage->param2;
			kT->scriptToRun = currentMessage->param1;
			kT->charTrigg = true;
			kT->SetId(gameObjects->GetFirstEmpty());
			outId = gameObjects->Add(kT);

		}
		else if (currentMessage->mess == ResourceMess_ADDGUITRIGGER)
		{

			GUIObject* obj = (GUIObject*)gameObjects->GetValue(currentMessage->param3);
			obj->SetTrigger(TriggerType(currentMessage->param1), currentMessage->param2+1);

		}
		else if (currentMessage->mess == ResourceMess_SIZEGUI)
		{

			GUIObject* obj = (GUIObject*)gameObjects->GetValue(currentMessage->param1);
			unsigned int pId = obj->GetParentID();
			Vector2 size = Vector2((float)currentMessage->param2, (float)currentMessage->param3);
			Vector2 parentAbSize = screen;
			Vector2 parentSize = Vector2(1.0f, 1.0f);

			while (pId > 0)
			{

				pId--;
				parentSize *= ((GUIObject*)gameObjects->GetValue(pId))->GetSize();
				pId = ((GUIObject*)gameObjects->GetValue(pId))->GetParentID();

			}

			parentAbSize = parentSize*screen;
			obj->SetSize(size / parentAbSize);

		}
		else if (currentMessage->mess == ResourceMess_RESNAPGUI)
		{

			GUIObject* obj = (GUIObject*)gameObjects->GetValue(currentMessage->param1);
			obj->SetHorizontalSnap(GUISnap(currentMessage->param2));
			obj->SetHorizontalSnap(GUISnap(currentMessage->param3));

		}
		else if (currentMessage->mess == ResourceMess_POSGUI)
		{

			GUIObject* obj = (GUIObject*)gameObjects->GetValue(currentMessage->param1);
			unsigned int pId = obj->GetParentID();
			Vector2 pos = Vector2((float)currentMessage->param2, (float)currentMessage->param3);
			Vector2 parentAbSize = screen;
			Vector2 parentSize = Vector2(1.0f, 1.0f);

			while (pId > 0)
			{

				pId--;
				parentSize *= ((GUIObject*)gameObjects->GetValue(pId))->GetSize();
				pId = ((GUIObject*)gameObjects->GetValue(pId))->GetParentID();

			}

			parentAbSize = parentSize*screen;
			obj->SetPosition(pos / parentAbSize);

		}
		else if (currentMessage->mess == ResourceMess_ADDGUITOGUI)
		{

			GUIObject* obj1 = (GUIObject*)gameObjects->GetValue(currentMessage->param1);
			GUIObject* obj2 = (GUIObject*)gameObjects->GetValue(currentMessage->param2);
			obj2->AddChild(obj1);

			if (obj1->IsEnabled() && obj2->IsOnScreen())
			{

				obj1->ToggleOnScreen(true);

				if (obj1->GetTrigger(TriggerType_KEYTRIGGER) > 0)
				{

					KeyTrigger* kt = (KeyTrigger*)gameObjects->GetValue(obj1->GetTrigger(TriggerType_KEYTRIGGER));

					messageBuffer[this->currentMessage].timeSent = time;
					messageBuffer[this->currentMessage].destination = MessageSource_INPUT;
					messageBuffer[this->currentMessage].type = MessageType_INPUT;
					messageBuffer[this->currentMessage].mess = InputMess_TOGGLESCRIPTTRIGGER_KEYPRESS;
					messageBuffer[this->currentMessage].param1 = kt->keyCode | ((int)kt->charTrigg << 16) | ((int)1 << 17);
					messageBuffer[this->currentMessage].param2 = currentMessage->param1;
					messageBuffer[this->currentMessage].param3 = kt->scriptToRun;
					messageBuffer[this->currentMessage].read = false;
					outQueue->PushMessage(&messageBuffer[this->currentMessage]);
					this->currentMessage = (this->currentMessage + 1) % outMessages;
				}
			}
		}

		if (currentMessage->source == MessageSource_CELSCRIPT)
		{

			messageBuffer[this->currentMessage].timeSent = time;			
			messageBuffer[this->currentMessage].destination = MessageSource_CELSCRIPT;
			messageBuffer[this->currentMessage].type = MessageType_SCRIPT;
			messageBuffer[this->currentMessage].mess = ScriptMess_RESUME;
			messageBuffer[this->currentMessage].param1 = currentMessage->senderId;
			messageBuffer[this->currentMessage].param2 = currentMessage->returnParam;
			messageBuffer[this->currentMessage].param3 = outId;
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
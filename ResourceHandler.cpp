#include "stdafx.h"
#include "ResourceHandler.h"

using namespace Resources;
using namespace CrossHandlers;
using namespace CelestialMath;

ResourceHandler::ResourceHandler() : IHandleMessages(200,MessageSource_RESOURCES)
{

	loader = new ResourceLoader();

	meshes = new CelestialSlicedList<MeshObject*>(32,nullptr);
	particleSystems = new CelestialSlicedList<IParticleEmitter*>(32, nullptr);
	guiObjects = new CelestialSlicedList<GUIObject*>(32, nullptr);
	scripts = new CelestialSlicedList<CelScriptCompiled*>(32, nullptr);
	lights = new CelestialSlicedList<ILight*>(32, nullptr);
	objects = new CelestialSlicedList<ResourceObject*>(32, nullptr);
	keyTriggers = new CelestialSlicedList<KeyTrigger*>(32, nullptr);

	filter = MessageType_RESOURCES;

}

///<summary>Initializes The handler and its underobjects</summary>
///<param name='card'>[in]The device to load from</param>
void ResourceHandler::Init(Graphics::CardHandler* &card, TextContainer* outText, Vector2 screen)
{

	loader->Init(card,outText);
	this->screen = screen;

}

CelestialSlicedList<CelScriptCompiled*>* ResourceHandler::GetScriptContainer()
{

	return scripts;

}

CelestialSlicedList<ResourceObject*>* ResourceHandler::GetResourceObjects()
{

	return objects;

}

CelestialSlicedList<GUIObject*>* ResourceHandler::GetGuiObjects()
{

	return guiObjects;

}

void ResourceHandler::Update(unsigned int time)
{

	Message* currentMessage = inQueue->PopMessage();

	while (currentMessage->type != MessageType_NA)
	{

		unsigned int outId = 0;

		if (currentMessage->mess == ResourceMess_ADDOBJ)
		{

			ResourceObject* ro = new ResourceObject();

			if (currentMessage->param1 > 0)
			{

				ro->SetMesh(meshes->GetValue(currentMessage->param1 - 1));

			}

			if (currentMessage->param2 > 0)
			{

				ro->SetLightEmitter(lights->GetValue(currentMessage->param2 - 1));

			}

			if (currentMessage->param3 > 0)
			{

				ro->SetParticleEmitter(particleSystems->GetValue(currentMessage->param3 - 1));

			}

			outId = objects->Add(ro);
			messageBuffer[this->currentMessage].timeSent = time;
			messageBuffer[this->currentMessage].destination = MessageSource_NA;
			messageBuffer[this->currentMessage].type = MessageType_NA;
			messageBuffer[this->currentMessage].mess = EventMess_OBJECTADDED;
			messageBuffer[this->currentMessage].param1 = outId;
			messageBuffer[this->currentMessage].read = false;
			outQueue->PushMessage(&messageBuffer[this->currentMessage]);
			this->currentMessage = (this->currentMessage + 1) % outMessages;

		}
		else if (currentMessage->mess == ResourceMess_LOADSCRIPT)
		{

			outId = scripts->Add(loader->LoadCLScript(currentMessage->stringParam));

			messageBuffer[this->currentMessage].timeSent = time;
			messageBuffer[this->currentMessage].destination = MessageSource_CELSCRIPT;
			messageBuffer[this->currentMessage].type = MessageType_SCRIPT;
			messageBuffer[this->currentMessage].mess = ScriptMess_ADD;
			messageBuffer[this->currentMessage].param1 = outId;
			messageBuffer[this->currentMessage].read = false;
			outQueue->PushMessage(&messageBuffer[this->currentMessage]);
			this->currentMessage = (this->currentMessage + 1) % outMessages;

		}
		else if (currentMessage->mess == ResourceMess_LOADLIGHT)
		{

			BaseLightDesc bld;
			bld.diff = CelestialMath::Vector4(((float)(currentMessage->param1 >> 0)) / 255.0f, ((float)(currentMessage->param1 >> 8)) / 255.0f, ((float)(currentMessage->param1 >> 16)) / 255.0f, ((float)(currentMessage->param1 >> 24)) / 255.0f);
			bld.intensity = (float)currentMessage->param2;
			bld.size = (float)currentMessage->param3;
			ILight* light = loader->LoadLight(LightType_POINT, bld);

			outId = lights->Add(light);

		}
		else if (currentMessage->mess == ResourceMess_LOADMESH)
		{
			
			outId = meshes->Add(loader->LoadMeshFromFile(currentMessage->stringParam));

		}
		else if (currentMessage->mess == ResourceMess_LOADGUI)
		{

			GUIObject* obj = loader->LoadGUIObject(GUIObjects(currentMessage->param1), GUISnap_LEFT, GUISnap_TOP);
			obj->Toggle(true);
			outId = guiObjects->Add(obj);

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
			outId = keyTriggers->Add(kT);

		}
		else if (currentMessage->mess == ResourceMess_LOADCHARKEYTRIGGER)
		{

			KeyTrigger* kT = new KeyTrigger();
			kT->keyCode = currentMessage->param2;
			kT->scriptToRun = currentMessage->param1;
			kT->charTrigg = true;
			outId = keyTriggers->Add(kT);

		}
		else if (currentMessage->mess == ResourceMess_ADDGUITRIGGER)
		{

			GUIObject* obj = guiObjects->GetValue(currentMessage->param3);
			obj->SetTrigger(TriggerType(currentMessage->param1), currentMessage->param2+1);

		}
		else if (currentMessage->mess == ResourceMess_SIZEGUI)
		{

			GUIObject* obj = guiObjects->GetValue(currentMessage->param1);
			unsigned int pId = obj->GetParentID();
			Vector2 size = Vector2((float)currentMessage->param2, (float)currentMessage->param3);
			Vector2 parentAbSize = screen;
			Vector2 parentSize = Vector2(1.0f, 1.0f);

			while (pId > 0)
			{

				pId--;
				parentSize *= guiObjects->GetValue(pId)->GetSize();
				pId = guiObjects->GetValue(pId)->GetParentID();

			}

			parentAbSize = parentSize*screen;
			obj->SetSize(size / parentAbSize);

		}
		else if (currentMessage->mess == ResourceMess_RESNAPGUI)
		{

			GUIObject* obj = guiObjects->GetValue(currentMessage->param1);
			obj->SetHorizontalSnap(GUISnap(currentMessage->param2));
			obj->SetHorizontalSnap(GUISnap(currentMessage->param3));

		}
		else if (currentMessage->mess == ResourceMess_POSGUI)
		{

			GUIObject* obj = guiObjects->GetValue(currentMessage->param1);
			unsigned int pId = obj->GetParentID();
			Vector2 pos = Vector2((float)currentMessage->param2, (float)currentMessage->param3);
			Vector2 parentAbSize = screen;
			Vector2 parentSize = Vector2(1.0f, 1.0f);

			while (pId > 0)
			{

				pId--;
				parentSize *= guiObjects->GetValue(pId)->GetSize();
				pId = guiObjects->GetValue(pId)->GetParentID();

			}

			parentAbSize = parentSize*screen;
			obj->SetPosition(pos / parentAbSize);

		}
		else if (currentMessage->mess == ResourceMess_ADDGUITOGUI)
		{

			GUIObject* obj1 = guiObjects->GetValue(currentMessage->param1);
			GUIObject* obj2 = guiObjects->GetValue(currentMessage->param2);
			obj2->AddChild(obj1);

			if (obj1->IsEnabled() && obj2->IsOnScreen())
			{

				obj1->ToggleOnScreen(true);

				if (obj1->GetTrigger(TriggerType_KEYTRIGGER) > 0)
				{

					KeyTrigger* kt = keyTriggers->GetValue(obj1->GetTrigger(TriggerType_KEYTRIGGER));

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
	meshes->KillList();
	delete meshes;
	particleSystems->KillList();
	delete particleSystems;
	guiObjects->KillList();
	delete guiObjects;
	scripts->KillList();
	delete scripts;
	lights->KillList();
	delete lights;
	objects->KillList();
	delete objects;
	keyTriggers->KillList();
	delete keyTriggers;

}
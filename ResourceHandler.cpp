#include "stdafx.h"
#include "ResourceHandler.h"
#include "CameraObject.h"
#include "GameRouteObject.h"
#include "GameTravelObject.h"
#include "HeapMemContainer.h"
#include "TaskObject.h"

using namespace Resources;
using namespace CrossHandlers;
using namespace CelestialMath;

ResourceHandler::ResourceHandler(unsigned int bufferFlips, unsigned int maxClock) : IHandleMessages(200, MessageSource_RESOURCES)
{

	this->maxClock = maxClock;

	loader = new ResourceLoader();
	gameObjects = new CelestialSlicedList<BaseObject*>(32, nullptr);
	
	filter = MessageType_RESOURCES;
	this->bufferFlips = bufferFlips;

	activeGUI = new CelestialSlicedList<GUIObject*>(32, nullptr);
	currentDic = new ResourceDictionary();

	this->rawCode = 0;

	task.iParams = new CelestialStack<int>(false);
	task.fParams = new CelestialStack<float>(false);
	task.sParams = new CelestialStack<std::string>(false);

}

unsigned int ResourceHandler::GetHeapContainer() const
{

	return heapObject;

}

///<summary>Initializes The handler and its underobjects</summary>
///<param name='card'>[in]The device to load from</param>
void ResourceHandler::Init(Graphics::CardHandler* &card, TextContainer* outText, vectorUI2 screen, unsigned int gameBoardGridCells, unsigned int gameBoardGridSize, unsigned int maxInstances)
{

	this->maxInstances = maxInstances;
	loader->Init(card,outText);
	gameBoardGridMesh = loader->LoadGrid(gameBoardGridCells, gameBoardGridSize);
	gameObjects->Add(nullptr);
	gameObjects->Add(nullptr);
	gameObjects->Add(nullptr);
	((BaseObject*)gameBoardGridMesh)->SetId(gameObjects->Add(gameBoardGridMesh,2));
	this->screen = screen;

}

unsigned int ResourceHandler::AssembleRaws(std::string path)
{

	if (gameObjects->GetValue(0) != nullptr)
	{

		gameObjects->Kill(0);
		gameObjects->Kill(1);

	}

	KubLingRaw* rawCode = loader->CompileFolder(path);
	HeapMemContainer* container = new HeapMemContainer(new Logic::HeapMemory(10240, rawCode->GetHeapVars()));
	container->SetId(0);
	gameObjects->Add(container, 0);
	heapObject = container->GetId();

	rawCode->SetId(gameObjects->Add(rawCode, 1));
	this->rawCode = rawCode->GetId();


	return this->rawCode;

}

CelestialSlicedList<BaseObject*>* ResourceHandler::GetObjectContainer() const
{

	return gameObjects;

}

GameObject* ResourceHandler::loadGameObject(unsigned int param1, GameObjectType type, std::string name)
{

	BaseObject* mesh = gameObjects->GetValue(param1);
	BoundingBox* baseBox = nullptr;
	BoundingSphere* baseSphere = nullptr;
	unsigned int meshId = 0;
	Vector3 baseScale = Vector3(1.0f,1.0f,1.0f);

	if (mesh != nullptr)
	{

		meshId = mesh->GetId();
		CelMesh* meshObj = (CelMesh*)mesh;
		baseBox = new BoundingBox(1, 1, 1, 0, 0, 0);

		//Create a sphere with the radi of sqrt(0.5^2 * 3), since math won't change we can use a magic number
		baseSphere = new BoundingSphere(0, 0, 0, 0.866025);

		baseScale = meshObj->GetScale();

	}

	GameObject* obj;
	
	if (type == GameObjectType_ROUTE) 
	{

		obj = new GameRouteObject(baseBox, baseSphere, meshId, name);

	}
	else if (type == GameObjectType_TRAVELING)
	{

		obj = new GameTravelObject(baseBox, baseSphere, meshId, name);

	}
	else if (type == GameObjectType_GRIDROUTE)
	{

		obj = new GameGridObject(baseBox, baseSphere, meshId, name);

	}
	else if(type == GameObjectType_SCENERY)
	{

		obj = new GameObject(baseBox, baseSphere, meshId, name);

	}

	obj->SetScale(baseScale);
	obj->UpdateMatrix();
	obj->SetId(gameObjects->Add(obj));
	return obj;

}

std::string ResourceHandler::getStringFromBuffer(unsigned char* buffer) const
{

	unsigned int size;
	memcpy(&size, buffer, sizeof(unsigned int));
	char* cBuff = (char*)buffer;

	return std::string(cBuff);

}

void ResourceHandler::handleMess(Message* currentMessage, unsigned int time)
{

	unsigned int outId = 0;

	if (currentMessage->mess == ResourceMess_LOADGAMEBOARD)
	{

		unsigned int param1 = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);
		GameBoard* bo = new GameBoard(param1, gameBoardGridMesh, bufferFlips, maxInstances, maxClock);
		bo->SetId(gameObjects->Add(bo));
		outId = bo->GetId();

		if (currentMessage->params[4] == 1)
		{

			GameObject* obj = loadGameObject(currentMessage->params[5] | ((int)currentMessage->params[6] << 8) | ((int)currentMessage->params[7] << 16) | ((int)currentMessage->params[8] << 24), GameObjectType_SCENERY, "board");
			bo->SetBoardObject(obj);

		}
	}
	else if (currentMessage->mess == ResourceMess_LOADOBJECT)
	{


		GameObject* obj = loadGameObject(currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24), GameObjectType(currentMessage->params[4]), getStringFromBuffer(&currentMessage->params[8]));
		outId = obj->GetId();

	}
	else if (currentMessage->mess == ResourceMess_LOADCOPYOBJECT)
	{

		GameObject* oldObj = (GameObject*)gameObjects->GetValue(currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24));
		outId = copyObject(oldObj,time);

	}
	else if (currentMessage->mess == ResourceMess_LOADCOPYOBJECTAT)
	{

		GameObject* oldObj = (GameObject*)gameObjects->GetValue(currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24));

		GameObject* obj = loadGameObject(oldObj->GetMeshId(), oldObj->GetType(), oldObj->GetObjectName());
		obj->SetNodeGroup(oldObj->GetNodeGroup());

		outId = obj->GetId();

		Vector3 oldScale = oldObj->GetScale();
		Vector3 oldRotation = oldObj->GetRotation();

		unsigned char tempBuff[36];
		memcpy(&tempBuff[0], &currentMessage->params[8], 4);
		memcpy(&tempBuff[4], &currentMessage->params[12], 4);
		memcpy(&tempBuff[8], &currentMessage->params[16], 4);
		memcpy(&tempBuff[12], &oldScale.x, 4);
		memcpy(&tempBuff[16], &oldScale.y, 4);
		memcpy(&tempBuff[20], &currentMessage->params[4], 4);
		memcpy(&tempBuff[24], &oldRotation.x, 4);
		memcpy(&tempBuff[28], &oldRotation.y, 4);
		memcpy(&tempBuff[32], &oldRotation.z, 4);

		Message* msg = mBuffer->GetCurrentMess();

		msg->timeSent = time;
		msg->destination = MessageSource_OBJECT;
		msg->type = MessageType_OBJECT;
		msg->mess = ObjectMess_COPY;
		msg->SetParams(tempBuff, 0, 36);
		msg->read = false;
		obj->Update(msg);

		for (unsigned int i = 0; i < oldObj->GetSubobjects(); i++)
		{

			GameObject* sub = (GameObject*)oldObj->GetSubobject(i);

			if (sub != nullptr)
			{

				unsigned int subOut = copyObject(sub, time);
				GameObject* newSub = (GameObject*)gameObjects->GetValue(subOut);
				obj->AddSubObject(newSub, sub->GetRelativePosition());

				msg = mBuffer->GetCurrentMess();

				msg->timeSent = time;
				msg->destination = MessageSource_ENTITIES;
				msg->type = MessageType_ENTITIES;
				msg->mess = GameBoardMess_ADDOBJECT;
				tempBuff[0] = subOut >> 0;
				tempBuff[1] = subOut >> 8;
				tempBuff[2] = subOut >> 16;
				tempBuff[3] = subOut >> 24;
				msg->SetParams(tempBuff, 0, 4);
				msg->read = false;

				mBuffer->PushMessageOut();

			}
		}

		if (oldObj->GetType() == GameObjectType_ROUTE)
		{

			GameRouteObject* routeOld = (GameRouteObject*)oldObj;
			GameRouteObject* routeNew = (GameRouteObject*)obj;

			routeNew->SetUpperNode(routeOld->GetMiddleNode());
			routeNew->SetLowerNode(routeOld->GetLowerNode());

			routeOld->SetLowerNode(routeOld->GetMiddleNode());
			routeOld->SetMiddleNode(0);

		}
		
		msg = mBuffer->GetCurrentMess();

		msg->timeSent = time;
		msg->destination = MessageSource_ENTITIES;
		msg->type = MessageType_ENTITIES;
		msg->mess = GameBoardMess_ADDOBJECT;
		tempBuff[0] = outId >> 0;
		tempBuff[1] = outId >> 8;
		tempBuff[2] = outId >> 16;
		tempBuff[3] = outId >> 24;
		msg->SetParams(tempBuff, 0, 4);
		msg->read = false;

		mBuffer->PushMessageOut();

	}
	else if (currentMessage->mess == ResourceMess_LOADSCRIPT)
	{

		std::string stringParam((char*)currentMessage->params);
		outId = 0;

		if (rawCode > 0)
		{

			KubLingRaw* raw = (KubLingRaw*)gameObjects->GetValue(rawCode);
			KubLingLabelObj label = raw->GetLabelObj(stringParam);
			KubLingLabel* point = new KubLingLabel(label);

			point->SetId(gameObjects->Add(point));
			currentDic->AddResource(point->GetId(), stringParam);
			outId = point->GetId();

		}
	}
	else if (currentMessage->mess == ResourceMess_LOADCAMERA)
	{

		CameraObject* cam = new CameraObject(screen.x, screen.y, 500.0f, bufferFlips);

		if (cam != nullptr)
		{

			cam->SetId(gameObjects->Add(cam));
			outId = cam->GetId();

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
		light->SetId(gameObjects->Add(light));
		outId = light->GetId();

	}
	else if (currentMessage->mess == ResourceMess_SAVEBOARD)
	{

		unsigned int param1 = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);
		std::string stringParam((char*)(&currentMessage->params[4])); 
		saveGameBoard(stringParam, param1);

	}
	else if (currentMessage->mess == ResourceMess_LOADBOARD)
	{

		unsigned int param1 = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);
		std::string stringParam((char*)(&currentMessage->params[4]));
		loadGameBoard(stringParam, param1, time);

	}
	else if (currentMessage->mess == ResourceMess_LOADMESH)
	{

		std::string stringParam((char*)(&currentMessage->params[0]));
		BaseObject* bo = loader->LoadMeshFromFile(stringParam);

		if (bo != nullptr)
		{

			bo->SetId(gameObjects->Add(bo));
			currentDic->AddResource(bo->GetId(), stringParam);
			outId = bo->GetId();

			Message* msg = mBuffer->GetCurrentMess();

			msg->timeSent = time;
			msg->destination = MessageSource_ENTITIES;
			msg->type = MessageType_ENTITIES;
			msg->mess = GameBoardMess_ADDMESH;
			unsigned char tempBuff[]{ outId >> 0, outId >> 8, outId >> 16, outId >> 24 };
			msg->SetParams(tempBuff, 0, 4);
			msg->read = false;

			mBuffer->PushMessageOut();

		}
	}
	else if (currentMessage->mess == ResourceMess_LOADGUI)
	{

		unsigned int param1 = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);
		std::string stringParam((char*)(&currentMessage->params[4]));
		GUIObject* obj = loader->LoadGUIObject(GUIObjects(param1), GUISnap_LEFT, GUISnap_TOP, stringParam);
		obj->SetActiveId(activeGUI->Add(obj) + 1);
		obj->ToggleVisibility(true);

		obj->SetId(gameObjects->Add(obj));
		outId = obj->GetId();
		ScreenTarget* target = obj->GetScreenTarget();
		target->SetId(gameObjects->Add(target));
		obj->UpdateScreenTarget();

	}
	else if (currentMessage->mess == ResourceMess_LOADKEYTRIGGER)
	{

		unsigned int param1 = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);
		unsigned int param2 = currentMessage->params[4] | ((int)currentMessage->params[5] << 8) | ((int)currentMessage->params[6] << 16) | ((int)currentMessage->params[7] << 24);

		KeyTrigger* kT = new KeyTrigger();
		kT->keyCode = param2;
		kT->scriptToRun = param1;
		kT->charTrigg = false;
		kT->SetId(gameObjects->Add(kT));
		outId = kT->GetId();

	}
	else if (currentMessage->mess == ResourceMess_LOADCHARKEYTRIGGER)
	{

		unsigned int param1 = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);
		unsigned int param2 = currentMessage->params[4] | ((int)currentMessage->params[5] << 8) | ((int)currentMessage->params[6] << 16) | ((int)currentMessage->params[7] << 24);

		KeyTrigger* kT = new KeyTrigger();
		kT->keyCode = param2;
		kT->scriptToRun = param1;
		kT->charTrigg = true;
		kT->SetId(gameObjects->Add(kT));
		outId = kT->GetId();

	}
	else if (currentMessage->mess == ResourceMess_CLEARBOARD)
	{

		unsigned int param1 = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);
		GameBoard* board = (GameBoard*)gameObjects->GetValue(param1);
		CelestialSlicedList<GameObject*>* activeObjects = board->GetActiveObjects();

		for (unsigned int i = 0; i < activeObjects->GetHighest(); i++)
		{

			GameObject* object = activeObjects->GetValue(i);

			if (object != nullptr && object->ShouldSave())
			{

				unloadObject(object->GetId(), time);

			}
		}

		for (unsigned int i = 0; i < activeGUI->GetHighest(); i++)
		{

			GUIObject* object = activeGUI->GetValue(i);

			if (object != nullptr && object->ShouldSave())
			{

				unloadObject(object->GetId(), time);

			}
		}

		board->ClearObjects();

	}
	else if (currentMessage->mess == ResourceMess_UNLOADOBJECT)
	{

		unsigned int param1 = currentMessage->params[0] | ((int)currentMessage->params[1] << 8) | ((int)currentMessage->params[2] << 16) | ((int)currentMessage->params[3] << 24);
		currentDic->RemoveResource(param1); 
		unloadObject(param1, time);

	}
	else if (currentMessage->mess == ResourceMess_ADDTASKPAR)
	{

		int type = 0;
		memcpy(&type, &(currentMessage->params[0]), sizeof(unsigned int));

		if (type == 0)
		{

			int i = 0;
			memcpy(&i, &(currentMessage->params[4]), sizeof(int));
			task.iParams->PushElement(i);

		}
		else if (type == 1)
		{

			float f = 0.0f;
			memcpy(&f, &(currentMessage->params[4]), sizeof(int));
			task.fParams->PushElement(f);

		}
		else
		{

			int i = 0;
			memcpy(&i, &(currentMessage->params[4]), sizeof(int));
			std::string s = std::string((char*)(&currentMessage->params[8]), i);

		}

	}
	else if (currentMessage->mess == ResourceMess_LOADTASK)
	{

		int script = 0;
		memcpy(&script, &(currentMessage->params[0]), sizeof(int));
		TaskObject* obj = new TaskObject(script, task.iParams, task.fParams, task.sParams);
		obj->SetId(gameObjects->Add(obj));

		outId = obj->GetId();

		task.iParams = new CelestialStack<int>(false);
		task.fParams = new CelestialStack<float>(false);
		task.sParams = new CelestialStack<std::string>(false);
		
	}

	if (currentMessage->source == MessageSource_CELSCRIPT && outId > 0 && currentMessage->returnParam > 0)
	{

		Message* msg = mBuffer->GetCurrentMess();

		msg->timeSent = time;
		msg->destination = MessageSource_CELSCRIPT;
		msg->type = MessageType_SCRIPT;
		msg->mess = ScriptMess_RESUME;
		unsigned char tempBuff[]{ currentMessage->senderId >> 0, currentMessage->senderId >> 8, currentMessage->senderId >> 16, currentMessage->senderId >> 24,
			currentMessage->returnParam >> 0, currentMessage->returnParam >> 8, currentMessage->returnParam >> 16, currentMessage->returnParam >> 24,
			outId >> 0, outId >> 8, outId >> 16, outId >> 24,
			currentMessage->returnMess >> 0, currentMessage->returnMess >> 8, currentMessage->returnMess >> 16, currentMessage->returnMess >> 24
		};
		msg->SetParams(tempBuff, 0, 16);
		msg->read = false;

		mBuffer->PushMessageOut();

	}
}

void ResourceHandler::unloadObject(unsigned int param1, unsigned int time)
{

	BaseObject* object = gameObjects->GetValue(param1);

	if (object != nullptr)
	{

		unsigned int activeId = object->GetActiveId();

		if (activeId > 0)
		{

			activeId -= 1;

			if (activeGUI->GetValue(activeId) != nullptr &&
				activeGUI->GetValue(activeId)->GetId() == param1)
			{

				activeGUI->Remove(activeId);

			}
		}

		gameObjects->Remove(param1);

		unsigned int kills = 0;
		Message** killMess = object->GetKillMessage(kills);

		if (killMess != nullptr)
		{

			for (unsigned int i = 0; i < kills; i++)
			{

				if (killMess[i] != nullptr &&
					killMess[i]->destination == MessageSource_RESOURCES)
				{

					handleMess(killMess[i], time);

				}
				else if (killMess[i] != nullptr)
				{

					Message* outMess = new Message(killMess[i]);
					outMess->killWhenDone = true;
					outQueue->PushMessage(outMess);

				}
			}
		}

		delete object;

	}
}

unsigned int ResourceHandler::copyObject(GameObject* objectToCopy, unsigned int time)
{

	GameObject* obj = loadGameObject(objectToCopy->GetMeshId(), objectToCopy->GetType(), objectToCopy->GetObjectName());
	obj->SetNodeGroup(objectToCopy->GetNodeGroup());
	unsigned int retVal = obj->GetId();

	Vector3 oldPos = objectToCopy->GetPosition();
	Vector3 oldScale = objectToCopy->GetScale();
	Vector3 oldRotation = objectToCopy->GetRotation();

	unsigned char tempBuff[36];
	memcpy(&tempBuff[0], &oldPos.x, 4);
	memcpy(&tempBuff[4], &oldPos.y, 4);
	memcpy(&tempBuff[8], &oldPos.z, 4);
	memcpy(&tempBuff[12], &oldScale.x, 4);
	memcpy(&tempBuff[16], &oldScale.y, 4);
	memcpy(&tempBuff[20], &oldScale.z, 4);
	memcpy(&tempBuff[24], &oldRotation.x, 4);
	memcpy(&tempBuff[28], &oldRotation.y, 4);
	memcpy(&tempBuff[32], &oldRotation.z, 4);

	Message* msg = mBuffer->GetCurrentMess();

	msg->timeSent = time;
	msg->destination = MessageSource_OBJECT;
	msg->type = MessageType_OBJECT;
	msg->mess = ObjectMess_COPY;
	msg->SetParams(tempBuff, 0, 36);
	msg->read = false;
	obj->Update(msg);

	return retVal;

}

void ResourceHandler::Update(unsigned int time)
{

	Message* currentMessage = inQueue->PopMessage();

	while (currentMessage->type != MessageType_NA)
	{

		handleMess(currentMessage, time);
		currentMessage->read = true;
		currentMessage = inQueue->PopMessage();

		if (currentMessage->killWhenDone)
		{

			delete currentMessage;

		}
	}
}

void ResourceHandler::saveGameBoard(std::string path, unsigned int gameBoard)
{

	unsigned int startTime = clock();

	Message* msg = mBuffer->GetCurrentMess();

	msg->timeSent = clock();
	msg->destination = MessageSource_SYSTEM;
	msg->type = MessageType_SYSTEM;
	msg->mess = SystemMess_SKIP;
	msg->read = false;

	mBuffer->PushMessageOut();

	GameBoard* board = (GameBoard*)gameObjects->GetValue(gameBoard);

	unsigned int dicSize;
	char* dicData = currentDic->Serialize(dicSize);

	unsigned int boardSize;
	char* boardData = board->Serialize(boardSize);

	unsigned int byteSize = dicSize + boardSize + sizeof(unsigned int) * 2;
	char* data = new char[byteSize];

	unsigned int offset = 0;

	memcpy(&data[0], &dicSize, sizeof(unsigned int));
	offset += sizeof(unsigned int);
	memcpy(&data[offset], dicData, dicSize);
	offset += dicSize;
	memcpy(&data[offset], &boardSize, sizeof(unsigned int));
	offset += sizeof(unsigned int);
	memcpy(&data[offset], boardData, boardSize);

	delete[] dicData;
	delete[] boardData;

	loader->SaveBoardToFile(path, data, byteSize);
	
	delete[] data;

	unsigned int passedTime = clock() - startTime;

	msg = mBuffer->GetCurrentMess();

	memcpy(msg->params, &passedTime, sizeof(passedTime));
	msg->timeSent = clock();
	msg->destination = MessageSource_SYSTEM;
	msg->type = MessageType_SYSTEM;
	msg->mess = SystemMess_SKIP;
	msg->read = false;

	mBuffer->PushMessageOut();

}

unsigned int ResourceHandler::loadGameBoard(std::string path, unsigned int gameBoard, unsigned int time)
{

	unsigned int startTime = clock();
	unsigned int byteSize;
	char* data = loader->LoadSaveFile(path, byteSize);

	if (byteSize <= 8 || data == nullptr)
	{

		return 0;

	}

	unsigned int newDicSize;
	memcpy(&newDicSize, &data[0], sizeof(unsigned int));
	unsigned int offset = sizeof(unsigned int);

	if (data[offset] != SerializableType_RESOURCEDICTIONARY)
	{

		return 0;

	}

	ResourceDictionary* saveDic = new ResourceDictionary();
	saveDic->Unserialize(&data[offset + 1]);
	Dictionary* translatedDict = saveDic->Translate(currentDic);
	offset += newDicSize;

	GameBoard* localBoard = (GameBoard*)gameObjects->GetValue(gameBoard);
	unsigned int boardSize;
	memcpy(&boardSize, &data[offset], sizeof(unsigned int));
	offset += sizeof(unsigned int);
	
	if (data[offset] != SerializableType_GAMEBOARD ||
		localBoard == nullptr ||
		boardSize == 0)
	{

		return 0;

	}

	offset++;
	unsigned int subSize = 0;
	memcpy(&subSize, &data[offset], sizeof(unsigned int));
	unsigned int readBytes = 0;
	offset += sizeof(unsigned int);

	while (subSize > readBytes)
	{

		unsigned int objectSize;
		GameObject* object = loadGameObject(&data[offset], objectSize, translatedDict, localBoard, time);
		offset += objectSize;
		readBytes += objectSize;

	}

	if (data[offset] == SerializableType_ROUTEMANAGER)
	{

		localBoard->Unserialize(&data[offset]);
		localBoard->TranslateScripts(translatedDict);

	}

	delete translatedDict;
	delete saveDic;

	delete[] data;
	unsigned int passedTime = clock() - startTime;

	unsigned char tempBuff[]{ startTime >> 0, startTime >> 8, startTime >> 16, startTime >> 24,
		localBoard->GetId() >> 0, localBoard->GetId() >> 8, localBoard->GetId() >> 16, localBoard->GetId() >> 24
	};

	Message* msg = mBuffer->GetCurrentMess();

	msg->SetParams(tempBuff, 0, 8);
	msg->timeSent = time;
	msg->destination = MessageSource_TASKS;
	msg->type = MessageType_TASKING;
	msg->mess = TaskMess_SETTIME;
	msg->read = false;

	mBuffer->PushMessageOut();

	return localBoard->GetId();
}

GameObject* ResourceHandler::loadGameObject(char* data, unsigned int &readBytes, Dictionary* translatedDictionary, GameBoard* board, unsigned int time)
{

	memcpy(&readBytes, data, sizeof(unsigned int));
	readBytes += sizeof(unsigned int);
	unsigned int offset = sizeof(unsigned int);
	GameObject* returnValue = nullptr;

	if (data[offset] == SerializableType_GAMEOBJECTROUTE)
	{

		returnValue = new GameRouteObject();

	}
	else if (data[offset] == SerializableType_GAMEOBJECTTRAVEL)
	{

		returnValue = new GameTravelObject();

	}
	else if (data[offset] == SerializableType_GAMEOBJECTSCENERY)
	{

		returnValue = new GameObject();

	}

	offset++;
	char* returnData = returnValue->Unserialize(&data[offset]);

	translateMesh(returnValue, translatedDictionary);
	returnValue->SetId(gameObjects->Add(returnValue));
	board->AddObject(returnValue);
	returnValue->TranslateScripts(translatedDictionary);
	returnValue->UpdateMatrix();

	if (returnData != nullptr)
	{

		unsigned int subSize;
		memcpy(&subSize, returnData, sizeof(unsigned int));
		unsigned int subOffset = sizeof(unsigned int);
		subSize += subOffset;

		while (subSize > subOffset)
		{

			Vector3 relPos;

			memcpy(&relPos.x, &returnData[subOffset], sizeof(float));
			subOffset += sizeof(float);
			memcpy(&relPos.y, &returnData[subOffset], sizeof(float));
			subOffset += sizeof(float);
			memcpy(&relPos.z, &returnData[subOffset], sizeof(float));
			subOffset += sizeof(float);
			
			unsigned int localSubSize;
			GameObject* subObject = loadGameObject(&returnData[subOffset], localSubSize, translatedDictionary, board, time);
			returnValue->AddSubObject(subObject, relPos);
			subOffset += localSubSize;
			
		}
	}

	return returnValue;

}

bool ResourceHandler::translateMesh(GameObject* object, Dictionary* translation)
{

	unsigned int translationMesh = translation->GetTranslation(object->GetMeshId());
	
	if (translationMesh == 0)
	{

		return false;

	}

	BaseObject* mesh = gameObjects->GetValue(translationMesh);

	if (mesh == nullptr)
	{

		return false;

	}

	CelMesh* meshObj = (CelMesh*)mesh;
	BoundingBox* baseBox = (BoundingBox*)meshObj->GetBoundingObjectCopy(Shape_BOX);
	BoundingSphere* baseSphere = (BoundingSphere*)meshObj->GetBoundingObjectCopy(Shape_SPHERE);

	object->SetMesh(baseBox, baseSphere, translationMesh);
	return true;

}

ResourceHandler::~ResourceHandler()
{

	delete task.fParams;
	delete task.iParams;
	delete task.sParams;

	delete loader;
	gameObjects->KillList();
	delete gameObjects;
	delete activeGUI;
	delete currentDic;

}
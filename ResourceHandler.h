#pragma once
#include "ResourceLoader.h"
#include "IHandleMessages.h"
#include "CelestialSlicedList.h"
#include "ResourceDictionary.h"
#include "Dictionary.h"
#include "GameBoard.h"

namespace Resources
{

	class ResourceHandler : public CrossHandlers::IHandleMessages
	{

	public: 
		ResourceHandler(unsigned int bufferFlips);
		///<summary>Initializes The handler and its underobjects</summary>
		///<param name='card'>[in]The device to load from</param>
		void Init(Graphics::CardHandler* &card, TextContainer* outText, CelestialMath::vectorUI2 screen,unsigned int , unsigned int gameBoardGridSize, unsigned int maxInstances);
		CrossHandlers::CelestialSlicedList<BaseObject*>* GetObjectContainer() const;
		void Update(unsigned int time);
		unsigned int GetCrossScriptObject() const;
		unsigned int AssembleRaws(std::string path);
		~ResourceHandler();

	private:
		void saveGameBoard(std::string path, unsigned int gameBoard);
		unsigned int loadGameBoard(std::string path, unsigned int gameBoard, unsigned int time);
		GameObject* loadGameObject(char* data, unsigned int &readBytes, CrossHandlers::Dictionary* translatedDictionary, GameBoard* board, unsigned int time);
		bool translateMesh(GameObject* object, CrossHandlers::Dictionary* translation);

		unsigned int copyObject(GameObject* objectToCopy, unsigned int time);
		void handleMess(CrossHandlers::Message* mess,unsigned int time);
		void unloadObject(unsigned int param1, unsigned int time);
		unsigned int crossScript;
		ResourceLoader* loader;
		GameObject* loadGameObject(unsigned int meshId, GameObjectType type);
		CrossHandlers::CelestialSlicedList<BaseObject*>* gameObjects;
		CelestialMath::vectorUI2 screen;
		unsigned int bufferFlips;
		CelMesh* gameBoardGridMesh;
		unsigned int maxInstances;

		CrossHandlers::CelestialSlicedList<GUIObject*>* activeGUI;
		ResourceDictionary* currentDic;

	};
}
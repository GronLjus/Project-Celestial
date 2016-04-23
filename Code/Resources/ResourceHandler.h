#pragma once
#include "ResourceLoader.h"
#include "IHandleMessages.h"
#include "CelestialSlicedList.h"

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
		~ResourceHandler();

	private:
		unsigned int crossScript;
		ResourceLoader* loader;
		GameObject* loadGameObject(unsigned int meshId);
		CrossHandlers::CelestialSlicedList<BaseObject*>* gameObjects;
		CelestialMath::vectorUI2 screen;
		unsigned int bufferFlips;
		MeshObject* gameBoardGridMesh;
		unsigned int maxInstances;

	};
}
#pragma once
#include "ResourceLoader.h"
#include "IHandleMessages.h"
#include "CelestialSlicedList.h"

namespace Resources
{

	class ResourceHandler : public CrossHandlers::IHandleMessages
	{

	public: 
		ResourceHandler();
		///<summary>Initializes The handler and its underobjects</summary>
		///<param name='card'>[in]The device to load from</param>
		void Init(Graphics::CardHandler* &card, TextContainer* outText, CelestialMath::Vector2 screen);
		CrossHandlers::CelestialSlicedList<CelScriptCompiled*>* GetScriptContainer();
		CrossHandlers::CelestialSlicedList<ResourceObject*>* GetResourceObjects();
		CrossHandlers::CelestialSlicedList<GUIObject*>* GetGuiObjects();
		void Update(unsigned int time);
		~ResourceHandler();

	private:
		ResourceLoader* loader;
		CrossHandlers::CelestialSlicedList<MeshObject*>* meshes;
		CrossHandlers::CelestialSlicedList<IParticleEmitter*>* particleSystems;
		CrossHandlers::CelestialSlicedList<GUIObject*>* guiObjects;
		CrossHandlers::CelestialSlicedList<CelScriptCompiled*>* scripts;
		CrossHandlers::CelestialSlicedList<ILight*>* lights;
		CrossHandlers::CelestialSlicedList<ResourceObject*>* objects;
		CrossHandlers::CelestialSlicedList<KeyTrigger*>* keyTriggers;

		CelestialMath::Vector2 screen;

	};
}
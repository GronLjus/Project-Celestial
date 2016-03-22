#pragma once
#include "IHandleMessages.h"
#include "GUIObject.h"
#include "ResourceObject.h"
#include "CelestialSlicedList.h"

namespace Entities
{

	class GUIEntityHandler : public CrossHandlers::IHandleMessages
	{

	public:
		GUIEntityHandler();
		void Init(CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* gameObjects);
		void Update(unsigned int time);
		~GUIEntityHandler();

	private:
		CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* gameObjects;
		Resources::GUIObject* focusedObject;
		Message keyMessage;
		
	};
}
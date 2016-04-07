#pragma once
#include "GameBoard.h"
#include "IHandleMessages.h"
#include "CelestialSlicedList.h"

namespace Entities
{

	class GameBoardHandler : public CrossHandlers::IHandleMessages
	{

		public:
			GameBoardHandler();
			void Init(CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* gameObjects);
			void UpdateMessages(unsigned int time);
			void Update(unsigned int time);
			~GameBoardHandler();

		private:
			CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* gameObjects;
			Resources::GameBoard* localGameBoard;
			void triggerMouseScript(unsigned int script, unsigned int objectId, unsigned int time, int mouseX, int mouseY);
			CelestialMath::Vector3 getMouseWorldLine(unsigned int mouseX, unsigned int mouseY) const;
			Resources::ScriptableObject* getMouseObject(CelestialMath::Vector3 direction) const;
			unsigned int dragScript;
			unsigned int dragId;
			CelestialMath::Vector3 boardPos;

	};
}
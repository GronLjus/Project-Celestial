#pragma once
#include "../../Resources/Objects/GameBoard.h"
#include "../../CrossHandler-Tools/Messaging/IHandleMessages.h"
#include "../../CrossHandler-Tools/Container/CelestialSlicedList.h"

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
			void triggerMouseScript(unsigned int script, unsigned int objectId, unsigned int time, int mouseX, int mouseY, short delta);
			void triggerMouseScript(unsigned int script, unsigned int objectId, unsigned int time, int mouseX, int mouseY, unsigned int dragStatus);

			CelestialMath::Vector3 getMouseWorldLine(unsigned int mouseX, unsigned int mouseY) const;
			Resources::ScriptableObject* getMouseObject(CelestialMath::Vector3 direction) const;
			unsigned int dragScript;
			unsigned int dragId;
			CelestialMath::Vector3 boardPos;
			void sendCommonScriptParams(unsigned int script, unsigned int objectId, unsigned int time, int mouseX, int mouseY);
			void runScript(unsigned int script, unsigned int time);

	};
}
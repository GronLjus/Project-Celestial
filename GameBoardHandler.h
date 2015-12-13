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
			void Update(unsigned int time);
			~GameBoardHandler();

		private:
			CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* gameObjects;
			Resources::GameBoard* localGameBoard;

	};
}
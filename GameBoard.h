#pragma once
#include "ObjectTree.h"
#include "DrawingBoard.h"

namespace Resources
{

	class GameBoard : public BaseObject
	{

		public:
			GameBoard(unsigned int cells);
			void AddObject(GameObject* object);
			Entities::DrawingBoard* GetDrawingBoard() const;
			~GameBoard();

		private:
			CrossHandlers::ObjectTree* objectRoot;
			Entities::DrawingBoard* drawingBoard;

	};
}
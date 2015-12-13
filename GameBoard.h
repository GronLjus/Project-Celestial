#pragma once
#include "ObjectTree.h"
#include "CameraObject.h"

namespace Resources
{

	class GameBoard : public BaseObject
	{

		public:
			GameBoard(unsigned int cells);
			void AddObject(GameObject* object);
			void FillInstanceBuffer();
			void SetCamera(Resources::CameraObject* cam);
			Resources::CameraObject* GetCam() const;
			Entities::DrawingBoard* GetDrawingBoard() const;
			~GameBoard();

		private:
			CrossHandlers::ObjectTree* objectRoot;
			Entities::DrawingBoard* drawingBoard;
			Resources::CameraObject* camera;

	};
}
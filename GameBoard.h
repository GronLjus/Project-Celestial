#pragma once
#include "ObjectTree.h"
#include "CameraObject.h"

namespace Resources
{

	class GameBoard : public BaseObject
	{

		public:
			GameBoard(unsigned int cells); 
			virtual void Update(CrossHandlers::Message* mess){}
			void AddObject(GameObject* object);
			void SetBoardObject(GameObject* object);
			GameObject* GetBoardObject() const;
			void FillInstanceBuffer();
			void SetCamera(Resources::CameraObject* cam);
			Resources::CameraObject* GetCam() const;
			Entities::DrawingBoard* GetDrawingBoard() const;
			unsigned int GetClosestObject(CelestialMath::Vector3 origin, CelestialMath::Vector3 unitDirection, float &smallestDistance) const;
			~GameBoard();

		private:
			CrossHandlers::ObjectTree* objectRoot;
			Entities::DrawingBoard* drawingBoard;
			Resources::CameraObject* camera;
			GameObject* boardObject;
			unsigned int cells;

	};
}
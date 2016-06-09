#pragma once
#include "ObjectTree.h"
#include "CameraObject.h"

namespace Resources
{

	class GameBoard : public ScriptableObject
	{

		public:
			GameBoard(unsigned int cells, MeshObject* GridObject, unsigned char maxFlip, unsigned int maxInstances);
			virtual void Update(CrossHandlers::Message* mess);
			void AddObject(GameObject* object);
			void SetBoardObject(GameObject* object);
			GameObject* GetBoardObject() const;
			void FillInstanceBuffer(GameObject* trackingObj);
			void SetCamera(Resources::CameraObject* cam);
			Resources::CameraObject* GetCam() const;
			Entities::DrawingBoard* GetDrawingBoard() const;
			unsigned int GetClosestObject(CelestialMath::Vector3 origin, CelestialMath::Vector3 unitDirection, float &smallestDistance) const;
			unsigned int GetCollidedObject(GameObject* object) const;
			bool GetBoardPosition(CelestialMath::Vector3 origin, CelestialMath::Vector3 unitDirection, Vector3 &position) const; 
			bool GetBoardPosition(CelestialMath::Vector3 origin, CelestialMath::Vector3 unitDirection, Vector3 &position, float floor) const;

			~GameBoard();

		private:
			CrossHandlers::ObjectTree* objectRoot;
			Entities::DrawingBoard* drawingBoard;
			Resources::CameraObject* camera;
			GameObject* boardObject;
			CelestialMath::Vector3 boardNormal;
			unsigned int cells;

	};
}
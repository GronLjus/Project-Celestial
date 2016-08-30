#pragma once
#include "ObjectTree.h"
#include "CameraObject.h"
#include "RoutingManager.h"

namespace Resources
{

	class GameBoard : public ScriptableObject
	{

		public:
			GameBoard(unsigned int cells, CelMesh* GridObject, unsigned char maxFlip, unsigned int maxInstances);
			virtual void Update(CrossHandlers::Message* mess);
			void AddObject(GameObject* object);
			void ClearObjects();
			CrossHandlers::CelestialSlicedList<GameObject*>* GetActiveObjects() const;
			void SetBoardObject(GameObject* object);
			GameObject* GetBoardObject() const;
			void FillInstanceBuffer(GameObject* trackingObj);
			void SetCamera(Resources::CameraObject* cam);
			Resources::CameraObject* GetCam() const;
			Entities::DrawingBoard* GetDrawingBoard() const;
			unsigned int GetClosestObject(CelestialMath::Vector3 origin, CelestialMath::Vector3 unitDirection, float &smallestDistance) const;
			unsigned int* GetCollidedObject(GameObject* object,unsigned int &amount) const;
			unsigned int* GetCollidedObject(CrossHandlers::BoundingSphere* sphere, GameObjectType filter, unsigned int &amount) const;
			bool GetBoardPosition(CelestialMath::Vector3 origin, CelestialMath::Vector3 unitDirection, Vector3 &position) const; 
			bool GetBoardPosition(CelestialMath::Vector3 origin, CelestialMath::Vector3 unitDirection, Vector3 &position, float floor) const;

			Entities::RoutingManager* GetRoutingManager() const;

			unsigned int AddRouteNode(CelestialMath::Vector3 position, float width, unsigned int* collided, unsigned int &collAmounts);
			unsigned int AddRouteNode(CelestialMath::Vector3 position, float width);

			void ReCalcPaths(unsigned int time);

			//Serializable Interface
			virtual char* Serialize(unsigned int &size);
			virtual char* Unserialize(char* data);

			~GameBoard();

		private:
			Entities::RoutingManager* routing;
			CrossHandlers::ObjectTree* objectRoot;
			CrossHandlers::CelestialSlicedList<GameObject*>* travelObjects;
			CrossHandlers::CelestialSlicedList<GameObject*>* activeObjects;
			unsigned int travelObjectsAmounts;

			Entities::DrawingBoard* drawingBoard;
			Resources::CameraObject* camera;
			GameObject* boardObject;
			CelestialMath::Vector3 boardNormal;
			unsigned int cells;

	};
}
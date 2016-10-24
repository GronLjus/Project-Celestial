#pragma once
#include "CelestialMath.h"
#include "GameBoard.h"
#include "MessageQueue.h"

namespace Entities
{

	class MouseHandler
	{
		private:
			CelestialMath::Vector3 worldMouse;
			CelestialMath::vectorUI2 screenMouse;

			Resources::GameBoard* board;
			Resources::ScriptableObject* hoverObject;

			CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* gameObjects; 
			CrossHandlers::Message* mBuffer;
			unsigned int currentMessage;
			unsigned int outMessages;
			CrossHandlers::MessageQueue* outQueue;

			CelestialMath::Vector3 getMouseWorldLine(CelestialMath::vectorUI2 mouse) const;
			Resources::ScriptableObject* getMouseObject(CelestialMath::Vector3 direction) const;

			unsigned int getClickScript(char button, Resources::ScriptableObject* obj) const;
			unsigned int getDragScript(char button, Resources::ScriptableObject* obj) const;

			void addScriptParamNum(unsigned int script, unsigned int num, unsigned int time);
			void addScriptParamFloat(unsigned int script, float num, unsigned int time);
			void sendCommonScriptParams(unsigned int script, unsigned int objectId, unsigned int time);
			void runScript(unsigned int script, unsigned int time);

			unsigned int dragScript;
			unsigned int dragId;

			bool hoocked;

		public:
			MouseHandler();
			void Init(CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* gameObjects,
				CrossHandlers::Message* mBuffer,
				unsigned int &currentMessage,
				CrossHandlers::MessageQueue* outQueue,
				unsigned int maxMess);

			CelestialMath::Vector3 GetWorldMouse() const;

			void StartDrag(char button, unsigned int time, Resources::PositionableObject* trackedObject);
			void Drag(unsigned int time);
			void StopDrag(unsigned int time);
			void Click(char button, unsigned int time, Resources::PositionableObject* trackedObject);
			void Wheel(unsigned int time, short delta, Resources::PositionableObject* trackedObject);

			void MoveMouse(CelestialMath::vectorUI2 newMouse);

			void SetWorldMouse(CelestialMath::Vector3 pos);
			void SetBoard(Resources::GameBoard* board);

			void Update(unsigned int time);

	};
}
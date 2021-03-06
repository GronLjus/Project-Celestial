#pragma once
#include "CelestialMath.h"
#include "GameBoard.h"
#include "MessageBuffer.h"
#include "GUILayout.h"
#include "CursorCodes.h"

namespace Entities
{

	class MouseHandler
	{
		private:
			CelestialMath::Vector3 worldMouse;
			CelestialMath::vectorUI2 screenMouse;

			Resources::GameBoard* board;
			Resources::ScriptableObject* hoverObject;
			Resources::ScriptableObject* gameObject;

			CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* gameObjects; 
			CrossHandlers::MessageBuffer* mBuffer;

			CelestialMath::Vector3 getMouseWorldLine(CelestialMath::vectorUI2 mouse) const;
			Resources::ScriptableObject* getMouseObject(CelestialMath::Vector3 direction) const;

			unsigned int getKeyScript(Input::CelestialKeyCategories, unsigned key, Resources::ScriptableObject* obj) const;
			unsigned int getClickScript(char button, Resources::ScriptableObject* obj) const;
			unsigned int getWheelScript(Resources::ScriptableObject* obj) const;
			unsigned int getDragScript(char button, Resources::ScriptableObject* obj) const;

			void addScriptParamNum(unsigned int script, unsigned int num, unsigned int time);
			void addScriptParamFloat(unsigned int script, float num, unsigned int time);
			void sendCommonScriptParams(unsigned int script, unsigned int objectId, unsigned int time);
			void runScript(unsigned int script, unsigned int time);

			void setCursor(Logic::CursorCode code, unsigned int time);

			unsigned int dragScript;
			unsigned int dragId;

			bool hoocked;

			Resources::ScreenTarget* MouseHandler::getScreenTarget(CelestialMath::vectorUI2 mouse, Resources::GUILayout* base);

			Resources::GUILayout* screenLayout;
			Resources::ScreenTarget* lastTarget;

		public:
			MouseHandler();
			void Init(CrossHandlers::CelestialSlicedList<Resources::BaseObject*>* gameObjects,
				CrossHandlers::MessageBuffer* mBuffer);

			CelestialMath::Vector3 GetWorldMouse() const;

			void HandleKey(Input::CelestialKeyCategories, unsigned char key, bool up, unsigned int time, Resources::PositionableObject* trackedObject);
			void StartDrag(char button, unsigned int time, Resources::PositionableObject* trackedObject);
			void Drag(unsigned int time);
			void StopDrag(unsigned int time);
			void UpDown(char button, bool up, unsigned int time);
			void Click(char button, unsigned int time, Resources::PositionableObject* trackedObject, bool trackedOccluded);
			void Wheel(unsigned int time, short delta, Resources::PositionableObject* trackedObject);

			void MoveMouse(CelestialMath::vectorUI2 newMouse, unsigned int time);

			void SetWorldMouse(CelestialMath::Vector3 pos);
			void SetBoard(Resources::GameBoard* board);

			void SetLayout(Resources::GUILayout* layout);

			void Update(unsigned int time);

	};
}
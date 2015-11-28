#pragma once
#include "ResourceObject.h"
#include "InputHandler.h"
#include "ILogicCamera.h"
#include "TerrainObject.h"
#include "IPlayerDebug.h"

namespace Logic
{

	///<summary>THis class handles all player interaction and movement</summary>
	class PlayerHandler : public IPlayerDebug
	{
		public:
		
			PlayerHandler();
		
			///<summary>Initilizes the object</summary>
			///<param name='ro'>[in]A pointer to the players object, can be nullptr</param>
			///<param name='cam'>[in]A pointer to the camera object</param>
			///<param name='iH'>[in]A pointer to the inputhandler object</param>
			void Init(Resources::ResourceObject* ro, ILogicCamera* cam, Input::InputHandler* iH);
			///<summary>Updates this handler and let it do it's stuff</summary>
			void Update();

			///<summary>Sets the terrain checked by the player</summary>
			///<param name='terrain'>[in]A pointer to terrainobject to use, can be nullptr to use no terrain</param>
			void SetTerrain(Resources::TerrainObject* terrain);
			virtual void ToggleFreeFlight(bool enabled);
			~PlayerHandler();

		private:
			///<summary>The object that represents the player</summary>
			Resources::ResourceObject* playerObject;
			///<summary>The camera used in the world</summary>
			ILogicCamera* cam;
			///<summary>The inputhandler used</summary>
			Input::InputHandler* iH;
			///<summary>An array containing the movement-speeds</summary>
			float* speeds;
			///<summary>An array containing the dimension-speed-modifers</summary>
			float* mods;
			///<summary>The direction to move in</summary>
			float* dir;
			///<summary>If the player can freefloat</summary>
			bool boundByGround;
			///<summary>A pointer to the terrainobject to get height from</summary>
			Resources::TerrainObject* terrain;
	};
}
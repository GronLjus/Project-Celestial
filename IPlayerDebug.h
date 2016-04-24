#pragma once

namespace Logic
{

	///<summary>This interface allows for debugging of the playerhandler</summary>
	class IPlayerDebug
	{

		public:
			///<summary>Toggles if the player can fly freely</summary>
			///<param name='enabled'>[in]If the player can fly</param>
			virtual void ToggleFreeFlight(bool enabled) = 0;

	};
}
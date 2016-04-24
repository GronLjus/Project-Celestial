#pragma once
#include "../../Resources/Objects/Lights/ILight.h"
#include "../GraphicCodes.h"
#include "../GraphicAPI.h"

namespace Graphics
{

	///<summary>Handles the constants for each light</summary>
	class ShadowMapConstants
	{

	public:
		ShadowMapConstants(ShadowQuality sQ);

		///<summary>Updates the quality of the shadowmap</summary>
		///<param val='sQ'>[in]The quality of the shadowmap</param>
		void UpdateQuality(ShadowQuality sQ);
		///<summary>Gets the viewport used by the light</summary>
		///<param val='light'>[in]The lighttype</param>
		///<returns>An arraf of viewports</returns>
		D3D10_VIEWPORT* GetViewPortsOfLight(Resources::LightType light);
		///<summary>Gets how many shadowmaps-subs this light has</summary>
		///<param val='light'>[in]The lighttype</param>
		///<returns>The amount of shadowmaps-sub associated with this light</returns>
		int GetSubs(Resources::LightType light);
		///<summary>Gets where on the shadowmap the sub-centers are</summary>
		///<param val='light'>[in]The lighttype</param>
		///<returns>An array of coordinates where the start of each sub in the range of [0,1]</returns>
		CelestialMath::Vector2* GetStarts(Resources::LightType light);

		~ShadowMapConstants();

	private:
		///<summary>An array of positions where the shadowmaps-subs are centered</summary>
		CelestialMath::Vector2** starts;
		///<summary>The amount of shadowmap-subs in this light</summary>
		int* subs;
		///<summary>An array of viewports for the shadowmap-subs</summary>
		D3D10_VIEWPORT** viewPorts;

	};
}
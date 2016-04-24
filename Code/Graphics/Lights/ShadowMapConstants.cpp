#include "../../../stdafx.h"
#include "ShadowMapConstants.h"

using namespace Graphics;
using namespace CrossHandlers;
using namespace CelestialMath;
using namespace Resources;

ShadowMapConstants::ShadowMapConstants(ShadowQuality sQ)
{

	subs = new int[LightType_NA];
	starts = new Vector2*[LightType_NA];
	viewPorts = new D3D10_VIEWPORT*[LightType_NA];

	subs[LightType_POINT] = 6;

	for (int i = 0; i < LightType_NA; i++)
	{

		starts[i] = new Vector2[subs[i]];
		viewPorts[i] = new D3D10_VIEWPORT[subs[i]];

	}

	UpdateQuality(sQ);

}

void ShadowMapConstants::UpdateQuality(ShadowQuality sQ)
{

	float yStart = 0;

	//Sets the pointlight
	for(int y=0;y<2;y++)
	{
		
		float xStart = 0;

		for(int x=0;x<3;x++)
		{
		
			starts[LightType_POINT][y * 3 + x] = Vector2(x, y);
			
			viewPorts[LightType_POINT][y * 3 + x].Width = sQ.shadowResolutionX/3;
			viewPorts[LightType_POINT][y * 3 + x].Height = sQ.shadowResolutionY/2;
			viewPorts[LightType_POINT][y * 3 + x].TopLeftX = x*(sQ.shadowResolutionX / 3);
			viewPorts[LightType_POINT][y * 3 + x].TopLeftY = y*(sQ.shadowResolutionY / 2);
			viewPorts[LightType_POINT][y * 3 + x].MinDepth = 0.0f;
			viewPorts[LightType_POINT][y * 3 + x].MaxDepth = 1.0f;

			xStart += (sQ.shadowResolutionX / 3 + 1);

		}

		yStart += sQ.shadowResolutionY / 2 + 1;

	}
}

D3D10_VIEWPORT* ShadowMapConstants::GetViewPortsOfLight(LightType light)
{

	if (light < LightType_NA)
	{

		return viewPorts[light];

	}

	return nullptr;

}

int ShadowMapConstants::GetSubs(LightType light)
{

	if (light < LightType_NA)
	{

		return subs[light];

	}

	return 0;

}

Vector2* ShadowMapConstants::GetStarts(LightType light)
{

	if (light < LightType_NA)
	{

		return starts[light];

	}

	return nullptr;

}

ShadowMapConstants::~ShadowMapConstants()
{

	for (int i = 0; i< LightType_NA; i++)
	{

		delete starts[i];
		delete viewPorts[i];

	}

	delete[] subs;
	delete[] starts;
	delete[] viewPorts;
	
}
#pragma once

namespace Graphics
{

	enum ShadowType{ ShadowType_NONE, ShadowType_VOLUME, ShadowType_MAP, ShadowType_SIZE };
	enum ShadowCode{ ShadowCode_NONE, ShadowCode_VOLUME, ShadowCode_UMBRAVOLUME, ShadowCode_PENUMBRAVOLUME, ShadowCode_SSVOLUME, ShadowCode_MAP, ShadowCode_UMBRAMAP, ShadowCode_PCSS, ShadowCode_SIZE };

	typedef struct ShadowQuality
	{

		UINT shadowResolutionX;
		UINT shadowResolutionY;
		float blockerSamples;
		float shadowSamples;
		ShadowCode shadowCoding;
		ShadowType shadowTyping;

	} ShadowQuality;

	typedef struct GraphicQuality
	{
		ShadowQuality shadows;
		float blurSamples;
		bool fullscreen;
		UINT resolutionX;
		UINT resolutionY;

	} GraphicQuality;


	typedef struct DrawingStyle
	{
		bool useNormalMaps;
		bool useParticle;
		bool motionBlur;
		bool enlighten;
		bool fullscreen;
		bool useSkybox;

	} DrawingStyle;
}
#pragma once
#include "../GraphicCodes.h"

namespace Graphics
{

	enum RT{RT_AMB,RT_DIFF,RT_NORM,RT_VEL,RT_DPTH,RT_SUM};
	///<summary>This interface creates a layer through which we can debug the cardhandler</summary>
	class ICardDebug
	{
	
		public:
			///<summary>Sets what to draw out</summary>
			///<param name='target'>[i]Which rendertarget to draw out</param>
			virtual void SetOut(RT target) = 0;
			///<summary>Toggle wether this should draw wireframes or triangles</summary>
			///<param name='val'>[in]If it should draw wireframes</param>
			virtual void ToggleWireFrame(bool val) = 0;
			///<summary>Toggle wether this should draw lights</summary>
			///<param name='val'>[in]If it should draw lights</param>
			virtual void ToggleGridding(bool enabled) = 0;
			///<summary>Toggle wether the card should apply the motionblureffect</summary>
			///<param name='val'>[in]If it should draw motion blur</param>
			virtual void ToggleNormalSpikes(bool val) = 0;
			///<summary>Toggle whether this should use normalmaps</summary>
			///<param name='val'>[in]If we should use normalmaps</param>
			virtual void ToggleGlobalBorders(bool val) = 0;
			~ICardDebug(){}
	};
}
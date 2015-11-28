#pragma once
#include <string>
#include "GraphicCodes.h"

namespace Graphics
{

	///<summary>The interface creates a layer to debug the graphichandler</summary>
	class IGraphicDebug
	{

		public:
			///<summary>Toggle the drawing of objects as wireframe</summary>
			///<param name='enabled'>[in]If it should be enabled</param>
			virtual void ToggleWireFrame(bool enabled) = 0;
			///<summary>Toggle the drawing of a grid on the bottom</summary>
			///<param name='enabled'>[in]If it should be enabled</param>
			virtual void ToggleGrid(bool enabled) = 0;
			///<summary>Toggle the drawing of debugtext</summary>
			///<param name='enabled'>[in]If it should be enabled</param>
			virtual void ToggleDebug(bool enabled) = 0;
			///<summary>Toggles the drawing of spikes for the normals</summary>
			///<param name='enabled'>[in]If it should be enabled</param>
			virtual void ToggleNSpikes(bool enabled) = 0;
			///<summary>Toggle whether this should draw borders for every object</summary>
			///<param name='val'>[in]If we should use borders</param>
			virtual void ToggleGlobalBorders(bool val) = 0;

			///<summary>Sets the text to draw out as command</summary>
			///<param name='text'>[in]The text to draw</param>
			virtual void SetCommandText(std::string text) = 0;
	};
}
#pragma once
#include "../../CrossHandler-Tools/Camera/CameraFrame.h"
#include "../Lights/ShadowMapConstants.h"
#include "../DrawScene.h"
#include "../Buffer/IBufferHandler.h"
#include "../../CrossHandler-Tools/TextContainer.h"
#include <string>

namespace Graphics
{
	///<summary>The interface to a shader that uses deffered rendering to draw a scene</summary>
	class IShaderHandler
	{

		public:

			///<summary>Initiates the shader</summary>
			///<param name='card'>[in]The device-pointer used to create the shader</param>
			///<param name='gQ'>[in]The variables to use for quality</param>
			///<param val='dS'>[in]What should be drawn</param>
			///<param val='backBuffer'>[in]A pointer to the texture for the backbuffer/</param>
			///<param val='errorOut'>[in]A pointer to a textcontainer to use for debugging</param>
			///<returns>Any eventual error-codes</returns>
			virtual HRESULT Init(ID3D10Device1* card, GraphicQuality gQ,DrawingStyle dS, CrossHandlers::TextureResourceObject* backBuffer,CrossHandlers::TextContainer* errorOut) = 0;

			///<summary>Lets the shader draw objects as wireframes instead of solid triangles</summary>
			///<param name='val'>[in]Should the shader draw wireframes</param>
			///<param name='light'>[in]Should lights and shadows be rendered</param>
			virtual void ToggleWireFrameMode(bool val,bool light) = 0;
			///<summary>Toggle if the shader should draw normalspikes</summary>
			///<param name='val'>[in]Should the shader draw spikes for the normals</param>
			virtual void ToggleNormalSpikes(bool val) = 0;
			///<summary>Toggle if the shader should draw borders for every geometry</summary>
			///<param name='val'>[in]Should the shader draw borders</param>
			virtual void ToggleGlobalBorders(bool val) = 0;

			///<summary>Set what shadowing to use</summary>
			///<param name='gQ'>[in]The variables to use for quality</param>
			///<returns>If the shader supports tho chosen quality</returns>
			virtual bool SetQuality(GraphicQuality gQ, CrossHandlers::TextureResourceObject* backBuffer) = 0;
			///<summary>Set what should be drawn</summary>
			///<param val='dS'>[in]What should be drawn</param>
			///<returns>If the shader supports it</returns>
			virtual bool SetDrawing(DrawingStyle dS) = 0;

			///<summary>Prepares the shader for drawing, must be called before any draws, otherwise strange behavior will occur</summary>
			virtual void StartDrawing() = 0;
			///<summary>Makes the shader draw out a scene</summary>
			///<param val='scene'>[in]A pointer to the scene to draw</param>
			///<param val='flip'>[in]The flip to use when drawing</param>
			virtual void DrawScene(DrawScene* scene, int flip) = 0;
			///<summary>Signals the end of the drawing-period, this method cleans up usages that were used for drawing, may not be necessary for all shaders but should still be called as good measure</summary>
			virtual void FinishDrawing() = 0;

			///<summary>Gets the shaders bufferhandler</summary>
			///<returns>A pointer to the bufferhandler this shader uses</returns>
			virtual IBufferHandler* GetBufferHandler() = 0;
			///<summary>Sets the camera-pointer used by the shader, should only be called when camera is changed and not when it's modified</summary>
			///<param name='cam'>[in]The pointer to the camera that should be used by the shader</param>
			virtual void SetCamera(CrossHandlers::CameraFrame* cam) = 0;

			///<summary>Releases all references used by the shader, must be called when the shaders usage is over</summary>
			virtual void Release() = 0;

			virtual ~IShaderHandler(){}
	};
}
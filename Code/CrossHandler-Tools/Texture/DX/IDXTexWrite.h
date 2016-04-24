#pragma once
#include "../../../Graphics/GraphicAPI.h"

namespace CrossHandlers
{
	///<summary>The interface for writing to dxtextureobjects</summary>
	class IDXTexWrite
	{

		public:
			///<summary>Creates a shaderview from the underlying texture</summary>
			///<param name='desc'>[in]The description to create the shader from</param>
			///<returns>If the function succeded</returns>
			virtual bool CreateShaderView(D3D11_SHADER_RESOURCE_VIEW_DESC* desc) = 0;
			///<summary>Creates a second shaderview from the underlying texture for reading the stencil of a depthbuffer</summary>
			///<param name='desc'>[in]The description to create the shader from</param>
			///<returns>If the function succeded</returns>
			virtual bool CreateSecondShaderView(D3D11_SHADER_RESOURCE_VIEW_DESC* desc) = 0;
			///<summary>Creates a rendertarget from the underlying texture</summary>
			///<param name='desc'>[in]The description to create the rendertarget from</param>
			///<returns>If the function succeded</returns>
			virtual bool CreateTargetView(D3D11_RENDER_TARGET_VIEW_DESC* desc) = 0;
			///<summary>Creates a depthstencil from the underlying texture</summary>
			///<param name='desc'>[in]The description to create the depthstencil from</param>
			///<returns>If the function succeded</returns>
			virtual bool CreateDepthView(D3D11_DEPTH_STENCIL_VIEW_DESC* desc) = 0;
			///<summary>Releases the views and texture used by the object</summary>
			virtual void Release() = 0;
	};
}
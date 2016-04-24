#pragma once
#include "../../../Graphics/GraphicHeader.h"

namespace CrossHandlers
{
	///<summary>The interface for reading from DXTextureobjects</summary>
	class IDXTexRead
	{

		public:
			///<summary>Gets the shaderview this class contains</summary>
			///<returns>A pointer to the shaderresourceview</returns>
			virtual ID3D11ShaderResourceView* GetShaderView() const = 0;
			///<summary>Gets the the second shaderview this class contains</summary>
			///<returns>A pointer to the shaderresourceview</returns>
			virtual ID3D11ShaderResourceView* GetSecondShaderView() const = 0;
			///<summary>Gets the targetview this class contains</summary>
			///<returns>A pointer to the rendertargetview</returns>
			virtual ID3D11RenderTargetView* GetTargetView() const = 0;
			///<summary>Gets the depthview this class contains</summary>
			///<returns>A pointer to the depthstencilview</returns>
			virtual ID3D11DepthStencilView* GetDepthView() const = 0;
			///<summary>Gets the texture this class contains</summary>
			///<returns>A pointer to the texture</returns>
			virtual ID3D11Texture2D* GetTexture() const = 0;

	};
}
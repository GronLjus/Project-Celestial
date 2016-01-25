#pragma once
#include "IDXTexRead.h"
#include "IDXTexWrite.h"

namespace CrossHandlers
{

	///<summary>This class contains a collection of ShaderResourceviews</summary>
	class DXTextureResource : public IDXTexRead, IDXTexWrite
	{

		public:
			///<param name='texture'>[in]A pointer to a texture to use as base</param>
			///<param name='card'>[in]A pointer to the card to use when making views</param>
			DXTextureResource(ID3D11Texture2D* texture,ID3D11Device* card);

			virtual ID3D11ShaderResourceView* GetShaderView() const;
			virtual ID3D11ShaderResourceView* GetSecondShaderView() const;
			virtual ID3D11RenderTargetView* GetTargetView() const;
			virtual ID3D11DepthStencilView* GetDepthView() const;
			virtual ID3D11Texture2D* GetTexture() const;

			virtual bool CreateShaderView(D3D11_SHADER_RESOURCE_VIEW_DESC* desc); 
			virtual bool CreateSecondShaderView(D3D11_SHADER_RESOURCE_VIEW_DESC* desc);
			virtual bool CreateTargetView(D3D11_RENDER_TARGET_VIEW_DESC* desc);
			virtual bool CreateDepthView(D3D11_DEPTH_STENCIL_VIEW_DESC* desc);
			virtual void Release();
			virtual ~DXTextureResource();

		protected:
			///<summary>The shaderview of the texture</summary>
			ID3D11ShaderResourceView* shaderView;
			///<summary>The second shaderview of the texture</summary>
			ID3D11ShaderResourceView* secondShaderView;
			///<summary>The targetview of the texture</summary>
			ID3D11RenderTargetView* targetView;
			///<summary>The depthview of the texture</summary>
			ID3D11DepthStencilView* depthView;
			///<summary>The texture of the texture</summary>
			ID3D11Texture2D* texture;
			///<summary>The device to use when creating views</summary>
			ID3D11Device* card;

	};
}
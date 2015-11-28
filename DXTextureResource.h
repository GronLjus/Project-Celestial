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
			DXTextureResource(ID3D10Texture2D* texture,ID3D10Device1* card);

			virtual ID3D10ShaderResourceView* GetShaderView() const;
			virtual ID3D10ShaderResourceView* GetSecondShaderView() const;
			virtual ID3D10RenderTargetView* GetTargetView() const;
			virtual ID3D10DepthStencilView* GetDepthView() const;
			virtual ID3D10Texture2D* GetTexture() const;

			virtual bool CreateShaderView(D3D10_SHADER_RESOURCE_VIEW_DESC* desc); 
			virtual bool CreateSecondShaderView(D3D10_SHADER_RESOURCE_VIEW_DESC* desc);
			virtual bool CreateTargetView(D3D10_RENDER_TARGET_VIEW_DESC* desc);
			virtual bool CreateDepthView(D3D10_DEPTH_STENCIL_VIEW_DESC* desc);
			virtual void Release();
			virtual ~DXTextureResource();

		protected:
			///<summary>The shaderview of the texture</summary>
			ID3D10ShaderResourceView* shaderView;
			///<summary>The second shaderview of the texture</summary>
			ID3D10ShaderResourceView* secondShaderView;
			///<summary>The targetview of the texture</summary>
			ID3D10RenderTargetView* targetView;
			///<summary>The depthview of the texture</summary>
			ID3D10DepthStencilView* depthView;
			///<summary>The texture of the texture</summary>
			ID3D10Texture2D* texture;
			///<summary>The device to use when creating views</summary>
			ID3D10Device1* card;

	};
}
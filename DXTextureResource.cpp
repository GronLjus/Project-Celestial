#include "stdafx.h"
#include "DXTextureResource.h"

using namespace CrossHandlers;

DXTextureResource::DXTextureResource(ID3D10Texture2D* texture,ID3D10Device1* card)
{

	shaderView = nullptr;
	secondShaderView = nullptr;
	targetView = nullptr;
	depthView = nullptr;
	this->texture = texture;
	this->card = card;

}

ID3D10ShaderResourceView* DXTextureResource::GetShaderView() const
{

	return shaderView;

}

ID3D10ShaderResourceView* DXTextureResource::GetSecondShaderView() const
{

	return secondShaderView;

}

ID3D10DepthStencilView* DXTextureResource::GetDepthView() const
{

	return depthView;

}

ID3D10RenderTargetView* DXTextureResource::GetTargetView() const
{

	return targetView;

}

ID3D10Texture2D* DXTextureResource::GetTexture() const
{

	return texture;

}

bool DXTextureResource::CreateShaderView(D3D10_SHADER_RESOURCE_VIEW_DESC* desc)
{
	
	if(shaderView != nullptr)
	{

		shaderView->Release();

	}

	HRESULT hr = card->CreateShaderResourceView(texture,desc,&shaderView);
	return hr == S_OK;

}

bool DXTextureResource::CreateSecondShaderView(D3D10_SHADER_RESOURCE_VIEW_DESC* desc)
{

	if (secondShaderView != nullptr)
	{

		secondShaderView->Release();

	}

	HRESULT hr = card->CreateShaderResourceView(texture, desc, &secondShaderView);
	return hr == S_OK;

}

bool DXTextureResource::CreateTargetView(D3D10_RENDER_TARGET_VIEW_DESC* desc)
{
	
	if(targetView != nullptr)
	{

		targetView->Release();

	}

	HRESULT hr = card->CreateRenderTargetView(texture,desc,&targetView);
	return hr == S_OK;

}

bool DXTextureResource::CreateDepthView(D3D10_DEPTH_STENCIL_VIEW_DESC* desc)
{
	
	if(depthView != nullptr)
	{

		depthView->Release();

	}

	HRESULT hr = card->CreateDepthStencilView(texture,desc,&depthView);
	return hr == S_OK;

}

void DXTextureResource::Release()
{

	if(shaderView != nullptr)
	{

		shaderView->Release();
		shaderView = nullptr;

	}

	if (secondShaderView != nullptr)
	{

		secondShaderView->Release();
		secondShaderView = nullptr;

	}

	if(targetView != nullptr)
	{

		targetView->Release();
		targetView = nullptr;

	}
	
	if(depthView != nullptr)
	{

		depthView->Release();
		depthView = nullptr;

	}
	
	if(texture != nullptr)
	{

		texture->Release();
		texture = nullptr;

	}
}

DXTextureResource::~DXTextureResource()
{
	
	Release();

}
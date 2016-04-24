#include "../../../../stdafx.h"
#include "DXTextureResourceViews.h"

using namespace CrossHandlers;

DXTextureResourceViews::DXTextureResourceViews(ID3D11Texture2D* texture,ID3D11Device1* card) : DXTextureResource(texture,card)
{

}

void DXTextureResourceViews::Release()
{

	if(shaderView != nullptr)
	{

		shaderView->Release();
		shaderView = nullptr;

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

		texture = nullptr;

	}
}

DXTextureResourceViews::~DXTextureResourceViews()
{
	
	Release();

}
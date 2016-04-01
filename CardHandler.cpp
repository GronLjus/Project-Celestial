#include "StdAfx.h"
#include <dxgidebug.h>
#include "CardHandler.h"
#include "Celestial2DDrawer.h"
#include <thread>

using namespace Graphics;
using namespace CrossHandlers;
using namespace Resources;
using namespace Entities;

std::wstring s2ws(const std::string& s)
{

	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;

}

CardHandler::CardHandler(int flips, bool useText)
{	
	
	drawFlag = true;
	bufferFlag = false;
	killFlag = false;
	underInitiated = false;
	totalFlips = flips;
	currentFrame = nullptr;
	currentFramwView = nullptr;

	commandList = nullptr;
	swapChain = nullptr;
	card = nullptr;
	context1 = nullptr;
	bufferContext = nullptr;

	bH = nullptr;
	inter = nullptr;
	shader = new CelestialShader();
	basic2DHandler = nullptr;

	this->backBuffer = new TextureResourceObject();

	debugText = -1;
	debugBrush = -1;

	if (useText)
	{

		basic2DHandler = new Celestial2DDrawer();

	}

	dtd = 0;
	out = RT_SUM;

}

HRESULT CardHandler::initBackBuffer()
{

	//Sets up the main render target view
	ID3D11Texture2D* backBuffer;
	HRESULT hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);

	if (FAILED(hr))
	{

		return hr;

	}

	this->backBuffer->SetDXT(new DXTextureResource(backBuffer, card));
	this->backBuffer->GetDXT()->CreateTargetView(nullptr);
	this->backBuffer->GetDXT()->CreateShaderView(nullptr);

	return hr;

}

HRESULT CardHandler::releaseBackBuffer()
{
	
	backBuffer->GetDXT()->Release();
	return S_OK;

}

HRESULT CardHandler::Init(HWND hwnd, GraphicQuality gQ, DrawingStyle dS)
{

	HRESULT hr = S_OK;
	quality = gQ;
	dStyle = dS;


	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory( &sd, sizeof(sd) );
	sd.BufferCount = 1;
	sd.BufferDesc.Width = gQ.resolutionX;
	sd.BufferDesc.Height = gQ.resolutionY;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	sd.OutputWindow = hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = true;
	bool stop = false;

	hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,  D3D11_CREATE_DEVICE_BGRA_SUPPORT, nullptr, 0, 
		D3D11_SDK_VERSION, &sd, &swapChain, &card, nullptr, &context1);

	if( FAILED(hr) )
	{
	
		return hr;

	}

	hr = card->CreateDeferredContext(0, &bufferContext);

	if (FAILED(hr))
	{

		return hr;

	}

	swapChain->SetFullscreenState(gQ.fullscreen, nullptr);
	DXGI_MODE_DESC mode;
	mode.Width = quality.resolutionX;
	mode.Height = quality.resolutionY;
	mode.RefreshRate.Numerator = 0;
	mode.RefreshRate.Denominator = 1;
	swapChain->ResizeTarget(&mode);

	if (FAILED(initBackBuffer()))
	{

		return hr;

	}

	wireFrame = false;
	shouldGrid = false;

	if (basic2DHandler != nullptr)
	{

		basic2DHandler->Init(backBuffer, gQ.resolutionX, gQ.resolutionY);
		basic2DHandler->InitFont(15, 0, FontWeight_NORMAL, 1, false, L"Helvetica", debugText);
		basic2DHandler->InitSolidBrush(0.5f, 0.5f, 0.5f, debugBrush);

	}

	return hr;

}

HRESULT CardHandler::InitShader(TextContainer* errorOut)
{

	errorOut->AddTextLine("Creating shaders");
	HRESULT hr = shader->Init(card, quality, dStyle, backBuffer, errorOut);
	if (FAILED(hr)){ return hr; }
	bH = (CelestialBufferHandler*)(shader->GetBufferHandler());
	shader->ToggleWireFrameMode(wireFrame, dStyle.enlighten);
	inter = new Intermediator(nullptr,true,true);
	underInitiated = true;
	return hr;

}

Intermediator* CardHandler::GetIntermediator()
{

	return inter;

}

void CardHandler::Kill()
{
	
	killFlag = true;

}

void CardHandler::UpdateMeshBuffers(DrawingBoard* db)
{

	bH->UpdateMeshBuffers(db, context1);
	ID3D11Buffer* vertices = bH->GetVertexBuffer();
	ID3D11Buffer* indices = bH->GetIndexBuffer();
	unsigned int offset = 0;
	unsigned int vStride = sizeof(BufferVertex);
	context1->IASetVertexBuffers(0, 1, &vertices, &vStride, &offset);//Set buffers
	context1->IASetIndexBuffer(indices, DXGI_FORMAT_R32_UINT, 0);//Set the index buffer


}

void CardHandler::UpdateInstanceBuffers(DrawingBoard* db, unsigned int flip)
{

	bH->UpdateInstanceBuffer(db, bufferContext, flip);

}

void CardHandler::SetInstanceBuffers(unsigned int flip)
{

	ID3D11Buffer* instances = bH->GetInstanceBuffer(flip);
	unsigned int iStride = sizeof(Instance);
	unsigned int offset = 0;
	context1->IASetVertexBuffers(1, 1, &instances, &iStride, &offset);

}

void CardHandler::playCommands()
{

}

void CardHandler::JustClear()
{

	playCommands();
	shader->NothingToDraw(context1);

}

void CardHandler::Draw(Entities::ViewObject* vObj, GraphicalMesh* meshes, unsigned int flip)
{

	if (underInitiated)
	{

		playCommands();
		shader->StartDrawing(vObj, context1,flip);
		shader->DrawScene(vObj, meshes, context1, flip);
		shader->FinishDrawing(context1);

	}
}

void CardHandler::Draw(GUIObject* object, unsigned int time)
{
	
	basic2DHandler->Begin();

	if (!underInitiated)
	{
		
		basic2DHandler->Clear();

	}
	
	if (object != nullptr && basic2DHandler != nullptr)
	{

		basic2DHandler->DrawGUIObject(object, time);

	}

	basic2DHandler->End();

}

void CardHandler::SetBorderBrush(GUIObject* object, Vector3 color)
{

	if (basic2DHandler != nullptr)
	{

		basic2DHandler->SetBorderBrush(object, color);

	}
}

void CardHandler::SetContentBrush(Resources::GUIObject* object, CelestialMath::Vector3 color)
{

	if (basic2DHandler != nullptr)
	{

		basic2DHandler->SetContentBrush(object, color);

	}
}

void CardHandler::Present()
{

	//Flip buffer
	HRESULT hr = swapChain->Present(0,0);
	
}
ImageResourceObject* CardHandler::Load2DImage(unsigned char* values, UINT bPC, UINT channels, UINT width, UINT height)
{

	return basic2DHandler->Load2DImage(values, bPC, channels, width, height);

}

TextureResourceObject* CardHandler::LoadTexture(byte* values, UINT bPC, UINT channels, char* channelOrder, UINT width, UINT height)
{

	D3D11_SUBRESOURCE_DATA data = D3D11_SUBRESOURCE_DATA();
	data.pSysMem = values;
	data.SysMemPitch = width * ((bPC/8) * channels);
	data.SysMemSlicePitch = 0;

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = -1;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	ID3D11Texture2D *pTexture = nullptr;
	HRESULT HR = card->CreateTexture2D(&desc, &data, &pTexture);

	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = desc.Format;
	srDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srDesc.Texture2D.MostDetailedMip = 0;
	srDesc.Texture2D.MipLevels = 1;

	DXTextureResource* dXT = new DXTextureResource(pTexture, card);
	dXT->CreateShaderView(&srDesc);
	TextureResourceObject* retVal = new TextureResourceObject();
	retVal->SetDXT(dXT);

	return retVal;

}

TextureResourceObject* CardHandler::CreateTexture(UCHAR r, UCHAR g, UCHAR b)
{

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = 2;
	desc.Height = 2;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = -1;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;

	UCHAR* pTexels = new UCHAR[desc.Height * desc.Width * 4];

	for (UINT row = 0; row < desc.Height; row++)
	{

		UINT rowStart = row * desc.Width;

		for (UINT col = 0; col < desc.Width; col++)
		{

			UINT colStart = col * 4;
			pTexels[rowStart + colStart + 0] = r; // Red
			pTexels[rowStart + colStart + 1] = g; // Green
			pTexels[rowStart + colStart + 2] = b;  // Blue
			pTexels[rowStart + colStart + 3] = 255;  // Alpha

		}
	}

	D3D11_SUBRESOURCE_DATA data = D3D11_SUBRESOURCE_DATA();
	data.pSysMem = pTexels;
	data.SysMemPitch = desc.Width * 4;
	data.SysMemSlicePitch = 0;

	ID3D11Texture2D *pTexture = nullptr;
	HRESULT HR = card->CreateTexture2D(&desc, &data, &pTexture);

	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = desc.Format;
	srDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srDesc.Texture2D.MostDetailedMip = 0;
	srDesc.Texture2D.MipLevels = 1;

	DXTextureResource* dXT = new DXTextureResource(pTexture, card);
	dXT->CreateShaderView(&srDesc);
	TextureResourceObject* retVal = new TextureResourceObject();
	retVal->SetDXT(dXT);
	delete[] pTexels;
	return retVal;

}

void CardHandler::UpdateQuality(GraphicQuality gQ)
{

	if (gQ.fullscreen != quality.fullscreen)
	{

		swapChain->SetFullscreenState(gQ.fullscreen, nullptr);

	}

	if (gQ.resolutionX != quality.resolutionX || gQ.resolutionY != quality.resolutionY)
	{

		releaseBackBuffer();

		DXGI_MODE_DESC mode;
		mode.Width = quality.resolutionX;
		mode.Height = quality.resolutionY;
		mode.RefreshRate.Numerator = 0;
		mode.RefreshRate.Denominator = 1;

		swapChain->ResizeTarget(&mode);
		swapChain->ResizeBuffers(0, gQ.resolutionX, gQ.resolutionY, DXGI_FORMAT_UNKNOWN, 0);
		initBackBuffer();

	}

	quality = gQ;
	shader->SetQuality(gQ,backBuffer);

}

void CardHandler::UpdateStyle(DrawingStyle dS)
{

	dStyle = dS;
	shader->SetDrawing(dS);

}

void CardHandler::ToggleWireFrame(bool val)
{

	wireFrame = val;
	shader->ToggleWireFrameMode(wireFrame,dStyle.enlighten);

}

void CardHandler::ToggleGridding(bool enabled)
{

	shouldGrid = enabled;

}

void CardHandler::ToggleNormalSpikes(bool val)
{

	shader->ToggleNormalSpikes(val);

}

void CardHandler::ToggleGlobalBorders(bool val)
{

	shader->ToggleGlobalBorders(val);

}

void CardHandler::SetOut(RT target)
{

	this->out = target;

}

void CardHandler::Release()
{


	if(currentFrame != nullptr)
		currentFrame->Release();
	currentFrame = nullptr;

	if( card ) 
	{
		
		context1->ClearState();
		context1->OMSetRenderTargets(0, nullptr, nullptr);
		context1->PSSetSamplers(0, 0, nullptr);
		context1->OMSetBlendState(nullptr, nullptr, 0);
		context1->OMSetDepthStencilState(nullptr, 0);
		context1->RSSetState(nullptr);
		context1->Release();
		bufferContext->Release();

	}

	if (bH != nullptr)
	{

		bH->Release();

	}

	if (basic2DHandler != nullptr)
	{

		basic2DHandler->Release();

	}

	if (commandList != nullptr)
	{

		commandList->Release();

	}

	shader->Release();
	releaseBackBuffer();
	swapChain->Release();
	card->Release();

}

CardHandler::~CardHandler()
{

	delete shader;

	if (basic2DHandler != nullptr)
	{

		delete basic2DHandler;

	}

	if (bH != nullptr)
	{

		delete bH;

	}

	delete backBuffer;
	delete inter;

}
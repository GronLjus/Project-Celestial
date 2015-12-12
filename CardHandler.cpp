#include "StdAfx.h"
#include <dxgidebug.h>
#include "CardHandler.h"
#include "Celestial2DDrawer.h"

using namespace Graphics;
using namespace CrossHandlers;
using namespace Resources;

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
	
	underInitiated = false;
	totalFlips = flips;
	currentFrame = nullptr;
	currentFramwView = nullptr;

	driverType = D3D10_DRIVER_TYPE_NULL;
	swapChain = nullptr;
	card = nullptr;
	camera = nullptr;

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
	ID3D10Texture2D* backBuffer;
	HRESULT hr = swapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&backBuffer);

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

	camera = nullptr;

	//Sets up the device and swapchains
	D3D10_DRIVER_TYPE driverTypes[] = 
	{
		D3D10_DRIVER_TYPE_HARDWARE,
		//D3D10_DRIVER_TYPE_REFERENCE,
	};

	UINT numDriverTypes = sizeof(driverTypes) / sizeof(driverTypes[0]);
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

	for( UINT i = 0; i < numDriverTypes && !stop; i++ )
	{
		driverType = driverTypes[i];
		hr = D3D10CreateDeviceAndSwapChain1(nullptr, driverType, nullptr, D3D10_CREATE_DEVICE_DEBUG | D3D10_CREATE_DEVICE_BGRA_SUPPORT, D3D10_FEATURE_LEVEL_10_1,
			D3D10_1_SDK_VERSION, &sd, &swapChain, &card );
		stop = SUCCEEDED(hr);
	}

	if( FAILED(hr) )
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

	errorOut->AddText("Creating shaders");
	HRESULT hr = shader->Init(card, quality, dStyle, backBuffer, errorOut);
	if (FAILED(hr)){ return hr; }
	bH = (CelestialBufferHandler*)(shader->GetBufferHandler());
	shader->ToggleWireFrameMode(wireFrame, dStyle.enlighten);
	inter = new Intermediator(bH,true,true);
	underInitiated = true;
	return hr;

}

Intermediator* CardHandler::GetIntermediator()
{

	return inter;

}

void CardHandler::SetCamera(CrossHandlers::CameraFrame* cam)
{
	
	camera = cam;
	shader->SetCamera(cam);

}

void CardHandler::UpdateMeshBuffers(Entities::DrawingBoard* db)
{

	bH->UpdateMeshBuffers(db);
	shader->SetVertexBuffers(bH->GetVertexBuffer(), bH->GetIndexBuffer());

}

void CardHandler::Draw(DrawScene* scene,int flip)
{

	if (underInitiated)
	{

		shader->StartDrawing();
		shader->DrawScene(scene, flip);
		shader->FinishDrawing();
	
	}
	
}

void CardHandler::Draw(CelestialList<GUIObject*>* objects)
{

	basic2DHandler->Begin();

	if (!underInitiated)
	{

		basic2DHandler->Clear();

	}

	CelestialListNode<GUIObject*>* object = objects->GetFirstNode();

	while (object != nullptr)
	{

		basic2DHandler->DrawGUIObject(object->GetNodeObject());
		object = object->GetNext();

	}

	basic2DHandler->End();

}

void CardHandler::DrawTextOnScreen(std::string text,float x,float y)
{
	
	if (basic2DHandler != nullptr && debugText >= 0 && debugBrush >= 0)
	{

		bool hr = basic2DHandler->DrawTextToTarget(s2ws(text).c_str(), x, y, debugText, debugBrush);

	}
}

void CardHandler::Present()
{

	//Flip buffer
	HRESULT hr = swapChain->Present(0,0);
	std::string debug = "";
	
}

TextureResourceObject* CardHandler::LoadTexture(byte* values, UINT bPC, UINT channels, char* channelOrder, UINT width, UINT height)
{

	D3D10_TEXTURE2D_DESC desc;
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = -1;
	desc.Usage = D3D10_USAGE_DYNAMIC;
	desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	ID3D10Texture2D *pTexture = nullptr;
	HRESULT HR = card->CreateTexture2D(&desc, nullptr, &pTexture);


	D3D10_MAPPED_TEXTURE2D mappedTex;
	pTexture->Map(D3D10CalcSubresource(0, 0, 1), D3D10_MAP_WRITE_DISCARD, 0, &mappedTex);//Map the underlying data

	UCHAR* pTexels = (UCHAR*)mappedTex.pData;

	for (UINT row = 0; row < desc.Height; row++)
	{
		UINT rowStart = row * mappedTex.RowPitch;
		UINT rowValue = row * width* channels;

		for (UINT col = 0; col < desc.Width; col++)
		{

			UINT colStart = col * 4;
			UINT colValues = col * channels;
			byte red = 0;
			byte green = 0;
			byte blue = 0;
			byte alpha = 255;

			if (bPC == 8)
			{

				for (UINT i = 0; i < channels; i++)
				{

					if (channelOrder[i] == 'r')
					{

						red = values[rowValue + colValues + i];

					}

					if (channelOrder[i] == 'g')
					{

						green = values[rowValue + colValues + i];

					}

					if (channelOrder[i] == 'b')
					{

						blue = values[rowValue + colValues + i];

					}

					if (channelOrder[i] == 'a')
					{

						alpha = values[rowValue + colValues + i];

					}
				}
			}

			pTexels[rowStart + colStart + 0] = red;
			pTexels[rowStart + colStart + 1] = green;
			pTexels[rowStart + colStart + 2] = blue;
			pTexels[rowStart + colStart + 3] = alpha;

		}
	}

	pTexture->Unmap(D3D10CalcSubresource(0, 0, 1));

	D3D10_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = desc.Format;
	srDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
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

	D3D10_TEXTURE2D_DESC desc;
	desc.Width = 2;
	desc.Height = 2;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = -1;
	desc.Usage = D3D10_USAGE_DYNAMIC;
	desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	ID3D10Texture2D *pTexture = nullptr;
	HRESULT HR = card->CreateTexture2D(&desc, nullptr, &pTexture);


	D3D10_MAPPED_TEXTURE2D mappedTex;
	pTexture->Map(D3D10CalcSubresource(0, 0, 1), D3D10_MAP_WRITE_DISCARD, 0, &mappedTex);//Map the underlying data

	UCHAR* pTexels = (UCHAR*)mappedTex.pData;
	for (UINT row = 0; row < desc.Height; row++)
	{
		UINT rowStart = row * mappedTex.RowPitch;
		for (UINT col = 0; col < desc.Width; col++)
		{
			UINT colStart = col * 4;
			pTexels[rowStart + colStart + 0] = r; // Red
			pTexels[rowStart + colStart + 1] = g; // Green
			pTexels[rowStart + colStart + 2] = b;  // Blue
			pTexels[rowStart + colStart + 3] = 255;  // Alpha
		}
	}

	pTexture->Unmap(D3D10CalcSubresource(0, 0, 1));

	D3D10_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = desc.Format;
	srDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	srDesc.Texture2D.MostDetailedMip = 0;
	srDesc.Texture2D.MipLevels = 1;

	DXTextureResource* dXT = new DXTextureResource(pTexture, card);
	dXT->CreateShaderView(&srDesc);
	TextureResourceObject* retVal = new TextureResourceObject();
	retVal->SetDXT(dXT);

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
		
		card->ClearState();
		card->OMSetRenderTargets(0,nullptr,nullptr);

	}

	if (bH != nullptr)
	{

		bH->Release();

	}

	if (basic2DHandler != nullptr)
	{

		basic2DHandler->Release();

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
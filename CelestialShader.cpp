#include "stdafx.h"

#include "CelestialShader.h"
#include "CelestialBufferHandler.h"

#include <D3Dcompiler.h>
#include <time.h>
#include <string>

using namespace Graphics;
using namespace std;
using namespace Resources;
using namespace CrossHandlers;
using namespace Entities;

CelestialShader::CelestialShader()
{

	globalBorders = false;
	bufferSize = 3;
	buffers = new ID3D11Buffer*[bufferSize];
	offSets = new UINT[bufferSize];
	strides = new UINT[bufferSize];

	srvs = new ID3D11ShaderResourceView*[SRVRegisters_SIZE];

	vertexShaders = new ID3D11VertexShader*[VertexShaders_SIZE];
	geometryShaders = new ID3D11GeometryShader*[GeometryShaders_SIZE];
	pixelShaders = new ID3D11PixelShader*[PixelShaders_SIZE];

	rc = renderConstants();
	fc = frameConstants();
	plc = perLightConstants();

	blendStates = new ID3D11BlendState*[BlendCode_SIZE];
	depthStates = new ID3D11DepthStencilState*[DepthCode_SIZE];
	rastStates = new ID3D11RasterizerState*[RastState_SIZE];
	sampleStates = new ID3D11SamplerState*[SampleState_SIZE];

	techs = new ShaderContainer**[Technique_SIZE];

	techs[Technique_PREDRAW] = new ShaderContainer*[PreDrawCode_SIZE];
	techs[Technique_GEOMETRY] = new ShaderContainer*[GeometryCode_SIZE];
	techs[Technique_TERRAIN] = new ShaderContainer*[GeometryCode_SIZE];
	techs[Technique_SHADOW] = new ShaderContainer*[CelestialShader::ShadowCode_SIZE];
	techs[Technique_PARTICLE] = new ShaderContainer*[ParticleCode_SIZE];
	techs[Technique_SCREEN] = new ShaderContainer*[ScreenCode_SIZE];
	techs[Technique_DEBUG] = new ShaderContainer*[DebugCode_SIZE];
	techs[Technique_LIGHT] = new ShaderContainer*[LightCode_SIZE];

	for (int i = 0; i < Technique_SIZE; i++)
	{

		int tSize = 0;

		if (i == Technique_PREDRAW)
		{

			tSize = PreDrawCode_SIZE;

		}
		else if (i == Technique_GEOMETRY)
		{

			tSize = GeometryCode_SIZE;

		}
		else if (i == Technique_TERRAIN)
		{

			tSize = GeometryCode_SIZE;

		}
		else if (i == Technique_SHADOW)
		{

			tSize = CelestialShader::ShadowCode_SIZE;

		}
		else if (i == Technique_PARTICLE)
		{

			tSize = ParticleCode_SIZE;

		}
		else if (i == Technique_SCREEN)
		{

			tSize = ScreenCode_SIZE;

		}
		else if (i == Technique_DEBUG)
		{

			tSize = DebugCode_SIZE;

		}
		else if (i == Technique_LIGHT)
		{

			tSize = LightCode_SIZE;

		}

		for (int j = 0; j < tSize; j++)
		{

			techs[i][j] = nullptr;

		}
	}

	mtrs = new TextureResourceObject*[MRTVal_SIZE];

	for (int i = 0; i < MRTVal_SIZE; i++)
	{

		mtrs[i] = nullptr;

	}

	shadowMap = nullptr;
	shadowInfo = nullptr;
	shadowFactor = nullptr;
	shadowFactor2 = nullptr;
	dpthStnc = nullptr;

}

HRESULT CompileShader(LPCWSTR name, string function, ID3DBlob** out, string target, TextContainer* errorOut)
{

	ID3DBlob* errors = nullptr;

	wstring pth = name;
	string path;
	path.resize(pth.size()); //make enough room in copy for the string 
	copy(pth.begin(), pth.end(), path.begin());
	errorOut->AddTextLine("Compiling: " + function + " in " + path);

	HRESULT hr = D3DCompileFromFile(
		name,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		function.c_str(),
		target.c_str(),
		D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_WARNINGS_ARE_ERRORS,
		0,
		out,
		&errors);
	
	if(FAILED(hr))
	{ 
		
		CHAR* temp = (CHAR*)errors->GetBufferPointer();
		int len = strlen(temp)+1;
		wchar_t *wText = new wchar_t[len];
		MultiByteToWideChar(  CP_ACP, 0,temp, -1, wText,len );
		MessageBox(0,wText,TEXT("Error"),0);
		delete[] wText;

		errorOut->AddTextLine(temp);
	}

	return hr;

}

HRESULT InitVertexShader(ID3DBlob* &vs, string vertexShaderVersion, ID3D11Device* card, ID3D11VertexShader* &shader, LPCWSTR shaderPath, string shaderName, TextContainer* errorOut)
{

	HRESULT hr = S_OK;

	hr = CompileShader(shaderPath, shaderName, &vs, vertexShaderVersion, errorOut);
	if (FAILED(hr)){ return hr; }
	hr = card->CreateVertexShader(vs->GetBufferPointer(), vs->GetBufferSize(), nullptr, &shader);
	return hr;

}

HRESULT InitGeometryShader(ID3DBlob* &vs, string geometryShaderVersion, ID3D11Device* card, ID3D11GeometryShader* &shader, LPCWSTR shaderPath, string shaderName, TextContainer* errorOut)
{

	HRESULT hr = S_OK;

	hr = CompileShader(shaderPath, shaderName, &vs, geometryShaderVersion, errorOut);
	if (FAILED(hr)){ return hr; }
	hr = card->CreateGeometryShader(vs->GetBufferPointer(), vs->GetBufferSize(), nullptr, &shader);
	return hr;

}

HRESULT InitPixelShader(ID3DBlob* &vs, string pixelShaderVersion, ID3D11Device* card, ID3D11PixelShader* &shader, LPCWSTR shaderPath, string shaderName, TextContainer* errorOut)
{

	HRESULT hr = S_OK;

	hr = CompileShader(shaderPath, shaderName, &vs, pixelShaderVersion, errorOut);
	if (FAILED(hr)){ return hr; }
	hr = card->CreatePixelShader(vs->GetBufferPointer(), vs->GetBufferSize(), nullptr, &shader);
	return hr;

}

HRESULT CelestialShader::initShaders(TextContainer* errorOut)
{
	
	HRESULT hr = S_OK;

	//Load shaders
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "EMPTY", 0, DXGI_FORMAT_R32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = 1;

	string vertexShaderVersion = "vs_4_1";
	string geometryShaderVersion = "gs_4_1";
	string pixelShaderVersion = "ps_4_1";


	ID3DBlob* temp = nullptr;

	hr = InitVertexShader(temp, vertexShaderVersion, card, vertexShaders[VertexShaders_VSPREDRAW], TEXT("Shaders_PreDraw.hlsl"), "VSPREDRAW", errorOut);
	if (FAILED(hr)){ return hr; }
	hr = card->CreateInputLayout(layout, numElements, temp->GetBufferPointer(), temp->GetBufferSize(), &screenLayout);
	temp->Release();
	hr = InitVertexShader(temp, vertexShaderVersion, card, vertexShaders[VertexShaders_VS], TEXT("Shaders_Geometry.hlsl"), "VS", errorOut);
	if (FAILED(hr)){ return hr; }
	hr = bH->InitVertexLayout(temp->GetBufferPointer(), temp->GetBufferSize());
	temp->Release();
	hr = InitVertexShader(temp, vertexShaderVersion, card, vertexShaders[VertexShaders_VSTERRAIN], TEXT("Shaders_Geometry.hlsl"), "VSTERRAIN", errorOut);
	if (FAILED(hr)){ return hr; }
	hr = bH->InitTerrainVertexLayout(temp->GetBufferPointer(), temp->GetBufferSize());
	temp->Release();
	hr = InitVertexShader(temp, vertexShaderVersion, card, vertexShaders[VertexShaders_VSSHADOW], TEXT("Shaders_Shadows.hlsl"), "VSSHADOW", errorOut);
	if (FAILED(hr)){ return hr; }
	temp->Release();
	hr = InitVertexShader(temp, vertexShaderVersion, card, vertexShaders[VertexShaders_VSTERRAINSHADOW], TEXT("Shaders_Shadows.hlsl"), "VSTERRAINSHADOW", errorOut);
	if (FAILED(hr)){ return hr; }
	temp->Release();
	hr = InitVertexShader(temp, vertexShaderVersion, card, vertexShaders[VertexShaders_VSLIGHT], TEXT("Shaders_Light.hlsl"), "VSLIGHT", errorOut);
	if (FAILED(hr)){ return hr; }
	temp->Release();
	hr = InitVertexShader(temp, vertexShaderVersion, card, vertexShaders[VertexShaders_VSSCREEN], TEXT("Shaders_Screen.hlsl"), "VSSCREEN", errorOut);
	if (FAILED(hr)){ return hr; }
	temp->Release();
	hr = InitVertexShader(temp, vertexShaderVersion, card, vertexShaders[VertexShaders_VSPARTICLE], TEXT("Shaders_Particle.hlsl"), "VSPARTICLE", errorOut);
	hr = bH->InitParticleVertexLayout(temp->GetBufferPointer(), temp->GetBufferSize());
	if (FAILED(hr)){ return hr; }
	temp->Release();

	hr = InitGeometryShader(temp, geometryShaderVersion, card, geometryShaders[GeometryShaders_GSTriangles], TEXT("Shaders_Geometry.hlsl"), "GSTriangles", errorOut);
	if (FAILED(hr)){ return hr; }
	temp->Release();
	hr = InitGeometryShader(temp, geometryShaderVersion, card, geometryShaders[GeometryShaders_GSLines], TEXT("Shaders_Geometry.hlsl"), "GSLines", errorOut);
	if (FAILED(hr)) { return hr; }
	temp->Release();
	hr = InitGeometryShader(temp, geometryShaderVersion, card, geometryShaders[GeometryShaders_GSTrianglesBorders], TEXT("Shaders_Geometry.hlsl"), "GSTrianglesBorders", errorOut);
	if (FAILED(hr)){ return hr; }
	temp->Release();
	hr = InitGeometryShader(temp, geometryShaderVersion, card, geometryShaders[GeometryShaders_GSSHADOW], TEXT("Shaders_Shadows.hlsl"), "GSSHADOWTriangles", errorOut);
	if (FAILED(hr)){ return hr; }
	temp->Release();
	hr = InitGeometryShader(temp, geometryShaderVersion, card, geometryShaders[GeometryShaders_GSSHADOWVolumes], TEXT("Shaders_Shadows.hlsl"), "GSSHADOWVolumeTriangles", errorOut);
	if (FAILED(hr)){ return hr; }
	temp->Release();
	hr = InitGeometryShader(temp, geometryShaderVersion, card, geometryShaders[GeometryShaders_GSSHADOWUmbraVolume], TEXT("Shaders_Shadows.hlsl"), "GSSHADOWUmbraVolume", errorOut);
	if (FAILED(hr)){ return hr; }
	temp->Release();
	hr = InitGeometryShader(temp, geometryShaderVersion, card, geometryShaders[GeometryShaders_GSSHADOWPenUmbraVolume], TEXT("Shaders_Shadows.hlsl"), "GSSHADOWPenUmbraVolume", errorOut);
	if (FAILED(hr)){ return hr; }
	temp->Release();
	hr = InitGeometryShader(temp, geometryShaderVersion, card, geometryShaders[GeometryShaders_GSSHADOWSSVolume], TEXT("Shaders_Shadows.hlsl"), "GSSHADOWSSVolume", errorOut);
	if (FAILED(hr)){ return hr; }
	temp->Release();
	hr = InitGeometryShader(temp, geometryShaderVersion, card, geometryShaders[GeometryShaders_GSLIGHT], TEXT("Shaders_Light.hlsl"), "GSLIGHT", errorOut);
	if (FAILED(hr)){ return hr; }
	temp->Release();
	hr = InitGeometryShader(temp, geometryShaderVersion, card, geometryShaders[GeometryShaders_GSSCREEN], TEXT("Shaders_Screen.hlsl"), "GSSCREEN", errorOut);
	if (FAILED(hr)){ return hr; }
	temp->Release();
	hr = InitGeometryShader(temp, geometryShaderVersion, card, geometryShaders[GeometryShaders_GSPARTICLE], TEXT("Shaders_Particle.hlsl"), "GSPARTICLETRIANGLE", errorOut);
	if (FAILED(hr)){ return hr; }
	temp->Release();
	hr = InitGeometryShader(temp, geometryShaderVersion, card, geometryShaders[GeometryShaders_GSSKYCUBE], TEXT("Shaders_Particle.hlsl"), "GSSKYCUBE", errorOut);
	if (FAILED(hr)){ return hr; }
	temp->Release();
	hr = InitGeometryShader(temp, geometryShaderVersion, card, geometryShaders[GeometryShaders_GSNORMALSPIKE], TEXT("Shaders_Debug.hlsl"), "GSNORMALSPIKE", errorOut);
	if (FAILED(hr)){ return hr; }
	temp->Release();

	hr = InitPixelShader(temp, pixelShaderVersion, card, pixelShaders[PixelShaders_MAPPER], TEXT("Shaders_Geometry.hlsl"), "MAPPER", errorOut);
	if (FAILED(hr)){ return hr; }
	temp->Release();
	hr = InitPixelShader(temp, pixelShaderVersion, card, pixelShaders[PixelShaders_SHADOWMAPPER], TEXT("Shaders_Shadows.hlsl"), "SHADOWMAPPER", errorOut);
	if (FAILED(hr)){ return hr; }
	temp->Release();
	hr = InitPixelShader(temp, pixelShaderVersion, card, pixelShaders[PixelShaders_SHADOWDBG], TEXT("Shaders_Shadows.hlsl"), "SHADOWDBG", errorOut);
	if (FAILED(hr)){ return hr; }
	temp->Release();
	hr = InitPixelShader(temp, pixelShaderVersion, card, pixelShaders[PixelShaders_SSVDEPTH], TEXT("Shaders_Shadows.hlsl"), "SSVDEPTH", errorOut);
	if (FAILED(hr)){ return hr; }
	temp->Release();
	hr = InitPixelShader(temp, pixelShaderVersion, card, pixelShaders[PixelShaders_SSGAUSSMAJOR], TEXT("Shaders_Shadows.hlsl"), "SSGAUSSMAJOR", errorOut);
	if (FAILED(hr)){ return hr; }
	temp->Release();
	hr = InitPixelShader(temp, pixelShaderVersion, card, pixelShaders[PixelShaders_SSGAUSSMINOR], TEXT("Shaders_Shadows.hlsl"), "SSGAUSSMINOR", errorOut);
	if (FAILED(hr)){ return hr; }
	temp->Release();
	hr = InitPixelShader(temp, pixelShaderVersion, card, pixelShaders[PixelShaders_POINTLIGHTER], TEXT("Shaders_Light.hlsl"), "POINTLIGHTER", errorOut);
	if (FAILED(hr)){ return hr; }
	temp->Release();
	hr = InitPixelShader(temp, pixelShaderVersion, card, pixelShaders[PixelShaders_POINTLIGHTERSM], TEXT("Shaders_Light.hlsl"), "POINTLIGHTERSM", errorOut);
	if (FAILED(hr)){ return hr; }
	temp->Release();
	hr = InitPixelShader(temp, pixelShaderVersion, card, pixelShaders[PixelShaders_POINTLIGHTERUMBRASM], TEXT("Shaders_Light.hlsl"), "POINTLIGHTERUMBRASM", errorOut);
	if (FAILED(hr)){ return hr; }
	temp->Release();
	hr = InitPixelShader(temp, pixelShaderVersion, card, pixelShaders[PixelShaders_POINTLIGHTERSSPCSS], TEXT("Shaders_Light.hlsl"), "POINTLIGHTERSSPCSSMINOR", errorOut);
	if (FAILED(hr)){ return hr; }
	temp->Release();
	hr = InitPixelShader(temp, pixelShaderVersion, card, pixelShaders[PixelShaders_MOTIONBLUR], TEXT("Shaders_Screen.hlsl"), "MOTIONBLUR", errorOut);
	if (FAILED(hr)){ return hr; }
	temp->Release();
	hr = InitPixelShader(temp, pixelShaderVersion, card, pixelShaders[PixelShaders_GRID], TEXT("Shaders_Screen.hlsl"), "GRID", errorOut);
	if (FAILED(hr)){ return hr; }
	temp->Release();
	hr = InitPixelShader(temp, pixelShaderVersion, card, pixelShaders[PixelShaders_LIGHTPARTICLE], TEXT("Shaders_Particle.hlsl"), "LIGHTPARTICLE", errorOut);
	if (FAILED(hr)){ return hr; }
	temp->Release();
	hr = InitPixelShader(temp, pixelShaderVersion, card, pixelShaders[PixelShaders_SKYBOX], TEXT("Shaders_Particle.hlsl"), "SKYBOX", errorOut);
	if (FAILED(hr)){ return hr; }
	temp->Release();
	hr = InitPixelShader(temp, pixelShaderVersion, card, pixelShaders[PixelShaders_DEBUGOUT], TEXT("Shaders_Debug.hlsl"), "DEBUGOUT", errorOut);
	if (FAILED(hr)){ return hr; }
	temp->Release();

	return hr;

}

HRESULT CelestialShader::initBuffers(TextContainer* errorOut)
{

	HRESULT hr = S_OK;

	//Create constantbuffers
	D3D11_BUFFER_DESC rbDesc;
    rbDesc.ByteWidth = sizeof( renderConstants );
    rbDesc.Usage = D3D11_USAGE_DYNAMIC;
    rbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    rbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    rbDesc.MiscFlags = 0;
    hr = card->CreateBuffer( &rbDesc, nullptr, &rcb );

	D3D11_BUFFER_DESC fbDesc;
    fbDesc.ByteWidth = sizeof( frameConstants );
    fbDesc.Usage = D3D11_USAGE_DYNAMIC;
    fbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    fbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    fbDesc.MiscFlags = 0;
    hr = card->CreateBuffer( &fbDesc, nullptr, &fcb );
	
	D3D11_BUFFER_DESC plbDesc;
    plbDesc.ByteWidth = sizeof( perLightConstants );
    plbDesc.Usage = D3D11_USAGE_DYNAMIC;
    plbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    plbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    plbDesc.MiscFlags = 0;
    hr = card->CreateBuffer( &plbDesc, nullptr, &plcb );
	

	return hr;
}

HRESULT CelestialShader::initStates(TextContainer* errorOut)
{

	HRESULT hr = S_OK;
	D3D11_RENDER_TARGET_BLEND_DESC blendState = D3D11_RENDER_TARGET_BLEND_DESC();
	blendState.BlendEnable = TRUE;
	blendState.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendState.DestBlend = D3D11_BLEND_DEST_ALPHA;
	blendState.BlendOp = D3D11_BLEND_OP_ADD;
	blendState.SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	blendState.DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;
	blendState.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendState.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	D3D11_BLEND_DESC targets = D3D11_BLEND_DESC();
	targets.RenderTarget[0] = blendState;
	hr = card->CreateBlendState(&targets, &blendStates[BlendCode_ALPHA]);
	
	blendState = D3D11_RENDER_TARGET_BLEND_DESC();
	blendState.BlendEnable = TRUE;
	blendState.DestBlend = D3D11_BLEND_ONE;
	blendState.SrcBlend = D3D11_BLEND_ONE;
	blendState.BlendOp = D3D11_BLEND_OP_ADD;
	blendState.SrcBlendAlpha = D3D11_BLEND_ONE;
	blendState.DestBlendAlpha = D3D11_BLEND_ZERO;
	blendState.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendState.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	targets = D3D11_BLEND_DESC();
	targets.RenderTarget[0] = blendState;
	hr = card->CreateBlendState(&targets, &blendStates[BlendCode_ADD]);
	
	blendState = D3D11_RENDER_TARGET_BLEND_DESC();
	targets = D3D11_BLEND_DESC();
	targets.RenderTarget[0] = blendState;
	hr = card->CreateBlendState(&targets, &blendStates[BlendCode_OPAQUE]);

	//Create the depth state
	D3D11_DEPTH_STENCIL_DESC depthState = D3D11_DEPTH_STENCIL_DESC();
	depthState.DepthEnable = TRUE;
	depthState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthState.DepthFunc = D3D11_COMPARISON_LESS;

	hr = card->CreateDepthStencilState(&depthState, &depthStates[DepthCode_RW]);

	//Create the zfail stencil state
	depthState = D3D11_DEPTH_STENCIL_DESC();
	depthState.DepthEnable = true;
	depthState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthState.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	depthState.StencilEnable = TRUE;
	depthState.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depthState.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	// Stencil operations if pixel is front-facing
	depthState.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthState.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthState.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthState.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	depthState.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthState.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthState.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthState.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	hr = card->CreateDepthStencilState(&depthState, &depthStates[DepthCode_ZFAILSINGLE]);

	//Create the zfail reverse stencil state
	depthState = D3D11_DEPTH_STENCIL_DESC();
	depthState.DepthEnable = true;
	depthState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthState.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	depthState.StencilEnable = TRUE;
	depthState.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depthState.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	// Stencil operations if pixel is front-facing
	depthState.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthState.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR_SAT;
	depthState.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthState.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	depthState.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthState.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR_SAT;
	depthState.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthState.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	hr = card->CreateDepthStencilState(&depthState, &depthStates[DepthCode_ZFAILREVERSE]);

	//Create the stencilcheck state
	depthState = D3D11_DEPTH_STENCIL_DESC();
	depthState.DepthEnable = FALSE;
	depthState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthState.DepthFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil test parameters
	depthState.StencilEnable = TRUE;
	depthState.StencilReadMask = 0xFF;
	depthState.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	depthState.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthState.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthState.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthState.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	// Stencil operations if pixel is back-facing
	depthState.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthState.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthState.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthState.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	hr = card->CreateDepthStencilState(&depthState, &depthStates[DepthCode_STCHCK]);

	//Create the stencillargecheck state
	depthState = D3D11_DEPTH_STENCIL_DESC();
	depthState.DepthEnable = TRUE;
	depthState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthState.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	depthState.StencilEnable = TRUE;
	depthState.StencilReadMask = 0xFF;
	depthState.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	depthState.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthState.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthState.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthState.FrontFace.StencilFunc = D3D11_COMPARISON_LESS;

	// Stencil operations if pixel is back-facing
	depthState.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthState.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthState.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthState.BackFace.StencilFunc = D3D11_COMPARISON_LESS;

	hr = card->CreateDepthStencilState(&depthState, &depthStates[DepthCode_LRGCHCK]);


	D3D11_RASTERIZER_DESC rastState = D3D11_RASTERIZER_DESC();
	rastState.CullMode = D3D11_CULL_BACK;
	rastState.FillMode = D3D11_FILL_SOLID;

	hr = card->CreateRasterizerState(&rastState, &rastStates[RastState_BACKCULL]);
	
	rastState = D3D11_RASTERIZER_DESC();
	rastState.CullMode = D3D11_CULL_NONE;
	rastState.FillMode = D3D11_FILL_SOLID;

	hr = card->CreateRasterizerState(&rastState, &rastStates[RastState_NOCULL]);

	rastState = D3D11_RASTERIZER_DESC();
	rastState.CullMode = D3D11_CULL_FRONT;
	rastState.FillMode = D3D11_FILL_SOLID;

	hr = card->CreateRasterizerState(&rastState, &rastStates[RastState_FRONTCULL]);

	rastState = D3D11_RASTERIZER_DESC();
	rastState.CullMode = D3D11_CULL_NONE;
	rastState.FillMode = D3D11_FILL_WIREFRAME;

	hr = card->CreateRasterizerState(&rastState, &rastStates[RastState_WIREFRAME]);

	D3D11_SAMPLER_DESC sampleState = D3D11_SAMPLER_DESC();
	sampleState.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampleState.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampleState.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampleState.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampleState.ComparisonFunc = D3D11_COMPARISON_NEVER;

	hr = card->CreateSamplerState(&sampleState, &sampleStates[SampleState_SINGLE]);

	sampleState = D3D11_SAMPLER_DESC();
	sampleState.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampleState.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampleState.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampleState.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampleState.ComparisonFunc = D3D11_COMPARISON_NEVER;

	hr = card->CreateSamplerState(&sampleState, &sampleStates[SampleState_ARRAY]);

	sampleState = D3D11_SAMPLER_DESC();
	sampleState.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	sampleState.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampleState.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampleState.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampleState.ComparisonFunc = D3D11_COMPARISON_GREATER;

	hr = card->CreateSamplerState(&sampleState, &sampleStates[SampleState_COMPARRAY]);

	return hr;

}

HRESULT CelestialShader::initTargets(GraphicQuality gQ, TextureResourceObject* backBuffer)
{

	HRESULT hr = S_OK;
	mtrs[MRTVal_AMB] = backBuffer;
	
	//Sets up the mtr
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = gQ.resolutionX;
	desc.Height = gQ.resolutionY;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;

	for (int i = 0; i<MRTVal_SIZE; i++)
	{

		

		if (i != MRTVal_AMB)
		{

			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

			if (i == MRTVal_DEPTH)
			{

				desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

				if (gQ.shadows.shadowTyping == ShadowType_VOLUME)
				{

					desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;

				}
				else
				{

					desc.Format = DXGI_FORMAT_R32_FLOAT;

				}

			}

			ID3D11Texture2D* tempTex;
			hr = card->CreateTexture2D(&desc, nullptr, &tempTex);

			if (mtrs[i] == nullptr)
			{

				mtrs[i] = new TextureResourceObject();

			}

			mtrs[i]->SetDXT(new DXTextureResource(tempTex, card));
			mtrs[i]->GetDXT()->CreateShaderView(nullptr);

			if (i != MRTVal_DEPTH)
			{

				mtrs[i]->GetDXT()->CreateTargetView(nullptr);

			}
		}
	}

	return hr;

}

HRESULT CelestialShader::initDepth(GraphicQuality gQ)
{

	HRESULT hr = S_OK;

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	descDSV.Flags = 0;

	// Create the depth stencil shader resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC descSRV;
	descSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	descSRV.Texture2D.MipLevels = 1;
	descSRV.Texture2D.MostDetailedMip = 0;

	// Create the secondary depth stencil shader resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC descSRV2;
	descSRV2.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	descSRV2.Texture2D.MipLevels = 1;
	descSRV2.Texture2D.MostDetailedMip = 0;

	// Create depth stencil texture
	ID3D11Texture2D* depthStencil;
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = gQ.resolutionX;
	descDepth.Height = gQ.resolutionY;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	if (gQ.shadows.shadowTyping == ShadowType_VOLUME)
	{

		descSRV.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		descSRV2.Format = DXGI_FORMAT_X24_TYPELESS_G8_UINT;
		descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepth.Format = DXGI_FORMAT_R24G8_TYPELESS;

	}
	else
	{

		descSRV.Format = DXGI_FORMAT_R32_FLOAT;
		descSRV2.Format = DXGI_FORMAT_R32_FLOAT;
		descDSV.Format = DXGI_FORMAT_D32_FLOAT;
		descDepth.Format = DXGI_FORMAT_R32_TYPELESS;

	}

	if (dpthStnc == nullptr)
	{

		dpthStnc = new TextureResourceObject();

	}

	hr = card->CreateTexture2D(&descDepth, nullptr, &depthStencil);
	dpthStnc->SetDXT(new DXTextureResource(depthStencil, card));

	if (FAILED(hr))
	{

		return hr;

	}

	dpthStnc->GetDXT()->CreateDepthView(&descDSV);
	dpthStnc->GetDXT()->CreateShaderView(&descSRV);
	dpthStnc->GetDXT()->CreateSecondShaderView(&descSRV2);

	return hr;

}

HRESULT CelestialShader::initShadowmap(GraphicQuality gQ)
{

	HRESULT hr = S_OK;

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = gQ.shadows.shadowTyping != ShadowType_VOLUME ? DXGI_FORMAT_D32_FLOAT : DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	descDSV.Flags = 0;

	// Create the depth stencil shader resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC descSRV;
	descSRV.Format = gQ.shadows.shadowTyping != ShadowType_VOLUME ? DXGI_FORMAT_R32_FLOAT : DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	descSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	descSRV.Texture2D.MipLevels = 1;
	descSRV.Texture2D.MostDetailedMip = 0;

	// Create shadowtexture
	ID3D11Texture2D* shadowDesc;

	D3D11_TEXTURE2D_DESC descShadow;
	descShadow.Width = gQ.shadows.shadowTyping != ShadowType_VOLUME ? gQ.shadows.shadowResolutionX : gQ.resolutionX;
	descShadow.Height = gQ.shadows.shadowTyping != ShadowType_VOLUME ? gQ.shadows.shadowResolutionY : gQ.resolutionY;
	descShadow.MipLevels = 1;
	descShadow.ArraySize = 1;
	descShadow.Format = gQ.shadows.shadowTyping != ShadowType_VOLUME ? DXGI_FORMAT_R32_TYPELESS : DXGI_FORMAT_R24G8_TYPELESS;
	descShadow.SampleDesc.Count = 1;
	descShadow.SampleDesc.Quality = 0;
	descShadow.Usage = D3D11_USAGE_DEFAULT;
	descShadow.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	descShadow.CPUAccessFlags = 0;
	descShadow.MiscFlags = 0;
	hr = card->CreateTexture2D(&descShadow, nullptr, &shadowDesc);

	if (shadowMap == nullptr)
	{

		shadowMap = new TextureResourceObject();

	}

	shadowMap->SetDXT(new DXTextureResource(shadowDesc, card));

	if (FAILED(hr))
	{

		return hr;

	}

	shadowMap->GetDXT()->CreateDepthView(&descDSV);
	shadowMap->GetDXT()->CreateShaderView(&descSRV);


	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = gQ.resolutionX;
	desc.Height = gQ.resolutionY;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.Format = DXGI_FORMAT_R32_FLOAT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	if (shadowInfo == nullptr)
	{

		shadowInfo = new TextureResourceObject();

	}

	ID3D11Texture2D* tempTex;
	hr = card->CreateTexture2D(&desc, nullptr, &tempTex);
	shadowInfo->SetDXT(new DXTextureResource(tempTex, card));
	shadowInfo->GetDXT()->CreateShaderView(nullptr);
	shadowInfo->GetDXT()->CreateTargetView(nullptr);

	if (shadowFactor == nullptr)
	{

		shadowFactor = new TextureResourceObject();

	}

	ID3D11Texture2D* tempTex2;
	hr = card->CreateTexture2D(&desc, nullptr, &tempTex2);
	shadowFactor->SetDXT(new DXTextureResource(tempTex2, card));
	shadowFactor->GetDXT()->CreateShaderView(nullptr);
	shadowFactor->GetDXT()->CreateTargetView(nullptr);

	if (shadowFactor2 == nullptr)
	{

		shadowFactor2 = new TextureResourceObject();

	}

	ID3D11Texture2D* tempTex3;
	hr = card->CreateTexture2D(&desc, nullptr, &tempTex3);
	shadowFactor2->SetDXT(new DXTextureResource(tempTex3, card));
	shadowFactor2->GetDXT()->CreateShaderView(nullptr);
	shadowFactor2->GetDXT()->CreateTargetView(nullptr);

	return hr;

}

void CelestialShader::initTechniques()
{

	//Initiate terrain rendering techniques
	techs[Technique_TERRAIN][GeometryCode_GEO] = new ShaderContainer(
		vertexShaders[VertexShaders_VSTERRAIN],
		geometryShaders[GeometryShaders_GSTriangles],
		pixelShaders[PixelShaders_MAPPER]);

	techs[Technique_TERRAIN][GeometryCode_GEOBORDERS] = new ShaderContainer(
		vertexShaders[VertexShaders_VSTERRAIN],
		geometryShaders[GeometryShaders_GSTrianglesBorders],
		pixelShaders[PixelShaders_MAPPER]);
	
	//Initiate geometry rendering techniques
	techs[Technique_GEOMETRY][GeometryCode_GEO] = new ShaderContainer(
		vertexShaders[VertexShaders_VS],
		geometryShaders[GeometryShaders_GSTriangles],
		pixelShaders[PixelShaders_MAPPER]);
	
	techs[Technique_GEOMETRY][GeometryCode_GEOBORDERS] = new ShaderContainer(
		vertexShaders[VertexShaders_VS],
		geometryShaders[GeometryShaders_GSTrianglesBorders],
		pixelShaders[PixelShaders_MAPPER]);

	techs[Technique_GEOMETRY][GeometryCode_GEOWIREFRAME] = new ShaderContainer(
		vertexShaders[VertexShaders_VS],
		geometryShaders[GeometryShaders_GSLines],
		pixelShaders[PixelShaders_MAPPER]);

	//Initiate shadowing techniques
	techs[Technique_SHADOW][CelestialShader::ShadowCode_TRNMAP] = new ShaderContainer(
		vertexShaders[VertexShaders_VSTERRAINSHADOW],
		geometryShaders[GeometryShaders_GSSHADOW],
		nullptr);
	
	techs[Technique_SHADOW][CelestialShader::ShadowCode_TRNVOLUME] = new ShaderContainer(
		vertexShaders[VertexShaders_VSSHADOW],
		geometryShaders[GeometryShaders_GSSHADOWVolumes],
		nullptr);

	techs[Technique_SHADOW][CelestialShader::ShadowCode_GEOMAP] = new ShaderContainer(
		vertexShaders[VertexShaders_VSSHADOW],
		geometryShaders[GeometryShaders_GSSHADOW],
		nullptr);

	techs[Technique_SHADOW][CelestialShader::ShadowCode_GEOVOLUME] = new ShaderContainer(
		vertexShaders[VertexShaders_VSSHADOW],
		geometryShaders[GeometryShaders_GSSHADOWVolumes],
		nullptr);

	techs[Technique_SHADOW][CelestialShader::ShadowCode_GEOUMBRAVOLUME] = new ShaderContainer(
		vertexShaders[VertexShaders_VSSHADOW],
		geometryShaders[GeometryShaders_GSSHADOWUmbraVolume],
		nullptr);// pixelShaders[PixelShaders_DEBUGOUT]);

	techs[Technique_SHADOW][CelestialShader::ShadowCode_GEOPENUMBRAVOLUME] = new ShaderContainer(
		vertexShaders[VertexShaders_VSSHADOW],
		geometryShaders[GeometryShaders_GSSHADOWPenUmbraVolume],
		nullptr);// pixelShaders[PixelShaders_DEBUGOUT]);

	techs[Technique_SHADOW][CelestialShader::ShadowCode_GEOSSVOLUME] = new ShaderContainer(
		vertexShaders[VertexShaders_VSSHADOW],
		geometryShaders[GeometryShaders_GSSHADOWSSVolume],
		pixelShaders[PixelShaders_SSVDEPTH]);

	techs[Technique_SHADOW][CelestialShader::ShadowCode_SSPCSSMAJOR] = new ShaderContainer(
		vertexShaders[VertexShaders_VSLIGHT],
		geometryShaders[GeometryShaders_GSLIGHT],
		pixelShaders[PixelShaders_SSGAUSSMAJOR]);

	//Initiate lighting techniques
	techs[Technique_LIGHT][LightCode_POINTNOSHADOWS] = new ShaderContainer(
		vertexShaders[VertexShaders_VSLIGHT],
		geometryShaders[GeometryShaders_GSLIGHT],
		pixelShaders[PixelShaders_POINTLIGHTER]);

	techs[Technique_LIGHT][LightCode_POINTSHADOWMAPPING] = new ShaderContainer(
		vertexShaders[VertexShaders_VSLIGHT],
		geometryShaders[GeometryShaders_GSLIGHT],
		pixelShaders[PixelShaders_POINTLIGHTERSM]);

	techs[Technique_LIGHT][LightCode_POINTUMBRASHADOWMAPPING] = new ShaderContainer(
		vertexShaders[VertexShaders_VSLIGHT],
		geometryShaders[GeometryShaders_GSLIGHT],
		pixelShaders[PixelShaders_POINTLIGHTERUMBRASM]);
	
	techs[Technique_LIGHT][LightCode_POINTSSPCSS] = new ShaderContainer(
		vertexShaders[VertexShaders_VSLIGHT],
		geometryShaders[GeometryShaders_GSLIGHT],
		pixelShaders[PixelShaders_POINTLIGHTERSSPCSS]);

	//Initiate particle techniques
	techs[Technique_PARTICLE][ParticleCode_LIGHT] = new ShaderContainer(
		vertexShaders[VertexShaders_VSPARTICLE],
		geometryShaders[GeometryShaders_GSPARTICLE],
		pixelShaders[PixelShaders_LIGHTPARTICLE]);
	
	techs[Technique_PARTICLE][ParticleCode_SKYBOX] = new ShaderContainer(
		vertexShaders[VertexShaders_VSPARTICLE],
		geometryShaders[GeometryShaders_GSSKYCUBE],
		pixelShaders[PixelShaders_SKYBOX]);

	//Initiate screen effect techniques
	techs[Technique_SCREEN][ScreenCode_MOTIONBLUR] = new ShaderContainer(
		vertexShaders[VertexShaders_VSSCREEN],
		geometryShaders[GeometryShaders_GSSCREEN],
		pixelShaders[PixelShaders_MOTIONBLUR]);
	
	techs[Technique_SCREEN][ScreenCode_GRID] = new ShaderContainer(
		vertexShaders[VertexShaders_VSSCREEN],
		geometryShaders[GeometryShaders_GSSCREEN],
		pixelShaders[PixelShaders_GRID]);
	
	//Initiate debug techniques
	techs[Technique_DEBUG][DebugCode_GNSPIKES] = new ShaderContainer(
		vertexShaders[VertexShaders_VS],
		geometryShaders[GeometryShaders_GSNORMALSPIKE],
		pixelShaders[PixelShaders_DEBUGOUT]);

	techs[Technique_DEBUG][DebugCode_TNSPIKES] = new ShaderContainer(
		vertexShaders[VertexShaders_VSTERRAIN],
		geometryShaders[GeometryShaders_GSNORMALSPIKE],
		pixelShaders[PixelShaders_DEBUGOUT]);


}

HRESULT CelestialShader::releaseTargets()
{

	for (int i = 1; i<MRTVal_SIZE; i++)
	{

		mtrs[i]->GetDXT()->Release();

	}

	return S_OK;

}

HRESULT CelestialShader::releaseDepth()
{

	
	dpthStnc->GetDXT()->Release();
	return S_OK;

}

HRESULT CelestialShader::releaseShadowMap()
{

	if (shadowMap != nullptr)
	{

		shadowMap->GetDXT()->Release();

	}

	if (shadowInfo != nullptr)
	{

		shadowInfo->GetDXT()->Release();

	}

	if (shadowFactor != nullptr)
	{

		shadowFactor->GetDXT()->Release();

	}

	if (shadowFactor2 != nullptr)
	{

		shadowFactor2->GetDXT()->Release();

	}
	return S_OK;

}

HRESULT CelestialShader::Init(ID3D11Device* card, GraphicQuality gQ, DrawingStyle dS, TextureResourceObject* backBuffer,TextContainer* errorOut)
{

	quality = gQ;
	dStyle = dS;

	this->card = card;
	bH = new CelestialBufferHandler(card,2);
	sMC = new ShadowMapConstants(gQ.shadows);

	HRESULT hr = initBuffers(errorOut);
	errorOut->AddTextLine("Creating shaders");
	hr = initShaders(errorOut);
	if (FAILED(hr)){ return hr; }
	errorOut->AddTextLine("Creating states");
	hr = initStates(errorOut);
	if (FAILED(hr)){ return hr; }
	errorOut->AddTextLine("Creating in/out-textures");
	hr = initTargets(gQ, backBuffer);
	if (FAILED(hr)){ return hr; }
	hr = initDepth(gQ);
	if (FAILED(hr)){ return hr; }
	hr = initShadowmap(gQ);
	if (FAILED(hr)){ return hr; }
	errorOut->AddTextLine("Creating techniques");
	initTechniques();

	plc.LightFalloff = 16;
	plc.BlockerSearch = gQ.shadows.blockerSamples;
	plc.ShadowSearch = gQ.shadows.shadowSamples;

	fc.GlobalAmbience = Vector4(0.074f,0.080f,0.086f,1);
	fc.GlobalLight = Vector4(0.05f,0.5f,0.7f,0);
	fc.GlobalDirection = Vector4(0.0f,1.0f,0.0f,0);
	fc.DebugCol = Vector4(0.9f,0.9f,0.9f,0);
	fc.BlurSamples = gQ.blurSamples;
	fc.BorderThickness = 0.1f;

	fc.Volumes = gQ.shadows.shadowTyping == ShadowType_VOLUME;

	float* screen = new float[1];
	screen[0] = 0.0f;
	screenStride = sizeof(float);

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = screenStride * 1; //total size of buffer in bytes
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = screen;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	hr = card->CreateBuffer( &bd, &initData, &buffers[2]);
	delete[] screen;

	light = true;
	wf = false;

	return hr;

}

void CelestialShader::SetVertexBuffers(ID3D11Buffer* vertices, ID3D11Buffer* indices, ID3D11DeviceContext* context)
{

	unsigned int offset = 0;
	unsigned int vStride = sizeof(BufferVertex);
	context->IASetVertexBuffers(0, 1, &vertices, &vStride, &offset);//Set buffers
	context->IASetIndexBuffer(indices, DXGI_FORMAT_R32_UINT, 0);//Set the index buffer

}

void CelestialShader::ToggleWireFrameMode(bool val,bool light)
{

	this->light = light;
	this->wf = val;

}

void CelestialShader::ToggleNormalSpikes(bool val)
{

	normalSpikes = val;

}

void CelestialShader::ToggleGlobalBorders(bool val)
{

	globalBorders = val;

}

bool CelestialShader::SetQuality(GraphicQuality gQ, TextureResourceObject* backBuffer)
{

	if (gQ.resolutionX != quality.resolutionX || gQ.resolutionY != quality.resolutionY)
	{

		releaseTargets();
		initTargets(gQ, backBuffer);
		initDepth(gQ);

	}

	if ((gQ.shadows.shadowResolutionX != quality.shadows.shadowResolutionX || gQ.shadows.shadowResolutionY != quality.shadows.shadowResolutionY) || gQ.shadows.shadowTyping != quality.shadows.shadowTyping)
	{

		releaseShadowMap();
		initShadowmap(gQ);
		sMC->UpdateQuality(gQ.shadows);

	}

	if (gQ.shadows.shadowTyping != quality.shadows.shadowTyping && (gQ.shadows.shadowTyping == ShadowType_VOLUME || quality.shadows.shadowTyping == ShadowType_VOLUME))
	{

		releaseDepth();
		initDepth(gQ);

	}

	quality = gQ;
	fc.BlurSamples = gQ.blurSamples;
	plc.BlockerSearch = gQ.shadows.blockerSamples;
	plc.ShadowSearch = gQ.shadows.shadowSamples;
	return true;

}

bool CelestialShader::SetDrawing(DrawingStyle dS)
{

	this->dStyle = dS;
	return true;

}

void CelestialShader::NothingToDraw(ID3D11DeviceContext* context)
{

	float temp[4] = { 0, 0, 0, 1 };//Clear rendertargets
	context->ClearDepthStencilView(dpthStnc->GetDXT()->GetDepthView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->ClearRenderTargetView(mtrs[MRTVal_AMB]->GetDXT()->GetTargetView(), temp);
	context->ClearRenderTargetView(mtrs[MRTVal_DIFF]->GetDXT()->GetTargetView(), temp);
	//card->ClearRenderTargetView(mtrs[MRTVal_VEL]->GetDXT()->GetTargetView(), temp);

}

void CelestialShader::StartDrawing(ViewObject* scene, ID3D11DeviceContext* context, unsigned int flip)
{

	context->PSSetSamplers(0, SampleState_SIZE, sampleStates);
	float temp[4] = { 0, 0, 0, 1 };
	float temp2[4] = { 1, 0, 0, 1 };
	//Clear rendertargets
	NothingToDraw(context);

	fc.FrameTime = clock();

	fc.CameraPos = Vector4(scene->GetPosition(flip), 0);

	fc.LastViewProjection = fc.ViewProjection;
	fc.InvertViewProjection = MatrixTranspose(scene->GetInverseViewProjection(flip));
	fc.View = MatrixTranspose(scene->GetView(flip));
	fc.Projection = MatrixTranspose(scene->GetProjection(flip));
	fc.ViewProjection = MatrixTranspose(scene->GetViewProjection(flip));

	fc.GlobalDirection = Vector4(0.0f, 1.0f, 0.0f, 0);
	fc.ZNear = scene->GetPort().minDepth;

	fc.FOV = scene->GetFov();

	if (!light)
	{

		fc.GlobalDirection = Vector4(0.0f, 0.0f, 0.0f, 0);

	}

	transferFrameConstants(context);

}

void CelestialShader::DrawScene(ViewObject* scene, GraphicalMesh* meshes, ID3D11DeviceContext* context, unsigned int flip)
{

	ID3D11RenderTargetView* target[4] = { mtrs[MRTVal_AMB]->GetDXT()->GetTargetView(), 
		mtrs[MRTVal_DIFF]->GetDXT()->GetTargetView(),
		mtrs[MRTVal_NORM]->GetDXT()->GetTargetView(),
		mtrs[MRTVal_VEL]->GetDXT()->GetTargetView() };

	//Prepare to draw geometry
	D3D11_VIEWPORT vp;
	ViewObject::ViewPort port = scene->GetPort();
	vp.Height = port.height;
	vp.MaxDepth = 1.0f;
	vp.MinDepth = port.minDepth;
	vp.TopLeftX = port.topX;
	vp.TopLeftY = port.topY;
	vp.Width = port.width;

	context->RSSetViewports(1, &vp);
	context->RSSetState(rastStates[RastState_BACKCULL]);

	context->OMSetDepthStencilState(depthStates[DepthCode_RW], 0);
	context->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
	context->OMSetRenderTargets(4, target, dpthStnc->GetDXT()->GetDepthView());

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ);
	context->IASetInputLayout(bH->GetVertexLayout());

	CelestialStack<ViewObject::Fragment>* instanceStack = scene->GetInstanceStack(flip);
	MeshType lastType = MeshType_NA;

	while (instanceStack->GetCount() > 0)
	{

		ViewObject::Fragment fragment = instanceStack->PopElement();
		GraphicalMesh mesh = meshes[fragment.mesh];
		MeshType thisType = mesh.GetMeshType();

		if (thisType != lastType)
		{

			GeometryCode rendCode = globalBorders ? GeometryCode_GEOBORDERS : GeometryCode_GEO;

			switch (thisType)
			{

				case MeshType_SOLID:
					context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ);
					context->RSSetState(rastStates[RastState_BACKCULL]);
					context->VSSetShader(techs[Technique_GEOMETRY][rendCode]->GetVertexShader(), nullptr, 0);
					context->GSSetShader(techs[Technique_GEOMETRY][rendCode]->GetGeometryShader(), nullptr, 0);
					context->PSSetShader(techs[Technique_GEOMETRY][rendCode]->GetPixelShader(), nullptr, 0);
					break;
				case MeshType_WF:
					//context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ);
					context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
					context->RSSetState(rastStates[RastState_WIREFRAME]);
					context->VSSetShader(techs[Technique_GEOMETRY][GeometryCode_GEOWIREFRAME]->GetVertexShader(), nullptr, 0);
					context->GSSetShader(techs[Technique_GEOMETRY][GeometryCode_GEOWIREFRAME]->GetGeometryShader(), nullptr, 0);
					context->PSSetShader(techs[Technique_GEOMETRY][GeometryCode_GEOWIREFRAME]->GetPixelShader(), nullptr, 0);
					break;

			}
			lastType = thisType;

		}

		srvs[0] = mesh.GetAmbientTexture() == nullptr ? nullptr : mesh.GetAmbientTexture()->GetShaderView();
		srvs[1] = mesh.GetDiffuseTexture() == nullptr ? nullptr : mesh.GetDiffuseTexture()->GetShaderView();

		int size = 2;

		rc.UseNormal[0] = false;

		if (dStyle.useNormalMaps && mesh.GetNormalTexture() != nullptr)
		{

			srvs[2] = mesh.GetNormalTexture()->GetShaderView();
			rc.UseNormal[0] = true;
			size++;

		}

		transferRenderConstants(context);
		context->PSSetShaderResources(SRVRegisters_AMB, size, srvs);
		context->DrawIndexedInstanced(mesh.GetIndexLength(), fragment.length, mesh.GetIndexStart(), 0, fragment.start);

	}

	instanceStack->Rewind();

}

//void CelestialShader::DrawScene(Graphics::DrawScene* scene, int flip)
//{
//
//	ID3D11RenderTargetView** target = new ID3D11RenderTargetView*[4];
//		
//	//Prepare to draw geometry
//	card->RSSetViewports(1, &camera->GetViewPort());
//
//	if (!wf)
//	{
//
//		card->RSSetState(rastStates[RastState_BACKCULL]);
//
//	}
//	else
//	{
//
//		card->RSSetState(rastStates[RastState_WIREFRAME]);
//
//	}
//
//	target[0] = mtrs[MRTVal_AMB]->GetDXT()->GetTargetView();
//	target[1] = mtrs[MRTVal_DIFF]->GetDXT()->GetTargetView();
//	target[2] = mtrs[MRTVal_NORM]->GetDXT()->GetTargetView();
//	target[3] = mtrs[MRTVal_VEL]->GetDXT()->GetTargetView();
//
//	card->OMSetDepthStencilState(depthStates[DepthCode_RW], 0);
//	card->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
//	card->OMSetRenderTargets(4, target, dpthStnc->GetDXT()->GetDepthView());
//
//	card->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ);
//
//	GeometryCode rendCode = globalBorders ? GeometryCode_GEOBORDERS : GeometryCode_GEO;
//	card->VSSetShader(techs[Technique_GEOMETRY][rendCode]->GetVertexShader());
//	card->GSSetShader(techs[Technique_GEOMETRY][rendCode]->GetGeometryShader());
//	card->PSSetShader(techs[Technique_GEOMETRY][rendCode]->GetPixelShader());
//
//	CelestialListNode<MeshObject*>* meshNode = scene->GetMeshes()->GetFirstNode();
//
//	while (meshNode != nullptr && meshNode->GetNodeObject() != nullptr)//Go through each mesh
//	{
//
//		MeshObject* mesh = meshNode->GetNodeObject();
//		//Draw the geometry of the mesh
//		int vSize = 0;
//		int iSize = 0;
//		int iMax = 0;
//		int iLast = 0;
//		UINT vStride = bH->GetStride(BufferTypes_VERTEX);
//		UINT iStride = bH->GetStride(BufferTypes_INSTANCE);
//
//		if (mesh->GetBuffer(BufferTypes_INSTANCE, flip) != nullptr)
//		{
//
//			DXBufferObject* inBuffer = mesh->GetBuffer(BufferTypes_INSTANCE, flip)->GetFirstDXBuffer();//Instance buffer
//
//			while (inBuffer != nullptr)//Go through each instancebuffer this mesh belongs to
//			{
//
//				if (inBuffer->GetMappedPointer() != nullptr)//Close the buffer if it's left hanging
//				{
//
//					inBuffer->UnMap();
//
//				}
//
//				strides[1] = iStride;
//				buffers[1] = inBuffer->GetBuffer();
//				offSets[1] = 0;
//
//				if (normalSpikes)//Draw out the spikes to debug
//				{
//
//					card->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
//					card->IASetInputLayout(bH->GetVertexLayout());
//
//					ID3D11VertexShader* tempVS;
//					card->VSGetShader(&tempVS);
//					ID3D11GeometryShader* tempGS;
//					card->GSGetShader(&tempGS);
//					ID3D11PixelShader* tempPS;
//					card->PSGetShader(&tempPS);
//
//					card->VSSetShader(techs[Technique_DEBUG][DebugCode_GNSPIKES]->GetVertexShader());
//					card->GSSetShader(techs[Technique_DEBUG][DebugCode_GNSPIKES]->GetGeometryShader());
//					card->PSSetShader(techs[Technique_DEBUG][DebugCode_GNSPIKES]->GetPixelShader());
//
//					DXBufferObject* vBuffer = mesh->GetBuffer(BufferTypes_VERTEX, 0)->GetFirstDXBuffer();//Vertex bufer
//
//					while (vBuffer != nullptr)//Go through each vertexbuffer in this mesh
//					{
//
//						buffers[0] = vBuffer->GetBuffer();
//						strides[0] = vStride;
//						offSets[0] = 0;
//						card->IASetVertexBuffers(0, 2, buffers, strides, offSets);//Set buffers
//						card->DrawInstanced(vBuffer->GetSize(), inBuffer->GetSize(), 0, 0);
//
//						vBuffer = vBuffer->GetNext();
//
//					}
//
//					card->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ);
//					card->VSSetShader(tempVS);
//					card->GSSetShader(tempGS);
//					card->PSSetShader(tempPS);
//					tempVS->Release();
//					tempGS->Release();
//					tempPS->Release();
//
//				}
//
//				DXBufferObject* iBuffer = mesh->GetBuffer(BufferTypes_INDEX, 0)->GetFirstDXBuffer();//Index buffer
//				DXBufferObject* vBuffer = iBuffer->GetCompany();//Vertex buffer
//
//				if (iBuffer != nullptr && vBuffer != nullptr && vBuffer->GetBuffer() != nullptr && iBuffer->GetBuffer() != nullptr)
//				{
//
//					buffers[0] = vBuffer->GetBuffer();
//					strides[0] = vStride;
//					offSets[0] = 0;
//					card->IASetVertexBuffers(0, 2, buffers, strides, offSets);//Set buffers
//					card->IASetInputLayout(bH->GetVertexLayout());
//					card->IASetIndexBuffer(iBuffer->GetBuffer(), DXGI_FORMAT_R32_UINT, 0);//Set the index buffer
//
//					for (int i = 0; i < mesh->GetMaterialSize(); i++)
//					{
//
//						if (mesh->getMaterials()[i]->GetCount() != 0)
//						{
//
//							srvs[0] = mesh->getMaterials()[i]->GetAmbient()->GetDXT()->GetShaderView();
//							srvs[1] = mesh->getMaterials()[i]->GetDiffuse()->GetDXT()->GetShaderView();
//
//							int size = 2;
//
//							rc.UseNormal[0] = false;
//
//							if (dStyle.useNormalMaps && mesh->getMaterials()[i]->GetNormal() != nullptr)
//							{
//
//								srvs[2] = mesh->getMaterials()[i]->GetNormal()->GetDXT()->GetShaderView();
//								rc.UseNormal[0] = true;
//								size++;
//
//							}
//
//							transferRenderConstants();
//							card->PSSetShaderResources(SRVRegisters_AMB, size, srvs);
//
//							card->DrawIndexedInstanced(mesh->getMaterials()[i]->GetCount(), inBuffer->GetSize(), mesh->getMaterials()[i]->GetStartingIndex(), 0, 0);
//
//						}
//					}
//				}
//
//				inBuffer = inBuffer->GetNext();
//
//			}
//		}
//
//		meshNode = meshNode->GetNext();
//
//	}
//
//	//All geometry has been rendered, Shade and light scene
//	card->OMSetRenderTargets(1, target, dpthStnc->GetDXT()->GetDepthView());
//	card->CopyResource(mtrs[MRTVal_DEPTH]->GetDXT()->GetTexture(), dpthStnc->GetDXT()->GetTexture());
//	srvs[0] = mtrs[MRTVal_DIFF]->GetDXT()->GetShaderView();
//	srvs[1] = mtrs[MRTVal_NORM]->GetDXT()->GetShaderView();
//	srvs[2] = mtrs[MRTVal_DEPTH]->GetDXT()->GetShaderView();
//	card->PSSetShaderResources(SRVRegisters_DIFF, 3, srvs);
//
//	CelestialListNode<ILight*>* lightNode = scene->GetLights()->GetFirstNode();
//	
//	while (lightNode != nullptr && lightNode->GetNodeObject() != nullptr && dStyle.enlighten)//GO through each light
//	{
//
//		ILight* light = lightNode->GetNodeObject();
//
//		//Prepare the light
//		plc.Angle = light->GetAngle();
//		plc.Diffuse = light->GetDiff();
//		plc.Forward = light->GetDirection();
//		plc.Position = light->GetPos();
//		plc.Radius = light->GetIntensity();
//		plc.Right = light->GetSide();
//		plc.Size = light->GetSize();
//		plc.Specular = light->GetSpec();
//
//		if (light->GetType() == LightType_POINT)
//		{
//
//			float invW = 1.0f / 3.0f;
//			float invH = 1.0f / 2.0f;
//			plc.InvRenderWidth = invW;
//			plc.InvRenderHeight = invH;
//
//			for (int i = 0; i < sMC->GetSubs(light->GetType()); i++)
//			{
//
//				plc.RenderCenter[i] = Vector4(sMC->GetStarts(light->GetType())[i].x, sMC->GetStarts(light->GetType())[i].y, 0, 0);
//
//			}
//		}
//
//		transferLightConstants();
//
//
//		//Prepare for shadowing
//		int repeat = 0;
//
//		if (quality.shadows.shadowCoding == ShadowCode_SSVOLUME)
//		{
//
//			repeat = 2;
//
//		}
//
//		while (repeat >= 0)
//		{
//
//			CelestialListNode<MeshObject*>* meshNode = scene->GetMeshes()->GetFirstNode();
//			
//			if (quality.shadows.shadowCoding == ShadowCode_SSVOLUME && repeat == 1)
//			{
//
//				card->OMSetDepthStencilState(depthStates[DepthCode_ZFAILSINGLE], 0);
//				card->CopyResource(shadowMap->GetDXT()->GetTexture(), dpthStnc->GetDXT()->GetTexture());
//				card->ClearDepthStencilView(shadowMap->GetDXT()->GetDepthView(), D3D11_CLEAR_DEPTH, 1.0f, 0);//Clear the shadowmap
//
//				float temp[4] = { 0, 0, 0, 1 };
//				card->ClearRenderTargetView(shadowInfo->GetDXT()->GetTargetView(), temp);
//
//				target[0] = shadowInfo->GetDXT()->GetTargetView();
//				card->OMSetRenderTargets(1, target, shadowMap->GetDXT()->GetDepthView());
//				card->OMSetDepthStencilState(depthStates[DepthCode_LRGCHCK], 0);
//				card->RSSetState(rastStates[RastState_NOCULL]);
//
//			}
//			else if (quality.shadows.shadowTyping == ShadowType_VOLUME)
//			{
//
//				if (quality.shadows.shadowCoding == ShadowCode_UMBRAVOLUME || quality.shadows.shadowCoding == ShadowCode_PENUMBRAVOLUME || (quality.shadows.shadowCoding == ShadowCode_SSVOLUME && repeat == 2))
//				{
//
//					card->OMSetDepthStencilState(depthStates[DepthCode_ZFAILREVERSE], 0);
//
//				}
//				else
//				{
//
//					card->OMSetDepthStencilState(depthStates[DepthCode_ZFAILSINGLE], 0);
//
//				}
//
//				card->RSSetViewports(1, &camera->GetViewPort());
//				card->ClearDepthStencilView(dpthStnc->GetDXT()->GetDepthView(), D3D11_CLEAR_STENCIL, 1.0f, 0);
//
//				card->OMSetRenderTargets(1, target, dpthStnc->GetDXT()->GetDepthView());
//
//				card->RSSetState(rastStates[RastState_NOCULL]);
//
//			}
//			else
//			{
//
//				card->OMSetDepthStencilState(depthStates[DepthCode_RW], 0);
//				card->ClearDepthStencilView(shadowMap->GetDXT()->GetDepthView(), D3D11_CLEAR_DEPTH, 1.0f, 0);//Clear the shadowmap
//				srvs[0] = nullptr;
//				card->PSSetShaderResources(SRVRegisters_SHADOWMAP, 1, srvs);
//
//				card->RSSetViewports(sMC->GetSubs(light->GetType()), sMC->GetViewPortsOfLight(light->GetType()));
//				card->OMSetRenderTargets(0, nullptr, shadowMap->GetDXT()->GetDepthView());
//				card->RSSetState(rastStates[RastState_BACKCULL]);
//
//			}
//
//			CelestialShader::ShadowCode shdwCode = quality.shadows.shadowTyping == ShadowType_MAP ? CelestialShader::ShadowCode_GEOMAP : CelestialShader::ShadowCode_GEOVOLUME;
//			shdwCode = quality.shadows.shadowCoding == ShadowCode_UMBRAVOLUME ? CelestialShader::ShadowCode_GEOUMBRAVOLUME : shdwCode;
//			shdwCode = quality.shadows.shadowCoding == ShadowCode_PENUMBRAVOLUME || ( quality.shadows.shadowCoding == ShadowCode_SSVOLUME && repeat == 2) ? CelestialShader::ShadowCode_GEOPENUMBRAVOLUME : shdwCode;
//			shdwCode = quality.shadows.shadowCoding == ShadowCode_SSVOLUME && repeat == 1 ? CelestialShader::ShadowCode_GEOSSVOLUME : shdwCode;
//
//			card->VSSetShader(techs[Technique_SHADOW][shdwCode]->GetVertexShader());
//			card->GSSetShader(techs[Technique_SHADOW][shdwCode]->GetGeometryShader());
//			card->PSSetShader(techs[Technique_SHADOW][shdwCode]->GetPixelShader());
//			card->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ);
//
//			while (meshNode != nullptr && meshNode->GetNodeObject() != nullptr)//Go through each mesh
//			{
//
//				MeshObject* mesh = meshNode->GetNodeObject();
//				int vSize = 0;
//				int iSize = 0;
//				int iMax = 0;
//				int iLast = 0;
//				UINT vStride = bH->GetStride(BufferTypes_VERTEX);
//				UINT iStride = bH->GetStride(BufferTypes_INSTANCE);
//
//				if (mesh->GetBuffer(BufferTypes_INSTANCE, flip) != nullptr && mesh->GetId() != 0)
//				{
//
//					DXBufferObject* inBuffer = mesh->GetBuffer(BufferTypes_INSTANCE, flip)->GetFirstDXBuffer();//Instance buffer
//
//					while (inBuffer != nullptr)//Go through each instancebuffer this mesh belongs to
//					{
//
//						if (inBuffer->GetMappedPointer() != nullptr)//Close the buffer if it's left hanging
//						{
//
//							inBuffer->UnMap();
//
//						}
//
//						strides[1] = iStride;
//						buffers[1] = inBuffer->GetBuffer();
//						offSets[1] = 0;
//
//						DXBufferObject* iBuffer = mesh->GetBuffer(BufferTypes_INDEX, 0)->GetFirstDXBuffer();//Index buffer
//
//						while (iBuffer != nullptr)//Go through each material in the mesh
//						{
//
//							DXBufferObject* vBuffer = iBuffer->GetCompany();//Vertex buffer
//							ID3D11Buffer* index = iBuffer->GetBuffer();
//
//							if (index != nullptr && vBuffer != nullptr && vBuffer->GetBuffer() != nullptr)
//							{
//
//								buffers[0] = vBuffer->GetBuffer();
//								strides[0] = vStride;
//								offSets[0] = 0;
//								card->IASetVertexBuffers(0, 2, buffers, strides, offSets);//Set buffers
//								card->IASetInputLayout(bH->GetVertexLayout());
//								card->IASetIndexBuffer(index, DXGI_FORMAT_R32_UINT, 0);//Set the index buffer
//
//								if (quality.shadows.shadowCoding == ShadowCode_UMBRAVOLUME || quality.shadows.shadowCoding == ShadowCode_PENUMBRAVOLUME || (quality.shadows.shadowCoding == ShadowCode_SSVOLUME && repeat == 2))
//								{
//
//									card->RSSetState(rastStates[RastState_FRONTCULL]);//Draw backfaces
//									card->DrawIndexedInstanced(iBuffer->GetSize(), inBuffer->GetSize(), 0, 0, 0);
//									card->RSSetState(rastStates[RastState_BACKCULL]);//Draw frontfaces
//									card->DrawIndexedInstanced(iBuffer->GetSize(), inBuffer->GetSize(), 0, 0, 0);
//
//								}
//								else
//								{
//
//									card->DrawIndexedInstanced(iBuffer->GetSize(), inBuffer->GetSize(), 0, 0, 0);
//
//								}
//							}
//
//							iBuffer = iBuffer->GetNext();
//
//						}
//
//						inBuffer = inBuffer->GetNext();
//
//					}
//				}
//
//				meshNode = meshNode->GetNext();
//
//			}
//
//			repeat--;
//
//		}
//
//		//Prepare to draw light
//		card->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
//		card->IASetInputLayout(screenLayout);
//		UINT offSet = 0;
//		card->IASetVertexBuffers(0, 1, &buffers[2], &screenStride, &offSet);//Put in 1 vertex to act as a point and build our quad in the GS
//		card->RSSetState(rastStates[RastState_BACKCULL]);
//
//		if (quality.shadows.shadowCoding == ShadowCode_SSVOLUME)
//		{
//
//			ID3D11RenderTargetView* target = shadowFactor->GetDXT()->GetTargetView();
//			card->OMSetRenderTargets(1, &target, nullptr);
//
//			srvs[0] = dpthStnc->GetDXT()->GetSecondShaderView();
//			srvs[1] = shadowInfo->GetDXT()->GetShaderView();
//			card->PSSetShaderResources(SRVRegisters_SHADOWSTENCIL, 2, srvs);
//
//			card->VSSetShader(techs[Technique_SHADOW][ShadowCode_SSPCSSMAJOR]->GetVertexShader());
//			card->GSSetShader(techs[Technique_SHADOW][ShadowCode_SSPCSSMAJOR]->GetGeometryShader());
//			card->PSSetShader(techs[Technique_SHADOW][ShadowCode_SSPCSSMAJOR]->GetPixelShader());
//
//			card->Draw(1, 0);
//			target = mtrs[MRTVal_AMB]->GetDXT()->GetTargetView();
//			card->OMSetRenderTargets(1, &target, nullptr);
//
//			srvs[0] = shadowFactor->GetDXT()->GetShaderView();
//			card->PSSetShaderResources(SRVRegisters_SHADOWMAP, 1, srvs);
//
//		}
//		else if (quality.shadows.shadowTyping == ShadowType_VOLUME)
//		{
//
//			card->OMSetDepthStencilState(depthStates[DepthCode_STCHCK], 0);
//			ID3D11RenderTargetView* target = mtrs[MRTVal_AMB]->GetDXT()->GetTargetView();
//			card->OMSetRenderTargets(1, &target, dpthStnc->GetDXT()->GetDepthView());
//
//		}
//		else
//		{
//
//			srvs[0] = shadowMap->GetDXT()->GetShaderView();
//			card->PSSetShaderResources(SRVRegisters_SHADOWMAP, 1, srvs);
//			ID3D11RenderTargetView* target = mtrs[MRTVal_AMB]->GetDXT()->GetTargetView();
//			card->OMSetRenderTargets(1, &target, nullptr);
//
//		}
//
//		card->RSSetViewports(1, &camera->GetViewPort());
//		card->OMSetBlendState(blendStates[BlendCode_ADD], nullptr, 0xFFFFFFFF);
//
//		if (light->GetType() == LightType_POINT)
//		{
//
//			if (quality.shadows.shadowCoding == ShadowCode_MAP)
//			{
//
//				card->VSSetShader(techs[Technique_LIGHT][LightCode_POINTSHADOWMAPPING]->GetVertexShader());
//				card->GSSetShader(techs[Technique_LIGHT][LightCode_POINTSHADOWMAPPING]->GetGeometryShader());
//				card->PSSetShader(techs[Technique_LIGHT][LightCode_POINTSHADOWMAPPING]->GetPixelShader());
//
//			}
//			else if (quality.shadows.shadowCoding == ShadowCode_PCSS || quality.shadows.shadowCoding == ShadowCode_SSVOLUME)
//			{
//
//				card->VSSetShader(techs[Technique_LIGHT][LightCode_POINTSSPCSS]->GetVertexShader());
//				card->GSSetShader(techs[Technique_LIGHT][LightCode_POINTSSPCSS]->GetGeometryShader());
//				card->PSSetShader(techs[Technique_LIGHT][LightCode_POINTSSPCSS]->GetPixelShader());
//
//			}
//			else
//			{
//
//				card->VSSetShader(techs[Technique_LIGHT][LightCode_POINTNOSHADOWS]->GetVertexShader());
//				card->GSSetShader(techs[Technique_LIGHT][LightCode_POINTNOSHADOWS]->GetGeometryShader());
//				card->PSSetShader(techs[Technique_LIGHT][LightCode_POINTNOSHADOWS]->GetPixelShader());
//
//			}
//		}
//
//		//Draw Light
//		card->Draw(1, 0);
//
//		lightNode = lightNode->GetNext();
//
//	}
//
//	if (dStyle.useParticle)
//	{
//
//		//Prep particles
//		ID3D11RenderTargetView* target = mtrs[MRTVal_AMB]->GetDXT()->GetTargetView();
//		card->OMSetRenderTargets(1, &target, nullptr);
//		card->IASetInputLayout(bH->GetParticleLayout());
//		card->OMSetBlendState(blendStates[BlendCode_ALPHA], nullptr, 0xFFFFFFFF);
//		card->GSSetConstantBuffers(0, 1, &fcb);
//		card->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
//
//		if (!wf)
//		{
//
//			card->RSSetState(rastStates[RastState_NOCULL]);
//
//		}
//		else
//		{
//
//			card->RSSetState(rastStates[RastState_WIREFRAME]);
//
//		}
//		
//		//Draw all particles
//		CelestialListNode<IParticleEmitter*>* particleNode = scene->GetParticleSystem()->GetFirstNode();
//		offSets[0] = 0;
//		offSets[1] = 0;
//
//		while (particleNode != nullptr && particleNode->GetNodeObject() != nullptr)
//		{
//		
//			IParticleEmitter* particleSystem = particleNode->GetNodeObject();
//
//			if (particleSystem->GetInputTexture() != nullptr)
//			{
//
//				srvs[0] = particleSystem->GetInputTexture()->GetDXT()->GetShaderView();
//				card->PSSetShaderResources(SRVRegisters_PARTMAT, 1, srvs);
//
//			}
//
//			strides[0] = particleSystem->GetVertexStride();
//			strides[1] = particleSystem->GetInstanceStride();
//			DXBufferObject* iBuffer = particleSystem->GetBuffer(BufferTypes_INSTANCE,flip)->GetFirstDXBuffer();
//
//			if (particleSystem->GetType() == ParticleSystem_Light)
//			{
//
//				card->VSSetShader(techs[Technique_PARTICLE][ParticleCode_LIGHT]->GetVertexShader());
//				card->GSSetShader(techs[Technique_PARTICLE][ParticleCode_LIGHT]->GetGeometryShader());
//				card->PSSetShader(techs[Technique_PARTICLE][ParticleCode_LIGHT]->GetPixelShader());
//
//			}
//			else if (particleSystem->GetType() == ParticleSystem_SkyBox)
//			{
//
//				card->VSSetShader(techs[Technique_PARTICLE][ParticleCode_SKYBOX]->GetVertexShader());
//				card->GSSetShader(techs[Technique_PARTICLE][ParticleCode_SKYBOX]->GetGeometryShader());
//				card->PSSetShader(techs[Technique_PARTICLE][ParticleCode_SKYBOX]->GetPixelShader());
//
//			}
//
//			while(iBuffer != nullptr)
//			{
//		
//				DXBufferObject* vBuffer = particleSystem->GetBuffer(BufferTypes_VERTEX,flip)->GetFirstDXBuffer();
//
//				while(vBuffer != nullptr)
//				{
//
//					if(iBuffer->GetMappedPointer() != nullptr)
//					{
//
//						iBuffer->UnMap();
//
//					}
//
//					buffers[0] = vBuffer->GetBuffer();
//					buffers[1] = iBuffer->GetBuffer();
//					card->IASetVertexBuffers(0,2,buffers,strides,offSets);//Set buffers
//					card->DrawInstanced(vBuffer->GetSize(),iBuffer->GetSize(),0,0);
//					vBuffer = vBuffer->GetNext();
//
//				}
//
//				iBuffer = iBuffer->GetNext();
//
//			}
//
//			particleNode = particleNode->GetNext();
//
//		}
//	}
//
//	//Prepare to draw on screen
//	//card->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
//
//	card->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
//	card->IASetInputLayout(screenLayout);
//	offSets[0] = 0;
//	card->IASetVertexBuffers(0, 1, &buffers[2], &screenStride, &offSets[0]);//Put in 1 vertex to act as a point and build our quad in the GS
//
//	if (false && dStyle.motionBlur)
//	{
//
//		//Flip the targets so we write to what we read and read from what we wrote
//		srvs[0] = mtrs[MRTVal_OLD]->GetDXT()->GetShaderView();
//		srvs[1] = mtrs[MRTVal_VEL]->GetDXT()->GetShaderView();
//		card->PSSetShaderResources(SRVRegisters_SCREEN, 2, srvs);
//		ID3D11RenderTargetView* target = mtrs[MRTVal_OLD]->GetDXT()->GetTargetView();
//		card->OMSetRenderTargets(1, &target, nullptr);
//
//		//Blur the results
//
//		card->VSSetShader(techs[Technique_SCREEN][ScreenCode_MOTIONBLUR]->GetVertexShader());
//		card->GSSetShader(techs[Technique_SCREEN][ScreenCode_MOTIONBLUR]->GetGeometryShader());
//		card->PSSetShader(techs[Technique_SCREEN][ScreenCode_MOTIONBLUR]->GetPixelShader());
//
//		card->Draw(1, 0);
//
//	}
//
//	delete[] target;
//
//}

void CelestialShader::FinishDrawing(ID3D11DeviceContext* context)
{

	for(int i=0;i<SRVRegisters_SIZE;i++)
	{
		
		srvs[i] = nullptr;

	}

	context->PSSetShaderResources(0, SRVRegisters_SIZE, srvs);
	context->OMSetRenderTargets(0, nullptr, nullptr);

}

CelestialBufferHandler* CelestialShader:: GetBufferHandler()
{

	return bH;

}

void CelestialShader::transferRenderConstants(ID3D11DeviceContext* context)
{

	D3D11_MAPPED_SUBRESOURCE mapped = D3D11_MAPPED_SUBRESOURCE();
	context->Map(rcb, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	memcpy(mapped.pData, &rc, sizeof(renderConstants));
	context->Unmap(rcb, 0);
	context->PSSetConstantBuffers(BufferRegisters_RENDER, 1, &rcb);

}

void CelestialShader::transferFrameConstants(ID3D11DeviceContext* context)
{
	
	D3D11_MAPPED_SUBRESOURCE mapped = D3D11_MAPPED_SUBRESOURCE();
	context->Map(fcb, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	memcpy(mapped.pData, &fc, sizeof(frameConstants));
	context->Unmap(fcb, 0);

	context->VSSetConstantBuffers(BufferRegisters_FRAME, 1, &fcb);
	context->GSSetConstantBuffers(BufferRegisters_FRAME, 1, &fcb);
	context->PSSetConstantBuffers(BufferRegisters_FRAME, 1, &fcb);

}

void CelestialShader::transferLightConstants(ID3D11DeviceContext* context)
{

	D3D11_MAPPED_SUBRESOURCE mapped = D3D11_MAPPED_SUBRESOURCE();
	context->Map(plcb, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	memcpy(mapped.pData, &plc, sizeof(perLightConstants));
	context->Unmap(plcb, 0);

	context->GSSetConstantBuffers(BufferRegisters_LIGHT, 1, &plcb);
	context->PSSetConstantBuffers(BufferRegisters_LIGHT, 1, &plcb);

}

void CelestialShader::Release()
{

	delete sMC;
	screenLayout->Release();
	buffers[2]->Release();
	ID3D11SamplerState* temp[] = {nullptr,nullptr,nullptr};

	for(int i=0;i<VertexShaders_SIZE;i++)
	{

		vertexShaders[i]->Release();

	}

	for(int i=0;i<GeometryShaders_SIZE;i++)
	{

		geometryShaders[i]->Release();

	}

	for(int i=0;i<PixelShaders_SIZE;i++)
	{

		pixelShaders[i]->Release();

	}


	for (int i = 0; i < Technique_SIZE; i++)
	{

		int tSize = 0;

		if (i == Technique_PREDRAW)
		{

			tSize = PreDrawCode_SIZE;

		}
		else if (i == Technique_GEOMETRY)
		{

			tSize = GeometryCode_SIZE;

		}
		else if (i == Technique_TERRAIN)
		{

			tSize = GeometryCode_SIZE;

		}
		else if (i == Technique_SHADOW)
		{

			tSize = CelestialShader::ShadowCode_SIZE;

		}
		else if (i == Technique_PARTICLE)
		{

			tSize = ParticleCode_SIZE;

		}
		else if (i == Technique_SCREEN)
		{

			tSize = ScreenCode_SIZE;

		}
		else if (i == Technique_DEBUG)
		{

			tSize = DebugCode_SIZE;

		}
		else if (i == Technique_LIGHT)
		{

			tSize = LightCode_SIZE;

		}

		for (int j = 0; j < tSize; j++)
		{

			if (techs[i][j] != nullptr)
			{

				delete techs[i][j];

			}
		}
	}

	for(int i=0;i<BlendCode_SIZE;i++)
	{

		blendStates[i]->Release();

	}

	for(int i=0;i<DepthCode_SIZE;i++)
	{

		depthStates[i]->Release();

	}

	for(int i=0;i<RastState_SIZE;i++)
	{

		rastStates[i]->Release();

	}

	for(int i=0;i<SampleState_SIZE;i++)
	{

		sampleStates[i]->Release();

	}

	releaseTargets();
	releaseDepth();
	releaseShadowMap();

	rcb->Release();
	fcb->Release();
	plcb->Release();
	
}
		
CelestialShader::~CelestialShader()
{

	delete[] buffers;
	delete[] strides;
	delete[] offSets;
	delete[] srvs;

	delete[] vertexShaders;
	delete[] geometryShaders;
	delete[] pixelShaders;

	for(int i=0;i<Technique_SIZE;i++)
	{

		delete[] techs[i];

	}

	for (int i = 1; i < MRTVal_SIZE; i++)
	{

		delete mtrs[i];

	}

	if (shadowMap != nullptr)
	{

		delete shadowMap;

	}

	if (shadowInfo != nullptr)
	{

		delete shadowInfo;

	}

	if (shadowFactor != nullptr)
	{

		delete shadowFactor;

	}

	if (shadowFactor2 != nullptr)
	{

		delete shadowFactor2;

	}

	if (dpthStnc != nullptr)
	{

		delete dpthStnc;

	}

	delete[] mtrs;
	delete[] techs;
	
	delete[] blendStates;
	delete[] depthStates;
	delete[] rastStates;
	delete[] sampleStates;

}
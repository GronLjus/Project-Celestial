#include "Shaders_Registers.hlsli"

struct GS_INPUTSHADOW
{
	float4 Pos : SV_POSITION;
};

struct PS_INPUTSCREEN
{
	
	float4 Pos : SV_POSITION;
	float2 Texcoord: TEXCOORD0;

};

/*Render predraw effects*/
GS_INPUTSHADOW VSPREDRAW(float empty : EMPTY)
{

	GS_INPUTSHADOW retVal;
	retVal.Pos = float4(0,0,0,0);
	return retVal;
}

[maxvertexcount(24)]//6*4
void GSSKYCUBE( point GS_INPUTSHADOW input[1], inout TriangleStream<PS_INPUTSCREEN> TStream )
{
	
	//Front Face
	PS_INPUTSCREEN p;
	p.Pos = mul(float4(CameraPos.x-1,CameraPos.y+1,CameraPos.z+1,1),ViewProjection);
	p.Texcoord = float2(0,0);
	TStream.Append(p);

	p.Pos = mul(float4(CameraPos.x+1,CameraPos.y+1,CameraPos.z+1,1),ViewProjection);
	p.Texcoord = float2(1.0f/3.0f,0);
	TStream.Append(p);
	
	p.Pos = mul(float4(CameraPos.x-1,CameraPos.y-1,CameraPos.z+1,1),ViewProjection);
	p.Texcoord = float2(0,1.0f/2.0f);
	TStream.Append(p);
	
	p.Pos = mul(float4(CameraPos.x+1,CameraPos.y-1,CameraPos.z+1,1),ViewProjection);
	p.Texcoord = float2(1.0f/3.0f,1.0f/2.0f);
	TStream.Append(p);
	TStream.RestartStrip();

	//Left Face
	p.Pos = mul(float4(CameraPos.x-1,CameraPos.y+1,CameraPos.z-1,1),ViewProjection);
	p.Texcoord = float2(1.0f/3.0f,0);
	TStream.Append(p);

	p.Pos = mul(float4(CameraPos.x-1,CameraPos.y+1,CameraPos.z+1,1),ViewProjection);
	p.Texcoord = float2(2.0f/3.0f,0);
	TStream.Append(p);
	
	p.Pos = mul(float4(CameraPos.x-1,CameraPos.y-1,CameraPos.z-1,1),ViewProjection);
	p.Texcoord = float2(1.0f/3.0f,1.0f/2.0f);
	TStream.Append(p);
	
	p.Pos = mul(float4(CameraPos.x-1,CameraPos.y-1,CameraPos.z+1,1),ViewProjection);
	p.Texcoord = float2(2.0f/3.0f,1.0f/2.0f);
	TStream.Append(p);
	TStream.RestartStrip();

	//Back Face
	p.Pos = mul(float4(CameraPos.x+1,CameraPos.y+1,CameraPos.z-1,1),ViewProjection);
	p.Texcoord = float2(2.0f/3.0f,0);
	TStream.Append(p);

	p.Pos = mul(float4(CameraPos.x-1,CameraPos.y+1,CameraPos.z-1,1),ViewProjection);
	p.Texcoord = float2(1,0);
	TStream.Append(p);
	
	p.Pos = mul(float4(CameraPos.x+1,CameraPos.y-1,CameraPos.z-1,1),ViewProjection);
	p.Texcoord = float2(2.0f/3.0f,1.0f/2.0f);
	TStream.Append(p);
	
	p.Pos = mul(float4(CameraPos.x-1,CameraPos.y-1,CameraPos.z-1,1),ViewProjection);
	p.Texcoord = float2(1,1.0f/2.0f);
	TStream.Append(p);
	TStream.RestartStrip();
	
	//Right Face
	p.Pos = mul(float4(CameraPos.x+1,CameraPos.y+1,CameraPos.z+1,1),ViewProjection);
	p.Texcoord = float2(0,1.0f/2.0f);
	TStream.Append(p);

	p.Pos = mul(float4(CameraPos.x+1,CameraPos.y+1,CameraPos.z-1,1),ViewProjection);
	p.Texcoord = float2(1.0f/3.0f,1.0f/2.0f);
	TStream.Append(p);
	
	p.Pos = mul(float4(CameraPos.x+1,CameraPos.y-1,CameraPos.z+1,1),ViewProjection);
	p.Texcoord = float2(0,1);
	TStream.Append(p);
	
	p.Pos = mul(float4(CameraPos.x+1,CameraPos.y-1,CameraPos.z-1,1),ViewProjection);
	p.Texcoord = float2(1.0f/3.0f,1);
	TStream.Append(p);
	TStream.RestartStrip();
	
	//Up Face
	p.Pos = mul(float4(CameraPos.x-1,CameraPos.y+1,CameraPos.z-1,1),ViewProjection);
	p.Texcoord = float2(1.0f/3.0f,1.0f/2.0f);
	TStream.Append(p);

	p.Pos = mul(float4(CameraPos.x+1,CameraPos.y+1,CameraPos.z-1,1),ViewProjection);
	p.Texcoord = float2(2.0f/3.0f,1.0f/2.0f);
	TStream.Append(p);
	
	p.Pos = mul(float4(CameraPos.x-1,CameraPos.y+1,CameraPos.z+1,1),ViewProjection);
	p.Texcoord = float2(1.0f/3.0f,1);
	TStream.Append(p);
	
	p.Pos = mul(float4(CameraPos.x+1,CameraPos.y+1,CameraPos.z+1,1),ViewProjection);
	p.Texcoord = float2(2.0f/3.0f,1);
	TStream.Append(p);
	TStream.RestartStrip();
	
	//Down Face
	p.Pos = mul(float4(CameraPos.x-1,CameraPos.y-1,CameraPos.z+1,1),ViewProjection);
	p.Texcoord = float2(2.0f/3.0f,1.0f/2.0f);
	TStream.Append(p);

	p.Pos = mul(float4(CameraPos.x+1,CameraPos.y-1,CameraPos.z+1,1),ViewProjection);
	p.Texcoord = float2(1,1.0f/2.0f);
	TStream.Append(p);
	
	p.Pos = mul(float4(CameraPos.x-1,CameraPos.y-1,CameraPos.z-1,1),ViewProjection);
	p.Texcoord = float2(2.0f/3.0f,1);
	TStream.Append(p);
	
	p.Pos = mul(float4(CameraPos.x+1,CameraPos.y-1,CameraPos.z-1,1),ViewProjection);
	p.Texcoord = float2(1,1);
	TStream.Append(p);
	TStream.RestartStrip();

}

float4 SKYBOX(PS_INPUTSCREEN input) : SV_Target
{

	return skyboxTex.Sample(SampleType, input.Texcoord);

}
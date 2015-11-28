static const float epsilon = 0.0009999f;
static const float pi = 3.14159265f;

cbuffer frameConstants : register(b0)
{

	float4x4 View : packoffset(c0);
	float4x4 Projection : packoffset(c4);
	float4x4 ViewProjection : packoffset(c8);
	float4x4 InvertViewProjection : packoffset(c12);
	float4x4 LastViewProjection : packoffset(c16);

	float4 DebugCol : packoffset(c20);
	float4 GlobalAmbience : packoffset(c21);
	float4 CameraPos : packoffset(c22);
	float4 GlobalLight : packoffset(c23);
	float4 GlobalDirection : packoffset(c24);

	float BlurSamples : packoffset(c25.x);
	int FrameTime : packoffset(c25.y);
	float BorderThickness : packoffset(c25.z);
	float ZNear : packoffset(c25.w);

	float FOV : packoffset(c26.x);
	bool ShadowVolumes : packoffset(c26.y);

};

cbuffer renderConstants : register(b1)
{

	bool UseNormal[16];

};

cbuffer perLightConstants : register(b2)
{

	float LightFalloff : packoffset(c0.x);
	float InvRenderWidth : packoffset(c0.y);
	float InvRenderHeight : packoffset(c0.z);
	float BlockerSearch : packoffset(c0.w);
	float ShadowSearch : packoffset(c1.x);

	float3 Position : packoffset(c1.y);
	float3 Forward : packoffset(c2);
	float Radius : packoffset(c2.w);
	float3 Right : packoffset(c3);
	float Angle : packoffset(c3.w);
	float4 Diffuse : packoffset(c4);
	float4 Specular : packoffset(c5);
	float Size : packoffset(c6.x);

	float4 RenderCenter[6] : packoffset(c7);

};

//The ambiant colours
Texture2D amb : register(t0);
//The diffuse colours
Texture2D diff : register(t1);
//The normals of each pixel
Texture2D norm : register(t2);

//The depths of each pixel from the eye
Texture2D<float> depth : register(t3);
//The depths of each pixel from the light
Texture2D<float> shadowMap : register(t4);
//The shadowstencil
Texture2D<uint2> shadowMapStencil : register(t5);

//The shadowinfo of each pixel
Texture2D shadowInfo : register(t6);
//The material of the skybox
Texture2D skyboxTex : register(t7);

//The current screen
Texture2D screen : register(t8);
//The velocity of each pixel
Texture2D velocity : register(t9);

SamplerState SampleType : register(s0)
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Clamp;
	AddressV = Clamp;

};
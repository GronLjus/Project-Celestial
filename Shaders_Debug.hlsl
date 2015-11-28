#include "Shaders_Registers.hlsli"

struct GS_INPUTRENDER
{
	float4 Pos : SV_POSITION;
	float2 Texcoord: TEXCOORD0;
	float4x4 OldMatrix : TEXCOORD1;
	float4x4 InvMatrix : TEXCOORD5;
	float3 Normal : NORMAL0;

};

struct PS_INPUTSHADOW
{

	float4 Pos : SV_POSITION;
	uint Viewport : SV_ViewportArrayIndex;

};

/*Render debugdata*/
[maxvertexcount(2)]//2 points in a line
void GSNORMALSPIKE(point GS_INPUTRENDER input[1], inout LineStream<PS_INPUTSHADOW> Ltream)
{

	PS_INPUTSHADOW o1;
	o1.Pos = input[0].Pos;
	o1.Pos = mul(o1.Pos,ViewProjection);
	o1.Viewport = 0;

	PS_INPUTSHADOW o2;
	o2.Pos = input[0].Pos+float4(input[0].Normal*1,0);
	o2.Pos = mul(o2.Pos,ViewProjection);
	o2.Viewport = 0;
	
	Ltream.Append(o1);
	Ltream.Append(o2);
	Ltream.RestartStrip();

}

float4 DEBUGOUT(float4 pos:SV_POSITION):SV_Target
{

	return float4(DebugCol.xyz,1);
	return float4(DebugCol.x*(1-pos.z/pos.w),DebugCol.y*(1-pos.z/pos.w),DebugCol.z*(1-pos.z/pos.w),1);

}
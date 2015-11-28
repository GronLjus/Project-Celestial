static const float motionEpsilon = 0.009f;
static const float gridEpsilon = 0.05f;

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

/*Render fullscreen effects*/
GS_INPUTSHADOW VSSCREEN(float empty : EMPTY)
{

	GS_INPUTSHADOW retVal;
	retVal.Pos = float4(0,0,0,0);
	return retVal;

}

[maxvertexcount(4)]
void GSSCREEN( point GS_INPUTSHADOW input[1], inout TriangleStream<PS_INPUTSCREEN> TStream )
{
	
	PS_INPUTSCREEN p;
	p.Pos = float4(-1,1,0,1);
	p.Texcoord = float2(0,0);
	TStream.Append(p);

	PS_INPUTSCREEN p2;
	p2.Pos = float4(1,1,0,1);
	p2.Texcoord = float2(1,0);
	TStream.Append(p2);
	
	PS_INPUTSCREEN p3;
	p3.Pos = float4(-1,-1,0,1);
	p3.Texcoord = float2(0,1);
	TStream.Append(p3);
	
	PS_INPUTSCREEN p4;
	p.Pos = float4(1,-1,0,1);
	p.Texcoord = float2(1,1);
	TStream.Append(p);

	TStream.RestartStrip();

}

float4 MOTIONBLUR(PS_INPUTSCREEN psInput):SV_Target
{

	//Sample the velocity of the current pixel
	float4 colour = velocity.Sample(SampleType, psInput.Texcoord);
	//Get the starting output
	float4 outPut = screen.Sample(SampleType, psInput.Texcoord);

	if(colour.b == 0)
	{

		return outPut;//clip(-1);

	}

	outPut.a = 1;
	int3 size;
	screen.GetDimensions(0,size.x,size.y,size.z);

	float2 vel = colour.rg-psInput.Texcoord;

	if(!(vel.x > -motionEpsilon && vel.x < motionEpsilon) || !(vel.y > -motionEpsilon && vel.y < motionEpsilon) )
	{

		float2 sampledVel = vel/BlurSamples;

		for(int i=1;i!=BlurSamples;i++)
		{

			float2 localPos = psInput.Texcoord + sampledVel*i;
			int2 values = int2(localPos.x*size.x,localPos.y*size.y);
			outPut += screen.Load(int3(values,0));

		}

		//average the colour
		outPut /= BlurSamples;
		saturate(outPut);

	}

	return outPut;

}

float4 GRID(PS_INPUTSCREEN input) : SV_Target
{
	
	float4 position;//Convert to clipspace
	position.x = 2*input.Texcoord.x-1;
	position.y = -(2*input.Texcoord.y-1);
	position.z = depth.Sample(SampleType, input.Texcoord).x;
	position.w = 1.0f;

	position = mul(position, InvertViewProjection);//Convert to worldspace
	position /= position.w;

	if(position.y >= 0 )
	{

		clip(-1);

	}

	float4 vec = position-CameraPos;
	float varX = CameraPos.y/vec.y;
	float3 newPos = CameraPos.xyz-(vec.xyz*varX);

	if(abs(newPos.x)%1 > gridEpsilon && abs(newPos.z)%1 > gridEpsilon)
	{

		clip(-1);

	}

	return DebugCol;

}
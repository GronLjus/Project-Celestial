#include "Shaders_Registers.hlsli"

struct VS_INPUTPARTICLE
{

	float3 Pos : POSITION; 
	float3 Vel : VELOCITY; 
	float3 Size: SIZE;
	float3 Time : TIME;
	int ParticleStartTime : PSTIME;

	int StartTime : STIME;
	float4x4 Transform : TRANSFORMATION; 
	float2 Factors : FACTOR;
	float3 Colour : COLOR;
	int EndTime : ETIME;
	float Glow : GLOW;
	float GFade : GFADE;

};

struct GS_INPUTPARTICLE
{

	float4 Pos : SV_POSITION;
	float3 Size: TEXCOORD0;
	bool Discard:TEXCOORD1;
	float Alpha:TEXCOORD2;
	float Glow:TEXCOORD3;
	float Fade:TEXCOORD4;
	float3 Colour : COLOR0;

};

struct PS_INPUTSCREEN
{

	float4 Pos : SV_POSITION;
	float2 Texcoord: TEXCOORD0;

};

struct PS_INPUTPARTICLE
{
	
	float4 Pos : SV_POSITION;
	float2 Texcoord: TEXCOORD0;
	float Alpha:TEXCOORD1;
	float Glow:TEXCOORD2;
	float Fade:TEXCOORD3;
	float3 Colour : COLOR0;

};

struct PS_INPUTPARTICLE4
{
	PS_INPUTPARTICLE p1;
	PS_INPUTPARTICLE p2;
	PS_INPUTPARTICLE p3;
	PS_INPUTPARTICLE p4;
};

struct PS_PARTICLEOUTPUT
{
	
	float4 Colour : SV_Target0;
	//float Depth : SV_Depth;

};

/*Render Particles*/
GS_INPUTPARTICLE VSPARTICLE(VS_INPUTPARTICLE input)
{

	GS_INPUTPARTICLE retVal = (GS_INPUTPARTICLE)0;

	float currentTime = FrameTime - input.StartTime;

	retVal.Discard = (input.ParticleStartTime > currentTime) || (input.Time.z >= 0 && currentTime > input.Time.z) || (input.EndTime >= 0 && currentTime > input.EndTime);

	if(!retVal.Discard)
	{
	
		currentTime -= input.ParticleStartTime;
		currentTime = currentTime%input.Time.x;
		retVal.Colour = input.Colour;
		retVal.Pos = mul(float4(input.Pos+(input.Vel*input.Factors.x)*(currentTime),1),input.Transform);
		retVal.Size = float3(input.Size*input.Factors.y);
		retVal.Glow = input.Glow;
		retVal.Fade = input.GFade;
		float alpha = 1;

		if(input.EndTime >= 0)
		{

			alpha -= currentTime/input.EndTime;

		}

		float timeFade = input.Time.x-input.Time.y;
		float glow = input.Glow;
		currentTime -= input.Time.y;

		if(timeFade > 0 && currentTime >= 0)
		{

			alpha -= currentTime/timeFade;
			
			float glowAlpha = alpha;

			if(alpha > 0.75f)
			{
				
				glow *= ((1-((alpha-0.75)/0.25f))*1/glow);

				if(glow < input.Glow)
				{

					glow = input.Glow;

				}
			}
			else
			{

				glow = 1;
				retVal.Fade = 0;

			}
		}
		
		retVal.Alpha = alpha;
		retVal.Glow = glow;

	}

	return retVal;

}

PS_INPUTPARTICLE4 commonParticleGeometry(GS_INPUTPARTICLE input[1])
{

	PS_INPUTPARTICLE4 outPut;
	float4 viewPoint = mul(input[0].Pos,View);

	outPut.p1.Pos = mul(float4(viewPoint.x-input[0].Size.x,viewPoint.y+input[0].Size.y,viewPoint.z,1),Projection);
	outPut.p1.Texcoord = float2(0,0);
	outPut.p1.Alpha = input[0].Alpha;
	outPut.p1.Colour = input[0].Colour;
	outPut.p1.Glow = input[0].Glow;
	outPut.p1.Fade = input[0].Fade;

	outPut.p2.Pos = mul(float4(viewPoint.x+input[0].Size.x,viewPoint.y+input[0].Size.y,viewPoint.z,1),Projection);
	outPut.p2.Texcoord = float2(1,0);
	outPut.p2.Alpha = input[0].Alpha;
	outPut.p2.Colour = input[0].Colour;
	outPut.p2.Glow = input[0].Glow;
	outPut.p2.Fade = input[0].Fade;

	outPut.p3.Pos = mul(float4(viewPoint.x-input[0].Size.x,viewPoint.y-input[0].Size.y,viewPoint.z,1),Projection);
	outPut.p3.Texcoord = float2(0,1);
	outPut.p3.Alpha = input[0].Alpha;
	outPut.p3.Colour = input[0].Colour;
	outPut.p3.Glow = input[0].Glow;
	outPut.p3.Fade = input[0].Fade;
		
	outPut.p4.Pos = mul(float4(viewPoint.x+input[0].Size.x,viewPoint.y-input[0].Size.y,viewPoint.z,1),Projection);
	outPut.p4.Texcoord = float2(1,1);
	outPut.p4.Alpha = input[0].Alpha;
	outPut.p4.Colour = input[0].Colour;
	outPut.p4.Glow = input[0].Glow;
	outPut.p4.Fade = input[0].Fade;
	return outPut;

}

[maxvertexcount(4)]//4 points
void GSPARTICLETRIANGLE(point GS_INPUTPARTICLE input[1], inout TriangleStream<PS_INPUTPARTICLE> Ttream)
{

	if(!input[0].Discard)
    {

		PS_INPUTPARTICLE4 outGoing = commonParticleGeometry(input[0]);
		Ttream.Append(outGoing.p1);
		Ttream.Append(outGoing.p2);
		Ttream.Append(outGoing.p3);
		Ttream.Append(outGoing.p4);
		Ttream.RestartStrip();

	}
}

[maxvertexcount(24)]//6*4
void GSSKYCUBE(point GS_INPUTPARTICLE input[1], inout TriangleStream<PS_INPUTSCREEN> TStream)
{

	//Front Face
	PS_INPUTSCREEN p;
	p.Pos = mul(float4(CameraPos.x - 1, CameraPos.y + 1, CameraPos.z + 1, 1), ViewProjection);
	p.Texcoord = float2(0, 0);
	TStream.Append(p);

	p.Pos = mul(float4(CameraPos.x + 1, CameraPos.y + 1, CameraPos.z + 1, 1), ViewProjection);
	p.Texcoord = float2(1.0f / 3.0f, 0);
	TStream.Append(p);

	p.Pos = mul(float4(CameraPos.x - 1, CameraPos.y - 1, CameraPos.z + 1, 1), ViewProjection);
	p.Texcoord = float2(0, 1.0f / 2.0f);
	TStream.Append(p);

	p.Pos = mul(float4(CameraPos.x + 1, CameraPos.y - 1, CameraPos.z + 1, 1), ViewProjection);
	p.Texcoord = float2(1.0f / 3.0f, 1.0f / 2.0f);
	TStream.Append(p);
	TStream.RestartStrip();

	//Left Face
	p.Pos = mul(float4(CameraPos.x - 1, CameraPos.y + 1, CameraPos.z - 1, 1), ViewProjection);
	p.Texcoord = float2(1.0f / 3.0f, 0);
	TStream.Append(p);

	p.Pos = mul(float4(CameraPos.x - 1, CameraPos.y + 1, CameraPos.z + 1, 1), ViewProjection);
	p.Texcoord = float2(2.0f / 3.0f, 0);
	TStream.Append(p);

	p.Pos = mul(float4(CameraPos.x - 1, CameraPos.y - 1, CameraPos.z - 1, 1), ViewProjection);
	p.Texcoord = float2(1.0f / 3.0f, 1.0f / 2.0f);
	TStream.Append(p);

	p.Pos = mul(float4(CameraPos.x - 1, CameraPos.y - 1, CameraPos.z + 1, 1), ViewProjection);
	p.Texcoord = float2(2.0f / 3.0f, 1.0f / 2.0f);
	TStream.Append(p);
	TStream.RestartStrip();

	//Back Face
	p.Pos = mul(float4(CameraPos.x + 1, CameraPos.y + 1, CameraPos.z - 1, 1), ViewProjection);
	p.Texcoord = float2(2.0f / 3.0f, 0);
	TStream.Append(p);

	p.Pos = mul(float4(CameraPos.x - 1, CameraPos.y + 1, CameraPos.z - 1, 1), ViewProjection);
	p.Texcoord = float2(1, 0);
	TStream.Append(p);

	p.Pos = mul(float4(CameraPos.x + 1, CameraPos.y - 1, CameraPos.z - 1, 1), ViewProjection);
	p.Texcoord = float2(2.0f / 3.0f, 1.0f / 2.0f);
	TStream.Append(p);

	p.Pos = mul(float4(CameraPos.x - 1, CameraPos.y - 1, CameraPos.z - 1, 1), ViewProjection);
	p.Texcoord = float2(1, 1.0f / 2.0f);
	TStream.Append(p);
	TStream.RestartStrip();

	//Right Face
	p.Pos = mul(float4(CameraPos.x + 1, CameraPos.y + 1, CameraPos.z + 1, 1), ViewProjection);
	p.Texcoord = float2(0, 1.0f / 2.0f);
	TStream.Append(p);

	p.Pos = mul(float4(CameraPos.x + 1, CameraPos.y + 1, CameraPos.z - 1, 1), ViewProjection);
	p.Texcoord = float2(1.0f / 3.0f, 1.0f / 2.0f);
	TStream.Append(p);

	p.Pos = mul(float4(CameraPos.x + 1, CameraPos.y - 1, CameraPos.z + 1, 1), ViewProjection);
	p.Texcoord = float2(0, 1);
	TStream.Append(p);

	p.Pos = mul(float4(CameraPos.x + 1, CameraPos.y - 1, CameraPos.z - 1, 1), ViewProjection);
	p.Texcoord = float2(1.0f / 3.0f, 1);
	TStream.Append(p);
	TStream.RestartStrip();

	//Up Face
	p.Pos = mul(float4(CameraPos.x - 1, CameraPos.y + 1, CameraPos.z - 1, 1), ViewProjection);
	p.Texcoord = float2(1.0f / 3.0f, 1.0f / 2.0f);
	TStream.Append(p);

	p.Pos = mul(float4(CameraPos.x + 1, CameraPos.y + 1, CameraPos.z - 1, 1), ViewProjection);
	p.Texcoord = float2(2.0f / 3.0f, 1.0f / 2.0f);
	TStream.Append(p);

	p.Pos = mul(float4(CameraPos.x - 1, CameraPos.y + 1, CameraPos.z + 1, 1), ViewProjection);
	p.Texcoord = float2(1.0f / 3.0f, 1);
	TStream.Append(p);

	p.Pos = mul(float4(CameraPos.x + 1, CameraPos.y + 1, CameraPos.z + 1, 1), ViewProjection);
	p.Texcoord = float2(2.0f / 3.0f, 1);
	TStream.Append(p);
	TStream.RestartStrip();

	//Down Face
	p.Pos = mul(float4(CameraPos.x - 1, CameraPos.y - 1, CameraPos.z + 1, 1), ViewProjection);
	p.Texcoord = float2(2.0f / 3.0f, 1.0f / 2.0f);
	TStream.Append(p);

	p.Pos = mul(float4(CameraPos.x + 1, CameraPos.y - 1, CameraPos.z + 1, 1), ViewProjection);
	p.Texcoord = float2(1, 1.0f / 2.0f);
	TStream.Append(p);

	p.Pos = mul(float4(CameraPos.x - 1, CameraPos.y - 1, CameraPos.z - 1, 1), ViewProjection);
	p.Texcoord = float2(2.0f / 3.0f, 1);
	TStream.Append(p);

	p.Pos = mul(float4(CameraPos.x + 1, CameraPos.y - 1, CameraPos.z - 1, 1), ViewProjection);
	p.Texcoord = float2(1, 1);
	TStream.Append(p);
	TStream.RestartStrip();

}

PS_PARTICLEOUTPUT LIGHTPARTICLE(PS_INPUTPARTICLE input)
{
	
	float depthHere = depth.Load(int3(input.Pos.x,input.Pos.y,0)).r;
	float particleDepth = input.Pos.z;///input.Pos.w;
	PS_PARTICLEOUTPUT outPut;
	float distance = length(input.Texcoord.xy*2-1);
	float alpha = input.Alpha;
	float3 colour = float3(1,1,1);

	if(distance > input.Alpha || depthHere < particleDepth)
	{

		clip(-1);

	}

	if(distance >= 1-input.Glow)
	{
	
		//Renormalize
		distance -= 1-input.Glow;
		distance /= input.Glow;
		float glowDistance = distance;

		if(distance > input.Fade)
		{

			//Renormalize
			distance -= input.Fade;
			distance /= 1-input.Fade;
			colour = input.Colour*1-distance;

		}
		else
		{

			//Renormalize
			distance /= input.Fade;
			float3 inverse = colour - input.Colour;
			colour -= inverse*distance;

		}

		alpha -= glowDistance;

	}

	outPut.Colour = float4(colour,alpha);
	return outPut;

}

float4 SKYBOX(PS_INPUTSCREEN input) : SV_Target
{

	float depthHere = depth.Load(int3(input.Pos.x, input.Pos.y, 0)).r;

	if (depthHere < 1.0f)
	{

		clip(-1);

	}

	return skyboxTex.Sample(SampleType, input.Texcoord);

}
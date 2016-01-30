static const float epsilon2 = 0.00000002f;

#include "Shaders_Registers.hlsli"

struct VS_INPUTRENDER
{

	float3 Pos : POSITION; 
	float2 Texcoord: TEXCOORD;
	float3 Normal:NORMAL;
	float4x4 World: MTRANSFORM;
	float4x4 OldMatrix: OMTRANSFORM;
	float4x4 InvWorldTran:INVTRANMTRANSFORM;

};

struct VS_INPUTTERRAIN
{

	float3 Pos : POSITION; 
	float2 Texcoord: TEXCOORD;
	float3 Normal:NORMAL;

};

struct GS_INPUTRENDER
{
	float4 Pos : SV_POSITION;
	float2 Texcoord: TEXCOORD0;
	float4x4 OldMatrix : TEXCOORD1;
	float4x4 InvMatrix : TEXCOORD5;
	float3 Normal : NORMAL0;

};

struct PS_INPUTRENDER
{

	float4 Pos : SV_POSITION;
	float2 Texcoord: TEXCOORD0;
	float4 OldPos: POSITION0;
	float3 Normal : NORMAL0;
	bool Border : BORDER0;

};

struct PS_PARTICLEOUTPUT
{
	
	float4 Colour : SV_Target0;
	//float Depth : SV_Depth;

};

struct PS_OUTPUT4
{

	float4 AmbColor : SV_Target0;
	float4 DiffColor : SV_Target1;
	float4 Normal : SV_Target2;
	float4 Velocity : SV_Target3;

};

/*Render geometry*/
GS_INPUTRENDER VS(VS_INPUTRENDER input)
{
	
	GS_INPUTRENDER gsInput;
	gsInput.Pos = mul(float4(input.Pos,1),input.World);
	gsInput.InvMatrix = transpose(input.InvWorldTran);
	gsInput.OldMatrix = mul(mul(gsInput.InvMatrix,input.OldMatrix),LastViewProjection);
	gsInput.Texcoord = input.Texcoord;
	gsInput.Normal = mul(float4(input.Normal,1),input.InvWorldTran).xyz;
	return gsInput;

}

GS_INPUTRENDER VSTERRAIN(VS_INPUTTERRAIN input, uint VID:SV_VertexID)
{
	
	GS_INPUTRENDER gsInput;
	gsInput.Pos = float4(input.Pos,1);
	gsInput.OldMatrix = float4x4(1,0,0,0,
								 0,1,0,0,
								 0,0,1,0,
								 0,0,0,1);

	gsInput.InvMatrix = float4x4(1,0,0,0,
								 0,1,0,0,
								 0,0,1,0,
								 0,0,0,1);

	gsInput.Texcoord = input.Texcoord;
	gsInput.Normal = input.Normal;
	return gsInput;

}

PS_INPUTRENDER common_Geometry(GS_INPUTRENDER input)
{

	PS_INPUTRENDER newPoint = (PS_INPUTRENDER)0;
	newPoint.Pos = input.Pos;
	newPoint.Normal = input.Normal;
	newPoint.Texcoord = input.Texcoord;
	return newPoint;

}

PS_INPUTRENDER common_Geometry(GS_INPUTRENDER input,float4x4 mat)
{

	PS_INPUTRENDER newPoint = common_Geometry(input);
	newPoint.Pos = mul(input.Pos,mat);
	return newPoint;

}

bool checkBack(float4 p1,float4 p2,float4 p3)
{

	float3 viewDirection = normalize(CameraPos.xyz-p1.xyz);

	float3 vector1 = p2.xyz-p1.xyz;
	float3 vector2 = p3.xyz-p1.xyz;

	float3 normal = normalize(cross(vector1,vector2));
	return dot(viewDirection,normal) < 0;

}

bool checkBack(float4 p1,float3 normal)
{

	float3 viewDirection = normalize(CameraPos.xyz-p1.xyz);
	return dot(viewDirection,normal) < 0;

}

float3 calculateTriangleNormal(float4 p1,float4 p2,float4 p3)
{
	
	float3 vector1 = p2.xyz-p1.xyz;
	float3 vector2 = p3.xyz-p1.xyz;

	return normalize(cross(vector1,vector2));
}

PS_INPUTRENDER commonFaceGeometry(GS_INPUTRENDER gsIn)
{
	
	PS_INPUTRENDER outPut = common_Geometry(gsIn,ViewProjection);

	//See where this vertex was in the last frame
	outPut.OldPos = mul(gsIn.Pos,gsIn.OldMatrix);
	outPut.Border = false;
	return outPut;

}

[maxvertexcount(3)]//3 points on the face
void GSTriangles(triangleadj GS_INPUTRENDER input[6], inout TriangleStream<PS_INPUTRENDER> OutStream )
{
	
	for(int j=0;j<3;j++)
	{
		
		PS_INPUTRENDER outPut = commonFaceGeometry(input[j*2]);
		OutStream.Append(outPut);

	}

	OutStream.RestartStrip();
}

[maxvertexcount(39)]//3 points on the face + 3 borders each having a maximum of 12 vertices (3+(3*12))
void GSTrianglesBorders(triangleadj GS_INPUTRENDER input[6], inout TriangleStream<PS_INPUTRENDER> OutStream )
{
	
	float3 faceNormal = calculateTriangleNormal(input[0].Pos,input[2].Pos,input[4].Pos);//Get the normal of the triangle

	if(!checkBack(input[0].Pos,faceNormal))//Perform backface culling
	{

		for(int j=0;j<3;j++)
		{
		
			PS_INPUTRENDER outPut = commonFaceGeometry(input[j*2]);
			OutStream.Append(outPut);

		}

		
		OutStream.RestartStrip();
		bool sharesEdge = false;

		for( uint k = 0; k < 3; k++ )
		{

			GS_INPUTRENDER point2 = input[(k*2)+1];//The adj
			uint firstPos = k*2;
			uint secondPos = (k*2+2)%(uint)6;

			float dist = distance(point2.Pos.xyz,input[firstPos].Pos.xyz);
			float3 borderFaceNormal = -faceNormal;

			if(dist > epsilon2)
			{
			
				borderFaceNormal = calculateTriangleNormal(input[firstPos].Pos,input[(k*2)+1].Pos,input[secondPos].Pos);

			}

			if(checkBack(input[firstPos].Pos,borderFaceNormal))//Shares border with backface
			{

				GS_INPUTRENDER point1 = input[firstPos];
				point2 = input[(k*2)+1];//The adj
				GS_INPUTRENDER point3 = input[secondPos];

				//float3 borderVector = point3.Pos.xyz-point1.Pos.xyz;

				float3 averageNormal = faceNormal;//+borderFaceNormal;
				//averageNormal /= 2;
				//averageNormal = normalize(averageNormal);

				PS_INPUTRENDER bp1 = common_Geometry(point1,ViewProjection);
				bp1.Border = true;

				GS_INPUTRENDER borderPoint1 = point1;
				borderPoint1.Pos.xyz += averageNormal*BorderThickness;
				PS_INPUTRENDER bp2 = common_Geometry(borderPoint1,ViewProjection);
				bp2.Border = true;
				
				GS_INPUTRENDER borderPoint2 = point3;
				borderPoint2.Pos.xyz += averageNormal*BorderThickness;
				PS_INPUTRENDER bp3 = common_Geometry(borderPoint2,ViewProjection);
				bp3.Border = true;
				
				PS_INPUTRENDER bp4 = common_Geometry(point3,ViewProjection);
				bp4.Border = true;

				GS_INPUTRENDER borderPoint3 = point1;
				borderPoint3.Pos.xyz += borderPoint3.Normal*BorderThickness;
				PS_INPUTRENDER bp5 = common_Geometry(borderPoint3,ViewProjection);
				bp5.Border = true;
				
				GS_INPUTRENDER borderPoint4 = point3;
				borderPoint4.Pos.xyz += borderPoint4.Normal*BorderThickness;
				PS_INPUTRENDER bp6 = common_Geometry(borderPoint4,ViewProjection);
				bp6.Border = true;

				OutStream.Append(bp1);
				OutStream.Append(bp2);
				OutStream.Append(bp3);
				OutStream.RestartStrip();

				OutStream.Append(bp1);
				OutStream.Append(bp3);
				OutStream.Append(bp4);
				OutStream.RestartStrip();

				OutStream.Append(bp1);
				OutStream.Append(bp5);
				OutStream.Append(bp3);
				OutStream.RestartStrip();

				OutStream.Append(bp4);
				OutStream.Append(bp3);
				OutStream.Append(bp6);
				OutStream.RestartStrip();

			}
		}
	}
}

PS_OUTPUT4 MAPPER(PS_INPUTRENDER psInput)
{

	PS_OUTPUT4 output;

	//See where this pixel was in the last frame
	float3 oldPosition = psInput.OldPos.xyz/psInput.OldPos.w;

	//Convert it from [-1,1] to [0,1]
	float2 uvOld = float2(0.5f*oldPosition.x+0.5f,-0.5f*oldPosition.y+0.5f);
		
	output.Velocity = float4(0,0,0,1);
	output.DiffColor = DebugCol;
	output.AmbColor = DebugCol;
	output.Normal = float4(0,0,0,0);

	if(!psInput.Border)
	{
		
		output.Velocity = float4(uvOld,1,1);
		output.DiffColor = diff.Sample(SampleType, psInput.Texcoord);
		output.Normal.rgb =  psInput.Normal;
		float3 globAmb = GlobalAmbience.rgb;

		if(UseNormal[0])
		{
		
		/*	float4 normData = normalize(2*norm.Sample(SampleType, psInput.Texcoord)-1);
			output.Normal.rgb = (output.Normal+normData).xyz;*/

			float4 normData = normalize(2 * norm.Sample(SampleType, psInput.Texcoord) - 1) + 1;
				normData.b -= 1;
			output.Normal.rgb = (output.Normal*normData).xyz;

			float dVal = dot(output.Normal.rgb, psInput.Normal);
			globAmb *= saturate(dVal);

		}
	
		output.AmbColor = float4(amb.Sample(SampleType, psInput.Texcoord).rgb*globAmb, 1);

		if(GlobalDirection.x != 0 || GlobalDirection.y != 0 || GlobalDirection.z != 0)
		{
	
			float3 globLight = output.DiffColor.rgb*saturate(dot(output.Normal.rgb, GlobalDirection.xyz))*GlobalLight.rgb;
			output.AmbColor.rgb += globLight;

		}

		output.Normal.rgb =  0.5*(normalize(output.Normal.rgb)+1.0);
		output.Normal.a = 1;

	}

	return output;

}
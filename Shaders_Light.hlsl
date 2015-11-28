#include "Shaders_Shadows.hlsl"

struct GS_INPUTLIGHT
{

	float4 Position : SV_POSITION;

};

struct PS_INPUTLIGHT
{
	
	float4 Pos : SV_POSITION;
	float2 Texcoord: TEXCOORD0;

};

/*Render Lights*/
GS_INPUTLIGHT VSLIGHT(float empty : EMPTY)
{

	GS_INPUTLIGHT outPut;
	outPut.Position = float4(0, 0, 0, 0);
	return outPut;

}

[maxvertexcount(4)]
void GSLIGHT( point GS_INPUTLIGHT input[1], inout TriangleStream<PS_INPUTLIGHT> TStream )
{

	PS_INPUTLIGHT p;
	p.Pos = float4(-1, 1, 0, 1);
	p.Texcoord = float2(0, 0);
	TStream.Append(p);

	PS_INPUTLIGHT p2;
	p2.Pos = float4(1, 1, 0, 1);
	p2.Texcoord = float2(1, 0);
	TStream.Append(p2);

	PS_INPUTLIGHT p3;
	p3.Pos = float4(-1, -1, 0, 1);
	p3.Texcoord = float2(0, 1);
	TStream.Append(p3);

	PS_INPUTLIGHT p4;
	p.Pos = float4(1, -1, 0, 1);
	p.Texcoord = float2(1, 1);
	TStream.Append(p);

	TStream.RestartStrip();

}

float4 computeLighting(float3 normal,float3 lightDir,float3 diffData,float3 position,float lightSize)
{
	
	float dist = (length(lightDir)); 
	float attenuation = (dist*dist)/(Radius*Radius); 
	attenuation *= attenuation;
	attenuation = 1.f/(attenuation*LightFalloff+1.f);
	float att_s = 1.f/(LightFalloff+1);//att_s contains now the value we have to subtract
	attenuation = attenuation-att_s;
	attenuation/=1.f-att_s;

	float coneAngle = tan(lightSize / dist);
	float lightAngle = max(acos(dot(normal, lightDir) / dist)-coneAngle,0);

	float diffuseLight = max(0,cos(lightAngle));
	float3 h = normalize(normalize(CameraPos.xyz-position)-lightDir);

	float3 diff = saturate(diffData*diffuseLight*attenuation);
	float4 specular = 0;//(SpecularColor[i].xyz*h,1);

	float4 retVal = 0;
	retVal.x = diff.x+specular.x;
	retVal.y = diff.y+specular.y;
	retVal.z = diff.z+specular.z;
	retVal.w = 1;
	return saturate(retVal);

}

float2 figureOutPointLightPosition(float2 pos,int rlFlip,int udFlip,
								   int index,int rightIndex,int leftIndex,int downIndex,int upIndex,
								   float2 rightAdd,float2 leftAdd,float2 upAdd,float2 downAdd,
									int2 rightMods,int2 leftMods,int2 upMods,int2 downMods)
{

	float2 rightLeftVal = float2(abs(pos.x)%1.0f,pos.y);
	float2 upDownVal = float2(pos.x,abs(pos.y)%1.0f);

	int downMod = (int)max(0.0f,pos.y);
	int upMod = abs((int)min(0.0f,pos.y-1.0f));
	int rightMod = (int)max(0.0f,pos.x);
	int leftMod = abs((int)min(0.0f,pos.x-1.0f));
			
	int totalmod = clamp((rightMod+leftMod+downMod+upMod),0,1);
	int rlmod = clamp((rightMod+leftMod),0,1);
	int udmod = clamp((downMod+upMod),0,1);

	//Flip the rl-values around
	float tempCoord = rightLeftVal.x;
	rightLeftVal.x -= rightLeftVal.x*rlmod*rlFlip;
	rightLeftVal.x += rightLeftVal.y*rlmod*rlFlip;
	rightLeftVal.y -= rightLeftVal.y*rlmod*rlFlip;
	rightLeftVal.y += tempCoord*rlmod*rlFlip;
	//Flip the ud-values around
	tempCoord = upDownVal.x;
	upDownVal.x -= upDownVal.x*udmod*udFlip;
	upDownVal.x += upDownVal.y*udmod*udFlip;
	upDownVal.y -= upDownVal.y*udmod*udFlip;
	upDownVal.y += tempCoord*udmod*udFlip;

	//Set the position on the shadowmap
	pos += RenderCenter[index].xy;
	////Goes beyond borders so reset the position
	//pos -= (RenderCenter[index].xy+pos)*totalmod;

	////If point exists ONLY to the right or left of the face
	//pos	+= (RenderCenter[rightIndex].xy+rightAdd+rightLeftVal*rightMods)*rightMod*(1-udmod);
	//pos	+= (RenderCenter[leftIndex].xy+leftAdd+rightLeftVal*leftMods)*leftMod*(1-udmod);
	//		
	////If point exists ONLY up or down of the face
	//pos	+= (RenderCenter[downIndex].xy+downAdd+upDownVal*downMods)*downMod*(1-rlmod);
	//pos	+= (RenderCenter[upIndex].xy+upAdd+upDownVal*upMods)*upMod*(1-rlmod);

	////Flip the coordinates around
	//rightLeftVal.y = rightLeftVal.x;
	//rightLeftVal.x = upDownVal.y;

	////If point exists to the right or left of the face AND below or up
	//pos	+= (RenderCenter[downIndex].xy+downAdd+rightLeftVal*downMods)*rlmod*downMod;
	//pos	+= (RenderCenter[upIndex].xy+upAdd+rightLeftVal*upMods)*rlmod*upMod;

	//normalize the coordinates
	pos.x *= InvRenderWidth;
	pos.y *= InvRenderHeight;

	return pos;

}

float4 POINTLIGHTER(PS_INPUTLIGHT psInput) : SV_Target
{
	
	float2 uv = psInput.Texcoord;
	float4 normalData = norm.Sample(SampleType, uv);

	if(normalData.x == 0 && normalData.y == 0 &&normalData.z == 0 &&normalData.w == 0)
	{

		clip(-1);

	}

	float4 position;//Convert to clipspace
	position.x = 2*uv.x-1;
	position.y = -(2*uv.y-1);
	position.z = depth.Load(uint3(psInput.Pos.xy,0)).x;
	position.w = 1.0f;
	
	position = mul(position, InvertViewProjection);//Convert to worldspace
	position /= position.w;

	float4 diffData = diff.Sample(SampleType, uv);

	return float4(computeLighting((2.0f * normalData.xyz - 1.0f),
	(Position-position.xyz),diffData.xyz*Diffuse.rgb,
	position.xyz, Size*0.5f).xyz, 1);

}

float4 POINTLIGHTERSSPCSSMINOR(PS_INPUTLIGHT psInput) : SV_Target
{

	float shadowFactor = SSGAUSSMINOR(psInput.Pos, psInput.Texcoord);

	if (1 - shadowFactor > 0)
	{

		return float4(POINTLIGHTER(psInput).xyz*(1 - shadowFactor), 1);

	}

	return 0;

}

void getMatVals(float4 wPos, out int index, out float3 forward, out float3 right, out float3 up, out float4 projVals)
{

	float3 lightSource = Position;
	float4 position = wPos;

	forward = Forward;
	right = Right;
	up = float3(
	forward.y*right.z - forward.z*right.y,
	forward.z*right.x - forward.x*right.z,
	forward.x*right.y - forward.y*right.x);
	index = 0;

	float3 distance = (lightSource - position.xyz);

	if (distance.x*distance.x + distance.y*distance.y + distance.z*distance.z > epsilon)
	{

		float planeXD = -(right.x*lightSource.x + right.y*lightSource.y + right.z*lightSource.z);//ax+by+cz+d=0 <> -d=ax+by+cz
		float planeYD = -(up.x*lightSource.x + up.y*lightSource.y + up.z*lightSource.z);//ax+by+cz+d=0 <> -d=ax+by+cz
		float planeZD = -(forward.x*lightSource.x + forward.y*lightSource.y + forward.z*lightSource.z);//ax+by+cz+d=0 <> -d=ax+by+cz

		float distanceX = position.x*right.x + position.y*right.y + position.z*right.z + planeXD;//Right is a normalized vector with length 1
		float distanceY = position.x*up.x + position.y*up.y + position.z*up.z + planeYD;//Since up is a crossproduct of two orthogonal normalized vectors and is thus a normalized vector '/1' is excluded
		float distanceZ = position.x*forward.x + position.y*forward.y + position.z*forward.z + planeZD;//Forward is a normalized vector with the length 1

		if (abs(distanceZ) >= max(abs(distanceY), abs(distanceX)))//Check if the point is in front of or behind the source
		{
			if (distanceZ < 0)//If the point is in back of the source
			{

				forward = -forward;
				right = -right;
				index = 2;

			}
		}
		else if (abs(distanceY) >= max(abs(distanceX), abs(distanceZ)))//Check if the point is above or below the source
		{

			//If the point is above the source
			float3 temp = forward;
				forward = up;
			up = -temp;
			index = 4;

			if (distanceY < 0)//If the point is below the source
			{

				forward = -forward;
				up = -up;
				index = 5;

			}
		}
		else if (abs(distanceX) >= max(abs(distanceZ), abs(distanceY)))//Check if the point is to the right of or to the left of the source
		{

			//If the point is to the right of the source
			float3 temp = forward;
				forward = right;
			right = -temp;
			index = 3;

			if (distanceX < 0)//If the point is to the left of the source
			{

				forward = -forward;
				right = -right;
				index = 1;
			}
		}
	}

	float yScale = 1 / tan(Angle * 0.5f);
	float zF = Radius*Radius + Size;
	float zN = Size;
	float z3 = (zF / (zF - zN));
	float z4 = -zN*zF / (zF - zN);

	projVals.x = yScale;
	projVals.y = zN;
	projVals.z = z3;
	projVals.w = z4;

}

float4x4 getPointMatrix(float4 wPos,out int index, out float4 projVals)
{

	float3 lightSource = Position;
		
	float3 forward = float3(0, 0, 0);
	float3 right = float3(0, 0, 0);
	float3 up = float3(0,0,0);
	index = 0;

	getMatVals(wPos, index, forward, right, up, projVals);

	float yScale = projVals.x;
	float z3 = projVals.z;
	float z4 = projVals.w;

	lightSource -= forward*Size;

	return float4x4(
			right.x*yScale,up.x*yScale,forward.x*z3,forward.x,
			right.y*yScale,up.y*yScale,forward.y*z3,forward.y,
			right.z*yScale,up.z*yScale,forward.z*z3,forward.z,
			-dot(right,lightSource)*yScale,-dot(up,lightSource)*yScale,-dot(forward,lightSource)*z3+z4,-dot(forward,lightSource));

}

float2 isInShadow(float4 wPos)
{

	float3 lightSource = Position;
	float4 position = wPos;
	int index = 0;
	float4 projs = float4(0,0,0,0);
	
	float4x4 viewProjectionMatrix = getPointMatrix(wPos,index,projs);

	float yScale = projs.x;
	float zN = projs.y;
	float z3 = projs.z;
	float z4 = projs.w;

	float4 smPosition = mul(position,viewProjectionMatrix);//Convert to clipspace of light
	smPosition /= smPosition.w;

	smPosition.x = 0.5*smPosition.x+0.5f;//Convert to [0,1]
	smPosition.y = -0.5*smPosition.y+0.5f;//Convert to [0,1]
	smPosition.xy += RenderCenter[index].xy;
	//normalize the coordinates
	smPosition.x *= InvRenderWidth;
	smPosition.y *= InvRenderHeight;
	float depthLin = abs(z4/(smPosition.z-z3)-zN);
	float blockDepth = shadowMap.Sample(SampleType, smPosition.xy).r;
	float blockLin = abs(z4/(blockDepth-z3)-zN);

	if(smPosition.z - blockDepth > epsilon)
	{
	
		return float2(depthLin,depthLin - blockLin);

	}
	else
	{

		return float2(0,0);

	}
}

float2 isInShadow( PS_INPUTLIGHT psInput)
{
	
	float2 uv = psInput.Texcoord;
	float4 position;//Convert to clipspace
	position.x = 2*uv.x-1;
	position.y = -(2*uv.y-1);
	position.z = depth.Load(uint3(psInput.Pos.xy,0)).x;
	position.w = 1.0f;
	
	position = mul(position, InvertViewProjection);//Convert to worldspace
	position /= position.w;

	return isInShadow(position);

}

float4 POINTLIGHTERSM( PS_INPUTLIGHT psInput) : SV_Target
{

	float2 uv = psInput.Texcoord;
	float4 normalData = norm.Sample(SampleType, uv);

	if(normalData.x == 0 && normalData.y == 0 &&normalData.z == 0 &&normalData.w == 0)
	{

		clip(-1);

	}

	float4 position;//Convert to clipspace
	position.x = 2*uv.x-1;
	position.y = -(2*uv.y-1);
	position.z = depth.Load(uint3(psInput.Pos.xy,0)).x;
	position.w = 1.0f;
	position = mul(position, InvertViewProjection);//Convert to worldspace
	position /= position.w;

	if(isInShadow(position).y > epsilon)
	{

		return 0;
		clip(-1);

	}

	float4 diffData = diff.Sample(SampleType, uv);

	return float4(computeLighting((2.0f * normalData.xyz - 1.0f),
		(Position - position.xyz), diffData.xyz*Diffuse.rgb,
		position.xyz, Size*0.5f).xyz, 1);

}

float4 POINTLIGHTERUMBRASM(PS_INPUTLIGHT psInput) : SV_Target
{

	float2 uv = psInput.Texcoord;
	float4 normalData = norm.Sample(SampleType, uv);

	if (normalData.x == 0 && normalData.y == 0 && normalData.z == 0 && normalData.w == 0)
	{

		clip(-1);

	}

	float4 position;//Convert to clipspace
	position.x = 2 * uv.x - 1;
	position.y = -(2 * uv.y - 1);
	position.z = depth.Load(uint3(psInput.Pos.xy, 0)).x;
	position.w = 1.0f;
	position = mul(position, InvertViewProjection);//Convert to worldspace
	position /= position.w;

	int index = 0;
	float4 projs = float4(0, 0, 0, 0);

	float4x4 mat = getPointMatrix(position, index, projs);
	
	float yScale = projs.x;
	float zN = projs.y;
	float z3 = projs.z;
	float z4 = projs.w;

	float4 sPos = (mul(position, mat));
	sPos.xyz /= sPos.w;
	float lDepth = sPos.z;

	float2 sPos2 = float2(0.5*sPos.x + 0.5f, -0.5*sPos.y + 0.5f);//Convert to [0,1]
	sPos2.xy += RenderCenter[index].xy;
	sPos2.x *= InvRenderWidth;
	sPos2.y *= InvRenderHeight;

	float bDepth = shadowMap.Sample(SampleType, sPos2.xy).r;
	bool isEnlightened = !(lDepth - bDepth > epsilon);

	float shadowDepthAvg = 0;
	float numBlockers = 0;

	if (!isEnlightened)
	{

		float bDepthLin = z4 / (bDepth - z3) - zN;
		float depthLin = z4 / (lDepth - z3) - zN;

		float searchRadius = ((1 / yScale)*depthLin);
		float blockInc = ((BlockerSearch - 1)*0.5f);

		for (float x = -blockInc; x <= blockInc ; x++)
		{

			for (float y = -blockInc; y <= blockInc ; y++)
			{

				float xFactor = (x / blockInc);
				float yFactor = (y / blockInc);

				float4 pos = float4(sPos.xy + (float2(xFactor*searchRadius, yFactor*searchRadius)), 0, 1);

				pos.x = 0.5*pos.x + 0.5f;//Convert to [0,1]
				pos.y = -0.5*pos.y + 0.5f;//Convert to [0,1]

				pos.xy += RenderCenter[index].xy;

				//normalize the coordinates
				pos.x *= InvRenderWidth;
				pos.y *= InvRenderHeight;

				float val = shadowMap.SampleLevel(SampleType, pos.xy,0).r;
				isEnlightened = !(lDepth - val > epsilon);

				if (!isEnlightened)
				{

					numBlockers++;

				}
			}
		}
	}

	if (numBlockers == 256)
	{

		return float4(numBlockers/256, numBlockers/256, numBlockers/256, 1);
		clip(-1);

	}

	float4 diffData = diff.Sample(SampleType, uv);

	return float4(computeLighting((2.0f * normalData.xyz - 1.0f),
	(Position - position.xyz), diffData.xyz*Diffuse.rgb,
	position.xyz, Radius*0.5f).xyz, 1);

}

float4 POINTLIGHTERSSPCSSPEN( PS_INPUTLIGHT psInput) : SV_Target
{

	float2 uv = psInput.Texcoord;
	float4 normalData = norm.Sample(SampleType, uv);

	if(normalData.x == 0 && normalData.y == 0 &&normalData.z == 0 &&normalData.w == 0)
	{

		return 0;

	}

	normalData.xyz = (2.0f * normalData.xyz - 1.0f);
	normalData.w = 1;
	//return normalData;

	float4 position;//Convert to clipspace
	position.x = 2*uv.x-1;
	position.y = -(2*uv.y-1);
	position.z = depth.Load(uint3(psInput.Pos.xy,0)).x;

	if(position.z >= 1)
	{
		
		return 0;

	}

	position.w = 1.0f;

	float4 wPos = mul(position,InvertViewProjection);
	int index = 0;
	float4 projs = float4(0,0,0,0);

	float4x4 mat = getPointMatrix(wPos,index,projs);
	float yScale = projs.x;
	float zN = projs.y;
	float z3 = projs.z;
	float z4 = projs.w;
		
	float4 sPos = (mul(wPos,mat));
	sPos.xyz /= sPos.w;
	float lDepth = sPos.z;
	
	float2 sPos2 = float2(0.5*sPos.x+0.5f,-0.5*sPos.y+0.5f);//Convert to [0,1]
	sPos2.xy += RenderCenter[index].xy;
	sPos2.x *= InvRenderWidth;
	sPos2.y *= InvRenderHeight;

	float bDepth = shadowMap.Sample(SampleType,sPos2.xy).r;
	float bDepthLin = z4/(bDepth-z3)-zN;
	float depthLin = z4/(lDepth-z3)-zN;

	//return float4(sPos.z ,sPos.z ,sPos.z ,1);

	float searchRadius = ((((depthLin-zN))/depthLin*Size))/((1/yScale)*depthLin);
	return float4(searchRadius,searchRadius,searchRadius,1);

	float shadowDepthAvg = 0;
	float numBlockers = 0;
	float blockInc = ((BlockerSearch-1)*0.5f);
	
	for(float x=-blockInc;x<=blockInc;x++)
	{
		
		for(float y=-blockInc;y<=blockInc;y++)
		{
			
			float xFactor = x/blockInc;
			float yFactor = y/blockInc;
			
			float4 pos = float4(sPos.xy+(float2(xFactor*searchRadius,yFactor*searchRadius)),0,1);

			pos.x = 0.5*pos.x+0.5f;//Convert to [0,1]
			pos.y = -0.5*pos.y+0.5f;//Convert to [0,1]

			pos.xy += RenderCenter[index].xy;

			//normalize the coordinates
			pos.x *= InvRenderWidth;
			pos.y *= InvRenderHeight;

			float val = shadowMap.Sample(SampleType, pos.xy).r;

			if(lDepth - val > epsilon)
			{
			
				shadowDepthAvg += z4/(val-z3)-zN;
				numBlockers++;

			}
		}
	}
	
	//return float4(numBlockers/16,numBlockers/16,numBlockers/16,1);

	float pUmbra = 0;

	if(numBlockers > 0)
	{
	
		shadowDepthAvg /= numBlockers;
		pUmbra = (depthLin-shadowDepthAvg)/depthLin*Size;

	}

	float blockDist = 0;

	if(lDepth - bDepth > epsilon)
	{
			
		blockDist = max(0,depthLin-bDepthLin);

	}

	return float4(pUmbra,pUmbra,pUmbra,1);
	return float4(numBlockers/16,numBlockers/16,numBlockers/16,1);
	return float4(depthLin-bDepthLin,depthLin-bDepthLin,depthLin-bDepthLin,1);

	//return float2(pUmbra,blockDist);

}

float2 SSPCSSX( PS_INPUTLIGHT psInput) : SV_Target
{
	
	float2 uv = psInput.Texcoord;
	float2 dpth = shadowInfo.Sample(SampleType, uv).rg;
	float4 normalData = norm.Sample(SampleType, uv);

	if(normalData.x == 0 && normalData.y == 0 &&normalData.z == 0 &&normalData.w == 0)
	{

		clip(-1);

	}
	
	//return float4(dpth.g,0,0,1);
	if(dpth.r == 0)
	{
		//return float4(1,0,0,1);
		return 0;

	}

	//return float4(0,1,0,1);
	float4 position;//Convert to clipspace
	position.x = 2*uv.x-1;
	position.y = -(2*uv.y-1);
	
	float linDepth = abs(Projection._43/(depth.Load(uint3(psInput.Pos.xy,0)).x-Projection._33)-ZNear);
	//return float4(linDepth,linDepth,linDepth,1);

	//Searchradius in worldspace
	float zN = Size*0.5f*tan((pi*0.5f)-(Angle*0.5));

	//float searchRadiusY = (dpth.x*0.5f)/((1/Projection._22)*linDepth);
	float searchRadius = (dpth.x)/((1/Projection._11)*linDepth);
	//return float4(searchRadius,searchRadius,searchRadius,1);

	float shadowFactor = 0;
	float blockInc = ((ShadowSearch-1)*0.5f);
	float search = ShadowSearch;
	float sigma = pow(search/6,2);
	float totalWeight = 0;
	float baseWeight = 1/sqrt(2*pi*sigma);

	for(float x=-blockInc;x<=blockInc;x++)
	{
		
		float2 pos = float2(position.xy+(float2(x/blockInc,0)*searchRadius));

		pos.x = 0.5*pos.x+0.5f;//Convert to [0,1]
		pos.y = -0.5*pos.y+0.5f;//Convert to [0,1]
		float weight = baseWeight*exp(-(pow(x,2)/(2*sigma)));
		totalWeight += weight;
		
		if(shadowInfo.SampleLevel(SampleType, pos,0).g > 0)
		{
				
			shadowFactor += weight;

		}

		if(pos.x > 1 || pos.x < 0 )
		{
			
			//shadowFactor--;
			//search--;

		}
	}

	shadowFactor /= totalWeight;
	//return float4(1-totalWeight,1-totalWeight,1-totalWeight,1);
	//return float4(shadowFactor,shadowFactor,shadowFactor,1);
	return float2((dpth.x)/((1/Projection._22)*linDepth),shadowFactor);

}

float4 SSPCSSY( PS_INPUTLIGHT psInput) : SV_Target
{
	
	
	float2 uv = psInput.Texcoord;
	float2 dpth = shadowInfo.Sample(SampleType, uv).rg;
	float4 normalData = norm.Sample(SampleType, uv);
	float shadowFactor = 0;
	float4 position;//Convert to clipspace
	position.x = 2*uv.x-1;
	position.y = -(2*uv.y-1);

	if(normalData.x == 0 && normalData.y == 0 &&normalData.z == 0 &&normalData.w == 0)
	{

		clip(-1);

	}

	if(dpth.r > 0)
	{
		
		//float searchRadiusY = (dpth.x*0.5f)/((1/Projection._22)*linDepth);
		float searchRadius = dpth.r;

		float blockInc = ((ShadowSearch-1)*0.5f);
		float search = ShadowSearch+(1-ShadowSearch%2);
		float sigma = pow(search/6,2);
		float baseWeight = 1/sqrt(2*pi*sigma);
		float totalWeight = baseWeight;
		shadowFactor = dpth.g*baseWeight;

		for(float y=-blockInc;y<=blockInc;y++)
		{
		
			float2 pos = float2(position.xy+(float2(0,y/blockInc)*searchRadius));

			pos.x = 0.5*pos.x+0.5f;//Convert to [0,1]
			pos.y = -0.5*pos.y+0.5f;//Convert to [0,1]
			float weight = baseWeight*exp(-(pow(y,2)/(2*sigma)));
			totalWeight += weight;
			float val = shadowInfo.SampleLevel(SampleType, pos,0).g*weight;
			shadowFactor += val;

			if(val >= 1)
			{
			
				return float4(0,1,0,1);
				//shadowFactor--;
				//search--;

			}
		}

		shadowFactor /= totalWeight;
		//return float4(shadowFactor,shadowFactor,shadowFactor,1);

	}
	
	//return float4(1,0,0,1);

	if(1-shadowFactor > 0)
	{
		
		position.z = depth.Load(uint3(psInput.Pos.xy,0)).x;
		position.w = 1.0f;
		position = mul(position, InvertViewProjection);//Convert to worldspace
		position /= position.w;
		float4 diffData = diff.Sample(SampleType, uv);
		return float4(computeLighting((2.0f * normalData.xyz - 1.0f),
		(Position-position.xyz),
		diffData.xyz*Diffuse.rgb,
		position.xyz, Size*0.5f).xyz*(1 - shadowFactor), 1);

	}

	return 0;

}
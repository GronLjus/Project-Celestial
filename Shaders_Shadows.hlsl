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

struct GS_INPUTSHADOW
{
	float4 Pos : SV_POSITION;
	float3 Normal : NORMAL0;
};

struct PS_INPUTSHADOW
{

	float4 Pos : SV_POSITION;
	uint Viewport : SV_ViewportArrayIndex;

};

struct PS_INPUTPUMBSHADOW
{

	float4 Pos : SV_POSITION;
	float3 SrcPos : UV0;
	float InvDst : UV1;

};

struct PS_SSVOUTPUT
{
	
	float SearchRegion : SV_Target0;
	float Depth : SV_Depth;

};

struct WedgePoints
{

	PS_INPUTSHADOW borderPoint1;
	PS_INPUTSHADOW borderPoint2;
	PS_INPUTSHADOW endPoint1;
	PS_INPUTSHADOW endPoint2;
	PS_INPUTSHADOW edgePoint1;
	PS_INPUTSHADOW edgePoint2;
	PS_INPUTSHADOW capPoint1;
	PS_INPUTSHADOW capPoint2;

};

struct WedgePoints2
{

	PS_INPUTPUMBSHADOW borderPoint1;
	PS_INPUTPUMBSHADOW borderPoint2;
	PS_INPUTPUMBSHADOW edgePoint1;
	PS_INPUTPUMBSHADOW edgePoint2;
	PS_INPUTPUMBSHADOW capPoint1;
	PS_INPUTPUMBSHADOW capPoint2;

};

struct UmbraPoints
{

	PS_INPUTSHADOW meshPoint1;
	PS_INPUTSHADOW meshPoint2;
	PS_INPUTSHADOW splitPoint1;
	PS_INPUTSHADOW splitPoint2;
	PS_INPUTSHADOW umbraPoint1;
	PS_INPUTSHADOW umbraPoint2;
	PS_INPUTSHADOW bottomPoint1;
	PS_INPUTSHADOW bottomPoint2;
	PS_INPUTSHADOW middlePoint;

};

struct PenUmbraPoints
{

	PS_INPUTPUMBSHADOW meshPoint1;
	PS_INPUTPUMBSHADOW meshPoint2;
	PS_INPUTPUMBSHADOW penUmbraPoint1;
	PS_INPUTPUMBSHADOW penUmbraPoint2;
	PS_INPUTPUMBSHADOW bottomPoint1;
	PS_INPUTPUMBSHADOW bottomPoint2;

};

/*Shadow geometry*/
GS_INPUTSHADOW VSSHADOW(VS_INPUTRENDER input  )
{
	
	GS_INPUTSHADOW gsInput;
	gsInput.Pos = mul(float4(input.Pos, 1), input.World);
	gsInput.Normal = mul(float4(input.Normal, 1), input.InvWorldTran).xyz;
	return gsInput;

}

GS_INPUTSHADOW VSTERRAINSHADOW(VS_INPUTTERRAIN input  )
{

	GS_INPUTSHADOW gsInput;
	gsInput.Pos = float4(input.Pos, 1);
	gsInput.Normal = input.Normal;
	return gsInput;

}

float4x4 getPointMatrix(int index)
{

	float3 lightSource = Position;

	/*position.x = 0;
	position.y = 10;
	position.z = 0;*/

	float3 forward = Forward;
	float3 right = Right;
	float3 up = float3(
	forward.y*right.z - forward.z*right.y,
	forward.z*right.x - forward.x*right.z,
	forward.x*right.y - forward.y*right.x);
	float3 temp = forward;

	if (index == 2)//If the point is in back of the source
	{

		forward = -forward;
		right = -right;

	}
	else if(index == 4)//If the point is in above of the source
	{

		forward = up;
		up = -temp;

	}
	else if (index == 5)//Check if the point is below the source
	{

		forward = -up;
		up = temp;

	}
	else if (index == 3)//Check if the point is to the right of the source
	{

		forward = right;
		right = -temp;

	}
	else if (index == 1)//Check if the point is to the left of the source
	{

		forward = -right;
		right = -temp;

	}

	float yScale = 1 / tan(Angle * 0.5f);
	float zF = Radius*Radius + Size;
	float zN = Size;
	float z3 = (zF / (zF - zN));
	float z4 = -zN*zF / (zF - zN);
	lightSource -= forward*Size;

	return float4x4(
		right.x*yScale, up.x*yScale, forward.x*z3, forward.x,
		right.y*yScale, up.y*yScale, forward.y*z3, forward.y,
		right.z*yScale, up.z*yScale, forward.z*z3, forward.z,
		-dot(right, lightSource)*yScale, -dot(up, lightSource)*yScale, -dot(forward, lightSource)*z3 + z4, -dot(forward, lightSource));

}

int getPointIndex(float4 wPos)
{

	float3 lightSource = Position;
		float4 position = wPos / wPos.w;
		int index = -1;

	float3 up = float3(
		Forward.y*Right.z - Forward.z*Right.y,
		Forward.z*Right.x - Forward.x*Right.z,
		Forward.x*Right.y - Forward.y*Right.x);

	float3 distance = (lightSource - position.xyz);

	if (distance.x*distance.x + distance.y*distance.y + distance.z*distance.z > epsilon)
	{

		float planeXD = -(Right.x*lightSource.x + Right.y*lightSource.y + Right.z*lightSource.z);//ax+by+cz+d=0 <> -d=ax+by+cz
		float planeYD = -(up.x*lightSource.x + up.y*lightSource.y + up.z*lightSource.z);//ax+by+cz+d=0 <> -d=ax+by+cz
		float planeZD = -(Forward.x*lightSource.x + Forward.y*lightSource.y + Forward.z*lightSource.z);//ax+by+cz+d=0 <> -d=ax+by+cz

		float distanceX = position.x*Right.x + position.y*Right.y + position.z*Right.z + planeXD;//Right is a normalized vector with length 1
		float distanceY = position.x*up.x + position.y*up.y + position.z*up.z + planeYD;//Since up is a crossproduct of two orthogonal normalized vectors and is thus a normalized vector '/1' is excluded
		float distanceZ = position.x*Forward.x + position.y*Forward.y + position.z*Forward.z + planeZD;//Forward is a normalized vector with the length 1

		index = 0;

		if (abs(distanceZ) >= max(abs(distanceY), abs(distanceX)))//Check if the point is in front of or behind the source
		{
			if (distanceZ < 0)//If the point is in back of the source
			{

				index = 2;

			}
		}
		else if (abs(distanceY) >= max(abs(distanceX), abs(distanceZ)))//Check if the point is above or below the source
		{

			//If the point is above the source
			index = 4;

			if (distanceY < 0)//If the point is below the source
			{

				index = 5;

			}
		}
		else if (abs(distanceX) >= max(abs(distanceZ), abs(distanceY)))//Check if the point is to the right of or to the left of the source
		{

			index = 3;

			if (distanceX < 0)//If the point is to the left of the source
			{

				index = 1;

			}
		}
	}

	return index;

}

bool checkBack(float4 p1, float3 normal)
{

	float3 viewDirection = normalize(Position.xyz - p1.xyz);
	return dot(viewDirection, normal) < 0;

}

bool checkBackWithSize(float4 p1, float3 normal)
{

	//float planeD = -(p1.x*normal.x + p1.y*normal.y + p1.z*normal.z);
	//float planeDist = (Position.x*normal.x + Position.y*normal.y + Position.z*normal.z + planeD);//Normal is a normalized vector and so has length 1
	//return planeDist > Size;

	float3 viewDirection = normalize((Position.xyz + Size*normal) - p1.xyz);
	return dot(viewDirection, normal) < 0;

}

float3 calculateTriangleNormal(float4 p1, float4 p2, float4 p3)
{

	float3 vector1 = p2.xyz - p1.xyz;
	float3 vector2 = p3.xyz - p1.xyz;

	return normalize(cross(vector1, vector2));

}

PS_INPUTSHADOW common_GeometryShadow(GS_INPUTSHADOW input, int index)
{

	PS_INPUTSHADOW newPoint = (PS_INPUTSHADOW)0;
	newPoint.Pos = mul(input.Pos, getPointMatrix(index));
	//newPoint.Pos = mul(input.Pos, ShadowViews[index]);
	newPoint.Viewport = index;
	return newPoint;

}

[maxvertexcount(18)]//3 points * 6 renders
void GSSHADOWTriangles( triangleadj GS_INPUTSHADOW input[6], inout TriangleStream<PS_INPUTSHADOW> OutStream )//Can cubemap
{

	for(int i=0;i<6;i++)
    {
          
		for( int k = 0; k < 3; k++ )
		{

			OutStream.Append(common_GeometryShadow(input[k*2],i));

		}

		OutStream.RestartStrip();

    }
}

PS_INPUTSHADOW common_VolumeShadow(GS_INPUTSHADOW input)
{
	
	PS_INPUTSHADOW val;
	val.Pos = input.Pos;
	val.Viewport = 0;
	return val;

}

PS_INPUTSHADOW transformPoint(PS_INPUTSHADOW input)
{

	input.Pos = mul(input.Pos, ViewProjection);
	return input;

}

PS_INPUTSHADOW common_BackCapGenerate(GS_INPUTSHADOW input)
{

	GS_INPUTSHADOW borderPoint = input;
	borderPoint.Pos.xyz = Position;
	borderPoint.Pos.xyz += normalize(input.Pos.xyz - Position)*Radius;
	return common_VolumeShadow(borderPoint);

}

[maxvertexcount(18)]//2 caps with 3 borders each having a maximum of 4 vertices (3*2)+(3*4)
void GSSHADOWVolumeTriangles(triangleadj GS_INPUTSHADOW input[6], inout TriangleStream<PS_INPUTSHADOW> OutStream)
{

	float3 faceNormal = calculateTriangleNormal(input[0].Pos, input[2].Pos, input[4].Pos);//Get the normal of the triangle

	if (!checkBack(input[0].Pos, faceNormal))//Perform backface culling
	{

		bool sharesEdge = false;

		//Add the frontcap
		GS_INPUTSHADOW borderPoint = input[0];
		PS_INPUTSHADOW bp = transformPoint(common_VolumeShadow(borderPoint));
		OutStream.Append(bp);

		borderPoint = input[2];
		bp = transformPoint(common_VolumeShadow(borderPoint));
		OutStream.Append(bp);

		borderPoint = input[4];
		bp = transformPoint(common_VolumeShadow(borderPoint));
		OutStream.Append(bp);
		OutStream.RestartStrip();

		//Add the backcap
		borderPoint = input[4];
		borderPoint.Pos.xyz += normalize(input[4].Pos.xyz - Position)*Radius;
		bp = transformPoint(common_VolumeShadow(borderPoint));
		OutStream.Append(bp);

		borderPoint = input[2];
		borderPoint.Pos.xyz += normalize(input[2].Pos.xyz - Position)*Radius;
		bp = transformPoint(common_VolumeShadow(borderPoint));
		OutStream.Append(bp);

		borderPoint = input[0];
		borderPoint.Pos.xyz += normalize(input[0].Pos.xyz - Position)*Radius;
		bp = transformPoint(common_VolumeShadow(borderPoint));
		OutStream.Append(bp);
		OutStream.RestartStrip();

		for (uint k = 0; k < 3; k++)
		{

			GS_INPUTSHADOW point2 = input[(k * 2) + 1];//The adj
			uint firstPos = k * 2;
			uint secondPos = (k * 2 + 2) % (uint)6;

			float dist = distance(point2.Pos.xyz, input[firstPos].Pos.xyz);
			float3 borderFaceNormal = -faceNormal;

			if (dist > epsilon)
			{

				borderFaceNormal = calculateTriangleNormal(input[firstPos].Pos, input[(k * 2) + 1].Pos, input[secondPos].Pos);

			}

			if (checkBack(input[firstPos].Pos, borderFaceNormal))//Shares border with a backface
			{

				GS_INPUTSHADOW point1 = input[firstPos];
				GS_INPUTSHADOW point3 = input[secondPos];

				float3 dir1 = normalize(point1.Pos.xyz - Position);
					float3 dir2 = normalize(point3.Pos.xyz - Position);
					//float3 borderVector = point3.Pos.xyz-point1.Pos.xyz;

					float3 averageNormal = faceNormal;//+borderFaceNormal;
					//averageNormal /= 2;
					//averageNormal = normalize(averageNormal);

					PS_INPUTSHADOW bp1 = transformPoint(common_VolumeShadow(point1));

				GS_INPUTSHADOW borderPoint1 = point1;
				borderPoint1.Pos.xyz += dir1*Radius;
				PS_INPUTSHADOW bp2 = transformPoint(common_VolumeShadow(borderPoint1));

				GS_INPUTSHADOW borderPoint2 = point3;
				borderPoint2.Pos.xyz += dir2*Radius;
				PS_INPUTSHADOW bp3 = transformPoint(common_VolumeShadow(borderPoint2));

				PS_INPUTSHADOW bp4 = transformPoint(common_VolumeShadow(point3));

				OutStream.Append(bp4);
				OutStream.Append(bp1);
				OutStream.Append(bp3);
				OutStream.Append(bp2);
				OutStream.RestartStrip();

			}
		}
	}
}

WedgePoints common_VolumeGetWedgePoints(GS_INPUTSHADOW point1, GS_INPUTSHADOW point2, int modifier)
{

	WedgePoints utPut;

	float3 sourcePos1 = (Position + ((Size)*point1.Normal)*modifier);
	float3 sourcePos2 = (Position + ((Size)*point2.Normal)*modifier);

	//The directionvectors from the light to the border
	float3 uDir1 = normalize(point1.Pos.xyz - sourcePos1);
	float3 uDir2 = normalize(point2.Pos.xyz - sourcePos2);

	//Create meshpoints
	utPut.borderPoint1 = common_VolumeShadow(point1);
	utPut.borderPoint2 = common_VolumeShadow(point2);
	float meshDist = length(point1.Pos.xyz - point2.Pos.xyz);

	//Create endpoints for the backcap
	GS_INPUTSHADOW tempPoint = point1;
	tempPoint.Pos.xyz = sourcePos1;
	tempPoint.Pos.xyz += uDir1*Radius;
	utPut.endPoint1 = common_VolumeShadow(tempPoint);

	tempPoint = point2;
	tempPoint.Pos.xyz = sourcePos2;
	tempPoint.Pos.xyz += uDir2*Radius;
	utPut.endPoint2 = common_VolumeShadow(tempPoint);

	//Add edgepoints
	utPut.edgePoint1 = utPut.endPoint1;
	utPut.edgePoint2 = utPut.endPoint2;

	float3 dir1 = normalize(point1.Pos.xyz - Position);
	float3 dir2 = normalize(point2.Pos.xyz - Position);

	//Create backcap-points
	tempPoint = point1;
	tempPoint.Pos.xyz = Position;
	tempPoint.Pos.xyz += dir1*Radius;
	utPut.capPoint1 = common_VolumeShadow(tempPoint);
	tempPoint = point2;
	tempPoint.Pos.xyz = Position;
	tempPoint.Pos.xyz += dir2*Radius;
	utPut.capPoint2 = common_VolumeShadow(tempPoint);

	//Figure out if and where the borderlines intersect

	//The normal of the face created by the border and the light
	float3 shadowFaceNormal = cross(point2.Pos.xyz - point1.Pos.xyz, Position - point1.Pos.xyz);
	//The normals and constants of the planes perpendicular to the border and containing the direction vectors of the light
	float3 borderPlane1 = cross(uDir1, point2.Pos.xyz - point1.Pos.xyz);
	float4 plane1 = float4(cross(borderPlane1, uDir1), 0);
	plane1.w = plane1.x*point1.Pos.x + plane1.y*point1.Pos.y+plane1.z*point1.Pos.z;
	float3 borderPlane2 = cross(point1.Pos.xyz - point2.Pos.xyz, uDir2);
	float4 plane2 = float4(cross(uDir2, borderPlane2), 0);
	plane2.w = plane2.x*point2.Pos.x + plane2.y*point2.Pos.y + plane2.z*point2.Pos.z;

	//The normal of the intersection between the 2 planes
	float3 interLine = cross(plane1.xyz, plane2.xyz);
	//Find the a point on the line  where the planes intersect
	float3 interPos = float3(0, 0, 0);
	bool theyIntersect = true;

	if (interLine.x != 0)//Let x be 0 solve for y & z
	{

		float det = plane1.y*plane2.z - plane1.z*plane2.y;
		interPos.y = (+plane2.z*plane1.w - plane1.z*plane2.w) / det;
		interPos.z = (-plane2.y*plane1.w + plane1.y*plane2.w) / det;

	}
	else if (interLine.y != 0)//Let y be 0 solve for x & z
	{

		float det = plane1.x*plane2.z - plane1.z*plane2.x;

		interPos.x = (+plane2.z*plane1.w - plane1.z*plane2.w) / det;
		interPos.z = (-plane2.x*plane1.w + plane1.x*plane2.w) / det;

	}
	else if (interLine.z != 0)//Let z be 0 solve for x & y
	{

		float det = plane1.x*plane2.y - plane1.y*plane2.x;

		interPos.x = (+plane2.y*plane1.w - plane1.y*plane2.w) / det;
		interPos.y = (-plane2.x*plane1.w + plane1.x*plane2.w) / det;

	}
	else//Planes do not intersect
	{
		
		theyIntersect = false;

	}

	/*utPut.endPoint1.Pos.xyz = interPos;
	utPut.endPoint2.Pos.xyz = interPos + normalize(interLine);*/

	if (theyIntersect)//See if the point is above or below the plane from the border to the light
	{


		//Find the distances between the points on the border to the planes containing the intersection
		float3 interPlane1 = cross(interLine, uDir2);
		float dir1Dist = dot(interPos - point1.Pos.xyz, interPlane1) / dot(uDir1, interPlane1);
		float3 interPlane2 = cross(interLine, uDir1);
		float dir2Dist = dot(interPos - point2.Pos.xyz, interPlane2) / dot(uDir2, interPlane2);

		if (dir1Dist > 0 && dir2Dist > 0)//The planes intersect below the border
		{

			//Modify the edgepoints
			utPut.edgePoint1.Pos.xyz = point1.Pos.xyz + uDir1*dir1Dist;
			utPut.edgePoint2.Pos.xyz = point2.Pos.xyz + uDir2*dir2Dist;

		}

	}
	return utPut;

}

WedgePoints common_TransformWedgePoints(WedgePoints input)
{
	
	input.borderPoint1 = transformPoint(input.borderPoint1);
	input.borderPoint2 = transformPoint(input.borderPoint2);
	input.endPoint1 = transformPoint(input.endPoint1);
	input.endPoint2 = transformPoint(input.endPoint2);
	input.edgePoint1 = transformPoint(input.edgePoint1);
	input.edgePoint2 = transformPoint(input.edgePoint2);
	input.capPoint1 = transformPoint(input.capPoint1);
	input.capPoint2 = transformPoint(input.capPoint2);
	return input;

}

[maxvertexcount(30)]//2 caps with 3 borders each having a maximum of 8 vertices (3*2)+(3*8)
void GSSHADOWUmbraVolume(triangleadj GS_INPUTSHADOW input[6], inout TriangleStream<PS_INPUTSHADOW> OutStream)
{

	float3 faceNormal = calculateTriangleNormal(input[0].Pos, input[2].Pos, input[4].Pos);//Get the normal of the triangle

	if (!checkBackWithSize(input[0].Pos, faceNormal))//Perform backface culling
	{

		bool sharesEdge = false;

		//Add the frontcap
		OutStream.Append(transformPoint(common_VolumeShadow(input[0])));
		OutStream.Append(transformPoint(common_VolumeShadow(input[2])));
		OutStream.Append(transformPoint(common_VolumeShadow(input[4])));
		OutStream.RestartStrip();

		//Add the backcap
		OutStream.Append(transformPoint(common_BackCapGenerate(input[4])));
		OutStream.Append(transformPoint(common_BackCapGenerate(input[2])));
		OutStream.Append(transformPoint(common_BackCapGenerate(input[0])));
		OutStream.RestartStrip();

		for (uint k = 0; k < 3; k++)
		{

			GS_INPUTSHADOW point2 = input[(k * 2) + 1];//The adj
			uint firstPos = k * 2;
			uint secondPos = (k * 2 + 2) % (uint)6;

			float dist = distance(point2.Pos.xyz, input[firstPos].Pos.xyz);
			float3 borderFaceNormal = -faceNormal;

			if (dist > epsilon)
			{

				borderFaceNormal = calculateTriangleNormal(input[firstPos].Pos, input[(k * 2) + 1].Pos, input[secondPos].Pos);

			}

			if (checkBackWithSize(input[firstPos].Pos, borderFaceNormal))//Shares border with a backface
			{

				WedgePoints cup = common_VolumeGetWedgePoints(input[firstPos], input[secondPos],1);
				float3 endLine = cup.endPoint2.Pos.xyz - cup.endPoint1.Pos.xyz;
				float3 capLine = cup.capPoint2.Pos.xyz - cup.capPoint1.Pos.xyz;
				float lineDots = dot(endLine, capLine);

				cup = common_TransformWedgePoints(cup);

				//Add the face
				OutStream.Append(cup.borderPoint2);
				OutStream.Append(cup.borderPoint1);
				OutStream.Append(cup.edgePoint2);
				OutStream.Append(cup.edgePoint1);
				OutStream.RestartStrip();

				if (lineDots >= 0)
				{

					OutStream.Append(cup.endPoint2);
					OutStream.Append(cup.endPoint1);
					OutStream.Append(cup.capPoint2);
					OutStream.Append(cup.capPoint1);
					OutStream.RestartStrip();

				}
				else
				{

					OutStream.Append(cup.endPoint1);
					OutStream.Append(cup.endPoint2);
					OutStream.Append(cup.capPoint2);
					OutStream.Append(cup.capPoint1);
					OutStream.RestartStrip();

				}
			}
		}
	}
}

[maxvertexcount(30)]//2 caps with 3 borders each having a maximum of 8 vertices (3*2)+(3*8)
void GSSHADOWPenUmbraVolume(triangleadj GS_INPUTSHADOW input[6], inout TriangleStream<PS_INPUTSHADOW> OutStream)
{

	float3 faceNormal = calculateTriangleNormal(input[0].Pos, input[2].Pos, input[4].Pos);//Get the normal of the triangle

	if (!checkBackWithSize(input[0].Pos, -faceNormal))//Perform backface culling
	{

		bool sharesEdge = false;

		//Add the frontcap
		OutStream.Append(transformPoint(common_VolumeShadow(input[0])));
		OutStream.Append(transformPoint(common_VolumeShadow(input[2])));
		OutStream.Append(transformPoint(common_VolumeShadow(input[4])));
		OutStream.RestartStrip();

		//Add the backcap
		OutStream.Append(transformPoint(common_BackCapGenerate(input[0])));
		OutStream.Append(transformPoint(common_BackCapGenerate(input[2])));
		OutStream.Append(transformPoint(common_BackCapGenerate(input[4])));
		OutStream.RestartStrip();

		for (uint k = 0; k < 3; k++)
		{

			GS_INPUTSHADOW point2 = input[(k * 2) + 1];//The adj
			uint firstPos = k * 2;
			uint secondPos = (k * 2 + 2) % (uint)6;

			float dist = distance(point2.Pos.xyz, input[firstPos].Pos.xyz);
			float3 borderFaceNormal = -faceNormal;

			if (dist > epsilon)
			{

				borderFaceNormal = calculateTriangleNormal(input[firstPos].Pos, input[(k * 2) + 1].Pos, input[secondPos].Pos);

			}

			if (checkBackWithSize(input[firstPos].Pos, -borderFaceNormal))//Shares border with a backface
			{

				WedgePoints cup = common_VolumeGetWedgePoints(input[firstPos], input[secondPos], -1);
				float3 endLine = cup.endPoint2.Pos.xyz - cup.endPoint1.Pos.xyz;
				float3 capLine = cup.capPoint2.Pos.xyz - cup.capPoint1.Pos.xyz;
				float lineDots = dot(endLine, capLine);

				cup = common_TransformWedgePoints(cup);

				//Add the face
				OutStream.Append(cup.borderPoint1);
				OutStream.Append(cup.borderPoint2);
				OutStream.Append(cup.edgePoint1);
				OutStream.Append(cup.edgePoint2);
				OutStream.RestartStrip();

				if (lineDots >= 0)
				{

					OutStream.Append(cup.endPoint1);
					OutStream.Append(cup.endPoint2);
					OutStream.Append(cup.capPoint1);
					OutStream.Append(cup.capPoint2);
					OutStream.RestartStrip();

				}
				else
				{

					OutStream.Append(cup.endPoint2);
					OutStream.Append(cup.endPoint1);
					OutStream.Append(cup.capPoint1);
					OutStream.Append(cup.capPoint2);
					OutStream.RestartStrip();

				}
			}
		}
	}
}

PS_INPUTPUMBSHADOW common_VolumePenShadow(PS_INPUTSHADOW input, float invDist)
{

	PS_INPUTPUMBSHADOW temp;
	temp.Pos = input.Pos;
	temp.SrcPos = input.Pos.xyz / input.Pos.w;
	temp.InvDst = invDist;
	return temp;

}

WedgePoints2 common_transFormWedge(WedgePoints wedge, GS_INPUTSHADOW point1, GS_INPUTSHADOW point2)
{

	WedgePoints2 utPut;

	float3 pDir1 = point1.Pos.xyz - (Position - (Size)*point1.Normal);
	float3 pDir2 = point2.Pos.xyz - (Position - (Size)*point2.Normal);

	float mP1Dist = length(pDir1);
	float mP2Dist = length(pDir2);

	mP1Dist = mP1Dist == 0 ? 0 : 1 / mP1Dist;
	mP2Dist = mP2Dist == 0 ? 0 : 1 / mP2Dist;

	utPut.borderPoint1 = common_VolumePenShadow(wedge.borderPoint1, mP1Dist);
	utPut.borderPoint2 = common_VolumePenShadow(wedge.borderPoint2, mP2Dist);

	utPut.edgePoint1 = common_VolumePenShadow(wedge.edgePoint1, mP1Dist);
	utPut.edgePoint1.SrcPos = utPut.borderPoint1.SrcPos;
	utPut.edgePoint2 = common_VolumePenShadow(wedge.edgePoint2, mP2Dist);
	utPut.edgePoint2.SrcPos = utPut.borderPoint2.SrcPos;

	utPut.capPoint1 = common_VolumePenShadow(wedge.capPoint1, mP1Dist);
	utPut.capPoint1.SrcPos = utPut.borderPoint1.SrcPos;
	utPut.capPoint2 = common_VolumePenShadow(wedge.capPoint2, mP2Dist);
	utPut.capPoint2.SrcPos = utPut.borderPoint2.SrcPos;

	return utPut;

}

PS_INPUTPUMBSHADOW transformPoint(PS_INPUTPUMBSHADOW input)
{

	input.Pos = mul(input.Pos, ViewProjection);
	return input;

}

WedgePoints2 common_TransformWedgePoints(WedgePoints2 input)
{

	input.borderPoint1 = transformPoint(input.borderPoint1);
	input.borderPoint2 = transformPoint(input.borderPoint2);
	input.edgePoint1 = transformPoint(input.edgePoint1);
	input.edgePoint2 = transformPoint(input.edgePoint2);
	return input;

}

[maxvertexcount(27)]// 3 borders each having a maximum of 8 vertices + a backcap (3*8)+3
void GSSHADOWSSVolume(triangleadj GS_INPUTSHADOW input[6], inout TriangleStream<PS_INPUTPUMBSHADOW> OutStream)
{

	float3 faceNormal = calculateTriangleNormal(input[0].Pos, input[2].Pos, input[4].Pos);//Get the normal of the triangle

	if (!checkBackWithSize(input[0].Pos, -faceNormal))//Perform backface culling
	{

		bool sharesEdge = false;

		//Add the backcap TODO: Fix Backcap
		PS_INPUTSHADOW backPoint1 = common_BackCapGenerate(input[4]);
		PS_INPUTSHADOW backPoint2 = common_BackCapGenerate(input[2]);
		PS_INPUTSHADOW backPoint3 = common_BackCapGenerate(input[0]);

		float bp1Dist = length(backPoint1.Pos.xyz - Position);
		float bp2Dist = length(backPoint2.Pos.xyz - Position);
		float bp3Dist = length(backPoint3.Pos.xyz - Position);

		bp1Dist = bp1Dist == 0 ? 0 : 1 / bp1Dist;
		bp2Dist = bp2Dist == 0 ? 0 : 1 / bp2Dist;
		bp3Dist = bp3Dist == 0 ? 0 : 1 / bp3Dist;

		/*OutStream.Append(transformPoint(common_VolumePenShadow(backPoint1, bp1Dist)));
		OutStream.Append(transformPoint(common_VolumePenShadow(backPoint2, bp2Dist)));
		OutStream.Append(transformPoint(common_VolumePenShadow(backPoint3, bp3Dist)));*/
		OutStream.RestartStrip();

		for (uint k = 0; k < 3; k++)
		{

			GS_INPUTSHADOW point2 = input[(k * 2) + 1];//The adj
			uint firstPos = k * 2;
			uint secondPos = (k * 2 + 2) % (uint)6;

			float dist = distance(point2.Pos.xyz, input[firstPos].Pos.xyz);
			float3 borderFaceNormal = -faceNormal;

			if (dist > epsilon)
			{

				borderFaceNormal = calculateTriangleNormal(input[firstPos].Pos, input[(k * 2) + 1].Pos, input[secondPos].Pos);

			}

			if (checkBackWithSize(input[firstPos].Pos, -borderFaceNormal))//Shares border with a backface
			{

				WedgePoints2 cup = common_transFormWedge(common_VolumeGetWedgePoints(input[firstPos], input[secondPos], -1), input[firstPos], input[secondPos]);

				cup = common_TransformWedgePoints(cup);

				//Add the face
				OutStream.Append(cup.borderPoint1);
				OutStream.Append(cup.borderPoint2);
				OutStream.Append(cup.edgePoint1);
				OutStream.Append(cup.edgePoint2);
				OutStream.RestartStrip();

				OutStream.Append(cup.edgePoint1);
				OutStream.Append(cup.edgePoint2);
				OutStream.Append(cup.capPoint1);
				OutStream.Append(cup.capPoint2);
				OutStream.RestartStrip();

			}
		}
	}
}

PS_SSVOUTPUT SSVDEPTH(PS_INPUTPUMBSHADOW input)
{

	//Get pixel in clipspace
	float4 position = input.Pos;
	float dpth = depth.Load(int3(input.Pos.xy, 0)).r;
	uint width = 1;
	uint height = 1;
	depth.GetDimensions(width, height);
	position.x /= width;
	position.y /= height;
	position.z = dpth;
	position.w = 1;

	position.x = 2 * position.x - 1;
	position.y = -(2 * position.y - 1);

	//Convert to worldspace
	position = mul(position, InvertViewProjection);
	position /= position.w;

	//Figure distance between points
	float worldDist = length(position.xyz - input.SrcPos.xyz);

	PS_SSVOUTPUT output;
	//Normalize depth
	output.Depth = (worldDist*Projection._33 + Projection._34) / (worldDist*Projection._43 + Projection._44);
	//Write out the search width in screen space
	output.SearchRegion = (worldDist*input.InvDst*Size)*(1 / (2 * tan(FOV / 2))) / (abs(Projection._43 / (dpth - Projection._33) - ZNear));
	return output;

}

float gaussBlur(float2 searchVector, float sigma, float searchRadius,float2 basePos,bool primary)
{

	float blockInc = ((ShadowSearch - 1)*0.5f);
	float shadowFactor = 0;
	float baseWeight = 1 / sqrt(2 * pi*sigma);
	float totalWeight = 0;
	uint width = 1;
	uint height = 1;
	depth.GetDimensions(width, height);

	for (float x = -blockInc; x <= blockInc; x++)
	{

		float2 pos = float2(basePos + (searchVector*((x / blockInc)*searchRadius)));
		pos.x = max(min((0.5*pos.x + 0.5f), 1), 0)*(width-1);
		pos.y = max(min((-0.5*pos.y + 0.5f),1),0)*(height-1);
		float weight = baseWeight*exp(-(pow(x, 2) / (2 * sigma)));
		totalWeight += weight;
		float facHere = primary ? shadowMap.Load(int3(pos, 0)).r : (float)(shadowMapStencil.Load(int3(pos, 0)).g);
		shadowFactor += weight*(min(facHere, 1));

	}

	shadowFactor /= totalWeight;

	return shadowFactor;

}

float4 SSGAUSSMAJOR(float4 pos:SV_POSITION, float2 screenSpace : TEXCOORD0) : SV_Target
{

	float3 normalData = 2.0f * norm.Load(int3(pos.xy, 0)).xyz - 1.0f; 

	float4 point1;//Position of the pixel in screenspace
	point1.x = 2 * screenSpace.x - 1;
	point1.y = -(2 * screenSpace.y - 1);
	point1.z = depth.Load(uint3(pos.xy, 0)).x;
	point1.w = 1.0f;

	float4 position = mul(point1, InvertViewProjection);//Convert the point to worldspace
	position /= position.w;
	
	float4 point2 = position + float4(normalData, 0);//A point in the direction of the normal
	point2 = mul(point2, ViewProjection);//Convert the point to screenspace
	point2 /= point2.w;

	float3 screenNormal = normalize(point2.xyz - point1.xyz);

	float para = dot(screenNormal, float3(0,0,1));
	
	//Figure out the eclipses axis
	float2 minorAxis = 1 - para < epsilon ? float2(0, 1) : normalize(screenNormal.xy);
		float2 majorAxis = 1 - para < epsilon ? float2(1, 0) : normalize(cross(float3(minorAxis, 0), screenNormal).xy);

	float search = ShadowSearch;
	float sigma = pow(search / 6, 2);
	float searchRadius = shadowInfo.Load(int3(pos.xy, 0)).x;

	return gaussBlur(majorAxis, sigma, searchRadius, float2(2 * screenSpace.x - 1, -(2 * screenSpace.y - 1)), !ShadowVolumes);

}

float SSGAUSSMINOR(float4 pos:SV_POSITION, float2 screenSpace : TEXCOORD0) : SV_Target
{

	float3 normalData = 2.0f * norm.Load(int3(pos.xy, 0)).xyz - 1.0f;

	float4 point1;//Position of the pixel in screenspace
	point1.x = 2 * screenSpace.x - 1;
	point1.y = -(2 * screenSpace.y - 1);
	point1.z = depth.Load(uint3(pos.xy, 0)).x;
	point1.w = 1.0f;

	float4 position = mul(point1, InvertViewProjection);//Convert the point to worldspace
	position /= position.w;

	float4 point2 = position + float4(normalData, 0);//A point in the direction of the normal
	point2 = mul(point2, ViewProjection);//Convert the point to screenspace
	point2 /= point2.w;

	float3 screenNormal = normalize(point2.xyz - point1.xyz);
	float para = dot(screenNormal, float3(0, 0, 1));

	//Figure out the eclipses axis
	float2 minorAxis = 1 - para < epsilon ? float2(0, 1) : normalize(screenNormal.xy);

	float3 eyeDir = normalize(CameraPos.xyz - position.xyz);
		para = dot(normalData, eyeDir);
	float search = ShadowSearch;

	float sigma = pow(search / 6, 2);
	float searchRadius = shadowInfo.Load(int3(pos.xy, 0)).x;

	return gaussBlur(minorAxis, sigma, searchRadius*para, float2(2 * screenSpace.x - 1, -(2 * screenSpace.y - 1)), true);

}

float4 SHADOWMAPPER(float4 pos:SV_POSITION):SV_Target{return float4((1-pos.z/pos.w),(1-pos.z/pos.w),(1-pos.z/pos.w),1);}

float4 SHADOWDBG(float4 pos:SV_POSITION) : SV_Target{ return Diffuse; }
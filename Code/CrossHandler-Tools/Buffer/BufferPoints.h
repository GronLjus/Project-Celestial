#pragma once
#include "../../CelestialMath/CelestialMath.h"
using namespace CelestialMath;

namespace CrossHandlers
{
	///<summary>This struct contains all data needed to represents a vertex</summary>
	struct BufferVertex
	{

		///<summary>The position of the vertex</summary>
		Vector3 pos;
		///<summary>The UV-coordinates of the vertex</summary>
		Vector2 texture;
		///<summary>The normal of the vertex</summary>
		Vector3 normal;

		///<param name='p'>[in]The position of the vector</param>
		///<param name='t'>[in]The texture of the vector</param>
		///<param name='n'>[in]The normal of the vector</param>
		BufferVertex(Vector3 p, Vector2 t, Vector3 n) : pos(p), texture(t), normal(n) {}
		BufferVertex(){}
	};

	///<summary>This struct contains the data needed to represent an instances transformation</summary>
	struct Instance
	{
		///<summary>The first column of the world-matrix</summary>
		Vector4 wA;
		///<summary>The second column of the world-matrix</summary>
		Vector4 wB;
		///<summary>The third column of the world-matrix</summary>
		Vector4 wC;
		///<summary>The fourth column of the world-matrix</summary>
		Vector4 wD;

		///<summary>The first column of the inverted transposed world-matrix</summary>
		Vector4 iWA;
		///<summary>The second column of the inverted transposed world-matrix</summary>
		Vector4 iWB;
		///<summary>The third column of the inverted transposed world-matrix</summary>
		Vector4 iWC;
		///<summary>The fourth column of the inverted transposed world-matrix</summary>
		Vector4 iWD;

		///<summary>The first column of the old world-matrix</summary>
		Vector4 oWA;
		///<summary>The second column of the old world-matrix</summary>
		Vector4 oWB;
		///<summary>The third column of the old world-matrix</summary>
		Vector4 oWC;
		///<summary>The fourth column of the old world-matrix</summary>
		Vector4 oWD;

		///<param name='w'>[in]The world-matrix of the transformation</param>
		///<param name='iWT'>[in]The inverted transposed world-matrix of the transformation</param>
		///<param name='w'>[in]The last world-matrix of the transformation</param>
		Instance(Matrix w, Matrix iWT, Matrix oWT)
		{

			wA = Vector4(w._11, w._21, w._31, w._41);
			wB = Vector4(w._12, w._22, w._32, w._42);
			wC = Vector4(w._13, w._23, w._33, w._43);
			wD = Vector4(w._14, w._24, w._34, w._44);

			iWA = Vector4(iWT._11, iWT._21, iWT._31, iWT._41);
			iWB = Vector4(iWT._12, iWT._22, iWT._32, iWT._42);
			iWC = Vector4(iWT._13, iWT._23, iWT._33, iWT._43);
			iWD = Vector4(iWT._14, iWT._24, iWT._34, iWT._44);

			oWA = Vector4(oWT._11, oWT._21, oWT._31, oWT._41);
			oWB = Vector4(oWT._12, oWT._22, oWT._32, oWT._42);
			oWC = Vector4(oWT._13, oWT._23, oWT._33, oWT._43);
			oWD = Vector4(oWT._14, oWT._24, oWT._34, oWT._44);

		}

		Instance(){}

	};
}
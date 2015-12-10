#pragma once
#include "CelestialMath.h"

namespace CrossHandlers
{
	///<summary>This struct contains all data needed to represents a vertex</summary>
	struct BufferVertex
	{

		///<summary>The position of the vertex</summary>
		CelestialMath::Vector3 pos;
		///<summary>The UV-coordinates of the vertex</summary>
		CelestialMath::Vector2 texture;
		///<summary>The normal of the vertex</summary>
		CelestialMath::Vector3 normal;

		///<param name='p'>[in]The position of the vector</param>
		///<param name='t'>[in]The texture of the vector</param>
		///<param name='n'>[in]The normal of the vector</param>
		BufferVertex(CelestialMath::Vector3 p, CelestialMath::Vector2 t, CelestialMath::Vector3 n) : pos(p), texture(t), normal(n) {}
		BufferVertex(){}
	};
}
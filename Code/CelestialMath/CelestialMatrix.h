#pragma once
namespace CelestialMath
{
	typedef struct Matrix
	{
		FLOAT _11;
		FLOAT _12;
		FLOAT _13;
		FLOAT _14;
		FLOAT _21;
		FLOAT _22;
		FLOAT _23;
		FLOAT _24;
		FLOAT _31;
		FLOAT _32;
		FLOAT _33;
		FLOAT _34;
		FLOAT _41;
		FLOAT _42;
		FLOAT _43;
		FLOAT _44;

		Matrix(){
			_11 = 1.0f; _12 = 0.0f; _13 = 0.0f; _14 = 0.0f;
			_21 = 0.0f; _22 = 1.0f; _23 = 0.0f; _24 = 0.0f;
			_31 = 0.0f; _32 = 0.0f; _33 = 1.0f; _34 = 0.0f;
			_41 = 0.0f; _42 = 0.0f; _43 = 0.0f; _44 = 1.0f;
		}

		Matrix(FLOAT _11, FLOAT _12, FLOAT _13, FLOAT _14,
			FLOAT _21, FLOAT _22, FLOAT _23, FLOAT _24,
			FLOAT _31, FLOAT _32, FLOAT _33, FLOAT _34,
			FLOAT _41, FLOAT _42, FLOAT _43, FLOAT _44)
			: _11(_11), _12(_12), _13(_13), _14(_14),
			_21(_21), _22(_22), _23(_23), _24(_24),
			_31(_31), _32(_32), _33(_33), _34(_34),
			_41(_41), _42(_42), _43(_43), _44(_44){}

	} Matrix;

}
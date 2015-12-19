#pragma once

namespace CelestialMath
{
	typedef struct Vector2
	{

		float x;
		float y;
		Vector2(){}
		Vector2(float x, float y) : x(x), y(y){}

		Vector2 operator - () const{ return Vector2(-x, -y); }

		Vector2 operator + (const Vector2& vec) const{ return Vector2(x + vec.x, y + vec.y); }
		Vector2 operator - (const Vector2& vec) const{ return Vector2(x - vec.x, y - vec.y); }
		Vector2 operator * (const Vector2& vec) const{ return Vector2(x * vec.x, y * vec.y); }
		Vector2 operator / (const Vector2& vec) const{ return Vector2(x / vec.x, y / vec.y); }
		Vector2 operator * (const float& amnt) const{ return Vector2(x * amnt, y * amnt); }
		Vector2 operator / (const float& amnt) const{ return Vector2(x / amnt, y / amnt); }

		Vector2& operator += (const Vector2& vec) { x += vec.x; y += vec.y; return *this; }
		Vector2& operator -= (const Vector2& vec) { x -= vec.x; y -= vec.y; return *this; }
		Vector2& operator *= (const Vector2& vec) { x *= vec.x; y *= vec.y; return *this; }
		Vector2& operator /= (const Vector2& vec) { x /= vec.x; y /= vec.y; return *this; }
		Vector2& operator *= (const float& amnt) { x *= amnt; y *= amnt; return *this; }
		Vector2& operator /= (const float& amnt) { x /= amnt; y /= amnt; return *this; }

		bool operator > (const float& val) const{ return x > val && y > val; }
		bool operator < (const float& val) const{ return x < val && y < val; }

	} Vector2;

	typedef struct vectorUI2
	{

		unsigned int x;
		unsigned int y;
		vectorUI2(){}
		vectorUI2(unsigned int x, unsigned int y) : x(x), y(y){}

		vectorUI2 operator - () const{ return vectorUI2(-x, -y); }

		vectorUI2 operator + (const vectorUI2& vec) const{ return vectorUI2(x + vec.x, y + vec.y); }
		vectorUI2 operator - (const vectorUI2& vec) const{ return vectorUI2(x - vec.x, y - vec.y); }
		vectorUI2 operator * (const vectorUI2& vec) const{ return vectorUI2(x * vec.x, y * vec.y); }
		vectorUI2 operator / (const vectorUI2& vec) const{ return vectorUI2(x / vec.x, y / vec.y); }
		vectorUI2 operator * (const unsigned int& amnt) const{ return vectorUI2(x * amnt, y * amnt); }
		vectorUI2 operator / (const unsigned int& amnt) const{ return vectorUI2(x / amnt, y / amnt); }

		vectorUI2& operator += (const vectorUI2& vec) { x += vec.x; y += vec.y; return *this; }
		vectorUI2& operator -= (const vectorUI2& vec) { x -= vec.x; y -= vec.y; return *this; }
		vectorUI2& operator *= (const vectorUI2& vec) { x *= vec.x; y *= vec.y; return *this; }
		vectorUI2& operator /= (const vectorUI2& vec) { x /= vec.x; y /= vec.y; return *this; }
		vectorUI2& operator *= (const unsigned int& amnt) { x *= amnt; y *= amnt; return *this; }
		vectorUI2& operator /= (const unsigned int& amnt) { x /= amnt; y /= amnt; return *this; }

		bool operator > (const unsigned int& val) const{ return x > val && y > val; }
		bool operator < (const unsigned int& val) const{ return x < val && y < val; }

	} vectorUI2;

	typedef struct Vector3
	{
		float x;
		float y;
		float z;
		Vector3(){}
		Vector3(float x, float y, float z) : x(x), y(y), z(z){}
		Vector3(Vector2 xy, float z) : x(xy.x), y(xy.y), z(z){}

		Vector3 operator - () const{ return Vector3(-x, -y, -z); }

		Vector3 operator + (const Vector3& vec) const{ return Vector3(x + vec.x, y + vec.y, z + vec.z); }
		Vector3 operator - (const Vector3& vec) const{ return Vector3(x - vec.x, y - vec.y, z - vec.z); }
		Vector3 operator * (const Vector3& vec) const{ return Vector3(x * vec.x, y * vec.y, z * vec.z); }
		Vector3 operator / (const Vector3& vec) const{ return Vector3(x / vec.x, y / vec.y, z / vec.z); }
		Vector3 operator * (const float& amnt) const{ return Vector3(x * amnt, y * amnt, z * amnt); }
		Vector3 operator / (const float& amnt) const{ return Vector3(x / amnt, y / amnt, z / amnt); }

		Vector3& operator += (const Vector3& vec) { x += vec.x; y += vec.y; z += vec.z; return *this; }
		Vector3& operator -= (const Vector3& vec) { x -= vec.x; y -= vec.y; z -= vec.z; return *this; }
		Vector3& operator *= (const Vector3& vec) { x *= vec.x; y *= vec.y; z *= vec.z; return *this; }
		Vector3& operator /= (const Vector3& vec) { x /= vec.x; y /= vec.y; z /= vec.z; return *this; }
		Vector3& operator *= (const float& amnt) { x *= amnt; y *= amnt; z *= amnt; return *this; }
		Vector3& operator /= (const float& amnt) { x /= amnt; y /= amnt; z /= amnt; return *this; }

		bool operator > (const float& val) const{ return x > val && y > val && z > val; }
		bool operator < (const float& val) const{ return x < val && y < val && z < val; }


	} Vector3;

	typedef struct Vector4
	{
		float x;
		float y;
		float z;
		float w;
		Vector4(){}
		Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w){}
		Vector4(Vector2 xy, Vector2 zw) : x(xy.x), y(xy.y), z(zw.x), w(zw.y){}
		Vector4(Vector3 xyz, float w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w){}

		Vector4 operator - () const{ return Vector4(-x, -y, -z,-w); }

		Vector4 operator + (const Vector4& vec) const{ return Vector4(x + vec.x, y + vec.y, z + vec.z, w + vec.w); }
		Vector4 operator - (const Vector4& vec) const{ return Vector4(x - vec.x, y - vec.y, z - vec.z, w - vec.w); }
		Vector4 operator * (const Vector4& vec) const{ return Vector4(x * vec.x, y * vec.y, z * vec.z, w * vec.w); }
		Vector4 operator / (const Vector4& vec) const{ return Vector4(x / vec.x, y / vec.y, z / vec.z, w / vec.w); }
		Vector4 operator * (const float& amnt) const{ return Vector4(x * amnt, y * amnt, z * amnt, w * amnt); }
		Vector4 operator / (const float& amnt) const{ return Vector4(x / amnt, y / amnt, z / amnt, w / amnt); }

		Vector4& operator += (const Vector4& vec) { x += vec.x; y += vec.y; z += vec.z; w += vec.w; return *this; }
		Vector4& operator -= (const Vector4& vec) { x -= vec.x; y -= vec.y; z -= vec.z; w -= vec.w; return *this; }
		Vector4& operator *= (const Vector4& vec) { x *= vec.x; y *= vec.y; z *= vec.z; w *= vec.w; return *this; }
		Vector4& operator /= (const Vector4& vec) { x /= vec.x; y /= vec.y; z /= vec.z; w /= vec.w; return *this; }
		Vector4& operator *= (const float& amnt) { x *= amnt; y *= amnt; z *= amnt; w *= amnt; return *this; }
		Vector4& operator /= (const float& amnt) { x /= amnt; y /= amnt; z /= amnt; w /= amnt; return *this; }

		bool operator > (const float& val) const{ return x > val && y > val && z > val && w > val; }
		bool operator < (const float& val) const{ return x < val && y < val && z < val && w < val; }

	} Vector4;

}
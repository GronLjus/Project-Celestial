#include "StdAfx.h"
#include "CelestialMath.h"
#include <cmath>

using namespace CelestialMath;

Matrix CelestialMath::MatrixLookAtLH(Vector3 &pos, Vector3 &lookAt, Vector3 &up)
{

	Vector3 zaxis = VectorNormal(lookAt - pos);
	Vector3 xaxis = VectorNormal(VectorCross(up, zaxis));
	Vector3 yaxis = VectorCross(zaxis, xaxis);

	return Matrix(xaxis.x,yaxis.x,zaxis.x,0,
		xaxis.y, yaxis.y, zaxis.y, 0,
		xaxis.z, yaxis.z, zaxis.z, 0,
		-VectorDot(xaxis, pos), -VectorDot(yaxis, pos), -VectorDot(zaxis, pos), 1);

}

Matrix CelestialMath::MatrixInverse(Matrix &input)
{

	double inv[16];
	float det;

	inv[0] = ((double)input._22) * ((double)input._33) * ((double)input._44) -
		((double)input._22) * ((double)input._34) * ((double)input._43) -
		((double)input._32) * ((double)input._23) * ((double)input._44) +
		((double)input._32) * ((double)input._24) * ((double)input._43) +
		((double)input._42) * ((double)input._23) * ((double)input._34) -
		((double)input._42) * ((double)input._24) * ((double)input._33);

	inv[1] = -((double)input._12) * ((double)input._33) * ((double)input._44) +
		((double)input._12) * ((double)input._34) * ((double)input._43) +
		((double)input._32) * ((double)input._13) * ((double)input._44) -
		((double)input._32) * ((double)input._14) * ((double)input._43) -
		((double)input._42) * ((double)input._13) * ((double)input._34) +
		((double)input._42) * ((double)input._14) * ((double)input._33);

	inv[2] = ((double)input._12) * ((double)input._23) * ((double)input._44) -
		((double)input._12) * ((double)input._24) * ((double)input._43) -
		((double)input._22) * ((double)input._13) * ((double)input._44) +
		((double)input._22) * ((double)input._14) * ((double)input._43) +
		((double)input._42) * ((double)input._13) * ((double)input._24) -
		((double)input._42) * ((double)input._14) * ((double)input._23);

	inv[3] = -((double)input._12) * ((double)input._23) * ((double)input._34) +
		((double)input._12) * ((double)input._24) * ((double)input._33) +
		((double)input._22) * ((double)input._13) * ((double)input._34) -
		((double)input._22) * ((double)input._14) * ((double)input._33) -
		((double)input._32) * ((double)input._13) * ((double)input._24) +
		((double)input._32) * ((double)input._14) * ((double)input._23);

	inv[4] = -((double)input._21) * ((double)input._33) * ((double)input._44) +
		((double)input._21) * ((double)input._34) * ((double)input._43) +
		((double)input._31) * ((double)input._23) * ((double)input._44) -
		((double)input._31) * ((double)input._24) * ((double)input._43) -
		((double)input._41) * ((double)input._23) * ((double)input._34) +
		((double)input._41) * ((double)input._24) * ((double)input._33);

	inv[5] = ((double)input._11) * ((double)input._33) * ((double)input._44) -
		((double)input._11) * ((double)input._34) * ((double)input._43) -
		((double)input._31) * ((double)input._13) * ((double)input._44) +
		((double)input._31) * ((double)input._14) * ((double)input._43) +
		((double)input._41) * ((double)input._13) * ((double)input._34) -
		((double)input._41) * ((double)input._14) * ((double)input._33);

	inv[6] = -((double)input._11) * ((double)input._23) * ((double)input._44) +
		((double)input._11) * ((double)input._24) * ((double)input._43) +
		((double)input._21) * ((double)input._13) * ((double)input._44) -
		((double)input._21) * ((double)input._14) * ((double)input._43) -
		((double)input._41) * ((double)input._13) * ((double)input._24) +
		((double)input._41) * ((double)input._14) * ((double)input._23);

	inv[7] = ((double)input._11) * ((double)input._23) * ((double)input._34) -
		((double)input._11) * ((double)input._24) * ((double)input._33) -
		((double)input._21) * ((double)input._13) * ((double)input._34) +
		((double)input._21) * ((double)input._14) * ((double)input._33) +
		((double)input._31) * ((double)input._13) * ((double)input._24) -
		((double)input._31) * ((double)input._14) * ((double)input._23);

	inv[8] = ((double)input._21) * ((double)input._32) * ((double)input._44) -
		((double)input._21) * ((double)input._34) * ((double)input._42) -
		((double)input._31) * ((double)input._22) * ((double)input._44) +
		((double)input._31) * ((double)input._24) * ((double)input._42) +
		((double)input._41) * ((double)input._22) * ((double)input._34) -
		((double)input._41) * ((double)input._24) * ((double)input._32);

	inv[9] = -((double)input._11) * ((double)input._32) * ((double)input._44) +
		((double)input._11) * ((double)input._34) * ((double)input._42) +
		((double)input._31) * ((double)input._12) * ((double)input._44) -
		((double)input._31) * ((double)input._14) * ((double)input._42) -
		((double)input._41) * ((double)input._12) * ((double)input._34) +
		((double)input._41) * ((double)input._14) * ((double)input._32);

	inv[10] = ((double)input._11) * ((double)input._22) * ((double)input._44) -
		((double)input._11) * ((double)input._24) * ((double)input._42) -
		((double)input._21) * ((double)input._12) * ((double)input._44) +
		((double)input._21) * ((double)input._14) * ((double)input._42) +
		((double)input._41) * ((double)input._12) * ((double)input._24) -
		((double)input._41) * ((double)input._14) * ((double)input._22);

	inv[11] = -((double)input._11) * ((double)input._22) * ((double)input._34) +
		((double)input._11) * ((double)input._24) * ((double)input._32) +
		((double)input._21) * ((double)input._12) * ((double)input._34) -
		((double)input._21) * ((double)input._14) * ((double)input._32) -
		((double)input._31) * ((double)input._12) * ((double)input._24) +
		((double)input._31) * ((double)input._14) * ((double)input._22);

	inv[12] = -((double)input._21) * ((double)input._32) * ((double)input._43) +
		((double)input._21) * ((double)input._33) * ((double)input._42) +
		((double)input._31) * ((double)input._22) * ((double)input._43) -
		((double)input._31) * ((double)input._23) * ((double)input._42) -
		((double)input._41) * ((double)input._22) * ((double)input._33) +
		((double)input._41) * ((double)input._23) * ((double)input._32);

	inv[13] = ((double)input._11) * ((double)input._32) * ((double)input._43) -
		((double)input._11) * ((double)input._33) * ((double)input._42) -
		((double)input._31) * ((double)input._12) * ((double)input._43) +
		((double)input._31) * ((double)input._13) * ((double)input._42) +
		((double)input._41) * ((double)input._12) * ((double)input._33) -
		((double)input._41) * ((double)input._13) * ((double)input._32);

	inv[14] = -((double)input._11) * ((double)input._22) * ((double)input._43) +
		((double)input._11) * ((double)input._23) * ((double)input._42) +
		((double)input._21) * ((double)input._12) * ((double)input._43) -
		((double)input._21) * ((double)input._13) * ((double)input._42) -
		((double)input._41) * ((double)input._12) * ((double)input._23) +
		((double)input._41) * ((double)input._13) * ((double)input._22);

	inv[15] = ((double)input._11) * ((double)input._22) * ((double)input._33) -
		((double)input._11) * ((double)input._23) * ((double)input._32) -
		((double)input._21) * ((double)input._12) * ((double)input._33) +
		((double)input._21) * ((double)input._13) * ((double)input._32) +
		((double)input._31) * ((double)input._12) * ((double)input._23) -
		((double)input._31) * ((double)input._13) * ((double)input._22);

	det = ((double)input._11) * inv[0] + ((double)input._12) * inv[4] + ((double)input._13) * inv[8] + ((double)input._14) * inv[12];

	if (det == 0)
	{

		return Matrix();

	}

	det = 1.0 / det;

	float invOut[16];

	for (int i = 0; i < 16; i++)
	{

		invOut[i] = (float)(inv[i] * det);

	}

	return Matrix(invOut[0], invOut[1], invOut[2], invOut[3],
		invOut[4], invOut[5], invOut[6], invOut[7],
		invOut[8], invOut[9], invOut[10], invOut[11],
		invOut[12], invOut[13], invOut[14], invOut[15]);

}

Matrix CelestialMath::MatrixPerspectiveFovLH(float fov, float aspect, float nearPlane, float farPlane)
{

	float height = 1 / tan(fov * 0.5f);
	float width = height/aspect;
	return Matrix(width, 0.0f, 0.0f, 0.0f,
		0.0f, height, 0.0f, 0.0f,
		0.0f, 0.0f, farPlane / (farPlane - nearPlane), 1.0f,
		0.0f, 0.0f, -nearPlane*farPlane / (farPlane - nearPlane), 0.0f);

}

Matrix CelestialMath::MatrixMultiply(Matrix &a, Matrix &b)
{

	Vector4 aRow1 = Vector4(a._11, a._12, a._13, a._14);
	Vector4 aRow2 = Vector4(a._21, a._22, a._23, a._24);
	Vector4 aRow3 = Vector4(a._31, a._32, a._33, a._34);
	Vector4 aRow4 = Vector4(a._41, a._42, a._43, a._44);

	Vector4 bColumn1 = Vector4(b._11, b._21, b._31, b._41);
	Vector4 bColumn2 = Vector4(b._12, b._22, b._32, b._42);
	Vector4 bColumn3 = Vector4(b._13, b._23, b._33, b._43);
	Vector4 bColumn4 = Vector4(b._14, b._24, b._34, b._44);

	return Matrix(VectorDot(aRow1, bColumn1), VectorDot(aRow1, bColumn2), VectorDot(aRow1, bColumn3), VectorDot(aRow1, bColumn4),
		VectorDot(aRow2, bColumn1), VectorDot(aRow2, bColumn2), VectorDot(aRow2, bColumn3), VectorDot(aRow2, bColumn4),
		VectorDot(aRow3, bColumn1), VectorDot(aRow3, bColumn2), VectorDot(aRow3, bColumn3), VectorDot(aRow3, bColumn4),
		VectorDot(aRow4, bColumn1), VectorDot(aRow4, bColumn2), VectorDot(aRow4, bColumn3), VectorDot(aRow4, bColumn4));

}

Matrix CelestialMath::MatrixTranspose(Matrix &m)
{

	return Matrix(m._11, m._21, m._31, m._41,
		m._12, m._22, m._32, m._42,
		m._13, m._23, m._33, m._43,
		m._14, m._24, m._34, m._44);

}

Matrix CelestialMath::MatrixRotationX(float angle)
{

	float cosAng = cos(angle);
	float sinAng = sin(angle);

	return Matrix(1,0,0,0,
		0,cosAng,sinAng,0,
		0, -sinAng, cosAng,0,
		0,0,0,1);

}

Matrix CelestialMath::MatrixRotationY(float angle)
{

	float cosAng = cos(angle);
	float sinAng = sin(angle);

	return Matrix(cosAng, 0, -sinAng, 0,
		0, 1, 0, 0,
		sinAng, 0, cosAng, 0,
		0, 0, 0, 1);
	
}

Matrix CelestialMath::MatrixRotationZ(float angle)
{

	float cosAng = cos(angle);
	float sinAng = sin(angle);

	return Matrix(cosAng, sinAng, 0, 0,
		-sinAng, cosAng, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

}

Matrix CelestialMath::MatrixRotationYawPitchRoll(float y, float x, float z)
{

	float cosY = cos(y);
	float sinY = sin(y);
	float cosX = cos(x);
	float sinX = sin(x);
	float cosZ = cos(z);
	float sinZ = sin(z);

	Matrix ret = Matrix();
	ret._11 = (cosZ * cosY) + (sinZ * sinX * sinY);
	ret._12 = (sinZ * cosX);
	ret._13 = (cosZ * -sinY) + (sinZ * sinX * cosY);
	ret._21 = (-sinZ * cosY) + (cosZ * sinX * sinY);
	ret._22 = (cosZ * cosX);
	ret._23 = (sinZ * sinY) + (cosZ * sinX * cosY);
	ret._31 = (cosX * sinY);
	ret._32 = -sinX;
	ret._33 = (cosX * cosY);
	return ret;

}

Matrix CelestialMath::MatrixRotationAxis(Vector3 axis, float angle)
{

	axis = VectorNormal(axis);
	Matrix ret = Matrix();
	float c = cos(angle);
	float s = sin(angle);
	float t = 1 - c;		
	
	ret._11 = t*axis.x * axis.x + c;
	ret._12 = t*axis.x * axis.y - s*axis.z;
	ret._13 = t*axis.z * axis.x + s*axis.y;
	ret._21 = t*axis.x * axis.y + s*axis.z;
	ret._22 = t*axis.y * axis.y + c;
	ret._23 = t*axis.y * axis.z - s*axis.x;
	ret._31 = t*axis.z * axis.x - s*axis.y;
	ret._32 = t*axis.y * axis.z + s*axis.x;
	ret._33 = t*axis.z * axis.z + c;

	return ret;

}

Matrix CelestialMath::MatrixScaling(float x, float y, float z)
{

	Matrix ret = Matrix();
	ret._11 = x;
	ret._22 = y;
	ret._33 = z;
	return ret;

}

Matrix CelestialMath::MatrixTranslation(float x, float y, float z)
{

	Matrix ret = Matrix();
	ret._41 = x;
	ret._42 = y;
	ret._43 = z;
	return ret;

}

Vector4 CelestialMath::VectorTransform(Vector4 &vec, Matrix &mat)
{

	return Vector4(vec.x*mat._11 + vec.y*mat._21 + vec.z*mat._31 + vec.w*mat._41,
		vec.x*mat._12 + vec.y*mat._22 + vec.z*mat._32 + vec.w*mat._42,
		vec.x*mat._13 + vec.y*mat._23 + vec.z*mat._33 + vec.w*mat._43,
		vec.x*mat._14 + vec.y*mat._24 + vec.z*mat._34 + vec.w*mat._44);

}

Vector3 CelestialMath::VectorTransform(Vector3 &vec, Matrix &mat)
{

	return Vector3(vec.x*mat._11 + vec.y*mat._21 + vec.z*mat._31 + 1*mat._41,
		vec.x*mat._12 + vec.y*mat._22 + vec.z*mat._32 + 1*mat._42,
		vec.x*mat._13 + vec.y*mat._23 + vec.z*mat._33 + 1*mat._43);

}

Vector3 CelestialMath::VectorCross(Vector3 &vecA, Vector3 &vecB)
{

	return Vector3(vecA.y*vecB.z - vecA.z*vecB.y,
		vecA.z*vecB.x - vecA.x*vecB.z,
		vecA.x*vecB.y - vecA.y*vecB.x);

}

float CelestialMath::VectorDot(Vector4 &vecA, Vector4 &vecB)
{

	return vecA.x*vecB.x + vecA.y*vecB.y + vecA.z*vecB.z + vecA.w*vecB.w;

}

float CelestialMath::VectorDot(Vector3 &vecA, Vector3 &vecB)
{

	return vecA.x*vecB.x + vecA.y*vecB.y + vecA.z*vecB.z;

}

float CelestialMath::VectorDot(Vector2 &vecA, Vector2 &vecB)
{

	return vecA.x*vecB.x + vecA.y*vecB.y;

}

Vector3 CelestialMath::VectorNormal(Vector3 &vecA)
{

	float length = sqrt(vecA.x*vecA.x + vecA.y*vecA.y + vecA.z*vecA.z);
	return Vector3(vecA.x / length, vecA.y / length, vecA.z / length);

}
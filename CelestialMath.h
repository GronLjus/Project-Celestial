#pragma once
#include "CelestialMatrix.h"
#include "CelestialVector.h"

namespace CelestialMath
{

	const double CELESTIAL_PI = 3.1415926535897931;

	Matrix MatrixLookAtLH(Vector3 &pos, Vector3 &lookAt, Vector3 &up);
	Matrix MatrixInverse(Matrix &input);
	Matrix MatrixPerspectiveFovLH(float fov, float aspect, float nearPlane, float farPlane);
	Matrix MatrixMultiply(Matrix &a, Matrix &b);
	Matrix MatrixTranspose(Matrix &m);

	Matrix MatrixRotationX(float angle);
	Matrix MatrixRotationY(float angle);
	Matrix MatrixRotationZ(float angle);
	Matrix MatrixRotationYawPitchRoll(float y, float x, float z);
	Matrix MatrixRotationAxis(Vector3 axis, float angle);
	Matrix MatrixScaling(float x, float y, float z);
	Matrix MatrixTranslation(float x, float y, float z);

	Vector4 VectorTransform(Vector4 &vec, Matrix &mat);
	Vector3 VectorCross(Vector3 &vecA, Vector3 &vecB);
	float VectorDot(Vector4 &vecA, Vector4 &vecB);
	float VectorDot(Vector3 &vecA, Vector3 &vecB);
	float VectorDot(Vector2 &vecA, Vector2 &vecB);
	Vector3 VectorNormal(Vector3 &vecA);

}
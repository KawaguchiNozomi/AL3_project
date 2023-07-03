#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"

Vector3 TransforNomal(const Vector3& v, const Matrix4x4& m);
/// <summary>
/// 長さ(ノルム)
/// </summary>
/// <param name="v"></param>
/// <returns></returns>
float Length(const Vector3& v);
/// <summary>
/// 正規化
/// </summary>
/// <param name="v"></param>
/// <returns></returns>
Vector3 Normalize(const Vector3& v);

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

Vector3 operator*=(Vector3& v, const float& f);
Vector3 operator*(const Vector3& v, const float& f);
Vector3 operator*=(Vector3& v1, const Vector3& v2);
Vector3 operator*(const Vector3& v1, const Vector3& v2);
Vector3 operator-=(Vector3& v1, const Vector3& v2);
Vector3 operator-(const Vector3& v1, const Vector3& v2);
Vector3 operator+=(Vector3& v1, const Vector3& v2);
Vector3 operator+(const Vector3& v1, const Vector3& v2);
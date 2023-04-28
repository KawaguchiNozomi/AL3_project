#pragma once
#include "Vector3.h"
#include <math.h>
/// <summary>
/// 4x4行列
/// </summary>
struct Matrix4x4 final {
	float m[4][4];
};

Matrix4x4 MakeScaleMatrix(const Vector3& Scale);
Matrix4x4 MakeRotationXMatrix(float theta);
Matrix4x4 MakeRotationYMatrix(float theta);
Matrix4x4 MakeRotationZMatrix(float theta);
Matrix4x4 MakeTranslateMatrix(const Vector3& trans);
/// <summary>
/// アフィン変換
/// </summary>
/// <param name="scale"></param>
/// <param name="rot"></param>
/// <param name="translate"></param>
/// <returns></returns>
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);

//代入演算子オーバーロード
Matrix4x4& operator*=(Matrix4x4& m1,const Matrix4x4& m2);
Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);

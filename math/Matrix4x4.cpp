#include "Matrix4x4.h"

Matrix4x4 MakeScaleMatrix(const Vector3& Scale) { 
	
	Matrix4x4 result;

	result = {
		Scale.x,0.0f,   0.0f,   0.0f,
		0.0f,   Scale.y,0.0f,   0.0f,
		0.0f,   0.0f,   Scale.z,0.0f,
		0.0f,   0.0f,   0.0f,   1.0f
	};


	return result; 
}

Matrix4x4 MakeRotationXMatrix(float theta) { 
	
	float sin = sinf(theta);
	float cos = cosf(theta);
	Matrix4x4 result;

	result = {
		1.0f,0.0f,0.0f,0.0f,
		0.0f,cos, sin, 0.0f,
		0.0f,-sin,cos, 0.0f,
		0.0f,0.0f,0.0f, 1.0f

	};

	return result;
}

Matrix4x4 MakeRotationYMatrix(float theta) { 
	
	float sin = sinf(theta);
	float cos = cosf(theta);
	Matrix4x4 result;

	result = {
	    cos, 0.0f, -sin, 0.0f,
		0.0f, 1.0f,  0.0f, 0.0f,
		sin,0.0f, cos,  0.0f, 
		0.0f, 0.0f, 0.0f, 1.0f

	};
	return result; 
}

Matrix4x4 MakeRotationZMatrix(float theta) {

	float sin = sinf(theta);
	float cos = cosf(theta);
	Matrix4x4 result;

	result = {
	    cos,  sin, 0.0f, 0.0f, 
		-sin, cos, 0.0f, 0.0f, 
		0.0f,0.0f, 1.0f, 0.0f, 
		0.0f, 0.0f, 0.0f,1.0f

	};
	return result;
}

Matrix4x4 MakeTranslateMatrix(const Vector3& trans) { 
	
	Matrix4x4 result;

	result = {
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		trans.x,trans.y,trans.z,1.0f
	};

	return result;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate) {

	Matrix4x4 matScale = MakeScaleMatrix(scale);
	Matrix4x4 matRotX = MakeRotationXMatrix(rot.x);
	Matrix4x4 matRotY = MakeRotationYMatrix(rot.y);
	Matrix4x4 matRotZ = MakeRotationZMatrix(rot.z);
	Matrix4x4 matRot = matRotZ * matRotX * matRotY;
	Matrix4x4 matTranslate = MakeTranslateMatrix(translate);

	Matrix4x4 result = matScale * matRot * matTranslate;

	return result;
}

Matrix4x4& operator*=(Matrix4x4& m1, const Matrix4x4& m2) {

	Matrix4x4 result = {};

	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			for (size_t k = 0; k < 4; k++) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}

	m1 = result;
	return m1; 
}

Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) { 
	
	Matrix4x4 result = m1;

	result *= m2;



	return result;
}

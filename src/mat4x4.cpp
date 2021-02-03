#include "mat4x4.h"

namespace WawM {

	mat4x4 MakeIdentity() {
		mat4x4 matrix;
		matrix.m[0][0] = 1.0f;
		matrix.m[1][1] = 1.0f;
		matrix.m[2][2] = 1.0f;
		matrix.m[3][3] = 1.0f;
		return matrix;
	}

	mat4x4 MakeRotationX(float angleRad) {
		mat4x4 matrix;
		matrix.m[0][0] =  1.0f;
		matrix.m[1][1] =  cosf(angleRad);
		matrix.m[1][2] =  sinf(angleRad);
		matrix.m[2][1] = -sinf(angleRad);
		matrix.m[2][2] =  cosf(angleRad);
		matrix.m[3][3] =  1.0f;
		return matrix;
	}

	mat4x4 MakeRotationY(float angleRad) {
		mat4x4 matrix;
		matrix.m[0][0] =  cosf(angleRad);
		matrix.m[0][2] =  sinf(angleRad);
		matrix.m[2][0] = -sinf(angleRad);
		matrix.m[1][1] =  1.0f;
		matrix.m[2][2] =  cosf(angleRad);
		matrix.m[3][3] =  1.0f;
		return matrix;
	}

	mat4x4 MakeRotationZ(float angleRad) {
		mat4x4 matrix;
		matrix.m[0][0] =  cosf(angleRad);
		matrix.m[0][1] =  sinf(angleRad);
		matrix.m[1][0] = -sinf(angleRad);
		matrix.m[1][1] =  cosf(angleRad);
		matrix.m[2][2] =  1.0f;
		matrix.m[3][3] =  1.0f;
		return matrix;
	}

	mat4x4 MakeTranslation(float x, float y, float z) {
		mat4x4 matrix;
		matrix.m[0][0] = 1.0f;
		matrix.m[1][1] = 1.0f;
		matrix.m[2][2] = 1.0f;
		matrix.m[3][3] = 1.0f;
		matrix.m[3][0] = x;
		matrix.m[3][1] = y;
		matrix.m[3][2] = z;
		return matrix;
	}

	mat4x4 MakeProjection(float fovDegrees, float aspectRatio, float near, float far) {
		float fFovRad = 1.0f / tanf(fovDegrees * 0.5f / 180.0f * 3.14159f);
		mat4x4 matrix;
		matrix.m[0][0] = aspectRatio * fFovRad;
		matrix.m[1][1] = fFovRad;
		matrix.m[2][2] = far / (far - near);
		matrix.m[3][2] = (-far * near) / (far - near);
		matrix.m[2][3] = 1.0f;
		matrix.m[3][3] = 0.0f;
		return matrix;
	}

	const vec3d operator*(const mat4x4& matrix, const vec3d& vector) {
		vec3d v;
		v.x = vector.x * matrix.m[0][0] 
			+ vector.y * matrix.m[1][0] 
			+ vector.z * matrix.m[2][0] 
			+ vector.w * matrix.m[3][0];

		v.y = vector.x * matrix.m[0][1] 
			+ vector.y * matrix.m[1][1] 
			+ vector.z * matrix.m[2][1] 
			+ vector.w * matrix.m[3][1];

		v.z = vector.x * matrix.m[0][2] 
			+ vector.y * matrix.m[1][2] 
			+ vector.z * matrix.m[2][2] 
			+ vector.w * matrix.m[3][2];

		v.w = vector.x * matrix.m[0][3] 
			+ vector.y * matrix.m[1][3] 
			+ vector.z * matrix.m[2][3] 
			+ vector.w * matrix.m[3][3];

		return v;
	}

	const mat4x4 operator*(const mat4x4& lhs, const mat4x4& rhs) {
		mat4x4 matrix;
		for (int c = 0; c < 4; c++)
			for (int r = 0; r < 4; r++)
				matrix.m[r][c] = lhs.m[r][0] * rhs.m[0][c] 
							   + lhs.m[r][1] * rhs.m[1][c]
							   + lhs.m[r][2] * rhs.m[2][c] 
							   + lhs.m[r][3] * rhs.m[3][c];
		return matrix;
	}

}
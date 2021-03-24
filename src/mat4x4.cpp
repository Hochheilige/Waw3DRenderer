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

	mat4x4 MakeRotationZ(const float angleRad) {
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

	mat4x4 MakeProjection(const float fovDegrees, const float aspectRatio, const float Fnear, const float Ffar) {
		float fFovRad = 1.0f / tanf(fovDegrees * 0.5f / 180.0f * 3.14159f);
		mat4x4 matrix;
		matrix.m[0][0] = aspectRatio * fFovRad;
		matrix.m[1][1] = fFovRad;
		matrix.m[2][2] = Ffar / (Ffar - Fnear);
		matrix.m[3][2] = (-Ffar * Fnear) / (Ffar - Fnear);
		matrix.m[2][3] = 1.0f;
		matrix.m[3][3] = 0.0f;
		return matrix;
	}

	mat4x4 PointAt(const vec3d& pos, const vec3d& target, const vec3d& up) {
		// Calculate new forward direction
		vec3d newForward = target - pos;
		newForward.Normalise();

		// Calculate new Up direction
		vec3d a = newForward * up.DotProduct(newForward);
		vec3d newUp = up - a;
		newUp.Normalise();

		// New Right direction is easy, its just cross product
		vec3d newRight = newUp.CrossProduct(newForward);

		// Construct Dimensioning and Translation Matrix	
		mat4x4 matrix;
		matrix.m[0][0] = newRight.x;	matrix.m[0][1] = newRight.y;	matrix.m[0][2] = newRight.z;	matrix.m[0][3] = 0.0f;
		matrix.m[1][0] = newUp.x;		matrix.m[1][1] = newUp.y;		matrix.m[1][2] = newUp.z;		matrix.m[1][3] = 0.0f;
		matrix.m[2][0] = newForward.x;	matrix.m[2][1] = newForward.y;	matrix.m[2][2] = newForward.z;	matrix.m[2][3] = 0.0f;
		matrix.m[3][0] = pos.x;			matrix.m[3][1] = pos.y;			matrix.m[3][2] = pos.z;			matrix.m[3][3] = 1.0f;
		return matrix;
	}

	mat4x4 QuickInverse(const mat4x4& m)  { // Only for Rotation/Translation Matrice
		mat4x4 matrix;
		matrix.m[0][0] = m.m[0][0]; matrix.m[0][1] = m.m[1][0]; matrix.m[0][2] = m.m[2][0]; matrix.m[0][3] = 0.0f;
		matrix.m[1][0] = m.m[0][1]; matrix.m[1][1] = m.m[1][1]; matrix.m[1][2] = m.m[2][1]; matrix.m[1][3] = 0.0f;
		matrix.m[2][0] = m.m[0][2]; matrix.m[2][1] = m.m[1][2]; matrix.m[2][2] = m.m[2][2]; matrix.m[2][3] = 0.0f;
		matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] + m.m[3][2] * matrix.m[2][0]);
		matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] + m.m[3][2] * matrix.m[2][1]);
		matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] + m.m[3][2] * matrix.m[2][2]);
		matrix.m[3][3] = 1.0f;
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
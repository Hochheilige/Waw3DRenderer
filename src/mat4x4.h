#pragma once

#include "vec3d.h"

namespace WawM {

	class mat4x4 {
	public:

		friend static mat4x4 MakeIdentity();

		friend static mat4x4 MakeRotationX(const float angleRad);

		friend static mat4x4 MakeRotationY(const float angleRad);

		friend static mat4x4 MakeRotationZ(const float angleRad);

		friend static mat4x4 MakeTranslation(const float x, const float y, const float z);

		friend static mat4x4 MakeProjection(const float fovDegrees, const float aspectRatio, const float near, const float far);

		friend const vec3d operator*(const mat4x4& matrix, const vec3d& vector);

		friend const mat4x4 operator*(const mat4x4& lhs, const mat4x4& rhs);


	private:
		float m[4][4] = { 0 };
	}
}
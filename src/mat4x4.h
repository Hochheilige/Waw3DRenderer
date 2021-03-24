#pragma once

#include "vec3d.h"

namespace WawM {

	struct mat4x4 {

		float m[4][4] = { 0 };
	};

    const vec3d operator*(const mat4x4& matrix, const vec3d& vector);

    const mat4x4 operator*(const mat4x4& lhs, const mat4x4& rhs);
    
    mat4x4 MakeIdentity();
    
    mat4x4 MakeRotationX(const float angleRad);
    
    mat4x4 MakeRotationY(const float angleRad);
    
    mat4x4 MakeRotationZ(const float angleRad);
    
    mat4x4 MakeTranslation(const float x, const float y, const float z);
    
    mat4x4 MakeProjection(const float fovDegrees, const float aspectRatio, const float Fnear, const float Ffar);

    mat4x4 PointAt(const vec3d& pos, const vec3d& target, const vec3d& up);

    mat4x4 QuickInverse(const mat4x4& m);

}
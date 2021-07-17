#pragma once

#include <cmath>

namespace WawMath {

	class vec3d {
	public:
		vec3d();

		vec3d(float x, float y, float z);

		vec3d CrossProduct(const vec3d& vec) const;

		void Normalise();

		float DotProduct(const vec3d& vec) const;

		float Length() const;

	public:
		float x, y, z;
		float w; // for matrix vector mult
	};

	const vec3d operator+(const vec3d& lhs, const vec3d& rhs);

	const vec3d operator-(const vec3d& lhs, const vec3d& rhs);

	const vec3d operator/(const vec3d& vec, const float k);

	const vec3d operator*(const vec3d& vec, const float k);

	vec3d IntersectPlane(const vec3d& plane_p, vec3d& plane_n, const vec3d& lineStart, const vec3d& lineEnd);

}
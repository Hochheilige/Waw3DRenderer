#pragma once

#include <cmath>

namespace WawM {

	class vec3d {
	public:
		vec3d();

		vec3d(float x, float y, float z);

		vec3d CrossProduct(const vec3d& vec);

		void Normalise();

		float DotProduct(const vec3d& vec);

		float Length();

		friend const vec3d operator+(const vec3d& lhs, const vec3d& rhs);

		friend const vec3d operator-(const vec3d& lhs, const vec3d& rhs);

		friend const vec3d operator/(const vec3d& vec, const float k);

		friend const vec3d operator*(const vec3d& vec, const float k);

	public:
		float x, y, z;
		float w; // for matrix vector mult
	};

}
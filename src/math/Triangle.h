#pragma once

#include <WinAPIWrapper.h>
#include "vec3d.h"

namespace WawMath {

	struct Triangle {
		vec3d p[3];
		Waw::Color color;
	};

}
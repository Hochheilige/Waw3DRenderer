#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include "vec3d.h"
#include "MathTriangle.h"

struct Mesh {
	std::vector<MathTriangle> tris;

	bool LoadObjFile(std::string file_name);
};
#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include "math/vec3d.h"
#include "math/Triangle.h"

struct Mesh {
	std::vector<WawMath::Triangle> tris;

	bool LoadObjFile(std::string file_name);
};
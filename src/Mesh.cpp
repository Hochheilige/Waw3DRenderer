#include "Mesh.h"

bool Mesh::LoadObjFile(std::string file_name) {
	using namespace std;
	ifstream file(file_name);
	if (!file) return false;

	vector<WawMath::vec3d> verts;
	string line;
	WawMath::vec3d v;
	int f[3];
	while (file) {
		getline(file, line);
		stringstream ss(line);

		if (line[0] == 'v') {
			ss.ignore(2);
			ss >> v.x >> v.y >> v.z;
			verts.push_back(v);
		}

		if (line[0] == 'f') {
			ss.ignore(2);
			ss >> f[0];
			ss.ignore(1);
			ss >> f[1];
			ss.ignore(1);
			ss >> f[2];
			tris.push_back({ verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] });
		}
	}
	return true;
}

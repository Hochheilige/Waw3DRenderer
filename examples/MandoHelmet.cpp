#include <WinAPIWrapper.h>

#include <vector>
#include <chrono>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <future>
#include <mutex>

//#include "src/WawMath.h"

using namespace Waw;
//using namespace WawM;

class FPS {
public:

	void Init() {
		using namespace std::chrono;
		fps = 0;
		count = 0;
		startTime = steady_clock::now();
		start = duration_cast<milliseconds>(startTime.time_since_epoch());
	}

	void Frame() {
		using namespace std::chrono;
		++count;
		milliseconds elapsed = duration_cast<milliseconds>(steady_clock::now().time_since_epoch());

		if (elapsed >= (start + 1000ms)) {
			fps = count;
			count = 0;

			startTime = steady_clock::now();
			start = duration_cast<seconds>(startTime.time_since_epoch());
		}
	}

	int32_t GetFPS() { return fps; }

private:
	int32_t fps, count;
	std::chrono::steady_clock::time_point startTime;
	std::chrono::milliseconds start;
};

struct vec3d {
	float x, y, z;
};

struct triangle {
	vec3d p[3];
	Color color;
};

struct mesh {
	std::vector<triangle> tris;

	bool LoadObjFile(std::string file_name) {
		std::ifstream file(file_name);
		if (!file) return false;

		std::vector<vec3d> verts;
		std::string line;
		vec3d v;
		int f[3];
		while (file) {
			std::getline(file, line);
			std::stringstream ss(line);

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
};

struct mat4x4 {
	float m[4][4] = { 0 };
};

void MultiplyMatrixVector(vec3d& input, vec3d& output, mat4x4& matrix) {
	output.x = input.x * matrix.m[0][0] + input.y * matrix.m[1][0] + input.z * matrix.m[2][0] + matrix.m[3][0];
	output.y = input.x * matrix.m[0][1] + input.y * matrix.m[1][1] + input.z * matrix.m[2][1] + matrix.m[3][1];
	output.z = input.x * matrix.m[0][2] + input.y * matrix.m[1][2] + input.z * matrix.m[2][2] + matrix.m[3][2];
	float w = input.x * matrix.m[0][3] + input.y * matrix.m[1][3] + input.z * matrix.m[2][3] + matrix.m[3][3];

	if (w != 0.0f) {
		output.x /= w;
		output.y /= w;
		output.z /= w;
	}
}

HDC memoryDC;
HBITMAP bitmap;
Triangle tr;
mesh meshCube;
Color color;
vec3d vCamera;

std::chrono::steady_clock::time_point begin;
std::chrono::steady_clock::time_point end;
float fTheta;

FPS fps;

LRESULT WINAPI WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR pCmdLine, int nCmdShow) {
	begin = std::chrono::steady_clock::now();

	// Load files
	meshCube.LoadObjFile("../../models/MandoHelmetTri.obj");

	// Create window
	Window window(hInstance, WinProc);
	window.CreateWawWindow(hInstance, nCmdShow, 800, 600, "Waw3DRenderer");
	color = Color(0, 255, 177);


	MSG msg;

	fps.Init();
	while (GetMessage(&msg, nullptr, 0, 0)) {
		end = std::chrono::steady_clock::now();
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		InvalidateRect(window.GetWindow(), NULL, TRUE);
	}
}

LRESULT WINAPI WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	RECT rect = { 0 };
	GetWindowRect(hWnd, &rect);
	int32_t width = rect.right - rect.left;
	int32_t height = rect.bottom - rect.top;

	// Projection matrix
	float fNear = 0.1f;
	float fFar = 1000.f;
	float fFov = 90.0f;
	float fAspectRatio = (float)height / (float)width; // Òóò ðàçìåðû ýêðàíà è íóæíî äîáàâèòü â êëàññ Window âîçìîæíîñòü èõ ïîëó÷èòü
	float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.141592f);

	mat4x4 projMat;
	projMat.m[0][0] = fAspectRatio * fFovRad;
	projMat.m[1][1] = fFovRad;
	projMat.m[3][3] = fFar / (fFar - fNear);
	projMat.m[3][2] = (-fFar * fNear) / (fFar - fNear);
	projMat.m[2][3] = 1.0f;
	projMat.m[3][3] = 0.0f;

	// Store triagles for rastering later
	std::vector<triangle> vecTrianglesToRaster;

	switch (message) {
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		memoryDC = CreateCompatibleDC(hdc);
		bitmap = CreateCompatibleBitmap(hdc, width, height);
		SelectObject(memoryDC, bitmap);

		// Rotation matrixes
		mat4x4 matRotZ, matRotX;
		std::chrono::duration<float> elapsed = end - begin;
		fTheta = 1.0f * elapsed.count();

		// Rotation Z
		matRotZ.m[0][0] = cosf(fTheta);
		matRotZ.m[0][1] = sinf(fTheta);
		matRotZ.m[1][0] = -sinf(fTheta);
		matRotZ.m[1][1] = cosf(fTheta);
		matRotZ.m[2][2] = 1;
		matRotZ.m[3][3] = 1;

		// Rotation X
		matRotX.m[0][0] = 1;
		matRotX.m[1][1] = cosf(fTheta * 0.5f);
		matRotX.m[1][2] = sinf(fTheta * 0.5f);
		matRotX.m[2][1] = -sinf(fTheta * 0.5f);
		matRotX.m[2][2] = cosf(fTheta * 0.5f);
		matRotX.m[3][3] = 1;

		// Draw the triangles 

		for (auto tri : meshCube.tris) {
			triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;

			triProjected.color = color;

			for (int i = 0; i < 3; ++i)
				MultiplyMatrixVector(tri.p[i], triRotatedZ.p[i], matRotZ);

			for (int i = 0; i < 3; ++i)
				MultiplyMatrixVector(triRotatedZ.p[i], triRotatedZX.p[i], matRotX);

			triTranslated = triRotatedZX;
			for (int i = 0; i < 3; ++i)
				triTranslated.p[i].z = triRotatedZX.p[i].z + 100.0f;

			// Use Cross-Product to get surface normal
			vec3d normal, line1, line2;
			line1.x = triTranslated.p[1].x - triTranslated.p[0].x;
			line1.y = triTranslated.p[1].y - triTranslated.p[0].y;
			line1.z = triTranslated.p[1].z - triTranslated.p[0].z;

			line2.x = triTranslated.p[2].x - triTranslated.p[0].x;
			line2.y = triTranslated.p[2].y - triTranslated.p[0].y;
			line2.z = triTranslated.p[2].z - triTranslated.p[0].z;

			normal.x = line1.y * line2.z - line1.z * line2.y;
			normal.y = line1.z * line2.x - line1.x * line2.z;
			normal.z = line1.x * line2.y - line1.y * line2.x;

			// It's normally normal to normalise the normal
			float l = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
			normal.x /= l; normal.y /= l; normal.z /= l;


			if (normal.x * (triTranslated.p[0].x - vCamera.x) +
				normal.y * (triTranslated.p[0].y - vCamera.y) +
				normal.z * (triTranslated.p[0].z - vCamera.z) < 0.0f) {
				for (int i = 0; i < 3; ++i) {

					// Illumination
					vec3d light_direction = { 0.0f, 0.0f, -1.0f };
					float l = sqrtf(light_direction.x * light_direction.x + light_direction.y * light_direction.y + light_direction.z * light_direction.z);
					light_direction.x /= l; light_direction.y /= l; light_direction.z /= l;

					// How similar is normal to light direction
					float dp = normal.x * light_direction.x + normal.y * light_direction.y + normal.z * light_direction.z;

					// Choose console colours as required (much easier with RGB)

					triProjected.color.red = triProjected.color.red * dp;
					triProjected.color.green = triProjected.color.green * dp;
					triProjected.color.blue = triProjected.color.blue * dp;

					MultiplyMatrixVector(triTranslated.p[i], triProjected.p[i], projMat);

					// Scale into view
					triProjected.p[i].x += 1.0f;
					triProjected.p[i].y += 1.0f;

					triProjected.p[i].x *= 0.5f * width;
					triProjected.p[i].y *= 0.5f * height;

				}
				vecTrianglesToRaster.push_back(triProjected);
			}
		}

		// Sort triangles from back to front
		std::sort(vecTrianglesToRaster.begin(), vecTrianglesToRaster.end(), [](const triangle& t1, const triangle& t2) {
			float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f; // midpoint of z 
			float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
			return z1 > z2;
		});

		for (auto& triProjected : vecTrianglesToRaster) {


			tr.SetVertexes(
				Point(triProjected.p[0].x, triProjected.p[0].y),
				Point(triProjected.p[1].x, triProjected.p[1].y),
				Point(triProjected.p[2].x, triProjected.p[2].y)
			);

			tr.SetColor(triProjected.color);

			tr.Draw(memoryDC);

			//DrawTriangle(
			//	memoryDC,
			//	Point(triProjected.p[0].x, triProjected.p[0].y),
			//	Point(triProjected.p[1].x, triProjected.p[1].y),
			//	Point(triProjected.p[2].x, triProjected.p[2].y),
			//	StandartColors::WHITE
			//);

		}
		fps.Frame();
		std::string buffer = std::to_string(fps.GetFPS()) + " FPS";
		TextOut(memoryDC, 20, 20, buffer.c_str(), strlen(buffer.c_str()));

		BitBlt(hdc, 0, 0, width, height, memoryDC, 0, 0, SRCCOPY);

		DeleteObject(bitmap);
		DeleteDC(memoryDC);



		EndPaint(hWnd, &ps);
		//Sleep(300);


		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
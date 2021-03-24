#include <WinAPIWrapper.h>

#include <vector>
#include <chrono>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>

#include "src/WawMath.h"

#define VK_A 0x41
#define VK_D 0x44
#define VK_W 0x57
#define VK_S 0x53

using namespace Waw;
using namespace WawM;

HDC memoryDC;
HBITMAP bitmap;
Triangle tr;
Mesh meshCube;
Color color;
vec3d camera;
vec3d lookDirection;
vec3d forward;

std::chrono::steady_clock::time_point begin;
std::chrono::steady_clock::time_point end;
float theta = 0.0f;
float yaw = 0.0f;

FPS fps;

int TriangleClipAgainstPlane(vec3d plane_p, vec3d plane_n, MathTriangle& in_tri, MathTriangle& out_tri1, MathTriangle& out_tri2);
LRESULT WINAPI WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR pCmdLine, int nCmdShow) {
	// Load files
	meshCube.LoadObjFile("models/axis.obj");

	Window window(hInstance, WinProc);
	window.CreateWawWindow(hInstance, nCmdShow, 800, 600, "Waw3DRenderer");
	color = Color(StandartColors::WHITE);

	MSG msg;

	fps.Init();
	begin = std::chrono::steady_clock::now();
	while (GetMessage(&msg, nullptr, 0, 0)) {
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

	std::chrono::duration<float> elapsed;

	// Projection matrix
	float Near = 0.1f;
	float Far = 1000.f;
	float fieldOfView = 90.0f;
	float aspectRatio = (float)height / (float)width;

	mat4x4 projectionMatrix = MakeProjection(fieldOfView, aspectRatio, Near, Far);

	// Store triagles for rastering later
	std::vector<MathTriangle> vecTrianglesToRaster;

	switch (message) {
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);

			memoryDC = CreateCompatibleDC(hdc);
			bitmap = CreateCompatibleBitmap(hdc, width, height);
			SelectObject(memoryDC, bitmap);
			     
			// Rotation matrixes
			mat4x4 matrixRotationZ = MakeRotationZ(theta);
			mat4x4 matrixRotationX = MakeRotationX(theta * 0.5f);

			// Translation matrix
			mat4x4 translationMatrix = MakeTranslation(0.0f, 0.0f, 15.0f);

			mat4x4 worldMatrix = MakeIdentity();
			worldMatrix = matrixRotationZ * matrixRotationX;
			worldMatrix = worldMatrix * translationMatrix;

			vec3d up = { 0, 1, 0 };
			vec3d target = { 0, 0, 1 };

			mat4x4 matrixCameraRotation = MakeRotationY(yaw);
			lookDirection = matrixCameraRotation * target;
			target = camera + lookDirection;

			mat4x4 matrixCamera = PointAt(camera, target, up);

			mat4x4 viewMatrix = QuickInverse(matrixCamera);

			for (const auto& tri : meshCube.tris) {
				MathTriangle triProjected, triTransformed, triViewed;

				triProjected.color = color;

				for (int i = 0; i < 3; ++i)
					triTransformed.p[i] = worldMatrix * tri.p[i];

				// Use Cross-Product to get surface normal
				vec3d normal, line1, line2;

				line1 = triTransformed.p[1] - triTransformed.p[0];
				line2 = triTransformed.p[2] - triTransformed.p[0];

				normal = line1.CrossProduct(line2);

				// It's normally normal to normalise the normal
				normal.Normalise();

				vec3d cameraRay = triTransformed.p[0] - camera;

				if (normal.DotProduct(cameraRay) < 0.0f) {

					// Illumination
					vec3d light_direction = { 0.0f, 1.0f, -1.0f };
					light_direction.Normalise();

					// How similar is normal to light direction
					float dp = max(0.1f, light_direction.DotProduct(normal));

					// Choose colours 
					triProjected.color.red = triProjected.color.red * dp;
					triProjected.color.green = triProjected.color.green * dp;
					triProjected.color.blue = triProjected.color.blue * dp;
					
					for (int i = 0; i < 3; ++i)
						triViewed.p[i] = viewMatrix * triTransformed.p[i];

					int clippedTriangles = 0;
					MathTriangle clipped[2];
					clippedTriangles = TriangleClipAgainstPlane({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, triViewed, clipped[0], clipped[1]);

					for (int n = 0; n < clippedTriangles; ++n) {

						for (int i = 0; i < 3; ++i)
							triProjected.p[i] = projectionMatrix * clipped[n].p[i];
						triProjected.color = clipped[n].color;

						for (int i = 0; i < 3; ++i)
							triProjected.p[i] = triProjected.p[i] / triProjected.p[i].w;

						// offset verts into visible normalixed space
						vec3d offsetView = { 1, 1, 0 };
						for (int i = 0; i < 3; ++i)
							triProjected.p[i] = triProjected.p[i] + offsetView;


						// Scale into view
						for (int i = 0; i < 3; ++i) {
							triProjected.p[i].x *= 0.5f * width;
							triProjected.p[i].y *= 0.5f * height;
						}
						vecTrianglesToRaster.push_back(triProjected);
					}
				}
			}

			// Sort triangles from back to front
			std::sort(vecTrianglesToRaster.begin(), vecTrianglesToRaster.end(), [](const MathTriangle& t1, const MathTriangle& t2) {
				float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f; // midpoint of z 
				float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
				return z1 < z2;
				 });

			for (auto& triProjected : vecTrianglesToRaster) {
				

				tr.SetVertexes(
					Point(triProjected.p[0].x, triProjected.p[0].y),
					Point(triProjected.p[2].x, triProjected.p[2].y),
					Point(triProjected.p[1].x, triProjected.p[1].y)
				);

				tr.SetFillColor(triProjected.color);
				tr.SetContourColor(StandartColors::BLACK);

				tr.Draw(memoryDC);
			
			}
			fps.Frame();
			std::string buffer = std::to_string(fps.GetFPS()) + " FPS";
			TextOut(memoryDC, 20, 20, buffer.c_str(), strlen(buffer.c_str()));

			BitBlt(hdc, 0, 0, width, height, memoryDC, 0, 0, SRCCOPY);

			DeleteObject(bitmap);
			DeleteDC(memoryDC);

			EndPaint(hWnd, &ps);
			break;
		} 
		case WM_KEYDOWN: {
			WPARAM key = wParam;
			forward = lookDirection * 0.5f;
			switch (key) {
				case VK_UP:
				{
					camera.y -= 0.8f;
					break;
				}
				case VK_DOWN:
				{
					camera.y += 0.8f;
					break;
				}
				case VK_LEFT:
				{
					camera.x -= 0.8f;
					break;
				}
				case VK_RIGHT:
				{
					camera.x += 0.8f;
					break;
				}
				case VK_A: 
				{
					yaw += 0.1f;
					break;
				}
				case VK_D:
				{
					yaw -= 0.1f;
					break;
				}
				case VK_W:
				{
					camera = camera + forward;
					break;
				}
				case VK_S:
				{
					camera = camera - forward;
					break;
				}
			}
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

int TriangleClipAgainstPlane(vec3d plane_p, vec3d plane_n, MathTriangle& in_tri, MathTriangle& out_tri1, MathTriangle& out_tri2) {
	plane_n.Normalise();

	auto dist = [&](vec3d& p) {
		p.Normalise();
		vec3d n = p;
		return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - plane_n.DotProduct(plane_p));
	};

	vec3d* inside_points[3];  int nInsidePointCount = 0;
	vec3d* outside_points[3]; int nOutsidePointCount = 0;

	float d0 = dist(in_tri.p[0]);
	float d1 = dist(in_tri.p[1]);
	float d2 = dist(in_tri.p[2]);

	if (d0 >= 0) { inside_points[nInsidePointCount++] = &in_tri.p[0]; }
	else { outside_points[nOutsidePointCount++] = &in_tri.p[0]; }
	if (d1 >= 0) { inside_points[nInsidePointCount++] = &in_tri.p[1]; }
	else { outside_points[nOutsidePointCount++] = &in_tri.p[1]; }
	if (d2 >= 0) { inside_points[nInsidePointCount++] = &in_tri.p[2]; }
	else { outside_points[nOutsidePointCount++] = &in_tri.p[2]; }

	if (nInsidePointCount == 0) 
		return 0; 
	

	if (nInsidePointCount == 3) {
		out_tri1 = in_tri;
		return 1; 
	}

	if (nInsidePointCount == 1 && nOutsidePointCount == 2) {
		out_tri1.color = Color(StandartColors::BLUE); 
		out_tri1.p[0] = *inside_points[0];
		out_tri1.p[1] = IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0]);
		out_tri1.p[2] = IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[1]);
		return 1; 
	}

	if (nInsidePointCount == 2 && nOutsidePointCount == 1) {
		out_tri1.color = Color(StandartColors::GREEN); 
		out_tri2.color = Color(StandartColors::RED); 
		out_tri1.p[0] = *inside_points[0];
		out_tri1.p[1] = *inside_points[1];
		out_tri1.p[2] = IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0]);
		out_tri2.p[0] = *inside_points[1];
		out_tri2.p[1] = out_tri1.p[2];
		out_tri2.p[2] = IntersectPlane(plane_p, plane_n, *inside_points[1], *outside_points[0]);
		return 2; 
	}
}
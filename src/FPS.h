#pragma once

#include <chrono>
#include <ctime>

class FPS {
public:

	void Init();

	void Frame();

	inline int32_t GetFPS() { return fps; }

private:
	int32_t fps, count;
	std::chrono::steady_clock::time_point startTime;
	std::chrono::milliseconds start;
};
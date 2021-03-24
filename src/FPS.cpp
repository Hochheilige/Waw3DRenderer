#include "FPS.h"

void FPS::Init() {
	using namespace std::chrono;
	fps = 0;
	count = 0;
	startTime = steady_clock::now();
	start = duration_cast<milliseconds>(startTime.time_since_epoch());
}

void FPS::Frame() {
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

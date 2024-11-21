#include "pch.h"
#include "TimerManager.h"

void TimerManager::Init()
{
	startTime = Clock::now();
	oldTime = startTime;
	deltaTime = 0;
	syncTime = 0;
}

void TimerManager::Update()
{
	TimePoint now_time = Clock::now();

	MilliSec elapsed = std::chrono::duration_cast<MilliSec>(now_time - oldTime);

	deltaTime = elapsed.count();

	oldTime = now_time;

	syncTime += deltaTime;
}

bool TimerManager::isSyncTime()
{
	Update();

	if (syncTime >= syncStand) {
		syncTime -= syncStand;

		return true;
	}

	return false;
}

std::chrono::milliseconds TimerManager::epochToMillis()
{
	return std::chrono::milliseconds();
}

std::chrono::milliseconds TimerManager::timeGap(std::chrono::milliseconds bullet_fire_t)
{
	return std::chrono::milliseconds();
}

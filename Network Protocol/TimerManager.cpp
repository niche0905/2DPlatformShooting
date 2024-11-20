#include "pch.h"
#include "TimerManager.h"

void TimerManager::Init()
{
	startTime = Clock::now();
	oldTime = startTime;
	deltaTime = 0.0;
	syncTime = 0.0;
}

void TimerManager::Update()
{
	TimePoint now_time = Clock::now();

	std::chrono::duration<double> elapsed = now_time - oldTime;

	deltaTime = elapsed.count();

	oldTime = now_time;

	syncTime += deltaTime;
}

bool TimerManager::isSyncTime()
{
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

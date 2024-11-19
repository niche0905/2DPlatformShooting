#include "pch.h"
#include "TimerManager.h"

void TimerManager::Init()
{
	oldTime = std::chrono::system_clock::now();
	deltaTime = std::chrono::milliseconds::zero();
}

void TimerManager::Update()
{

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

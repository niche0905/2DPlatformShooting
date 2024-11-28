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

	MicroSec elapsed = std::chrono::duration_cast<MicroSec>(now_time - oldTime);

	deltaTime = elapsed.count();

	oldTime = now_time;

	syncTime += deltaTime;
}

bool TimerManager::isSyncTime()
{
	if (syncTime >= syncStand) {
		syncTime -= syncStand;

		return true;
	}

	return false;
}

std::chrono::microseconds TimerManager::epochToMillis()
{
	MicroSec return_value = std::chrono::duration_cast<MicroSec>(oldTime.time_since_epoch());

	return return_value;
}

std::chrono::microseconds TimerManager::timeGap(std::chrono::microseconds bullet_fire_t)
{
	MicroSec this_epoch_delta = epochToMillis();
	MicroSec result = this_epoch_delta - bullet_fire_t;

	return result;
}

const int64_t TimerManager::getDeltaTime() const
{
	return deltaTime;
}

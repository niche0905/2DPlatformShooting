#pragma once
#include <chrono>


class TimerManager
{
private:
	using Clock = std::chrono::high_resolution_clock;
	using Epoch = std::chrono::system_clock;
	using TimePoint = std::chrono::time_point<Clock>;
	using EpochPoint = std::chrono::time_point<Epoch>;
	using MicroSec = std::chrono::microseconds;

private:
	//static constexpr int64_t syncStand = (1000000 / 30);		// 1000000 <= (int)myNP::microToSecond
	static constexpr int64_t syncStand = (1000000 / 2);		// 1/2
	//static constexpr int64_t syncStand = 1000000;

private:
	TimePoint startTime;	// 타이머 시작 시간 (동기화 시간을 구하기 위한)
	TimePoint oldTime;		// 이전 업데이트 프레임 시점
	int64_t deltaTime;		// deltatTime
	int64_t syncTime;		// deltatTime

public:
	void Init();		// 초기화
	void Update();		// Update
	bool isSyncTime();	// 동기화 시간인지 알려줌 (지금이 타이밍인지)
	std::chrono::microseconds epochToMillis();		// epoch 시간으로 부터의 차이 (우리만의 epoch)
	std::chrono::microseconds timeGap(std::chrono::microseconds bullet_fire_t);		// 총알이 발사 되었을 때와 시차 (ping 차이를 보간하기 위한)

	const int64_t getDeltaTime() const;

};


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
	TimePoint startTime;	// Ÿ�̸� ���� �ð� (����ȭ �ð��� ���ϱ� ����)
	TimePoint oldTime;		// ���� ������Ʈ ������ ����
	int64_t deltaTime;		// deltatTime
	int64_t syncTime;		// deltatTime

public:
	void Init();		// �ʱ�ȭ
	void Update();		// Update
	bool isSyncTime();	// ����ȭ �ð����� �˷��� (������ Ÿ�̹�����)
	std::chrono::microseconds epochToMillis();		// epoch �ð����� ������ ���� (�츮���� epoch)
	std::chrono::microseconds timeGap(std::chrono::microseconds bullet_fire_t);		// �Ѿ��� �߻� �Ǿ��� ���� ���� (ping ���̸� �����ϱ� ����)

	const int64_t getDeltaTime() const;

};


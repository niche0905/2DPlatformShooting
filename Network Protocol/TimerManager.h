#pragma once
#include <chrono>


class TimerManager
{
private:
	static constexpr int64_t syncStand = (1000 / 30);

private:
	using Clock = std::chrono::high_resolution_clock;
	using TimePoint = std::chrono::time_point<Clock>;
	using MilliSec = std::chrono::milliseconds;

	TimePoint startTime;	// Ÿ�̸� ���� �ð� (����ȭ �ð��� ���ϱ� ����)
	TimePoint oldTime;		// ���� ������Ʈ ������ ����
	int64_t deltaTime;		// deltatTime
	int64_t syncTime;		// deltatTime

public:
	void Init();		// �ʱ�ȭ
	void Update();		// Update
	bool isSyncTime();	// ����ȭ �ð����� �˷��� (������ Ÿ�̹�����)
	std::chrono::milliseconds epochToMillis();		// epoch �ð����� ������ ���� (�츮���� epoch)
	std::chrono::milliseconds timeGap(std::chrono::milliseconds bullet_fire_t);		// �Ѿ��� �߻� �Ǿ��� ���� ���� (ping ���̸� �����ϱ� ����)

};


#pragma once
#include <chrono>


class TimerManager
{
private:
	static constexpr double syncStand = (1.0 / 30);

private:
	using Clock = std::chrono::high_resolution_clock;
	using TimePoint = std::chrono::time_point<Clock>;

	TimePoint startTime;	// Ÿ�̸� ���� �ð� (����ȭ �ð��� ���ϱ� ����)
	TimePoint oldTime;		// ���� ������Ʈ ������ ����
	double deltaTime;		// deltatTime
	double syncTime;		// deltatTime

public:
	void Init();		// �ʱ�ȭ
	void Update();		// Update
	bool isSyncTime();	// ����ȭ �ð����� �˷��� (������ Ÿ�̹�����)
	std::chrono::milliseconds epochToMillis();		// epoch �ð����� ������ ���� (�츮���� epoch)
	std::chrono::milliseconds timeGap(std::chrono::milliseconds bullet_fire_t);		// �Ѿ��� �߻� �Ǿ��� ���� ���� (ping ���̸� �����ϱ� ����)

};


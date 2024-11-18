#pragma once
#include <chrono>


class TimerManager
{
private:
	std::chrono::system_clock::time_point oldTime;		// ���� ������Ʈ ������ ����
	std::chrono::milliseconds deltaTime;				// deltatTime

public:
	void Init();		// �ʱ�ȭ
	void Update();		// Update
	bool isSyncTime();	// ����ȭ �ð����� �˷��� (������ Ÿ�̹�����)
	std::chrono::milliseconds epochToMillis();		// epoch �ð����� ������ ���� (�츮���� epoch)
	std::chrono::milliseconds timeGap(std::chrono::milliseconds bullet_fire_t);		// �Ѿ��� �߻� �Ǿ��� ���� ���� (ping ���̸� �����ϱ� ����)

};


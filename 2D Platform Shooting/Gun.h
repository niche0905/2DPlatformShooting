#pragma once

// cham. Class Gun�� Structȭ. 
struct Gun
{
	std::string name{};	// ���� �̸�
	float damage{};		// ���� ������ (�°� ���ư��� �� ���ϱ� ����)
	float speed{};		// �Ѿ��� �ӵ� (�󸶳� ���� ���ư�����)
	int mag{};			// ź�� �󸶳� ���Ҵ���
	int RPM{};			// �д� �߻� �ӵ�
	int ratio{};		// ���� ���� Ȯ��(Ŭ���� ����) 0�̸� �ƿ� �ȳ���
	int id{};

	Gun() = default;

	Gun(std::string name, float damage, float speed, int curMag, int RPM, int proMag)
		: name(name), damage(damage), speed(speed), mag(curMag), RPM(RPM), ratio(proMag) {}

	friend std::ostream& operator<<(std::ostream& os, const Gun& gun);
	friend std::istream& operator>>(std::istream& os, Gun& gun);
};

// cham TODO: guns�� �����ϴ� �Ŵ����� �ʿ��ҵ�.
extern std::unordered_map<uint8_t, Gun> g_guns;

const int getRandomGunId();
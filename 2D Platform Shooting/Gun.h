#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <random>
#include <ranges>

class Gun
{
private:

	// [cham] 9.22
	// �̸��� �ٲ�.
	std::string name;	// ���� �̸�
	float damage;		// ���� ������ (�°� ���ư��� �� ���ϱ� ����)
	float speed;		// �Ѿ��� �ӵ� (�󸶳� ���� ���ư�����)
	int mag;			// ź�� �󸶳� ���Ҵ���
	int RPM;			// �д� �߻� �ӵ�
	int ratio;			// ���� ���� Ȯ��(Ŭ���� ����) 0�̸� �ƿ� �ȳ���

public:
	Gun()
	{

	}
	Gun(std::string name) : name(name)
	{

	}
	Gun(std::string name, float damage, float speed, int curMag, int RPM, int proMag)
		: name(name), damage(damage), speed(speed), mag(curMag), RPM(RPM), ratio(proMag)
	{

	}

	friend std::ostream& operator<<(std::ostream& os, const Gun& gun);
	friend std::istream& operator>>(std::istream& os, Gun& gun);

	std::string	getName() const { return name; }
	float		getDamage() const { return damage; }
	float		getSpeed() const { return speed; }
	float		getMagazine() const { return mag; }
	float		getRPM() const { return RPM; }
	int			getRatio() const { return ratio; }
};


// [cham] 9.22: 
extern std::unordered_map<uint8_t, Gun> g_guns;

const int getRandomGun();
#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

class Gun
{
private:
	std::string name;	// ���� �̸�
	float damage;		// ���� ������ (�°� ���ư��� �� ���ϱ� ����)
	float speed;		// �Ѿ��� �ӵ� (�󸶳� ���� ���ư�����)
	int curMag;			// ź�� �󸶳� ���Ҵ���
	int RPM;			// �д� �߻� �ӵ�
	int proMag;			// ���� ���� Ȯ��(Ŭ���� ����) 0�̸� �ƿ� �ȳ���

public:
	Gun()
	{

	}
	Gun(std::string name) : name(name)
	{

	}
	Gun(std::string name, float damage, float speed, int curMag, int RPM, int proMag) : name(name), damage(damage), speed(speed), curMag(curMag), RPM(RPM), proMag(proMag)
	{

	}

	friend std::ostream& operator<<(std::ostream& os, const Gun& gun);
	friend std::istream& operator>>(std::istream& os, Gun& gun);
	

	/*void getPistol() { name = "Pistol"; }
	void getAK47() { name = "AK-47"; }*/

	std::string	getName() { return name; }
	float		getSpeed() { return speed; }
};


// [cham] 9.22: 
extern std::unordered_map<uint8_t, Gun> g_guns;
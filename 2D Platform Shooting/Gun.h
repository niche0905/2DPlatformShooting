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
	// 이름좀 바꿈.
	std::string name;	// 총의 이름
	float damage;		// 총의 데미지 (맞고 날아가는 힘 정하기 위한)
	float speed;		// 총알의 속도 (얼마나 빨리 날아가는지)
	int mag;			// 탄이 얼마나 남았는지
	int RPM;			// 분당 발사 속도
	int ratio;			// 총이 나올 확률(클수록 높다) 0이면 아예 안나옴

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
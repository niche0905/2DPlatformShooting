#pragma once

// cham. Class Gun의 Struct화. 
struct Gun
{
	std::string name{};	// 총의 이름
	float damage{};		// 총의 데미지 (맞고 날아가는 힘 정하기 위한)
	float speed{};		// 총알의 속도 (얼마나 빨리 날아가는지)
	int mag{};			// 탄이 얼마나 남았는지
	int RPM{};			// 분당 발사 속도
	int ratio{};		// 총이 나올 확률(클수록 높다) 0이면 아예 안나옴
	int id{};

	Gun() = default;

	Gun(std::string name, float damage, float speed, int curMag, int RPM, int proMag)
		: name(name), damage(damage), speed(speed), mag(curMag), RPM(RPM), ratio(proMag) {}

	friend std::ostream& operator<<(std::ostream& os, const Gun& gun);
	friend std::istream& operator>>(std::istream& os, Gun& gun);
};

// cham TODO: guns를 관리하는 매니저가 필요할듯.
extern std::unordered_map<uint8_t, Gun> g_guns;

const int getRandomGunId();
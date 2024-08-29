#pragma once
#include <string>
#include <fstream>


class Gun
{
private:
	std::string name;	// 총의 이름
	float damage;		// 총의 데미지 (맞고 날아가는 힘 정하기 위한)
	float speed;		// 총알의 속도 (얼마나 빨리 날아가는지)
	int curMag;			// 탄이 얼마나 남았는지
	int RPM;			// 분당 발사 속도
	int proMag;			// 총이 나올 확률(클수록 높다) 0이면 아예 안나옴

public:
	Gun(float damage, float speed, int curMag, int RPM) : damage(damage), speed(speed), curMag(curMag), RPM(RPM)
	{

	}

	friend std::ostream& operator<<(std::ostream& os, const Gun& gun);
	friend std::istream& operator>>(std::istream& os, Gun& gun);

};


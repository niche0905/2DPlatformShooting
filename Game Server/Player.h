#pragma once
#include "Object.h"

constexpr int BaseLife = 3;
constexpr uint8_t BaseGunID = 0;


class Player : public Object
{
private:
	// TODO : player_id 저장 해야 함

	bool directon;
	int life;
	uint8_t gunId;
	int curMag;

public:
	Player() = default;

	Player(bool dir);
	Player(float pos_x, float pos_y, bool dir);

	void GunInit();
	void GunUpdate(uint8_t gun_id);

	Position GunFire();

};


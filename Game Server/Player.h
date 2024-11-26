#pragma once
#include "Object.h"

constexpr int BaseLife = 3;
constexpr uint8_t BaseGunID = 0;


class Player : public Object
{
private:
	uint32_t player_id;
	bool directon;
	int life;
	uint8_t gunId;
	int curMag;

public:
	Player() = default;

	Player(uint32_t p_id, bool dir);
	Player(uint32_t p_id, float pos_x, float pos_y, bool dir);

	void GunInit();
	void GunUpdate(uint8_t gun_id);

	Position GunFire();

};


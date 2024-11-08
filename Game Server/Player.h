#pragma once
#include "Object.h"

constexpr int BaseLife = 3;
constexpr uint8_t BaseGunID = 0;


class Player : Object
{
private:
	bool directon;
	int life;
	uint8_t gunId;
	int curMag;

public:
	Player() = default;

	Player(bool dir);

};


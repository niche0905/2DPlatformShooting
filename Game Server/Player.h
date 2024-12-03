#pragma once
#include "Object.h"
#include "Bullet.h"

constexpr int BaseLife = 3;
constexpr uint8_t BaseGunID = 0;

class Bullet;


class Player : public Object
{
private:
	uint32_t player_id;
	bool directon;
	int life;
	uint8_t gunId;
	std::list<Bullet> bullets;
	int curMag;

public:
	Player() = default;

	Player(uint32_t p_id, bool dir);
	Player(uint32_t p_id, float pos_x, float pos_y, bool dir);

	void Update(int64_t delta_time);

	uint32_t GetPlayerID() const;
	void SetPlayerID(uint32_t p_id);
	
	std::list<Bullet>& GetBullets();
	const std::list<Bullet>& GetBullets() const;

	bool getDirection() const { return directon; }
	void SetDirection(const bool value) { directon = value; }

	void GunInit();
	void GunUpdate(uint8_t gun_id);

	Position GunFire();


};


#pragma once
#include "MovingObject.h"


class Bullet : public MovingObject
{
private:
	uint32_t b_id;

	uint8_t b_type;
	bool direction;

public:
	Bullet() = default;

	Bullet(uint32_t bullet_id, uint8_t bullet_type, bool dir);
	Bullet(float x, float y, uint32_t bullet_id, uint8_t bullet_type, bool dir);

	void Update(int64_t delta_time);

	uint32_t GetBulletID() const;
	uint8_t GetBulletType() const;
	bool GetDirection() const;

};


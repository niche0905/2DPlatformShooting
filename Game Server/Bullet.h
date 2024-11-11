#pragma once
#include "Object.h"




class Bullet : Object
{
private:
	uint8_t b_type;
	bool direction;

public:
	Bullet() = default;

	Bullet(uint8_t bullet_type, bool dir);
	Bullet(float x, float y, uint8_t bullet_type, bool dir);

	void Update();

};


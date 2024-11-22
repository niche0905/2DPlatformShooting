#pragma once
#include "MovingObject.h"


class Item : public MovingObject
{
private:
	float dropSpeed; // <- ³Ê ¾ø¾îµµ µÊ ¤·¤·
	bool OnAir;

public:
	Item() = default;

	Item(float x, float y);

	void Update(int64_t delta_time);

};


#pragma once
#include "MovingObject.h"


class Item : public MovingObject
{
private:
	float dropSpeed;
	bool OnAir;

public:
	Item() = default;

	Item(float x, float y);

	void Update();

};


#pragma once
#include "Object.h"


class Item : public Object
{
private:
	float dropSpeed;
	bool OnAir;

public:
	Item() = default;

	Item(float x, float y);

	void Update();

};


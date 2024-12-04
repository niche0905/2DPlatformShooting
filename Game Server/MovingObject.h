#pragma once
#include "Object.h"


class MovingObject : public Object
{
protected:
	float velX, velY;

public:
	MovingObject();
	MovingObject(float pos_x, float pos_y, float width, float height, float pivot_x, float pivot_y);

	void UpdateVelocity(int64_t delta_time);

	Velocity GetVelocity() const;

	void SetVelocity(Velocity velocity);
	void SetVelocity(float vel_x, float vel_y);

};


#pragma once
#include "Object.h"


class MovingObject : public Object
{
private:
	float velX, velY;

public:
	MovingObject();
	MovingObject(float pos_x, float pos_y, float width, float height, float pivot_x, float pivot_y);

	void Update();

	Velocity getVelocity() const;

	void setVelocity(Velocity velocity);
	void setVelocity(float vel_x, float vel_y);

};


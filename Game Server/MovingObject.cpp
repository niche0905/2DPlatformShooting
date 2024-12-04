#include "pch.h"
#include "MovingObject.h"


MovingObject::MovingObject()
	: Object()
	, velX(0.0f)
	, velY(0.0f)
{
}

MovingObject::MovingObject(float pos_x, float pos_y, float width, float height, float pivot_x, float pivot_y)
	: Object(pos_x, pos_y, width, height, pivot_x, pivot_y)
	, velX(0.0f)
	, velY(0.0f)
{
}

void MovingObject::Update(int64_t delta_time)
{
	// TODO : deltaTime 만큼 velocity 만큼 이동하여야 함
	double time_elapsed = delta_time / myNP::microToSecond;

	posX += velX * time_elapsed;
	posY += velY * time_elapsed;
}

Velocity MovingObject::GetVelocity() const
{
	return Velocity(velX, velY);
}

void MovingObject::SetVelocity(Velocity velocity)
{
	velX = velocity.velX;
	velY = velocity.velY;
}

void MovingObject::SetVelocity(float vel_x, float vel_y)
{
	velX = vel_x;
	velY = vel_y;
}

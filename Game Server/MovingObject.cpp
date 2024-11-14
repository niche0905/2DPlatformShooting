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

void MovingObject::Update()
{
	// TODO : deltaTime ��ŭ velocity ��ŭ �̵��Ͽ��� ��
}

Velocity MovingObject::getVelocity() const
{
	return Velocity(velX, velY);
}

void MovingObject::setVelocity(Velocity velocity)
{
	velX = velocity.velX;
	velY = velocity.velY;
}

void MovingObject::setVelocity(float vel_x, float vel_y)
{
	velX = vel_x;
	velY = vel_y;
}
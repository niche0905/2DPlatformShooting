#include "pch.h"
#include "Object.h"


Object::Object()
{

}

Object::Object(float pos_x, float pos_y, float width, float height, float pivot_x, float pivot_y)
{

}

void Object::Update()
{

}

void Object::SetPos(float pos_x, float pos_y)
{

}

void Object::SetPivot(float pivot_x, float pivot_y)
{

}

Position Object::GetPos()
{
	return Position();
}

BoundingBox Object::GetBB()
{
	return BoundingBox();
}

bool Object::Collision(const Object& other)
{
	return false;
}

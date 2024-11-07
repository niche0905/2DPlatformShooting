#include "pch.h"
#include "Object.h"
#include <algorithm>


Object::Object()
{

}

Object::Object(float pos_x, float pos_y, float width, float height, float pivot_x, float pivot_y)
	: posX(pos_x)
	, posY(pos_y)
	, width(width)
	, height(height)
	, pivotX(pivot_x)
	, pivotY(pivot_y)
{

}

void Object::Update()
{
	// 오버라이드 될 것
	// 기본적으로 할 것은 없다
	// Object 하위 클래스로 Moving Object를 만들어야 사용 할 것 같음
}

void Object::SetPos(float pos_x, float pos_y)
{
	posX = pos_x;
	posY = pos_y;
}

void Object::SetPivot(float pivot_x, float pivot_y)
{
	float valid_x = std::clamp(pivot_x, 0.0f, 1.0f);
	float valid_y = std::clamp(pivot_y, 0.0f, 1.0f);

	pivotX = valid_x;
	pivotY = valid_y;
}

Position Object::GetPos() const
{
	Position pos{ posX, posY };
	return pos;
}

BoundingBox Object::GetBB() const
{
	BoundingBox bb;
	bb.left = posX - pivotX * width;
	bb.right = posX + (1.0f - pivotX) * width;
	bb.top = posY + pivotY * height;
	bb.bottom = posY + (1.0f - pivotY) * height;
	return bb;
}

bool Object::Collision(const Object& other)
{
	return false;
}

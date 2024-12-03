#include "pch.h"
#include "Object.h"
#include <algorithm>


Object::Object()
	: posX(0.0f)
	, posY(0.0f)
	, width(0.0f)
	, height(0.0f)
	, pivotX(0.0f)
	, pivotY(0.0f)
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

void Object::SetElement(float pos_x, float pos_y, float width, float height, float pivot_x, float pivot_y)
{
	posX = pos_x;
	posY = pos_y;
	width = width;
	height = height;
	pivotX = pivot_x;
	pivotY = pivot_y;
}

void Object::Update(int64_t delta_time)
{
	// 오버라이드 될 것
	// 기본적으로 할 것은 없다
	// Object 하위 클래스로 Moving Object를 만들어야 사용 할 것 같음
}

void Object::SetPos(float pos_x, float pos_y)
{
	posX = pos_x;
	posY = pos_y;
	//cout << posX << ", " << posY << "\n";
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
	// TODO: position 을 피봇값으로 바꾸어야 한다
	return pos;
}

BoundingBox Object::GetBB() const
{
	BoundingBox bb;
	bb.left = posX - pivotX * width;
	bb.right = posX + (1.0f - pivotX) * width;
	bb.top = posY - pivotY * height;
	bb.bottom = posY + (1.0f - pivotY) * height;
	return bb;
}

bool Object::Collision(const Object& other)
{
	BoundingBox a_bb = GetBB();
	BoundingBox b_bb = other.GetBB();

	// Logging
	cout << "A_BB : " << a_bb.left << " " << a_bb.right << "" << a_bb.top << " " << a_bb.bottom << "\n";
	cout << "B_BB : " << b_bb.left << " " << b_bb.right << "" << b_bb.top << " " << b_bb.bottom << "\n";

	return a_bb.left < b_bb.right && a_bb.right > b_bb.left
		&& a_bb.top < b_bb.bottom && a_bb.bottom > b_bb.top;
}

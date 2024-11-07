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
	// �������̵� �� ��
	// �⺻������ �� ���� ����
	// Object ���� Ŭ������ Moving Object�� ������ ��� �� �� ����
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
	return Position();
}

BoundingBox Object::GetBB() const
{
	return BoundingBox();
}

bool Object::Collision(const Object& other)
{
	return false;
}

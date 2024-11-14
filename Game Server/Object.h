#pragma once

struct Position
{
	float posX, posY;
};

struct Velocity
{
	float velX, velY;
};

struct BoundingBox
{
	float left, right, top, bottom;
};

class Object
{
private:
	float posX, posY;		// 오브젝트 위치값
	float width, height;	// 오브젝트 넓이, 높이
	float pivotX, pivotY;	// 오브젝트 피봇값 0.0 ~ 1.0

public:
	Object();
	Object(float pos_x, float pos_y, float width, float height, float pivot_x, float pivot_y);

	void Update();
	void SetPos(float pos_x, float pos_y);
	void SetPivot(float pivot_x, float pivot_y);

	Position GetPos() const;
	BoundingBox GetBB() const;

	bool Collision(const Object& other);

};

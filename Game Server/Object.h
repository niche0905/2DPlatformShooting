#pragma once

struct Position
{
	float posX, posY;
};

struct BoundingBox
{
	float left, right, top, bottom;
};

class Object
{
private:
	float posX, posY;		// ������Ʈ ��ġ��
	float width, height;	// ������Ʈ ����, ����
	float pivotX, pivotY;	// ������Ʈ �Ǻ���

public:
	Object();
	Object(float pos_x, float pos_y, float width, float height, float pivot_x, float pivot_y);

	void Update();
	void SetPos(float pos_x, float pos_y);
	void SetPivot(float pivot_x, float pivot_y);

	Position GetPos();
	BoundingBox GetBB();

	bool Collision(const Object& other);

};

#include "pch.h"
#include "Item.h"


uint32_t Item::nextID = 0;

Item::Item(float x, float y, Level* level)
	: MovingObject{ x, y, myNP::ItemWidth, myNP::ItemHeight, 0.5f, 1.0f }
	, level{ level }
	, itemID{ ++nextID }
	, OnAir{ true }
{
}

void Item::Update(int64_t delta_time)
{
	if (OnAir) {
		velY += GravityAcc * GravityMul * (delta_time / myNP::microToSecond);
	}

	// �ӵ���ŭ ������
	MovingObject::UpdateVelocity(delta_time);

	// �ϳ��� ����ִ� �÷����� �ִ��� üũ
	bool blocking = level->Collsion(*this);
	if (blocking) {
		OnAir = false;
		velY = 0.0f;
	}
	else {
		OnAir = true;
	}
}

uint32_t Item::GetItemID() const
{
	return itemID;
}

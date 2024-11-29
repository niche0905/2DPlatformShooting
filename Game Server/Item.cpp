#include "pch.h"
#include "Item.h"


uint32_t Item::nextID = 0;

Item::Item(float x, float y)
	: MovingObject{ x, y, myNP::ItemWidth, myNP::ItemHeight, 0.5f, 1.0f }
	, itemID{ ++nextID }
	, OnAir{ true }
{
	//SetVelocity(0.0f, 0.0f); �߷°����� �ӵ� ���� ����
}

void Item::Update(int64_t delta_time)
{
	// TODO : �÷����� ������ �÷����� �浹ó��
	// �ٴڿ� ������ �� �ְ� ����
}

uint32_t Item::GetItemID() const
{
	return itemID;
}

#include "pch.h"
#include "Item.h"


Item::Item(float x, float y)
	: MovingObject(x, y, myNP::ItemWidth, myNP::ItemHeight, 0.5f, 1.0f)
	, OnAir(true)
{
	//SetVelocity(0.0f, 0.0f); �߷°����� �ӵ� ���� ����
}

void Item::Update()
{
	// TODO : �÷����� ������ �÷����� �浹ó��
	// �ٴڿ� ������ �� �ְ� ����
}

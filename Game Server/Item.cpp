#include "pch.h"
#include "Item.h"


uint32_t Item::nextID = 0;

Item::Item(float x, float y)
	: MovingObject{ x, y, myNP::ItemWidth, myNP::ItemHeight, 0.5f, 1.0f }
	, itemID{ ++nextID }
	, OnAir{ true }
{
	//SetVelocity(0.0f, 0.0f); 중력값으로 속도 수정 ㅇㅇ
}

void Item::Update(int64_t delta_time)
{
	// TODO : 플랫폼이 나오면 플랫폼과 충돌처리
	// 바닥에 안착할 수 있게 설정
}

uint32_t Item::GetItemID() const
{
	return itemID;
}

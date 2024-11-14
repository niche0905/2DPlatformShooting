#include "pch.h"
#include "Item.h"


Item::Item(float x, float y)
	: MovingObject(x, y, myNP::ItemWidth, myNP::ItemHeight, 0.5f, 1.0f)
	, OnAir(true)
{
	//SetVelocity(0.0f, 0.0f); 중력값으로 속도 수정 ㅇㅇ
}

void Item::Update()
{
	// TODO : 플랫폼이 나오면 플랫폼과 충돌처리
	// 바닥에 안착할 수 있게 설정
}

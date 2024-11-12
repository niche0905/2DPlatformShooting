#include "pch.h"
#include "Item.h"


Item::Item(float x, float y)
	: Object(x, y, myNP::ItemWidth, myNP::ItemHeight, 0.5f, 1.0f)
{

}

void Item::Update()
{
	// TODO : 플랫폼이 나오면 플랫폼과 충돌처리
	// 바닥에 안착할 수 있게 설정
}

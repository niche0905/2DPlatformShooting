#pragma once
#include "pch.h"
#include <vector>
#include "CorePch.h"


class Object
{
	int temp;
};

class World
{
private:
	// vector가 적합한가? list로의 변환 가능성
	std::vector<Object> objects;	// 월드에 있는 객체들 (모든 것 플레이어, 총알, 아이템 등등)

	// 여기에 따로 충돌 처리를 위해 다른 자료구조들이 필요해 보인다?
	// Player 1, 2
	// Bullets
	// Items

public:
	World();

	void Update();	// 인자로 deltaTime을 넘겨야 할 것으로 보인다
	void Process();	// 패킷을 처리하는 부분이 될 것 같다
	void Recv();	// recv 스레드에서 받은 패킷들을 전달 받는 부분이 될 것으로 보인다

};


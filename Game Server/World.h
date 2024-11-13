#pragma once
#include "pch.h"
#include <vector>
#include "CorePch.h"
#include "Object.h"
#include "Player.h"
#include "Bullet.h"
#include "Item.h"
#include "list"


class Object;

class World
{
private:
	// vector가 적합한가? list로의 변환 가능성
	std::vector<Object> objects;	// 월드에 있는 객체들 (모든 것 플레이어, 총알, 아이템 등등)

	// 여기에 따로 충돌 처리를 위해 다른 자료구조들이 필요해 보인다?
	// Player 1, 2
	// Bullets
	// Items
	// 송승호 코멘트 : 이러면 어차피 Bullec 삽입삭제 Item 삽입삭제 하려면 검색이 필요한데
	//				 객체별로 나누어진 컨테이너가 필요한거 아닌가?
	//				 objects 의 존재 이유를 모르겠다

	Player p1;
	Player p2;

	std::list<Bullet> b1;	// p1이 쏜 총알
	std::list<Bullet> b2;	// p2이 쏜 총알

	std::list<Item> items;	// item들 정보를 담고 있는 자료구조

public:
	World();

	void Update();	// 인자로 deltaTime을 넘겨야 할 것으로 보인다
	void Process();	// 패킷을 처리하는 부분이 될 것 같다
	void Recv();	// recv 스레드에서 받은 패킷들을 전달 받는 부분이 될 것으로 보인다

};


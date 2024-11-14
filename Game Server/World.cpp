#include "pch.h"
#include "World.h"


World::World()
{
	objects.clear();
	//objects.reserve(n);
}

void World::Update()
{
	// p1 p2는 위치값을 MOVE 패킷으로 받을 예정

	for (Bullet& b : b1)
		b.Update();
	for (Bullet& b : b2)
		b.Update();
	for (Item& item : items)
		item.Update();

	// 충돌처리 구현
	CollisionCheck();
}

void World::CollisionCheck()
{
	// TODO : 충돌 처리 후 데미지 패킷 전송 부분을 구현해야 한다
	for (Bullet& b : b2) {
		if (p1.Collision(b)) {
			// 데미지 패킷 전송
		}
	}
	for (Bullet& b : b1) {
		if (p2.Collision(b)) {
			// 데미지 패킷 전송
		}
	}

	// TODO : 아이템을 먹고 랜덤으로 총기를 주어주는 로직을 가져오고
	//		  Gun Update 전송 패킷을 구현하고 전송해야 함

	// 아이템 충돌처리의 순서 편파 판정 아님?
	for (Item& item : items) {
		bool p1_collision = p1.Collision(item);
		bool p2_collision = p2.Collision(item);

		if (p1_collision and p2_collision) {
			// 이거 어떻게 함?
		}
		else if (p1_collision) {
			// 총 업데이트 처리 및 패킷 보내기
		}
		else if (p2_collision) {
			// 총 업데이트 처리 및 패킷 보내기
		}
		// 방어적인 코딩
	}
}

void World::Process()
{

}

void World::Recv()
{

}

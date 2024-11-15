#include "pch.h"
#include "World.h"


World::World()
{
	//objects.clear();
	//objects.reserve(n);
}

void World::Update()
{
	// TODO : 플레이어 위치값 받은거 적용하기 -> Process Queue 에서 해야할 것 같은 내용
	
	// TODO : 총알들 업데이트 후 총알이 맵 밖으로 나가면 삭제 처리
	for (auto it = b1.begin(); it != b1.end(); ) {
		//it->update(deltaTime);
		//if (it->isOutBounds(level->leftBound - 1000.0f, level->rightBound + 1000.0f))
		//	it = bullets.erase(it);
		//else
		//	++it;
	}

	for (auto it = b2.begin(); it != b2.end(); ) {
		//it->update(deltaTime);
		//if (it->isOutBounds(level->leftBound - 1000.0f, level->rightBound + 1000.0f))
		//	it = bullets.erase(it);
		//else
		//	++it;
	}

	for (Item& item : items)
		item.Update();

	// 충돌처리 구현
	CollisionCheck();
}

void World::CollisionCheck()
{
	// TODO : 충돌 처리 후 데미지 패킷 전송 부분을 구현해야 한다
	for (auto it = b2.begin(); it != b2.end(); ) {
		if (p1.Collision(*it)) {
			// 데미지 패킷 전송
			it = b2.erase(it);
		}
		else
			++it;
	}
	for (auto it = b2.begin(); it != b2.end(); ) {
		if (p1.Collision(*it)) {
			// 데미지 패킷 전송
			it = b2.erase(it);
		}
		else
			++it;
	}

	// TODO : 아이템을 먹고 랜덤으로 총기를 주어주는 로직을 가져오고
	//		  Gun Update 전송 패킷을 구현하고 전송해야 함

	for (auto it = items.begin(); it != items.end(); ) {
		bool p1_collision = p1.Collision(*it);
		bool p2_collision = p2.Collision(*it);

		if (p1_collision and p2_collision) {
			p1.GunUpdate(GunInfo.getRandomGunId());
			p2.GunUpdate(GunInfo.getRandomGunId());

			// TODO : Gun Update 패킷 보내기 둘다 두번씩

			it = items.erase(it);
		}
		else if (p1_collision) {
			p1.GunUpdate(GunInfo.getRandomGunId());

			// TODO : Gun Update 패킷 보내기 둘다 p1의 업데이트에 대해

			it = items.erase(it);
		}
		else if (p2_collision) {
			p2.GunUpdate(GunInfo.getRandomGunId());

			// TODO : Gun Update 패킷 보내기 둘다 p2의 업데이트에 대해

			it = items.erase(it);
		}
		else {
			++it;
		}
	}
}

void World::Process()
{

}

void World::Recv()
{

}

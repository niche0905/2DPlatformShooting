#include "pch.h"
#include "World.h"
#include "Platform.h"


World::World()
{
	tm = new TimerManager();

	level.load();

	//objects.clear();
	//objects.reserve(n);

	tm->Update();
}

void World::Update()
{
	// TODO : 플레이어 위치값 받은거 적용하기 -> Process Queue 에서 해야할 것 같은 내용

	tm->Update();

	int64_t delta_time = tm->getDeltaTime();

	// TODO : 총알들 업데이트 후 총알이 맵 밖으로 나가면 삭제 처리
	for (auto it = b1.begin(); it != b1.end(); ) {
		it->Update(delta_time);
		//if (it->isOutBounds(level->leftBound - 1000.0f, level->rightBound + 1000.0f))
		//	it = bullets.erase(it);
		//else
		//	++it;
	}

	for (auto it = b2.begin(); it != b2.end(); ) {
		it->Update(delta_time);
		//if (it->isOutBounds(level->leftBound - 1000.0f, level->rightBound + 1000.0f))
		//	it = bullets.erase(it);
		//else
		//	++it;
	}

	for (Item& item : items)
		item.Update(delta_time);

	// 플레이어 위치 검사
	// TODO : array로 바꾸면 적용해야 함
	if (p1.GetPos().posY > myNP::DeadZone)
	{
		// 모두에게 전송 (UI 업데이트를 위한)
		SNMgr.SendPacket<myNP::SC_LIFE_UPDATE_PACKET>(static_cast<int32_t>(0), 0);
		SNMgr.SendPacket<myNP::SC_LIFE_UPDATE_PACKET>(static_cast<int32_t>(1), 0);
	}
	if (p2.GetPos().posY > myNP::DeadZone)
	{
		// 모두에게 전송 (UI 업데이트를 위한)
		SNMgr.SendPacket<myNP::SC_LIFE_UPDATE_PACKET>(static_cast<int32_t>(0), 0);
		SNMgr.SendPacket<myNP::SC_LIFE_UPDATE_PACKET>(static_cast<int32_t>(1), 0);
	}

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

			it = items.erase(it);
		}
		else if (p1_collision) {
			p1.GunUpdate(GunInfo.getRandomGunId());

			it = items.erase(it);
		}
		else if (p2_collision) {
			p2.GunUpdate(GunInfo.getRandomGunId());

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

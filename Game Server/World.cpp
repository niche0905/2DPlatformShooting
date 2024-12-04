#include "pch.h"
#include "World.h"
#include "Platform.h"
#include "Item.h"

std::uniform_real_distribution<float> item_spawn_point{ 0.0, 600.0 };


World::World()
	: p1{0u, 100.f, 400.f, true}
	, p2{1u, 400.f, 400.f, true}
{
	tm = new TimerManager();

	level = new Level();
	level->load();

	//objects.clear();
	//objects.reserve(n);

	tm->Update();
}

void World::Init()
{
	// init timer
	tm->Update();
	itemMakingTime = std::chrono::system_clock::now();
	
	// init player id
	p1.SetPlayerID(0);
	p2.SetPlayerID(1);
}

void World::Update()
{
	// TODO : 플레이어 위치값 받은거 적용하기 -> Process Queue 에서 해야할 것 같은 내용
	if (not SNMgr.IsPlaying())
		return;

	tm->Update();
	int64_t delta_time = tm->getDeltaTime();

	// 아이템 생성 로직 가져오기
	std::chrono::system_clock::time_point nowTime = std::chrono::system_clock::now();
	auto timeDiff = std::chrono::duration_cast<std::chrono::milliseconds>(nowTime - itemMakingTime);
	// 현재 시간을 구하고 저번 Item을 생성한 시간과 10초 이상 차이가 나면 생성

	// ItemSpawn은 bool으로 아이템 스폰에 성공했는지 반환 (성공 하든 안하든 시간은 갱신)
	if (timeDiff.count() > 10000) {
		ItemSpawn();
		itemMakingTime = nowTime;
	}

	p1.Update(delta_time);
	p2.Update(delta_time);

	for (Item& item : items)
		item.Update(delta_time);

	// 플레이어 위치 검사
	// TODO : array로 바꾸면 적용해야 함
	if (p1.GetPos().posY > myNP::DeadZone)
	{
		p1.Dead();
	}
	if (p2.GetPos().posY > myNP::DeadZone)
	{
		p2.Dead();
	}

	// 충돌처리 구현
	CollisionCheck();
}

void World::CollisionCheck()
{
	std::list<Bullet>& b1 = p1.GetBullets();
	for (auto it = b1.begin(); it != b1.end(); ) {
		if (p2.Collision(*it)) {
			uint32_t bullet_id = (*it).GetBulletID();
			uint8_t bullet_type = (*it).GetBulletType();
			bool bullet_dir = (*it).GetDirection();

			float damage = GunLoader::Instance().GetGunTable()[bullet_type].damage;

			// P2의 p_id는 1
			SNMgr.SendPacket<myNP::SC_PLAYER_DAMAGE_PACKET>(static_cast<int32_t>(1), damage, bullet_dir);

			// P1의 p_id는 0
			SNMgr.SendPacket<myNP::SC_BULLET_REMOVE_PACKET>(static_cast<int32_t>(0), 0, bullet_id);
			SNMgr.SendPacket<myNP::SC_BULLET_REMOVE_PACKET>(static_cast<int32_t>(1), 0, bullet_id);
			it = b1.erase(it);
		}
		else
			++it;
	}
	std::list<Bullet>& b2 = p2.GetBullets();
	for (auto it = b2.begin(); it != b2.end(); ) {
		if (p1.Collision(*it)) {
			uint32_t bullet_id = (*it).GetBulletID();
			uint8_t bullet_type = (*it).GetBulletType();
			bool bullet_dir = (*it).GetDirection();

			float damage = GunLoader::Instance().GetGunTable()[bullet_type].damage;

			// P1의 p_id는 0
			SNMgr.SendPacket<myNP::SC_PLAYER_DAMAGE_PACKET>(static_cast<int32_t>(0), damage, bullet_dir);

			// P2의 p_id는 1
			SNMgr.SendPacket<myNP::SC_BULLET_REMOVE_PACKET>(static_cast<int32_t>(0), 1, bullet_id);
			SNMgr.SendPacket<myNP::SC_BULLET_REMOVE_PACKET>(static_cast<int32_t>(1), 1, bullet_id);
			it = b2.erase(it);
		}
		else
			++it;
	}

	for (auto it = items.begin(); it != items.end(); ) {
		bool p1_collision = p1.Collision(*it);
		bool p2_collision = p2.Collision(*it);

		uint32_t item_id = (*it).GetItemID();

		if (p1_collision or p2_collision) {
			// Logging
			cout << "Item Collision\n";

			// 이 코드로 압축 가능
			SNMgr.SendPacket<myNP::SC_ITEM_REMOVE_PACKET>(static_cast<int32_t>(0), item_id);
			SNMgr.SendPacket<myNP::SC_ITEM_REMOVE_PACKET>(static_cast<int32_t>(1), item_id);

			it = items.erase(it);

			if (p1_collision and p2_collision) {
				p1.GunUpdate(GunInfo.getRandomGunId());
				p2.GunUpdate(GunInfo.getRandomGunId());
			}
			else if (p1_collision) {
				p1.GunUpdate(GunInfo.getRandomGunId());
			}
			else if (p2_collision) {
				p2.GunUpdate(GunInfo.getRandomGunId());
			}
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

bool World::ItemSpawn()
{
	if (items.size() >= 3)
		return false;

	// Logging
	cout << "Item Spawn\n";

	float x_pos = item_spawn_point(RANDOM_ENGINE);
	Item item{ x_pos, itemSpawnHeight, level };
	uint32_t item_id = item.GetItemID();
	items.push_back(item);
	SNMgr.SendPacket<myNP::SC_ITEM_CREATE_PACKET>(static_cast<int32_t>(0), item_id, x_pos, itemSpawnHeight);
	SNMgr.SendPacket<myNP::SC_ITEM_CREATE_PACKET>(static_cast<int32_t>(1), item_id, x_pos, itemSpawnHeight);

	return true;
}

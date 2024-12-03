#include "pch.h"
#include "World.h"
#include "Platform.h"
#include "Item.h"

std::uniform_real_distribution<float> item_spawn_point{ 0.0, 600.0 };


World::World()
{
	tm = new TimerManager();

	level.load();

	//objects.clear();
	//objects.reserve(n);

	tm->Update();
}

void World::Init()
{
	itemMakingTime = std::chrono::system_clock::now();
}

void World::Update()
{
	// TODO : �÷��̾� ��ġ�� ������ �����ϱ� -> Process Queue ���� �ؾ��� �� ���� ����

	tm->Update();
	int64_t delta_time = tm->getDeltaTime();

	// ������ ���� ���� ��������
	std::chrono::system_clock::time_point nowTime = std::chrono::system_clock::now();
	auto timeDiff = std::chrono::duration_cast<std::chrono::milliseconds>(nowTime - itemMakingTime);
	// ���� �ð��� ���ϰ� ���� Item�� ������ �ð��� 10�� �̻� ���̰� ���� ����

	// ItemSpawn�� bool���� ������ ������ �����ߴ��� ��ȯ (���� �ϵ� ���ϵ� �ð��� ����)
	if (timeDiff.count() > 10000) {
		ItemSpawn();
		itemMakingTime = nowTime;
	}

	p1.Update(delta_time);
	p2.Update(delta_time);

	for (Item& item : items)
		item.Update(delta_time);

	// �÷��̾� ��ġ �˻�
	// TODO : array�� �ٲٸ� �����ؾ� ��
	if (p1.GetPos().posY > myNP::DeadZone)
	{
		// ��ο��� ���� (UI ������Ʈ�� ����)
		SNMgr.SendPacket<myNP::SC_LIFE_UPDATE_PACKET>(static_cast<int32_t>(0), 0);
		SNMgr.SendPacket<myNP::SC_LIFE_UPDATE_PACKET>(static_cast<int32_t>(1), 0);
	}
	if (p2.GetPos().posY > myNP::DeadZone)
	{
		// ��ο��� ���� (UI ������Ʈ�� ����)
		SNMgr.SendPacket<myNP::SC_LIFE_UPDATE_PACKET>(static_cast<int32_t>(0), 0);
		SNMgr.SendPacket<myNP::SC_LIFE_UPDATE_PACKET>(static_cast<int32_t>(1), 0);
	}

	// �浹ó�� ����
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

			// P2�� p_id�� 1
			SNMgr.SendPacket<myNP::SC_PLAYER_DAMAGE_PACKET>(static_cast<int32_t>(1), damage, bullet_dir);

			// P1�� p_id�� 0
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
			// ���⿡ ���� ������ ó��
			if (bullet_dir) {
				damage = -damage;
			}

			// P1�� p_id�� 0
			SNMgr.SendPacket<myNP::SC_PLAYER_DAMAGE_PACKET>(static_cast<int32_t>(0), damage, bullet_dir);

			// P2�� p_id�� 1
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
			// �� �ڵ�� ���� ����
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

	float x_pos = item_spawn_point(RANDOM_ENGINE);
	Item item{ x_pos, itemSpawnHeight };
	uint32_t item_id = item.GetItemID();
	items.push_back(item);
	SNMgr.SendPacket<myNP::SC_ITEM_CREATE_PACKET>(static_cast<int32_t>(0), item_id, x_pos, itemSpawnHeight);
	SNMgr.SendPacket<myNP::SC_ITEM_CREATE_PACKET>(static_cast<int32_t>(1), item_id, x_pos, itemSpawnHeight);
}

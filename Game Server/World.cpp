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
	// TODO : �÷��̾� ��ġ�� ������ �����ϱ� -> Process Queue ���� �ؾ��� �� ���� ����

	tm->Update();

	int64_t delta_time = tm->getDeltaTime();

	// TODO : �Ѿ˵� ������Ʈ �� �Ѿ��� �� ������ ������ ���� ó��
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
	// TODO : �浹 ó�� �� ������ ��Ŷ ���� �κ��� �����ؾ� �Ѵ�
	for (auto it = b2.begin(); it != b2.end(); ) {
		if (p1.Collision(*it)) {
			// ������ ��Ŷ ����
			it = b2.erase(it);
		}
		else
			++it;
	}
	for (auto it = b2.begin(); it != b2.end(); ) {
		if (p1.Collision(*it)) {
			// ������ ��Ŷ ����
			it = b2.erase(it);
		}
		else
			++it;
	}

	// TODO : �������� �԰� �������� �ѱ⸦ �־��ִ� ������ ��������
	//		  Gun Update ���� ��Ŷ�� �����ϰ� �����ؾ� ��

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

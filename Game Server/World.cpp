#include "pch.h"
#include "World.h"


World::World()
{
	//objects.clear();
	//objects.reserve(n);
}

void World::Update()
{
	// TODO : �÷��̾� ��ġ�� ������ �����ϱ� -> Process Queue ���� �ؾ��� �� ���� ����
	
	// TODO : �Ѿ˵� ������Ʈ �� �Ѿ��� �� ������ ������ ���� ó��
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

			// TODO : Gun Update ��Ŷ ������ �Ѵ� �ι���

			it = items.erase(it);
		}
		else if (p1_collision) {
			p1.GunUpdate(GunInfo.getRandomGunId());

			// TODO : Gun Update ��Ŷ ������ �Ѵ� p1�� ������Ʈ�� ����

			it = items.erase(it);
		}
		else if (p2_collision) {
			p2.GunUpdate(GunInfo.getRandomGunId());

			// TODO : Gun Update ��Ŷ ������ �Ѵ� p2�� ������Ʈ�� ����

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

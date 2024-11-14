#include "pch.h"
#include "World.h"


World::World()
{
	objects.clear();
	//objects.reserve(n);
}

void World::Update()
{
	// p1 p2�� ��ġ���� MOVE ��Ŷ���� ���� ����

	for (Bullet& b : b1)
		b.Update();
	for (Bullet& b : b2)
		b.Update();
	for (Item& item : items)
		item.Update();

	// �浹ó�� ����
	CollisionCheck();
}

void World::CollisionCheck()
{
	// TODO : �浹 ó�� �� ������ ��Ŷ ���� �κ��� �����ؾ� �Ѵ�
	for (Bullet& b : b2) {
		if (p1.Collision(b)) {
			// ������ ��Ŷ ����
		}
	}
	for (Bullet& b : b1) {
		if (p2.Collision(b)) {
			// ������ ��Ŷ ����
		}
	}

	// TODO : �������� �԰� �������� �ѱ⸦ �־��ִ� ������ ��������
	//		  Gun Update ���� ��Ŷ�� �����ϰ� �����ؾ� ��

	// ������ �浹ó���� ���� ���� ���� �ƴ�?
	for (Item& item : items) {
		bool p1_collision = p1.Collision(item);
		bool p2_collision = p2.Collision(item);

		if (p1_collision and p2_collision) {
			// �̰� ��� ��?
		}
		else if (p1_collision) {
			// �� ������Ʈ ó�� �� ��Ŷ ������
		}
		else if (p2_collision) {
			// �� ������Ʈ ó�� �� ��Ŷ ������
		}
		// ������� �ڵ�
	}
}

void World::Process()
{

}

void World::Recv()
{

}

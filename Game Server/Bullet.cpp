#include "pch.h"
#include "Bullet.h"


Bullet::Bullet(uint8_t bullet_type, bool dir)
	: MovingObject(0.0f, 0.0f, myNP::BulletWidth, myNP::BulletHeight, 0.5, 0.5)
	, b_type(bullet_type)
	, direction(dir)
{
	//SetVelocity(0.0f, 0.0f); b_type�� �°� �ӵ� ����
}

Bullet::Bullet(float x, float y, uint8_t bullet_type, bool dir)
	: MovingObject(x, y, myNP::BulletWidth, myNP::BulletHeight, 0.5, 0.5)
	, b_type(bullet_type)
	, direction(dir)
{
	//SetVelocity(0.0f, 0.0f); b_type�� �°� �ӵ� ����
}

void Bullet::Update()
{
	// TODO : deltaTime ��ŭ �̵�
	//		  ���⼭�� gun_id (b_type) ���� ���

	
}

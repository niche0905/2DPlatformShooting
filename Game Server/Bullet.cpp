#include "pch.h"
#include "Bullet.h"


Bullet::Bullet(uint8_t bullet_type, bool dir)
	: Object(0.0f, 0.0f, myNP::BulletWidth, myNP::BulletHeight, 0.5, 0.5)
	, b_type(bullet_type)
	, direction(dir)
{

}

Bullet::Bullet(float x, float y, uint8_t bullet_type, bool dir)
	: Object(x, y, myNP::BulletWidth, myNP::BulletHeight, 0.5, 0.5)
	, b_type(bullet_type)
	, direction(dir)
{

}

void Bullet::Update()
{
	// TODO : deltaTime ��ŭ �̵�
	//		  ���⼭�� gun_id (b_type) ���� ���


}

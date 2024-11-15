#include "pch.h"
#include "Bullet.h"


Bullet::Bullet(uint8_t bullet_type, bool dir)
	: MovingObject(0.0f, 0.0f, myNP::BulletWidth, myNP::BulletHeight, 0.5, 0.5)
	, b_type(bullet_type)
	, direction(dir)
{
	//SetVelocity(0.0f, 0.0f); b_type에 맞게 속도 설정
}

Bullet::Bullet(float x, float y, uint8_t bullet_type, bool dir)
	: MovingObject(x, y, myNP::BulletWidth, myNP::BulletHeight, 0.5, 0.5)
	, b_type(bullet_type)
	, direction(dir)
{
	//SetVelocity(0.0f, 0.0f); b_type에 맞게 속도 설정
}

void Bullet::Update()
{
	// TODO : deltaTime 만큼 이동
	//		  여기서도 gun_id (b_type) 으로 사용

	
}

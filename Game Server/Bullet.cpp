#include "pch.h"
#include "Bullet.h"


Bullet::Bullet(uint32_t bullet_id, uint8_t bullet_type, bool dir)
	: MovingObject{ 0.0f, 0.0f, myNP::BulletWidth, myNP::BulletHeight, 0.5, 0.5 }
	, b_id{ bullet_id }
	, b_type{ bullet_type }
	, direction{ dir }
{
}

Bullet::Bullet(float x, float y, uint32_t bullet_id, uint8_t bullet_type, bool dir)
	: MovingObject{ x, y, myNP::BulletWidth, myNP::BulletHeight, 0.5, 0.5 }
	, b_id{ bullet_id }
	, b_type{ bullet_type }
	, direction{ dir }
{
}

void Bullet::Update(int64_t delta_time)
{
	// TODO : deltaTime 만큼 이동
	//		  여기서도 gun_id (b_type) 으로 사용
}

uint32_t Bullet::GetBulletID() const
{
	return b_id;
}

uint8_t Bullet::GetBulletType() const
{
	return b_type;
}

bool Bullet::GetDirection() const
{
	return direction;
}

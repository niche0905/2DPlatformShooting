#include "pch.h"
#include "Bullet.h"


Bullet::Bullet(uint32_t bullet_id, uint8_t bullet_type, bool dir)
	: MovingObject{ 0.0f, 0.0f, myNP::BulletWidth, myNP::BulletHeight, 0.5, 0.5 }
	, b_id{ bullet_id }
	, b_type{ bullet_type }
	, direction{ dir }
{
	// SetVelocity(0.0f, 0.0f); b_type에 맞게 속도 설정
	// bullets.push_back(Bullet(direction, position, GunLoader::Instance().GetGunTable()[gunId].speed, GunLoader::Instance().GetGunTable()[gunId].damage));
	
	float speed = GunLoader::Instance().GetGunTable()[bullet_type].speed;
	if (true == dir) {
		speed *= -1.0;
	}

	SetVelocity(speed, 0.0f);
}

Bullet::Bullet(float x, float y, uint32_t bullet_id, uint8_t bullet_type, bool dir)
	: MovingObject{ x, y, myNP::BulletWidth, myNP::BulletHeight, 0.5, 0.5 }
	, b_id{ bullet_id }
	, b_type{ bullet_type }
	, direction{ dir }
{
	//SetVelocity(0.0f, 0.0f); b_type에 맞게 속도 설정

	float speed = GunLoader::Instance().GetGunTable()[bullet_type].speed;
	if (true == dir) {
		speed *= -1.0;
	}

	SetVelocity(speed, 0.0f);

}

void Bullet::Update(int64_t delta_time)
{
	// TODO : deltaTime 만큼 이동
	//		  여기서도 gun_id (b_type) 으로 사용
	UpdateVelocity(delta_time);
}

bool Bullet::isOutBounds(float left, float right)
{
	return posX < left or right < posY;
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

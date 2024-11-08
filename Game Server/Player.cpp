#include "pch.h"
#include "Player.h"


Player::Player(bool dir) 
	: directon(dir)
	, life(BaseLife)
{
	GunInit();
}

Player::Player(float pos_x, float pos_y, bool dir)
	: Object(pos_x, pos_y, myNP::PlayerWidth, myNP::PlayerHeight, 0.5f, 1.0f)
	, directon(dir)
	, life(BaseLife)
{
	GunInit();
}

void Player::GunInit()
{
	gunId = BaseGunID;
	curMag = -1;
}

void Player::GunUpdate(uint8_t gun_id)
{
	gunId = gun_id;
	// TODO : gun_id를 통해 총 정보를 받는 자료구조 필요
	//		  클라이언트와 같이 처리하면 될 것으로 보임
}

Position Player::GunFire()
{
	Position fire_pos = GetPos();

	// TODO : Bullet 생성 및 gunId에 대한 총알로 만들어서 넣어주어야 함
	
	// 총알을 다 썻다면 기본 총으로 초기화
	if (--curMag <= 0) {
		GunInit();
	}

	return fire_pos;
}

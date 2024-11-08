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

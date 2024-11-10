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
	// TODO : gun_id�� ���� �� ������ �޴� �ڷᱸ�� �ʿ�
	//		  Ŭ���̾�Ʈ�� ���� ó���ϸ� �� ������ ����
}

Position Player::GunFire()
{
	Position fire_pos = GetPos();

	// TODO : Bullet ���� �� gunId�� ���� �Ѿ˷� ���� �־��־�� ��
	
	// �Ѿ��� �� ���ٸ� �⺻ ������ �ʱ�ȭ
	if (--curMag <= 0) {
		GunInit();
	}

	return fire_pos;
}

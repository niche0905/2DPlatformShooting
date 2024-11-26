#include "pch.h"
#include "Player.h"


Player::Player(uint32_t p_id, bool dir)
	: player_id{ p_id }
	, directon{ dir }
	, life{ BaseLife }
{
	GunInit();
}

Player::Player(uint32_t p_id, float pos_x, float pos_y, bool dir)
	: Object{ pos_x, pos_y, myNP::PlayerWidth, myNP::PlayerHeight, 0.5f, 1.0f }
	, player_id{ p_id }
	, directon{ dir }
	, life{ BaseLife }
{
	GunInit();
}

uint32_t Player::GetPlayerID() const
{
	return player_id;
}

void Player::SetPlayerID(uint32_t p_id)
{
	player_id = p_id;
}

void Player::GunInit()
{
	gunId = BaseGunID;
	curMag = -1;
}

void Player::GunUpdate(uint8_t gun_id)
{
	gunId = gun_id;
	curMag = GunLoader::Instance().GetGunTable()[gun_id].mag;
	// TODO : player_id �־�� �ϰ� ������ �޾ƾ� �� (���� ��� ���� �־�� ��)
	
	//myNP::SC_GUN_UPDATE_PACKET::MakePacket();
	SNMgr.SendPacket<myNP::SC_GUN_UPDATE_PACKET>(static_cast<int32_t>(player_id),
		player_id, gun_id
	);
}

Position Player::GunFire()
{
	Position fire_pos = GetPos();

	// TODO : Bullet ���� �� gunId�� ���� �Ѿ˷� ���� �־��־�� ��
	
	// �Ѿ��� �� ���ٸ� �⺻ ������ �ʱ�ȭ
	if (--curMag <= 0) {
		// TODO : �̶� GunUpdate�� �ҷ��� �� ������ ���δ�
		GunInit();
	}

	return fire_pos;
}

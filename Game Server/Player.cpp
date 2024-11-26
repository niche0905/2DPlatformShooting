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
	// TODO : player_id 있어야 하고 소켓을 받아야 함 (양쪽 모두 보내 주어야 함)
	
	//myNP::SC_GUN_UPDATE_PACKET::MakePacket();
	SNMgr.SendPacket<myNP::SC_GUN_UPDATE_PACKET>(static_cast<int32_t>(player_id),
		player_id, gun_id
	);
}

Position Player::GunFire()
{
	Position fire_pos = GetPos();

	// TODO : Bullet 생성 및 gunId에 대한 총알로 만들어서 넣어주어야 함
	
	// 총알을 다 썻다면 기본 총으로 초기화
	if (--curMag <= 0) {
		// TODO : 이때 GunUpdate를 불러야 할 것으로 보인다
		GunInit();
	}

	return fire_pos;
}

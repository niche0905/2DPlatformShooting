#include "pch.h"
#include "Player.h"
#include "Bullet.h"


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

void Player::Update(int64_t delta_time)
{
	// TODO : Bullet에 isOutBounds 함수를 구현하고 
	// Player에서 Level의 *를 가지고 left, right bound에 접근할 수 있어야 함
	for (auto it = bullets.begin(); it != bullets.end(); ) {
		it->Update(delta_time);
		//if (it->isOutBounds(level->leftBound - 1000.0f, level->rightBound + 1000.0f))
		//	it = bullets.erase(it);
		//else
		//	++it;
	}
}

uint32_t Player::GetPlayerID() const
{
	return player_id;
}

void Player::SetPlayerID(uint32_t p_id)
{
	player_id = p_id;
}

std::list<Bullet>& Player::GetBullets()
{
	return bullets;
}

const std::list<Bullet>& Player::GetBullets() const
{
	return bullets;
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

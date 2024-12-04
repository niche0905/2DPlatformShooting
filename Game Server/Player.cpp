#include "pch.h"
#include "Player.h"
#include "Bullet.h"


Player::Player(uint32_t p_id, bool dir)
	: Object{ 0.0f, 0.0f, myNP::PlayerWidth, myNP::PlayerHeight, 0.5f, 1.0f }
	, player_id{ p_id }
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
		if (it->isOutBounds(-11000.f, 11000.0f)) {
			it = bullets.erase(it);
			std::cout << "총알 삭제\n";
		}
		else
			++it;
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
	int32_t other_player_id = 1 - player_id;
	SNMgr.SendPacket<myNP::SC_GUN_UPDATE_PACKET>(other_player_id,
		player_id, gun_id
	);
}

// 총 쏘는 함수
Position Player::GunFire(float x, float y, uint8_t bullet_type, bool dir)
{
	Position fire_pos = GetPos();

	// 월드에 불렛 추가하기
	std::cout << "총알 생성. 현재 탄창수: " << curMag << '\n';
	bullets.emplace_back(x, y, bulletId++, bullet_type, dir);


	// 총알을 다 썻다면 기본 총으로 초기화
	if (--curMag <= 0 && gunId != 0u) {
		std::cout << "총 다씀..\n";
		GunInit();
		// 서버도 초기화를 해주고 클라이언트에게 전송해준다
		SNMgr.SendPacket<myNP::SC_GUN_UPDATE_PACKET>(static_cast<int32_t>(player_id),
			player_id, BaseGunID
		);
		int32_t other_player_id = 1 - static_cast<int32_t>(player_id);
		SNMgr.SendPacket<myNP::SC_GUN_UPDATE_PACKET>(other_player_id,
			player_id, BaseGunID
		);
	}

	return fire_pos;
}

void Player::Revive()
{
	if (isActive)
		return;

	float y_pos = GetPos().posY;
	if (y_pos <= myNP::DeadZone)
		isActive = true;
}

void Player::Dead()
{
	if (not isActive)
		return;

	life -= 1;
	isActive = false;

	// 모두에게 전송 (UI 업데이트를 위한)
	SNMgr.SendPacket<myNP::SC_LIFE_UPDATE_PACKET>(static_cast<int32_t>(0), player_id);
	SNMgr.SendPacket<myNP::SC_LIFE_UPDATE_PACKET>(static_cast<int32_t>(1), player_id);
}

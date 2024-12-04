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
	// TODO : Bullet�� isOutBounds �Լ��� �����ϰ� 
	// Player���� Level�� *�� ������ left, right bound�� ������ �� �־�� ��
	for (auto it = bullets.begin(); it != bullets.end(); ) {
		it->Update(delta_time);
		if (it->isOutBounds(-11000.f, 11000.0f)) {
			it = bullets.erase(it);
			std::cout << "�Ѿ� ����\n";
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
	// TODO : player_id �־�� �ϰ� ������ �޾ƾ� �� (���� ��� ���� �־�� ��)
	
	//myNP::SC_GUN_UPDATE_PACKET::MakePacket();
	SNMgr.SendPacket<myNP::SC_GUN_UPDATE_PACKET>(static_cast<int32_t>(player_id),
		player_id, gun_id
	);
	int32_t other_player_id = 1 - player_id;
	SNMgr.SendPacket<myNP::SC_GUN_UPDATE_PACKET>(other_player_id,
		player_id, gun_id
	);
}

// �� ��� �Լ�
Position Player::GunFire(float x, float y, uint8_t bullet_type, bool dir)
{
	Position fire_pos = GetPos();

	// ���忡 �ҷ� �߰��ϱ�
	std::cout << "�Ѿ� ����. ���� źâ��: " << curMag << '\n';
	bullets.emplace_back(x, y, bulletId++, bullet_type, dir);


	// �Ѿ��� �� ���ٸ� �⺻ ������ �ʱ�ȭ
	if (--curMag <= 0 && gunId != 0u) {
		std::cout << "�� �پ�..\n";
		GunInit();
		// ������ �ʱ�ȭ�� ���ְ� Ŭ���̾�Ʈ���� �������ش�
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

	// ��ο��� ���� (UI ������Ʈ�� ����)
	SNMgr.SendPacket<myNP::SC_LIFE_UPDATE_PACKET>(static_cast<int32_t>(0), player_id);
	SNMgr.SendPacket<myNP::SC_LIFE_UPDATE_PACKET>(static_cast<int32_t>(1), player_id);
}

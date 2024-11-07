#pragma once

#include <cstdint>
#include <chrono>

// ���̺귯��ȭ �� ����� ����

namespace myNP {

	enum PacketID : uint8_t
	{
		// 0�� ������ ����� ���� Ȯ���ϱ� ���� �����
		CS_MATCHMAKING = 1,
		CS_MOVE,
		CS_FIRE,
		SC_MATCHMAKING,
		SC_MY_MOVE,
		SC_PLAYER_DAMAGE,
		SC_FIRE,
		SC_BULLET_REMOVE,
		SC_ITEM_CREATE,
		SC_ITEM_REMOVE,
		SC_GUN_UPDATE,
		SC_LIFE_UPDATE,
		SC_GAMEOVER

	};

	constexpr int MaxPacketSize{ 32 };		// ��Ŷ �ִ� ������ (���� ����� ���ϴ�)

	constexpr int SyncTime{ 30 };			// ����ȭ �ð� 1�ʿ� 30�� (1/30 ��)

	constexpr float BulletWidth{ 20.0f };	// �Ѿ� ����
	constexpr float BulletHeight{ 6.0f };	// �Ѿ� ũ��

	constexpr float PlayerWidth{ 50.0f };	// �÷��̾� ����
	constexpr float PlayerHeight{ 50.0f };	// �÷��̾� ����

	constexpr float GravityAcc = 9.810f;    // �߷� ���ӵ�
	constexpr float GravityMul = 120.0f;    // �߷� ���ӵ��� ���ϴ� ��(���Ӽ��� ����)
	constexpr float PlatformUp = 10.0f;     // �÷��� ���� �ִٴ� ����(�浹 ó�� �� ���)


	//---------------------------------------------------------
	// ��Ŷ��
	//---------------------------------------------------------

#pragma pack(push, 1)

// ��Ŷ ���
	struct BASE_PACKET
	{
		uint8_t size;
		PacketID id;

		BASE_PACKET(uint8_t packet_size, PacketID packet_id) : size(packet_size), id(packet_id) {}
		BASE_PACKET() : size(2), id(static_cast<PacketID>(0)) {}
	};

	// client to server ��ġ����ŷ ��´�
	struct CS_MATCHMAKING_PACKET : public BASE_PACKET
	{
		CS_MATCHMAKING_PACKET() : BASE_PACKET(sizeof(CS_MATCHMAKING_PACKET), PacketID::CS_MATCHMAKING) {}
	};

	// client to server �÷��̾ �ش� ��ġ�� �̵��ߴ�
	struct CS_MOVE_PACKET : public BASE_PACKET
	{
		uint32_t p_id;
		float posX, posY;
		bool dir;

		CS_MOVE_PACKET(uint32_t player_id, float x, float y, bool direction)
			: BASE_PACKET(sizeof(CS_MOVE_PACKET), PacketID::CS_MOVE)
			, p_id(player_id)
			, posX(x)
			, posY(y)
			, dir(direction) {}
	};

	// client to server �÷��̾ �ش� ��ġ���� �Ѿ��� �߻� �Ͽ���
	struct CS_FIRE_PACKET : public BASE_PACKET
	{
		uint32_t b_id;
		float posX, posY;
		bool dir;
		uint32_t type;
		std::chrono::milliseconds fire_t;

		CS_FIRE_PACKET(uint32_t bullet_id, float x, float y, bool direction,
			uint32_t bullet_type, std::chrono::milliseconds fire_time)
			: BASE_PACKET(sizeof(CS_FIRE_PACKET), PacketID::CS_FIRE)
			, b_id(bullet_id)
			, posX(x)
			, posY(y)
			, dir(direction)
			, type(bullet_type)
			, fire_t(fire_time) {}
	};

	// server to client ��ġ����ŷ ������
	struct SC_MATCHMAKING_PACKET : public BASE_PACKET
	{
		bool succ;

		SC_MATCHMAKING_PACKET(bool success = true)
			: BASE_PACKET(sizeof(SC_MATCHMAKING_PACKET), PacketID::SC_MATCHMAKING)
			, succ(success) {}
	};

	// server to client �÷��̾ �������� �̵��ߴ�
	struct SC_MOVE_PACKET : public BASE_PACKET
	{
		uint32_t p_id;
		float posX, posY;
		bool dir;

		SC_MOVE_PACKET(uint32_t player_id, float x, float y, bool direction)
			: BASE_PACKET(sizeof(SC_MOVE_PACKET), PacketID::SC_MY_MOVE)
			, p_id(player_id)
			, posX(x)
			, posY(y)
			, dir(direction) {}
	};

	// server to client �÷��̾ �������� �Ծ���
	struct SC_PLAYER_DAMAGE_PACKET : public BASE_PACKET
	{
		float damage;

		SC_PLAYER_DAMAGE_PACKET(float damaged)
			: BASE_PACKET(sizeof(SC_PLAYER_DAMAGE_PACKET), PacketID::SC_PLAYER_DAMAGE)
			, damage(damaged) {}
	};

	// server to client �÷��̾ ���� �� ����� �˸�
	struct SC_FIRE_PACKET : public BASE_PACKET
	{
		uint32_t b_id;
		float posX, posY;
		bool dir;
		uint32_t type;
		std::chrono::milliseconds fire_t;

		SC_FIRE_PACKET(uint32_t bullet_id, float x, float y, bool direction,
			uint32_t bullet_type, std::chrono::milliseconds fire_time)
			: BASE_PACKET(sizeof(SC_FIRE_PACKET), PacketID::SC_FIRE)
			, b_id(bullet_id)
			, posX(x)
			, posY(y)
			, dir(direction)
			, type(bullet_type)
			, fire_t(fire_time) {}
	};

	// server to client �ش� �Ѿ��� ��������� �˸��� ��Ŷ
	struct SC_BULLET_REMOVE_PACKET : public BASE_PACKET
	{
		uint32_t p_id;
		uint32_t b_id;

		SC_BULLET_REMOVE_PACKET(uint32_t player_id, uint32_t bullet_id)
			: BASE_PACKET(sizeof(SC_BULLET_REMOVE_PACKET), PacketID::SC_BULLET_REMOVE)
			, p_id(player_id)
			, b_id(bullet_id) {}
	};

	// server to client ������ �ش� ��ġ�� ���� �Ǿ���
	struct SC_ITEM_CREATE_PACKET : public BASE_PACKET
	{
		uint32_t i_id;
		float posX, posY;

		SC_ITEM_CREATE_PACKET(uint32_t item_id, float x, float y)
			: BASE_PACKET(sizeof(SC_ITEM_CREATE_PACKET), PacketID::SC_ITEM_CREATE)
			, i_id(item_id)
			, posX(x)
			, posY(y) {}
	};

	// server to client �ش� �������� �����Ǿ���
	struct SC_ITEM_REMOVE_PACKET : public BASE_PACKET
	{
		uint32_t i_id;

		SC_ITEM_REMOVE_PACKET(uint32_t item_id)
			: BASE_PACKET(sizeof(SC_ITEM_REMOVE_PACKET), PacketID::SC_ITEM_REMOVE)
			, i_id(item_id) {}
	};

	// server to client �ش� �÷��̾ �ش� ������ ����Ǿ���
	struct SC_GUN_UPDATE_PACKET : public BASE_PACKET
	{
		uint32_t p_id;
		uint32_t g_id;

		SC_GUN_UPDATE_PACKET(uint32_t player_id, uint32_t gun_id)
			: BASE_PACKET(sizeof(SC_GUN_UPDATE_PACKET), PacketID::SC_GUN_UPDATE)
			, p_id(player_id)
			, g_id(gun_id) {}
	};

	// server to client �ش� �÷��̾��� ����� �����ߴ�
	struct SC_LIFE_UPDATE_PACKET : public BASE_PACKET
	{
		uint32_t p_id;

		SC_LIFE_UPDATE_PACKET(uint32_t player_id)
			: BASE_PACKET(sizeof(SC_LIFE_UPDATE_PACKET), PacketID::SC_LIFE_UPDATE)
			, p_id(player_id) {}
	};

	// server to client ������ ����Ǿ����� �˸�
	struct SC_GAMEOVER_PACKET : public BASE_PACKET
	{
		SC_GAMEOVER_PACKET()
			: BASE_PACKET(sizeof(SC_GAMEOVER_PACKET), PacketID::SC_GAMEOVER) {}
	};

#pragma pack(pop)

}
#pragma once

#include <cstdint>
#include <chrono>

// ���̺귯��ȭ �� ����� ����

namespace myNP {

	//---------------------------------------------------------
	// �����
	//---------------------------------------------------------

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
		SC_GAMEOVER,

		END	// �� ������ ��Ŷ ID���� ��
	};

	constexpr int MaxPacketSize{ 32 };		// ��Ŷ �ִ� ������ (���� ����� ���ϴ�)

	constexpr int SyncTime{ 30 };			// ����ȭ �ð� 1�ʿ� 30�� (1/30 ��)

	constexpr float BulletWidth{ 20.0f };	// �Ѿ� ����
	constexpr float BulletHeight{ 6.0f };	// �Ѿ� ũ��

	constexpr float PlayerWidth{ 50.0f };	// �÷��̾� ����
	constexpr float PlayerHeight{ 50.0f };	// �÷��̾� ����

	constexpr float ItemWidth{ 35.0f };
	constexpr float ItemHeight{ 50.0f };

	constexpr float GravityAcc = 9.810f;    // �߷� ���ӵ�
	constexpr float GravityMul = 120.0f;    // �߷� ���ӵ��� ���ϴ� ��(���Ӽ��� ����)
	constexpr float PlatformUp = 10.0f;     // �÷��� ���� �ִٴ� ����(�浹 ó�� �� ���)

	constexpr float DeadZone = 1000.0f;		// ��� ���� y��

	constexpr float microToSecond{ 1000000.0f };	// Update���� ����ϱ� ���� ������ ���


	//---------------------------------------------------------
	// ��Ŷ��
	//---------------------------------------------------------

#pragma pack(push, 1)

// ��Ŷ ���

	// BASE_PACKET
	//  size: ��Ŷ�� ������
	//  id: ��Ŷ�� id
	struct BASE_PACKET
	{
		uint8_t size;
		PacketID id;

		BASE_PACKET(uint8_t packet_size, PacketID packet_id) : size(packet_size), id(packet_id) {}
		BASE_PACKET() : size(2), id(static_cast<PacketID>(0)) {}
		
		void ntohByteOrder();
	};

	// client to server ��ġ����ŷ ��´�
	struct CS_MATCHMAKING_PACKET : public BASE_PACKET
	{
		CS_MATCHMAKING_PACKET() 
			: BASE_PACKET(sizeof(CS_MATCHMAKING_PACKET), PacketID::CS_MATCHMAKING) {}
		static CS_MATCHMAKING_PACKET MakePacket();
		void ntohByteOrder();
	};

	// client to server �÷��̾ �ش� ��ġ�� �̵��ߴ�
	//  uint32	p_id: �÷��̾� id
	//  float	posX, posY: �÷����� ��ġ
	//  bool	dir: �÷��̾��� ����
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
		static CS_MOVE_PACKET MakePacket(uint32_t player_id, float x, float y, bool direction);
		void ntohByteOrder();
	};

	// client to server �÷��̾ �ش� ��ġ���� �Ѿ��� �߻� �Ͽ���
	//  uint32	b_id: �Ѿ��� id
	//  float	posX, posY: �Ѿ��� ��ġ
	//  bool	dir: �Ѿ��� ����
	//  uint32	type: 
	//  microseconds	fire_t: �߻� �ð�
	struct CS_FIRE_PACKET : public BASE_PACKET
	{
		uint32_t b_id;
		float posX, posY;
		bool dir;
		uint32_t type;
		std::chrono::microseconds fire_t;

		CS_FIRE_PACKET(uint32_t bullet_id, float x, float y, bool direction,
			uint32_t bullet_type, std::chrono::microseconds fire_time)
			: BASE_PACKET(sizeof(CS_FIRE_PACKET), PacketID::CS_FIRE)
			, b_id(bullet_id)
			, posX(x)
			, posY(y)
			, dir(direction)
			, type(bullet_type)
			, fire_t(fire_time) {}
		static CS_FIRE_PACKET MakePacket(uint32_t bullet_id, float x, float y, bool direction,
			uint32_t bullet_type, std::chrono::microseconds fire_time);
		void ntohByteOrder();
	};

	// server to client ��ġ����ŷ ������
	//  bool	succ: ���� ����
	//  uint32	p_id: �÷��̾� ID
	struct SC_MATCHMAKING_PACKET : public BASE_PACKET
	{
		bool succ;
		uint32_t p_id;

		SC_MATCHMAKING_PACKET(bool success, uint32_t player_id)
			: BASE_PACKET(sizeof(SC_MATCHMAKING_PACKET), PacketID::SC_MATCHMAKING)
			, succ(success)
			, p_id(player_id) {}
		static SC_MATCHMAKING_PACKET MakePacket(bool success, uint32_t player_id);
		void ntohByteOrder();
	};

	// server to client �÷��̾ �������� �̵��ߴ�
	//  uint32	p_id: �÷��̾� id
	//  float	posX, posY: �÷����� ��ġ
	//  bool	dir: �÷��̾��� ����
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
		static SC_MOVE_PACKET MakePacket(uint32_t player_id, float x, float y, bool direction);
		void ntohByteOrder();
	};

	// server to client �÷��̾ �������� �Ծ���
	//  float	damage: ���� ������
	struct SC_PLAYER_DAMAGE_PACKET : public BASE_PACKET
	{
		float damage;

		SC_PLAYER_DAMAGE_PACKET(float damaged)
			: BASE_PACKET(sizeof(SC_PLAYER_DAMAGE_PACKET), PacketID::SC_PLAYER_DAMAGE)
			, damage(damaged) {}
		static SC_PLAYER_DAMAGE_PACKET MakePacket(float damaged);
		void ntohByteOrder();
	};

	// server to client �÷��̾ ���� �� ����� �˸�
	//  uint32	b_id: �Ѿ��� id
	//  float	posX, posY: �Ѿ��� ��ġ
	//  bool	dir: �Ѿ��� ����
	//  uint32	type: �Ѿ��� Ÿ��
	//  microseconds	fire_t: �߻� �ð�
	struct SC_FIRE_PACKET : public BASE_PACKET
	{
		uint32_t b_id;
		float posX, posY;
		bool dir;
		uint32_t type;
		std::chrono::microseconds fire_t;

		SC_FIRE_PACKET(uint32_t bullet_id, float x, float y, bool direction,
			uint32_t bullet_type, std::chrono::microseconds fire_time)
			: BASE_PACKET(sizeof(SC_FIRE_PACKET), PacketID::SC_FIRE)
			, b_id(bullet_id)
			, posX(x)
			, posY(y)
			, dir(direction)
			, type(bullet_type)
			, fire_t(fire_time) {}
		static SC_FIRE_PACKET MakePacket(uint32_t bullet_id, float x, float y, bool direction,
			uint32_t bullet_type, std::chrono::microseconds fire_time);
		void ntohByteOrder();
	};

	// server to client �ش� �Ѿ��� ��������� �˸��� ��Ŷ
	//  uint32	p_id: �÷��̾��� id
	//  uint32	b_id: ����� �Ѿ��� id
	struct SC_BULLET_REMOVE_PACKET : public BASE_PACKET
	{
		uint32_t p_id;
		uint32_t b_id;

		SC_BULLET_REMOVE_PACKET(uint32_t player_id, uint32_t bullet_id)
			: BASE_PACKET(sizeof(SC_BULLET_REMOVE_PACKET), PacketID::SC_BULLET_REMOVE)
			, p_id(player_id)
			, b_id(bullet_id) {}
		static SC_BULLET_REMOVE_PACKET MakePacket(uint32_t player_id, uint32_t bullet_id);
		void ntohByteOrder();
	};

	// server to client ������ �ش� ��ġ�� ���� �Ǿ���
	//  uint32	p_id: �������� id
	//  float	posX, posY: �������� ��ġ
	struct SC_ITEM_CREATE_PACKET : public BASE_PACKET
	{
		uint32_t i_id;
		float posX, posY;

		SC_ITEM_CREATE_PACKET(uint32_t item_id, float x, float y)
			: BASE_PACKET(sizeof(SC_ITEM_CREATE_PACKET), PacketID::SC_ITEM_CREATE)
			, i_id(item_id)
			, posX(x)
			, posY(y) {}
		static SC_ITEM_CREATE_PACKET MakePacket(uint32_t item_id, float x, float y);
		void ntohByteOrder();
	};

	// server to client �ش� �������� �����Ǿ���
	//  uint32	p_id: �������� id
	struct SC_ITEM_REMOVE_PACKET : public BASE_PACKET
	{
		uint32_t i_id;

		SC_ITEM_REMOVE_PACKET(uint32_t item_id)
			: BASE_PACKET(sizeof(SC_ITEM_REMOVE_PACKET), PacketID::SC_ITEM_REMOVE)
			, i_id(item_id) {}
		static SC_ITEM_REMOVE_PACKET MakePacket(uint32_t item_id);
		void ntohByteOrder();
	};

	// server to client �ش� �÷��̾ �ش� ������ ����Ǿ���
	//  uint32	p_id: �÷��̾��� id
	//  uint32	g_id: ���� id
	struct SC_GUN_UPDATE_PACKET : public BASE_PACKET
	{
		uint32_t p_id;
		uint32_t g_id;

		SC_GUN_UPDATE_PACKET(uint32_t player_id, uint32_t gun_id)
			: BASE_PACKET(sizeof(SC_GUN_UPDATE_PACKET), PacketID::SC_GUN_UPDATE)
			, p_id(player_id)
			, g_id(gun_id) {}
		static SC_GUN_UPDATE_PACKET MakePacket(uint32_t player_id, uint32_t gun_id);
		void ntohByteOrder();
	};

	// server to client �ش� �÷��̾��� ����� �����ߴ�
	//  uint32	p_id: �÷��̾��� id
	struct SC_LIFE_UPDATE_PACKET : public BASE_PACKET
	{
		uint32_t p_id;

		SC_LIFE_UPDATE_PACKET(uint32_t player_id)
			: BASE_PACKET(sizeof(SC_LIFE_UPDATE_PACKET), PacketID::SC_LIFE_UPDATE)
			, p_id(player_id) {}
		static SC_LIFE_UPDATE_PACKET MakePacket(uint32_t player_id);
		void ntohByteOrder();
	};

	// server to client ������ ����Ǿ����� �˸�
	struct SC_GAMEOVER_PACKET : public BASE_PACKET
	{
		SC_GAMEOVER_PACKET()
			: BASE_PACKET(sizeof(SC_GAMEOVER_PACKET), PacketID::SC_GAMEOVER) {}
		static SC_GAMEOVER_PACKET MakePacket();
		void ntohByteOrder();
	};

#pragma pack(pop)

}
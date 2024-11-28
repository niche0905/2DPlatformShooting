#pragma once

#include <cstdint>
#include <chrono>

// 라이브러리화 로 사용할 예정

namespace myNP {

	//---------------------------------------------------------
	// 상수들
	//---------------------------------------------------------

	enum PacketID : uint8_t
	{
		// 0은 문제가 생기는 것을 확인하기 위해 비워둠
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

		END	// 이 사이의 패킷 ID여야 함
	};

	constexpr int MaxPacketSize{ 32 };		// 패킷 최대 사이즈 (버퍼 사이즈를 정하는)

	constexpr int SyncTime{ 30 };			// 동기화 시간 1초에 30번 (1/30 초)

	constexpr float BulletWidth{ 20.0f };	// 총알 넓이
	constexpr float BulletHeight{ 6.0f };	// 총알 크기

	constexpr float PlayerWidth{ 50.0f };	// 플레이어 넓이
	constexpr float PlayerHeight{ 50.0f };	// 플레이어 넓이

	constexpr float ItemWidth{ 35.0f };
	constexpr float ItemHeight{ 50.0f };

	constexpr float GravityAcc = 9.810f;    // 중력 가속도
	constexpr float GravityMul = 120.0f;    // 중력 가속도에 곱하는 수(게임성을 위해)
	constexpr float PlatformUp = 10.0f;     // 플랫폼 위에 있다는 기준(충돌 처리 시 사용)

	constexpr float DeadZone = 1000.0f;		// 사망 판정 y값

	constexpr float microToSecond{ 1000000.0f };	// Update에서 사용하기 위한 나누는 상수


	//---------------------------------------------------------
	// 패킷들
	//---------------------------------------------------------

#pragma pack(push, 1)

// 패킷 헤더

	// BASE_PACKET
	//  size: 패킷의 사이즈
	//  id: 패킷의 id
	struct BASE_PACKET
	{
		uint8_t size;
		PacketID id;

		BASE_PACKET(uint8_t packet_size, PacketID packet_id) : size(packet_size), id(packet_id) {}
		BASE_PACKET() : size(2), id(static_cast<PacketID>(0)) {}
		
		void ntohByteOrder();
	};

	// client to server 매치메이킹 잡는다
	struct CS_MATCHMAKING_PACKET : public BASE_PACKET
	{
		CS_MATCHMAKING_PACKET() 
			: BASE_PACKET(sizeof(CS_MATCHMAKING_PACKET), PacketID::CS_MATCHMAKING) {}
		static CS_MATCHMAKING_PACKET MakePacket();
		void ntohByteOrder();
	};

	// client to server 플레이어가 해당 위치로 이동했다
	//  uint32	p_id: 플레이어 id
	//  float	posX, posY: 플레어의 위치
	//  bool	dir: 플레이어의 방향
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

	// client to server 플레이어가 해당 위치에서 총알을 발사 하였다
	//  uint32	b_id: 총알의 id
	//  float	posX, posY: 총알의 위치
	//  bool	dir: 총알의 방향
	//  uint32	type: 
	//  microseconds	fire_t: 발사 시간
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

	// server to client 매치메이킹 잡혔다
	//  bool	succ: 성공 여부
	//  uint32	p_id: 플레이어 ID
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

	// server to client 플레이어가 이쪽으로 이동했다
	//  uint32	p_id: 플레이어 id
	//  float	posX, posY: 플레어의 위치
	//  bool	dir: 플레이어의 방향
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

	// server to client 플레이어가 데미지를 입었다
	//  float	damage: 입은 데미지
	struct SC_PLAYER_DAMAGE_PACKET : public BASE_PACKET
	{
		float damage;

		SC_PLAYER_DAMAGE_PACKET(float damaged)
			: BASE_PACKET(sizeof(SC_PLAYER_DAMAGE_PACKET), PacketID::SC_PLAYER_DAMAGE)
			, damage(damaged) {}
		static SC_PLAYER_DAMAGE_PACKET MakePacket(float damaged);
		void ntohByteOrder();
	};

	// server to client 플레이어가 총을 쏜 사실을 알림
	//  uint32	b_id: 총알의 id
	//  float	posX, posY: 총알의 위치
	//  bool	dir: 총알의 방향
	//  uint32	type: 총알의 타입
	//  microseconds	fire_t: 발사 시간
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

	// server to client 해당 총알이 사라졌음을 알리는 패킷
	//  uint32	p_id: 플레이어의 id
	//  uint32	b_id: 사라진 총알의 id
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

	// server to client 아이템 해당 위치에 생성 되었다
	//  uint32	p_id: 아이템의 id
	//  float	posX, posY: 아이템의 위치
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

	// server to client 해당 아이템이 삭제되었다
	//  uint32	p_id: 아이템의 id
	struct SC_ITEM_REMOVE_PACKET : public BASE_PACKET
	{
		uint32_t i_id;

		SC_ITEM_REMOVE_PACKET(uint32_t item_id)
			: BASE_PACKET(sizeof(SC_ITEM_REMOVE_PACKET), PacketID::SC_ITEM_REMOVE)
			, i_id(item_id) {}
		static SC_ITEM_REMOVE_PACKET MakePacket(uint32_t item_id);
		void ntohByteOrder();
	};

	// server to client 해당 플레이어가 해당 총으로 변경되었다
	//  uint32	p_id: 플레이어의 id
	//  uint32	g_id: 총의 id
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

	// server to client 해당 플레이어의 목숨이 감소했다
	//  uint32	p_id: 플레이어의 id
	struct SC_LIFE_UPDATE_PACKET : public BASE_PACKET
	{
		uint32_t p_id;

		SC_LIFE_UPDATE_PACKET(uint32_t player_id)
			: BASE_PACKET(sizeof(SC_LIFE_UPDATE_PACKET), PacketID::SC_LIFE_UPDATE)
			, p_id(player_id) {}
		static SC_LIFE_UPDATE_PACKET MakePacket(uint32_t player_id);
		void ntohByteOrder();
	};

	// server to client 게임이 종료되었음을 알림
	struct SC_GAMEOVER_PACKET : public BASE_PACKET
	{
		SC_GAMEOVER_PACKET()
			: BASE_PACKET(sizeof(SC_GAMEOVER_PACKET), PacketID::SC_GAMEOVER) {}
		static SC_GAMEOVER_PACKET MakePacket();
		void ntohByteOrder();
	};

#pragma pack(pop)

}
#include <iostream>
#include <chrono>

// .h 파일을 복사하는 것이 아닌 다른 패킷 (프로토콜) 공유 방법 찾아볼 예정

enum PacketID
{
	// 0은 문제가 생기는 것을 확인하기 위해 비워둠
	CS_MATCHMAKING = 1,
	CS_MOVE,
	CS_FIRE,
	SC_MATCHMAKING,
	SC_MOVE,
	SC_PLAYER_DAMAGE,
	SC_FIRE,
	SC_BULLET_REMOVE,
	SC_ITEM_CREATE,
	SC_ITEM_REMOVE,
	SC_GUN_UPDATE,
	SC_LIVE_UPDATE,
	SC_GAMEOVER
	// 필요 시 보고서에 반영하고 추가하기
};

constexpr int MaxPacketSize{ 32 };		// 패킷 최대 사이즈 (버퍼 사이즈를 정하는)

constexpr int SyncTime{ 30 };			// 동기화 시간 1초에 30번 (1/30 초)

constexpr float BulletWidth{ 20.0f };	// 총알 넓이
constexpr float BulletHeight{ 6.0f };	// 총알 크기

constexpr float PlayerWidth{ 50.0f };	// 플레이어 넓이
constexpr float PlayerHeight{ 50.0f };	// 플레이어 넓이

constexpr float GravityAcc = 9.810f;    // 중력 가속도
constexpr float GravityMul = 120.0f;    // 중력 가속도에 곱하는 수(게임성을 위해)
constexpr float PlatformUp = 10.0f;     // 플랫폼 위에 있다는 기준(충돌 처리 시 사용)


//---------------------------------------------------------
// 패킷들
//---------------------------------------------------------

// 패킷 헤더
struct BASE_PACKET
{
	uint8_t size;
	uint8_t id;
};

// client to server 매치메이킹 잡는다
struct CS_MATCHMAKING_PACKET : public BASE_PACKET
{

};

// client to server 플레이어가 해당 위치로 이동했다
struct CS_MOVE_PACKET : public BASE_PACKET
{
	uint32_t p_id;
	float posX, posY;
	bool dir;
};

// client to server 플레이어가 해당 위치에서 총알을 발사 하였다
struct CS_FIRE_PACKET : public BASE_PACKET
{
	uint32_t b_id;
	float posX, posY;
	bool dir;
	uint32_t type;
	std::chrono::milliseconds fire_t;
};

// server to client 매치메이킹 잡혔다
struct SC_MATCHMAKING : public BASE_PACKET
{
	bool succ;
};

// server to client 플레이어가 이쪽으로 이동했다
struct SC_MOVE_PAKCET : public BASE_PACKET
{
	uint32_t p_id;
	float posX, posY;
	bool dir;
};

// server to client 플레이어가 데미지를 입었다
struct SC_PLAYER_DAMAGE_PACKET : public BASE_PACKET
{
	float damage;
};

// server to client 플레이어가 총을 쏜 사실을 알림
struct SC_FIRE_PACKET : public BASE_PACKET
{
	uint32_t b_id;
	float posX, posY;
	bool dir;
	uint32_t type;
	std::chrono::milliseconds fire_t;
};

// server to client 해당 총알이 사라졌음을 알리는 패킷
struct SC_BULLET_REMOVE_PACKET : public BASE_PACKET
{
	uint32_t p_id;
	uint32_t b_id;
};

// server to client 아이템 해당 위치에 생성 되었다
struct SC_ITEM_CREATE_PACKET : public BASE_PACKET
{
	uint32_t i_id;
	float posX, posY;
};

// server to client 해당 아이템이 삭제되었다
struct SC_ITEM_REMOVE_PACKET : public BASE_PACKET
{
	uint32_t i_id;
};

// server to client 해당 플레이어가 해당 총으로 변경되었다
struct SC_GUN_UPDATE_PACKET : public BASE_PACKET
{
	uint32_t p_id;
	uint32_t g_id;
};

// server to client 해당 플레이어의 목숨이 감소했다
struct SC_LIFE_UPDATE_PACKET : public BASE_PACKET
{
	uint32_t p_id;
};

// server to client 게임이 종료되었음을 알림
struct SC_GAMEOVER_PACKET : public BASE_PACKET
{
	
};

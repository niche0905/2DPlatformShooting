
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

constexpr int SyncTime{ 30 };			// 동기화 시간 1초에 30번 (1/30 초)

constexpr float BulletWidth{ 20.0f };	// 총알 넓이
constexpr float BulletHeight{ 6.0f };	// 총알 크기

constexpr float PlayerWidth{ 50.0f };	// 플레이어 넓이
constexpr float PlayerHeight{ 50.0f };	// 플레이어 넓이

constexpr float GravityAcc = 9.810f;    // 중력 가속도
constexpr float GravityMul = 120.0f;    // 중력 가속도에 곱하는 수(게임성을 위해)
constexpr float PlatformUp = 10.0f;     // 플랫폼 위에 있다는 기준(충돌 처리 시 사용)

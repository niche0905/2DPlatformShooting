#include <iostream>
#include <chrono>

// .h ������ �����ϴ� ���� �ƴ� �ٸ� ��Ŷ (��������) ���� ��� ã�ƺ� ����

enum PacketID
{
	// 0�� ������ ����� ���� Ȯ���ϱ� ���� �����
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
	// �ʿ� �� ������ �ݿ��ϰ� �߰��ϱ�
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

// ��Ŷ ���
struct BASE_PACKET
{
	uint8_t size;
	uint8_t id;
};

// client to server ��ġ����ŷ ��´�
struct CS_MATCHMAKING_PACKET : public BASE_PACKET
{

};

// client to server �÷��̾ �ش� ��ġ�� �̵��ߴ�
struct CS_MOVE_PACKET : public BASE_PACKET
{
	uint32_t p_id;
	float posX, posY;
	bool dir;
};

// client to server �÷��̾ �ش� ��ġ���� �Ѿ��� �߻� �Ͽ���
struct CS_FIRE_PACKET : public BASE_PACKET
{
	uint32_t b_id;
	float posX, posY;
	bool dir;
	uint32_t type;
	std::chrono::milliseconds fire_t;
};

// server to client ��ġ����ŷ ������
struct SC_MATCHMAKING : public BASE_PACKET
{
	bool succ;
};

// server to client �÷��̾ �������� �̵��ߴ�
struct SC_MOVE_PAKCET : public BASE_PACKET
{
	uint32_t p_id;
	float posX, posY;
	bool dir;
};

// server to client �÷��̾ �������� �Ծ���
struct SC_PLAYER_DAMAGE_PACKET : public BASE_PACKET
{
	float damage;
};

// server to client �÷��̾ ���� �� ����� �˸�
struct SC_FIRE_PACKET : public BASE_PACKET
{
	uint32_t b_id;
	float posX, posY;
	bool dir;
	uint32_t type;
	std::chrono::milliseconds fire_t;
};

// server to client �ش� �Ѿ��� ��������� �˸��� ��Ŷ
struct SC_BULLET_REMOVE_PACKET : public BASE_PACKET
{
	uint32_t p_id;
	uint32_t b_id;
};

// server to client ������ �ش� ��ġ�� ���� �Ǿ���
struct SC_ITEM_CREATE_PACKET : public BASE_PACKET
{
	uint32_t i_id;
	float posX, posY;
};

// server to client �ش� �������� �����Ǿ���
struct SC_ITEM_REMOVE_PACKET : public BASE_PACKET
{
	uint32_t i_id;
};

// server to client �ش� �÷��̾ �ش� ������ ����Ǿ���
struct SC_GUN_UPDATE_PACKET : public BASE_PACKET
{
	uint32_t p_id;
	uint32_t g_id;
};

// server to client �ش� �÷��̾��� ����� �����ߴ�
struct SC_LIFE_UPDATE_PACKET : public BASE_PACKET
{
	uint32_t p_id;
};

// server to client ������ ����Ǿ����� �˸�
struct SC_GAMEOVER_PACKET : public BASE_PACKET
{
	
};

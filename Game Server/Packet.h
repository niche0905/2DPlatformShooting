
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

constexpr int SyncTime{ 30 };			// ����ȭ �ð� 1�ʿ� 30�� (1/30 ��)

constexpr float BulletWidth{ 20.0f };	// �Ѿ� ����
constexpr float BulletHeight{ 6.0f };	// �Ѿ� ũ��

constexpr float PlayerWidth{ 50.0f };	// �÷��̾� ����
constexpr float PlayerHeight{ 50.0f };	// �÷��̾� ����

constexpr float GravityAcc = 9.810f;    // �߷� ���ӵ�
constexpr float GravityMul = 120.0f;    // �߷� ���ӵ��� ���ϴ� ��(���Ӽ��� ����)
constexpr float PlatformUp = 10.0f;     // �÷��� ���� �ִٴ� ����(�浹 ó�� �� ���)

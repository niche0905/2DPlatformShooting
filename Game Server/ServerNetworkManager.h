#pragma once


using BufferType = std::array<int, myNP::MaxPacketSize>;
using PacketType = BufferType;
using QueueType = std::queue<BufferType>;

class ServerNetworkManager
{
public:

	// SOCKET socket{NULL}; // ����
	// HANDLE thread{}; // ������
	std::vector<QueueType> processQueue; // ������ ���� ť ����
	std::array<HANDLE, 2> recvEvent{};
	std::array<HANDLE, 2> processEvent{}; // ������ ����ȭ�� ���� �̺�Ʈ

	// �߰��� ����
	SOCKET listenSocket{ NULL }; // ����
	HANDLE updateThread{};
	HANDLE lobbyThread{};

public:
	ServerNetworkManager();
	~ServerNetworkManager();

	void Init();
	void CreateLobbyThread(); // �κ� ������ ����
	void CreateUpdateThread(); // update ������ ����
	void CreateRecvThread(SOCKET socket); // recv ������ ����
	void PushBuffer(BufferType buffer); // ���� Push
	QueueType& GetQueue();
	void SendPacket(PacketType packet);

	// �߰��� �Լ�
	void NetworkInit();
	void AcceptAndRecv();

};

// worker

// ���� �ð� ��ŭ ��ü�� ���¸� update(����: �̺�Ʈ �ڵ� update)
DWORD WINAPI workerUpdate(LPVOID arg);

// ��Ŷ�� �޾� queue�� ��� �� ���� ����(����: �̺�Ʈ �ڵ� recv)
DWORD WINAPI workerRecv(LPVOID arg);

// ��ġ����ŷ �˻�(����: ����)
DWORD WINAPI workerLobby(LPVOID arg);
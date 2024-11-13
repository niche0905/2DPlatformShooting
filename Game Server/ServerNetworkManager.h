#pragma once


using BufferType = std::array<char, myNP::MaxPacketSize>;
using PacketType = BufferType;
using QueueType  = std::queue<BufferType>;
using PacketSize = uint8;

class ServerNetworkManager
{
private:

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
	void CreateRecvThread(SOCKET sock) const; // recv ������ ����
	void SendPacket(PacketType packet);

	// �߰��� �Լ�
	// Brief: ��Ʈ��ũ�� �ʱ�ȭ ���ش�.
	void NetworkInit();
	
	// Brief: listen ���Ͽ� accept�� ȣ���ϰ� ���� �������� recv �����带 �����.
	void Accept();

	// Brief:	Recv thread���� ȣ���ϴ� ����, ���� ���� recv
	// Param:	Recv �� ���� buffer�� ����
	// Return:	���� ���� ���� (true: ����, false: ����)
	static bool doRecv(SOCKET sock, BufferType& buffer);

	// ������ �Լ�
	// void PushBuffer(BufferType buffer);
	// QueueType& GetQueue();
};

// worker

// ���� �ð� ��ŭ ��ü�� ���¸� update(����: �̺�Ʈ �ڵ� update)
DWORD WINAPI workerUpdate(LPVOID arg);

// ��Ŷ�� �޾� queue�� ��� �� ���� ����(����: �̺�Ʈ �ڵ� recv)
DWORD WINAPI workerRecv(LPVOID arg);

// ��ġ����ŷ �˻�(����: ����)
DWORD WINAPI workerLobby(LPVOID arg);
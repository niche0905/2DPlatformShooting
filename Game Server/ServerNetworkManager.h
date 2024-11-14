#pragma once


using BufferType = std::array<char, myNP::MaxPacketSize>;
using QueueType  = std::queue<BufferType>;
using PacketSize = uint8;

class ServerNetworkManager
{
private:

	std::vector<QueueType> processQueue; // ������ ���� ť ����
	std::array<HANDLE, 2> recvEvent{};
	std::array<HANDLE, 2> processEvent{}; // ������ ����ȭ�� ���� �̺�Ʈ

	// �߰��� ����
	SOCKET listenSocket{ NULL }; // ����
	HANDLE updateThread{};
	HANDLE lobbyThread{};

	// ������ ����
	// SOCKET socket{NULL}; // ����
	// HANDLE thread{}; // ������

public:
	ServerNetworkManager();
	~ServerNetworkManager();

	void Init();
	void CreateLobbyThread(); // �κ� ������ ����
	void CreateUpdateThread(); // update ������ ����
	void CreateRecvThread(SOCKET sock) const; // recv ������ ����

	// �߰��� �Լ�

	// Brief: ��Ʈ��ũ�� �ʱ�ȭ ���ش�.
	void NetworkInit();
	
	// Brief: listen ���Ͽ� accept�� ȣ���ϰ� ���� �������� recv �����带 �����.
	void Accept();

	// Brief
	//  Recv thread���� ȣ���ϴ� ����, ���� ���� recv
	// Param
	//  sock: Ŭ���̾�Ʈ ����
	//  buffer: Recv�� �޾ƿ� ������ ������ ����
	// Return
	//  ���� ���� ���� (true: ����, false: ����)
	static bool doRecv(SOCKET sock, BufferType& buffer);


	// Brief
	//  sock�� buffer�� ������ ������.
	// Param
	//  sock: ���� client�� socket
	//  buffer: ���� ��Ŷ�� ������ ��� �ִ� buffer
	// Return
	//  ���� ���� ���� (true: ����, false: ����)
	static bool doSend(SOCKET sock, const BufferType& buffer);

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
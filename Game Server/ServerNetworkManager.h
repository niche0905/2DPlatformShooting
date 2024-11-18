#pragma once


using BufferType		= std::array<char, myNP::MaxPacketSize>;
using QueueType			= std::queue<BufferType>;
using PacketSizeType	= uint8_t;
using namespace myNP;


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


private:
	// private �Լ�

	// Brief: sock�� buffer�� ������ ������.
	// Return: ���� ���� ���� (true: ����, false: ����)
	// Params:
	//  sock: ���� client�� socket
	//  buffer: ���� ��Ŷ�� ������ ��� �ִ� buffer
	static bool doSend(SOCKET sock, const BufferType& buffer);


	// Brief
	//  sock�� buffer�� ������ ������.
	// Param
	//  sock: ���� client�� socket
	//  packet: ��Ŷ ��ü.
	// Return
	//  ���� ���� ���� (true: ����, false: ����)
	template <class _Packet>
	static bool doSend(SOCKET sock, _Packet packet);


public:
	ServerNetworkManager();
	~ServerNetworkManager();

	void Init();
	void CreateLobbyThread(); // �κ� ������ ����
	void CreateUpdateThread(); // update ������ ����
	void CreateRecvThread(SOCKET sock) const; // recv ������ ����

	// Brief: _Packet Ŭ������ ��Ŷ�� ����� �־� ������.
	// template class: _Packet: ��Ŷ�� ���� Ŭ���� 
	// Params:
	//  sock: ���� Ŭ���̾�Ʈ�� ����
	//  Args...: ��Ŷ�� ���� �� ���ڷ� ���� ���ڵ�
	template <class _Packet, class ...Args>
	static void SendPacket(SOCKET sock, Args... args);

	// �߰��� �Լ�

	// Brief: ��Ʈ��ũ�� �ʱ�ȭ ���ش�.
	void NetworkInit();
	
	// Brief: listen ���Ͽ� accept�� ȣ���ϰ� ���� �������� recv �����带 �����.
	void Accept();

	// Brief: Recv thread���� ȣ���ϴ� ����, ���� ���� recv
	// Return: ���� ���� ���� (true: ����, false: ����)
	// Params:
	//  sock: Ŭ���̾�Ʈ ����
	//  buffer: Recv�� �޾ƿ� ������ ������ ����
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


template<class _Packet, class ...Args>
inline void ServerNetworkManager::SendPacket(SOCKET sock, Args ...args)
{
	auto packet{ _Packet::MakePacket(std::forward<Args>(args)...)};
	doSend(sock, packet);
}

template<class _Packet>
inline bool ServerNetworkManager::doSend(SOCKET sock, _Packet packet)
{
	BufferType buf{};
	::memcpy_s(
		buf.data(),
		buf.size(),
		reinterpret_cast<const char*>(&packet),
		sizeof(_Packet)
	);

	return doSend(sock, buf);
}

#pragma once


using BufferType		= std::array<char, myNP::MaxPacketSize>;
using QueueType			= std::queue<BufferType>;
using PacketSizeType	= uint8_t;
using namespace myNP;


class ServerNetworkManager
{
private:

	std::vector<QueueType>	processQueue; // ������ ���� ť ����
	std::array<HANDLE, 2>	recvEvent{};
	std::array<HANDLE, 2>	processEvent{}; // ������ ����ȭ�� ���� �̺�Ʈ

	
	// �߰��� ����
	SOCKET listenSocket{ NULL }; // ����
	HANDLE updateThread{};
	HANDLE lobbyThread{};

	std::queue<int>	matchmakingList{};
	int				nextId{ 0 };
	bool			playing{ false };


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
	bool doSend(SOCKET sock, const BufferType& buffer) const;


	// Brief
	//  sock�� buffer�� ������ ������.
	// Param
	//  sock: ���� client�� socket
	//  packet: ��Ŷ ��ü.
	// Return
	//  ���� ���� ���� (true: ����, false: ����)
	template <class _Packet>
	bool doSend(SOCKET sock, _Packet packet) const;


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
	void SendPacket(SOCKET sock, Args... args) const;

	// �߰��� �Լ�

	// Brief: ��Ʈ��ũ�� �ʱ�ȭ ���ش�.
	void NetworkInit();

	void EventInit();
	
	// Brief: listen ���Ͽ� accept�� ȣ���ϰ� ���� �������� recv �����带 �����.
	void Accept();

	// Brief: Recv thread���� ȣ���ϴ� ����, ���� ���� recv
	// Return: ���� ���� ���� (true: ����, false: ����)
	// Params:
	//  sock: Ŭ���̾�Ʈ ����
	//  buffer: Recv�� �޾ƿ� ������ ������ ����
	bool DoRecv(SOCKET sock, BufferType& buffer) const;

	// getter and setter
	int GetNextId() { return nextId++; }
	bool IsPlaying() const { return playing; }
	void DecreaseNextId() { nextId--; }

	// handle recv events
	void SetRecvEvent(const int c_id) { SetEvent(recvEvent[c_id]); }
	void WaitforRecvEvent() { WaitForMultipleObjects(2, recvEvent.data(), TRUE, INFINITE); }

	// handle process events
	void SetProcessEvent() { SetEvent(processEvent[0]); SetEvent(processEvent[1]); }
	void WaitforProcessEvent(const int c_id) { WaitForSingleObject(recvEvent[c_id], INFINITE); }
	


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
inline void ServerNetworkManager::SendPacket(SOCKET sock, Args ...args) const
{
	auto packet{ _Packet::MakePacket(std::forward<Args>(args)...)};
	doSend(sock, packet);
}

template<class _Packet>
inline bool ServerNetworkManager::doSend(SOCKET sock, _Packet packet) const
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

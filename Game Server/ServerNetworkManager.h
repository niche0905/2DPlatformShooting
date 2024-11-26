#pragma once


using BufferType		= std::array<char, myNP::MaxPacketSize>;
using QueueType			= std::queue<BufferType>;
using PacketSizeType	= uint8_t;
using namespace myNP;


class ServerNetworkManager
{
private:

	std::array<QueueType, 2>	processQueue{}; // ������ ���� ť ����
	std::array<HANDLE, 2>		recvEvent{};
	std::array<HANDLE, 2>		processEvent{}; // ������ ����ȭ�� ���� �̺�Ʈ
	std::array<SOCKET, 2>		socketArr{};
		
	// �߰��� ����
	SOCKET listenSocket{ NULL }; // ����
	HANDLE updateThread{};
	HANDLE lobbyThread{};

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

	template <class _Packet, class ...Args>
	void SendPacket(const int client_id, Args... args) const
	{
		SendPacket<_Packet>(socketArr[client_id], std::forward<Args>(args)...);
	}

	// �߰��� �Լ�

	// ��Ʈ��ũ�� �ʱ�ȭ�Ѵ�.
	void NetworkInit();

	// �̺�Ʈ�� �ʱ�ȭ�Ѵ�.
	void EventInit();
	
	// listen ���Ͽ� accept�� ȣ���ϰ� ���� �������� recv �����带 �����.
	void Accept();

	// Brief: Recv thread���� ȣ���ϴ� ����, ���� ���� recv
	// Return: ���� ���� ���� (true: ����, false: ����)
	// Params:
	//  sock: Ŭ���̾�Ʈ ����
	//  buffer: Recv�� �޾ƿ� ������ ������ ����
	bool DoRecv(SOCKET sock, BufferType& buffer) const;

	// ť�� �ִ� ������ ���������� ó���Ѵ�.
	void ProcessPackets();


	// getter and setter
	int GetNextId() { return nextId++ % 2; }
	void DecreaseNextID() { --nextId; };
	bool IsPlaying() const { return playing; }
	void setPlaying(const bool value) { playing = value; }
	void setProcessQueue(const QueueType& queue_, const int client_id) {
		processQueue[client_id] = queue_;
	}
	void setSocketArr(SOCKET sock, const int client_id) {
		socketArr[client_id] = sock;
	}


	// handle recv events
	void SetRecvEvent(const int c_id) { SetEvent(recvEvent[c_id]); }
	void WaitforRecvEvent() { WaitForMultipleObjects(2, recvEvent.data(), TRUE, INFINITE); }

	// handle process events
	void SetProcessEvent() { SetEvent(processEvent[0]); SetEvent(processEvent[1]); }
	void WaitforProcessEvent(const int c_id) { WaitForSingleObject(processEvent[c_id], INFINITE); }
	


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

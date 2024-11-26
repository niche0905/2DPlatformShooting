#pragma once


using BufferType		= std::array<char, myNP::MaxPacketSize>;
using QueueType			= std::queue<BufferType>;
using PacketSizeType	= uint8_t;
using namespace myNP;


class ServerNetworkManager
{
private:

	std::array<QueueType, 2>	processQueue{}; // 스레드 전달 큐 벡터
	std::array<HANDLE, 2>		recvEvent{};
	std::array<HANDLE, 2>		processEvent{}; // 스레드 동기화를 위한 이벤트
	std::array<SOCKET, 2>		socketArr{};
		
	// 추가된 변수
	SOCKET listenSocket{ NULL }; // 소켓
	HANDLE updateThread{};
	HANDLE lobbyThread{};

	int				nextId{ 0 };
	bool			playing{ false };


	// 삭제된 변수
	// SOCKET socket{NULL}; // 소켓
	// HANDLE thread{}; // 스레드


private:
	// private 함수

	// Brief: sock에 buffer의 내용을 보낸다.
	// Return: 정상 종료 여부 (true: 정상, false: 오류)
	// Params:
	//  sock: 보낼 client의 socket
	//  buffer: 보낼 패킷의 내용이 담겨 있는 buffer
	bool doSend(SOCKET sock, const BufferType& buffer) const;


	// Brief
	//  sock에 buffer의 내용을 보낸다.
	// Param
	//  sock: 보낼 client의 socket
	//  packet: 패킷 자체.
	// Return
	//  정상 종료 여부 (true: 정상, false: 오류)
	template <class _Packet>
	bool doSend(SOCKET sock, _Packet packet) const;


public:
	ServerNetworkManager();
	~ServerNetworkManager();

	void Init();
	void CreateLobbyThread(); // 로비 스레드 생성
	void CreateUpdateThread(); // update 스레드 생성
	void CreateRecvThread(SOCKET sock) const; // recv 스레드 생성

	// Brief: _Packet 클래스로 패킷을 만들어 넣어 보낸다.
	// template class: _Packet: 패킷을 보낼 클래스 
	// Params:
	//  sock: 보낼 클라이언트의 소켓
	//  Args...: 패킷을 만들 때 인자로 넣을 인자들
	template <class _Packet, class ...Args>
	void SendPacket(SOCKET sock, Args... args) const;

	template <class _Packet, class ...Args>
	void SendPacket(const int client_id, Args... args) const
	{
		SendPacket<_Packet>(socketArr[client_id], std::forward<Args>(args)...);
	}

	// 추가된 함수

	// 네트워크를 초기화한다.
	void NetworkInit();

	// 이벤트를 초기화한다.
	void EventInit();
	
	// listen 소켓에 accept를 호출하고 받은 소켓으로 recv 쓰레드를 만든다.
	void Accept();

	// Brief: Recv thread에서 호출하는 고정, 가변 길이 recv
	// Return: 정상 종료 여부 (true: 정상, false: 오류)
	// Params:
	//  sock: 클라이언트 소켓
	//  buffer: Recv로 받아온 내용을 저장할 버퍼
	bool DoRecv(SOCKET sock, BufferType& buffer) const;

	// 큐에 있는 내용을 실질적으로 처리한다.
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
	


	// 삭제된 함수
	// void PushBuffer(BufferType buffer);
	// QueueType& GetQueue();
};

// worker

// 지난 시간 만큼 객체의 상태를 update(인자: 이벤트 핸들 update)
DWORD WINAPI workerUpdate(LPVOID arg);

// 패킷을 받아 queue에 등록 후 정보 전송(인자: 이벤트 핸들 recv)
DWORD WINAPI workerRecv(LPVOID arg);

// 매치메이킹 검사(인자: 없음)
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

#pragma once


using BufferType = std::array<int, myNP::MaxPacketSize>;
using PacketType = BufferType;
using QueueType = std::queue<BufferType>;

class ServerNetworkManager
{
public:

	// SOCKET socket{NULL}; // 소켓
	// HANDLE thread{}; // 스레드
	std::vector<QueueType> processQueue; // 스레드 전달 큐 벡터
	std::array<HANDLE, 2> recvEvent{};
	std::array<HANDLE, 2> processEvent{}; // 스레드 동기화를 위한 이벤트

	// 추가된 변수
	SOCKET listenSocket{ NULL }; // 소켓
	HANDLE updateThread{};
	HANDLE lobbyThread{};

public:
	ServerNetworkManager();
	~ServerNetworkManager();

	void Init();
	void CreateLobbyThread(); // 로비 스레드 생성
	void CreateUpdateThread(); // update 스레드 생성
	void CreateRecvThread(SOCKET socket); // recv 스레드 생성
	void PushBuffer(BufferType buffer); // 버퍼 Push
	QueueType& GetQueue();
	void SendPacket(PacketType packet);

	// 추가된 함수
	void NetworkInit();
	void AcceptAndRecv();

};

// worker

// 지난 시간 만큼 객체의 상태를 update(인자: 이벤트 핸들 update)
DWORD WINAPI workerUpdate(LPVOID arg);

// 패킷을 받아 queue에 등록 후 정보 전송(인자: 이벤트 핸들 recv)
DWORD WINAPI workerRecv(LPVOID arg);

// 매치메이킹 검사(인자: 없음)
DWORD WINAPI workerLobby(LPVOID arg);
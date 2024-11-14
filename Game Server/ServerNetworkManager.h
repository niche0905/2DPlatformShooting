#pragma once


using BufferType = std::array<char, myNP::MaxPacketSize>;
using QueueType  = std::queue<BufferType>;
using PacketSize = uint8;

class ServerNetworkManager
{
private:

	std::vector<QueueType> processQueue; // 스레드 전달 큐 벡터
	std::array<HANDLE, 2> recvEvent{};
	std::array<HANDLE, 2> processEvent{}; // 스레드 동기화를 위한 이벤트

	// 추가된 변수
	SOCKET listenSocket{ NULL }; // 소켓
	HANDLE updateThread{};
	HANDLE lobbyThread{};

	// 삭제된 변수
	// SOCKET socket{NULL}; // 소켓
	// HANDLE thread{}; // 스레드

public:
	ServerNetworkManager();
	~ServerNetworkManager();

	void Init();
	void CreateLobbyThread(); // 로비 스레드 생성
	void CreateUpdateThread(); // update 스레드 생성
	void CreateRecvThread(SOCKET sock) const; // recv 스레드 생성

	// 추가된 함수

	// Brief: 네트워크를 초기화 해준다.
	void NetworkInit();
	
	// Brief: listen 소켓에 accept를 호출하고 받은 소켓으로 recv 쓰레드를 만든다.
	void Accept();

	// Brief
	//  Recv thread에서 호출하는 고정, 가변 길이 recv
	// Param
	//  sock: 클라이언트 소켓
	//  buffer: Recv로 받아온 내용을 저장할 버퍼
	// Return
	//  정상 종료 여부 (true: 정상, false: 오류)
	static bool doRecv(SOCKET sock, BufferType& buffer);


	// Brief
	//  sock에 buffer의 내용을 보낸다.
	// Param
	//  sock: 보낼 client의 socket
	//  buffer: 보낼 패킷의 내용이 담겨 있는 buffer
	// Return
	//  정상 종료 여부 (true: 정상, false: 오류)
	static bool doSend(SOCKET sock, const BufferType& buffer);

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
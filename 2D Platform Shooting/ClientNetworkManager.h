#pragma once
#include "Utilities.h"

class ClientNetworkManager {
private:
	// 소켓
	SOCKET clientSocket;
	// 스레드
	HANDLE clientThread;
	// 스레드 전달 큐
	std::queue<std::array<char, MAX_SIZE>> process_queue;
	// 클라이언트 ID
	int ClientID;
	// 이벤트
	HANDLE recvEvent;
	HANDLE processEvent;

public:
	ClientNetworkManager();
	~ClientNetworkManager();

	void Init();
	void Connect();

	// Recv() 스레드 생성
	void CreateRecvThread();
	// enQueue
	// 서버에서 데이터가 넘어오면 Event 처리 스레드에게 할 일을 넘기는 함수
	void PushBuffer(char buf[MAX_SIZE]);
	// 패킷 전송
	void SendPacket(char buf[MAX_SIZE]);

	HANDLE GetRecvEvent() { return recvEvent; }
	HANDLE GetProcessEvent() { return processEvent; }

	void Update();
};


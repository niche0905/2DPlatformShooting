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
	void PushBuffer(char buf[MAX_SIZE]);
	// 패킷 전송
	void SendPacket(char buf[MAX_SIZE]);

	void PushBuffer();
	void Update();
	void Draw();
};


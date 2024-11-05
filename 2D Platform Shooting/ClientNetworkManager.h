#pragma once
#include "Utilities.h"

class ClientNetworkManager {
private:
	HANDLE socket;
	HANDLE thread;
	std::queue<std::array<char, MAX_SIZE>> process_queue;
	int ClientID;
	HANDLE recvEvent;
	HANDLE processEvent;

public:
	ClientNetworkManager() {}
	~ClientNetworkManager() {}

	void Connect();
	void CreateRecvThread();
	void PushBuffer(char buf[MAX_SIZE]);
	void SendPacket(char buf[MAX_SIZE]);
	void PushBuffer();
	void Update();
	void Draw();
};


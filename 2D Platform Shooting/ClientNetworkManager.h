#pragma once
#include "Utilities.h"

class ClientNetworkManager {
private:
	// ����
	HANDLE socket;
	// ������
	HANDLE thread;
	// ������ ���� ť
	std::queue<std::array<char, MAX_SIZE>> process_queue;
	// Ŭ���̾�Ʈ ID
	int ClientID;
	// �̺�Ʈ
	HANDLE recvEvent;
	HANDLE processEvent;

public:
	ClientNetworkManager();
	~ClientNetworkManager();

	void Init();
	void Connect();

	// Recv() ������ ����
	void CreateRecvThread();
	// enQueue
	void PushBuffer(char buf[MAX_SIZE]);
	// ��Ŷ ����
	void SendPacket(char buf[MAX_SIZE]);

	void PushBuffer();
	void Update();
	void Draw();
};


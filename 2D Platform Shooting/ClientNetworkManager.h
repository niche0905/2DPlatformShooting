#pragma once
#include "Utilities.h"

class ClientNetworkManager {
private:
	// ����
	SOCKET clientSocket;
	// ������
	HANDLE clientThread;
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
	// �������� �����Ͱ� �Ѿ���� Event ó�� �����忡�� �� ���� �ѱ�� �Լ�
	void PushBuffer(char buf[MAX_SIZE]);
	// ��Ŷ ����
	void SendPacket(char buf[MAX_SIZE]);

	HANDLE GetRecvEvent() { return recvEvent; }
	HANDLE GetProcessEvent() { return processEvent; }

	void Update();
};


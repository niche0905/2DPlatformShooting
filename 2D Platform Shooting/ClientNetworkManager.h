#pragma once
#include "Utilities.h"
#include "Scene.h"
#include "SceneManager.h"

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

	// ���� ��
	std::shared_ptr<Scene> currentScene;

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
	void SendPacket(char* buf, uint8_t packet_id);

	HANDLE GetRecvEvent() { return recvEvent; }
	HANDLE GetProcessEvent() { return processEvent; }
	SOCKET GetSocket() { return clientSocket;  }

	void ProcessPacket();
	void ProcessPlayerMove(myNP::CS_MOVE_PACKET* move_packet);
	void ProcessMatchMaking(myNP::CS_MATCHMAKING_PACKET* matchmaking_packet);
	void ProcessFirebullet(myNP::CS_FIRE_PACKET* fire_packet);

	void Update();
};


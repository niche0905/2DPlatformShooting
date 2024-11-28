#pragma once
#include "Utilities.h"
#include "Scene.h"
#include "SceneManager.h"

DWORD WINAPI WorkerRecv(LPVOID arg);

class ClientNetworkManager {
private:
	// ����
	SOCKET clientSocket;
	// ������
	HANDLE clientThread;
	// ������ ���� ť
	std::queue<std::array<char, MAX_SIZE>> process_queue;
	// Ŭ���̾�Ʈ ID
	int32_t ClientID{ -1 };
	// �̺�Ʈ
	HANDLE recvEvent;
	HANDLE processEvent;

	// ���� ��
	std::shared_ptr<Scene> currentScene;

public:
	ClientNetworkManager() {};
	~ClientNetworkManager() {};

	void Init();
	void Connect();

	// Recv() ������ ����
	void CreateRecvThread();
	// enQueue
	// �������� �����Ͱ� �Ѿ���� Event ó�� �����忡�� �� ���� �ѱ�� �Լ�
	//void PushBuffer(char buf[MAX_SIZE]);
	// ��Ŷ ����
	void SendPacket(char* buf, uint8_t packet_id);

	void PushBuffer(char buf[MAX_SIZE]);

	HANDLE GetRecvEvent() { return recvEvent; }
	HANDLE GetProcessEvent() { return processEvent; }
	SOCKET GetSocket() { return clientSocket;  }
	int32_t GetClientID() { return ClientID; }

	void ProcessPacket();
	void ProcessDummyMove(myNP::SC_MOVE_PACKET* move_packet);
	void ProcessMatchMaking(myNP::SC_MATCHMAKING_PACKET* matchmaking_packet);
	void ProcessFirebullet(myNP::SC_FIRE_PACKET* fire_packet);
	void ProcessLifeUpdate(myNP::SC_LIFE_UPDATE_PACKET* life_packet);
	void ProcessGunUpdate(myNP::SC_GUN_UPDATE_PACKET* gun_packet);
};


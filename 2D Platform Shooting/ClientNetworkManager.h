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
	uint32_t ClientID{static_cast<uint32_t>(-1)};
	
	// �̺�Ʈ
	HANDLE recvEvent;
	HANDLE processEvent;

	// ���� ��
	std::shared_ptr<Scene> currentScene;

	// �߰��� ��� ����
	bool send_move{ false };
	long long remain_delta{};
	bool playing{ false };

public:


	ClientNetworkManager() {};
	~ClientNetworkManager() {};

	void Init();
	void Connect();

	// Recv() ������ ����
	void CreateRecvThread();
	// enQueueg
	// �������� �����Ͱ� �Ѿ���� Event ó�� �����忡�� �� ���� �ѱ�� �Լ�
	//void PushBuffer(char buf[MAX_SIZE]);
	// ��Ŷ ����
	void SendPacket(char* buf, uint8_t packet_id);

	void PushBuffer(char buf[MAX_SIZE]);

	HANDLE GetRecvEvent() { return recvEvent; }
	HANDLE GetProcessEvent() { return processEvent; }
	SOCKET GetSocket() { return clientSocket;  }
	uint32_t GetClientId() { return ClientID; }
	void setSendMove(const bool value) { send_move = value; }
	bool getSendMove() const { return send_move; }

	void ProcessPacket();
	void ProcessPlayerMove(myNP::SC_MOVE_PACKET* move_packet);
	void ProcessMatchMaking(myNP::SC_MATCHMAKING_PACKET* matchmaking_packet);
	void ProcessFirebullet(myNP::SC_FIRE_PACKET* fire_packet);
	void ProcessLifeUpdate(myNP::SC_LIFE_UPDATE_PACKET* life_packet);
	void ProcessGunUpdate(myNP::SC_GUN_UPDATE_PACKET* gun_packet);
};


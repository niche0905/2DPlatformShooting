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

	// ����������
	bool playing{ false };

	// �̺�Ʈ
	HANDLE recvEvent;
	HANDLE processEvent;

	// ���� ��
	std::shared_ptr<Scene> currentScene;

	// ����� �ּ�
	std::array<char, INET_ADDRSTRLEN> addr{};

public:
	// ������ �Ҹ���
	ClientNetworkManager();
	~ClientNetworkManager() {};

	// �ʱ�ȭ
	void Init();

	// connect
	void Connect();

	// Recv() ������ ����
	void CreateRecvThread();

	// ��Ŷ ����
	void SendPacket(char* buf, uint8_t packet_id);

	// �޾ƿ� ���۸� ť�� push
	void PushBuffer(char buf[MAX_SIZE]);

	// Getter
	HANDLE GetRecvEvent() { return recvEvent; }
	HANDLE GetProcessEvent() { return processEvent; }
	SOCKET GetSocket() { return clientSocket;  }
	int32_t GetClientID() { return ClientID; }

	// Process
	void ProcessPacket();
	void ProcessPlayerMove(myNP::SC_MOVE_PACKET* move_packet);
	void ProcessMatchMaking(myNP::SC_MATCHMAKING_PACKET* matchmaking_packet);
	void ProcessFirebullet(myNP::SC_FIRE_PACKET* fire_packet);
	void ProcessLifeUpdate(myNP::SC_LIFE_UPDATE_PACKET* life_packet);
	void ProcessGunUpdate(myNP::SC_GUN_UPDATE_PACKET* gun_packet);
	void ProcessCreateItem(myNP::SC_ITEM_CREATE_PACKET* item_create_packet);
	void ProcessRemoveItem(myNP::SC_ITEM_REMOVE_PACKET* item_remove_packet);
	void ProcessRemoveBullet(myNP::SC_BULLET_REMOVE_PACKET* bullet_remove_packet);
	void ProcessPlayerDamage(myNP::SC_PLAYER_DAMAGE_PACKET* player_damage_packet);
	void ProcessGameover(myNP::SC_GAMEOVER_PACKET* gameover_packet);
};


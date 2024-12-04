#pragma once
#include "Utilities.h"
#include "Scene.h"
#include "SceneManager.h"

DWORD WINAPI WorkerRecv(LPVOID arg);

class ClientNetworkManager {
private:
	// 소켓
	SOCKET clientSocket;

	// 스레드
	HANDLE clientThread;

	// 스레드 전달 큐
	std::queue<std::array<char, MAX_SIZE>> process_queue;

	// 클라이언트 ID
	int32_t ClientID{ -1 };

	// 게임중인지
	bool playing{ false };

	// 이벤트
	HANDLE recvEvent;
	HANDLE processEvent;

	// 현재 씬
	std::shared_ptr<Scene> currentScene;

	// 사용할 주소
	std::array<char, INET_ADDRSTRLEN> addr{};

public:
	// 생성자 소멸자
	ClientNetworkManager();
	~ClientNetworkManager() {};

	// 초기화
	void Init();

	// connect
	void Connect();

	// Recv() 스레드 생성
	void CreateRecvThread();

	// 패킷 전송
	void SendPacket(char* buf, uint8_t packet_id);

	// 받아온 버퍼를 큐에 push
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


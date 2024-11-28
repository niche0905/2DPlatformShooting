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
	uint32_t ClientID{static_cast<uint32_t>(-1)};

	// 현재 게임중인지
	bool playing{false};

	// 이벤트
	HANDLE recvEvent;
	HANDLE processEvent;

	// 현재 씬
	std::shared_ptr<Scene> currentScene;

public:
	// 임시. 나중에 private화 필요
	// TODO:
	bool send_move{ false };
	bool recv_move{ false };
	long long remain_delta{};


	ClientNetworkManager() {};
	~ClientNetworkManager() {};

	void Init();
	void Connect();

	// Recv() 스레드 생성
	void CreateRecvThread();
	// enQueue
	// 서버에서 데이터가 넘어오면 Event 처리 스레드에게 할 일을 넘기는 함수
	//void PushBuffer(char buf[MAX_SIZE]);
	// 패킷 전송
	void SendPacket(char* buf, uint8_t packet_id);

	void PushBuffer(char buf[MAX_SIZE]);

	HANDLE GetRecvEvent() { return recvEvent; }
	HANDLE GetProcessEvent() { return processEvent; }
	SOCKET GetSocket() { return clientSocket;  }
	uint32_t GetClientId() { return ClientID; }

	void ProcessPacket();
	void ProcessPlayerMove(myNP::SC_MOVE_PACKET* move_packet);
	void ProcessMatchMaking(myNP::SC_MATCHMAKING_PACKET* matchmaking_packet);
	void ProcessFirebullet(myNP::SC_FIRE_PACKET* fire_packet);
	void ProcessLifeUpdate(myNP::SC_LIFE_UPDATE_PACKET* life_packet);
	void ProcessGunUpdate(myNP::SC_GUN_UPDATE_PACKET* gun_packet);
};


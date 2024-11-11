#pragma once


using BufferType = std::array<int, myNP::MaxPacketSize>;
using PacketType = BufferType;
using QueueType = std::queue<BufferType>;

class ServerNetworkManager
{
public:
	SOCKET socket{}; // 소켓
	HANDLE thread{}; // 스레드
	std::vector<QueueType> processQueue; // 스레드 전달 큐 벡터
	std::array<HANDLE, 2> recvEvent{NULL};
	std::array<HANDLE, 2> processEvent{NULL}; // 스레드 동기화를 위한 이벤트

public:
	ServerNetworkManager();

	void Init();
	void CreateLobbyThread(); // 로비 스레드 생성
	void CreateUpdateThread(); // update 스레드 생성
	void CreateRecvThread(HANDLE socket); // recv 스레드 생성
	void PushBuffer(BufferType buffer); // 버퍼 Push
	QueueType& GetQueue();
	void SendPacket(PacketType packet);
};
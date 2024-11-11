#pragma once


using BufferType = std::array<int, myNP::MaxPacketSize>;
using PacketType = BufferType;
using QueueType = std::queue<BufferType>;

class ServerNetworkManager
{
public:
	SOCKET socket{}; // ����
	HANDLE thread{}; // ������
	std::vector<QueueType> processQueue; // ������ ���� ť ����
	std::array<HANDLE, 2> recvEvent{NULL};
	std::array<HANDLE, 2> processEvent{NULL}; // ������ ����ȭ�� ���� �̺�Ʈ

public:
	ServerNetworkManager();

	void Init();
	void CreateLobbyThread(); // �κ� ������ ����
	void CreateUpdateThread(); // update ������ ����
	void CreateRecvThread(HANDLE socket); // recv ������ ����
	void PushBuffer(BufferType buffer); // ���� Push
	QueueType& GetQueue();
	void SendPacket(PacketType packet);
};
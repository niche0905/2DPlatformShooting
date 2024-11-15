#include "pch.h"
#include "ServerNetworkManager.h"
// TEMP : �׽�Ʈ�� ���� (11/15 �۽�ȣ) �Ʒ� �����ؾ� ��
#include <thread>

using _SNM = ServerNetworkManager;
using namespace myNP;

ServerNetworkManager::ServerNetworkManager()
{
}

ServerNetworkManager::~ServerNetworkManager()
{
	if (NULL != listenSocket) {
		closesocket(listenSocket);
	}

	WSACleanup();
}

void ServerNetworkManager::Init()
{
	NetworkInit();
	CreateLobbyThread();
}

void ServerNetworkManager::NetworkInit()
{
	
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {

	}

	// ���� ����
	listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET) {
		// TODO: change to err_quit
		exit(-1);
	}

	// bind
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	auto retval = bind(listenSocket, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) {
		// TODO: change to err_quit
		exit(-1);
	}

	// listen
	retval = listen(listenSocket, SOMAXCONN);
	if (retval == SOCKET_ERROR) {
		// TODO: err_quit("listen()");
		exit(-1);
	}
}

void ServerNetworkManager::Accept()
{
	while (true) {

		// accept()
		sockaddr_in clientaddr{};
		int addrlen{ sizeof(clientaddr) };
		auto client_socket{ accept(listenSocket, (struct sockaddr*)&clientaddr, &addrlen) };
		if (client_socket == INVALID_SOCKET) {
			// err_display("accept()");
			continue;
		}

		// recv ������ ����
		CreateRecvThread(client_socket);
	}
}

bool ServerNetworkManager::doRecv(SOCKET sock, BufferType& buffer)
{
	// ���� ���� recv
	// BASE_PACKET ��ŭ ���� �д´�.
	auto retval{ ::recv(
		sock,
		buffer.data(),
		sizeof(BASE_PACKET),
		MSG_WAITALL
	) };

	if (SOCKET_ERROR == retval) {
		// err_display("recv()");
		return false;
	}

	// BASE PACKET ���� �˻�
	BASE_PACKET* base{ reinterpret_cast<BASE_PACKET*>(buffer.data()) };
	if (not (0 <= base->size and base->size <= MaxPacketSize) ||
		not (0 < base->id and base->id <= PacketID::END)) {
		cout << "SNM::doRecv(): Invaild Packet.\n";
		return false;
	}

	// ���� ���� recv
	retval = { ::recv(
		sock,
		buffer.data() + sizeof(BASE_PACKET),
		base->size - static_cast<PacketSizeType>(sizeof(BASE_PACKET)),
		MSG_WAITALL
	) };

	if (SOCKET_ERROR == retval) {
		// err_display("recv()");
		return false;
	}

	return true;
}

void ServerNetworkManager::CreateLobbyThread()
{
	lobbyThread = { CreateThread(NULL, 0, workerLobby, NULL, 0, NULL) };
}

void ServerNetworkManager::CreateUpdateThread()
{
	updateThread = { CreateThread(NULL, 0, workerUpdate, NULL, 0, NULL) };
}

void ServerNetworkManager::CreateRecvThread(SOCKET socket) const
{
	auto th{ CreateThread(NULL, 0, workerRecv,
			reinterpret_cast<LPVOID>(&socket), 0, NULL) };
	if (NULL == th) { closesocket(socket); }
	else { CloseHandle(th); }
}

bool ServerNetworkManager::doSend(SOCKET sock, const BufferType& buffer)
{
	// ���� ���� send
	auto retval{ ::send(
		sock,
		buffer.data(),
		buffer[0],
		0
	) };

	if (SOCKET_ERROR == retval) {
		// err_display("send()");
		return false;
	}

	return true;
}

DWORD WINAPI workerUpdate(LPVOID arg)
{
	for (int i = 0; i < 5; ++i) {
		std::cout << "HI From Update Thread\n";
		Sleep(5000);
	}
	return 0;
}

DWORD WINAPI workerRecv(LPVOID arg)
{
	auto client_socket{ *reinterpret_cast<SOCKET*>(arg) };

	cout << "Hi From Recv Thread\n";

	// recv
	QueueType local_queue{};

	// TODO: �ӽ÷� send �غ���.

	// move ��Ŷ �ӽ÷� �ϳ� ����
	// doSend ȣ��.


	//BufferType buffer{};
	//cout << "Waiting for Send...\n";
	//if (not _SNM::doRecv(client_socket, buffer)) {
	//	cout << "workerRecv() ERROR: Recv Failed.\n";
	//	closesocket(client_socket);
	//	return 0;
	//}

	// TEMP : �׽�Ʈ�� ���� (11/15 �۽�ȣ) ���� �ּ� ���� �Ʒ��� ���� �ؾ���

	char buffer[myNP::MaxPacketSize];
	memset(buffer, 0, myNP::MaxPacketSize);
	int recv_size = recv(client_socket, buffer, sizeof(myNP::BASE_PACKET), MSG_WAITALL);
	std::cout << recv_size << std::endl;
	std::cout << static_cast<int>(buffer[0]) << std::endl;
	std::cout << static_cast<int>(buffer[1]) << std::endl;

	recv_size = recv(client_socket, buffer+ recv_size, buffer[0] - recv_size, MSG_WAITALL);
	std::cout << recv_size << std::endl;

	myNP::CS_MOVE_PACKET* packet = reinterpret_cast<myNP::CS_MOVE_PACKET*>(buffer);
	cout << "test1\n";
	packet->ntohByteOrder();

	cout << "test2\n";
	std::cout << "id : " << static_cast<int>(packet->id) << std::endl;
	std::cout << "size : " << static_cast<int>(packet->size) << std::endl;
	std::cout << "p_id : " << packet->p_id << std::endl;
	std::cout << "posX : " << packet->posX << std::endl;
	std::cout << "posY : " << packet->posY << std::endl;
	std::cout << "dir : " << packet->dir << std::endl;

	// TEMP : �׽�Ʈ�� ���� (11/15 �۽�ȣ) �Ʒ��� �ּ� ���� �ؾ���
	
	// ť�� ���� ����ֱ�
	//local_queue.push(buffer);


	// TODO: ���ʿ��� ���� ��Ŷ�� ������ �Ǹ� ť�� 
	return 0;
}

DWORD WINAPI workerLobby(LPVOID arg)
{
	for (int i = 0; i < 100; ++i) {
		std::cout << "HI From Lobby Thread\n";
		Sleep(10000);
	}
	return 0;
}

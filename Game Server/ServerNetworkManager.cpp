#include "pch.h"
#include "ServerNetworkManager.h"

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
	
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {

	}

	// 소켓 생성
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

void ServerNetworkManager::AcceptAndRecv()
{
	while (true) {

		// accept()
		sockaddr_in clientaddr{};
		int addrlen{ sizeof(clientaddr) };
		
		// 여기서 blocking.
		auto client_sock{ accept(listenSocket, (struct sockaddr*)&clientaddr, &addrlen) };
		if (client_sock == INVALID_SOCKET) {
			// err_display("accept()");
			continue;
		}

		// recv 스레드 생성
		auto th{ CreateThread(NULL, 0, workerRecv,
			reinterpret_cast<LPVOID>(&client_sock), 0, NULL) };
		if (NULL == th) { closesocket(client_sock); }
		else { CloseHandle(th); }
	}
}



void ServerNetworkManager::CreateLobbyThread()
{
	lobbyThread = { CreateThread(NULL, 0, workerLobby, NULL, 0, NULL) };
}

void ServerNetworkManager::CreateUpdateThread()
{
	updateThread = { CreateThread(NULL, 0, workerUpdate, NULL, 0, NULL) };
}

void ServerNetworkManager::CreateRecvThread(HANDLE socket)
{
}

void ServerNetworkManager::PushBuffer(BufferType buffer)
{
}

QueueType& ServerNetworkManager::GetQueue()
{
	return processQueue[0];
}

void ServerNetworkManager::SendPacket(PacketType packet)
{
}

DWORD WINAPI workerUpdate(LPVOID arg)
{
	auto client_sock = *reinterpret_cast<SOCKET*>(arg);

	for (int i = 0; i < 5; ++i) {
		std::cout << "HI From Update Thread\n";
		Sleep(5000);
	}

	closesocket(client_sock);
	return 0;
}

DWORD WINAPI workerRecv(LPVOID arg)
{
	for (int i = 0; i < 5; ++i) {
		std::cout << "HI From Recv Thread\n";
		Sleep(5000);
	}
	return 0;
}

DWORD WINAPI workerLobby(LPVOID arg)
{
	for (int i = 0; i < 5; ++i) {
		std::cout << "HI From Lobby Thread\n";
		Sleep(5000);
	}
	return 0;
}

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
	// 扩加 檬扁拳
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {

	}

	// 家南 积己
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

void ServerNetworkManager::CreateLobbyThread()
{
}

void ServerNetworkManager::CreateUpdateThread()
{
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
	return 0;
}

DWORD WINAPI workerRecv(LPVOID arg)
{
	return 0;
}

DWORD WINAPI workerLobby(LPVOID arg)
{
	return 0;
}

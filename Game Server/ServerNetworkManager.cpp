#include "pch.h"
#include "ServerNetworkManager.h"

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

		// recv 스레드 생성
		CreateRecvThread(client_socket);
	}
}

bool ServerNetworkManager::doRecv(SOCKET sock, BufferType& buffer)
{
	PacketSize	len{};
	BufferType	buf{};

	// 고정 길이 recv
	auto retval{ ::recv(
		sock,
		reinterpret_cast<char*>(&len),
		sizeof(PacketSize),
		MSG_WAITALL
	) };

	if (SOCKET_ERROR == retval) {
		// err_display("recv()");
		return false;
	}

	// 가변 길이 recv
	retval = { ::recv(
		sock,
		buffer.data(),
		len,
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
	// 고정 길이 send
	auto retval{ ::send(
		sock,
		&buffer[0],
		sizeof(PacketSize),
		0
	)};

	if (SOCKET_ERROR == retval) {
		// err_display("send()");
		return false;
	}

	// 가변 길이 send
	retval = { ::send(
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

	// TODO: 임시로 send 해보기.

	// move 패킷 임시로 하나 만들어서
	// doSend 호출.


	BufferType buffer{};
	cout << "Waiting for Send...\n";
	if (not _SNM::doRecv(client_socket, buffer)) {
		cout << "workerRecv() ERROR: Recv Failed.\n";
		closesocket(client_socket);
		return 0;
	}

	// 큐에 정보 집어넣기
	local_queue.push(buffer);


	// TODO: 양쪽에서 무브 패킷이 들어오게 되면 큐를 
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

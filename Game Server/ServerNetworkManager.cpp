#include "pch.h"
#include "ServerNetworkManager.h"
// TEMP : 테스트용 수정 (11/15 송승호) 아래 삭제해야 함
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
	// 고정 길이 recv
	// BASE_PACKET 만큼 먼저 읽는다.
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

	// BASE PACKET 오류 검사
	BASE_PACKET* base{ reinterpret_cast<BASE_PACKET*>(buffer.data()) };
	if (not (0 <= base->size and base->size <= MaxPacketSize) ||
		not (0 < base->id and base->id <= PacketID::END)) {
		cout << "SNM::doRecv(): Invaild Packet.\n";
		return false;
	}

	// 가변 길이 recv
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
	// 가변 길이 send
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

	// TODO: 임시로 send 해보기.

	// move 패킷 임시로 하나 만들어서
	// doSend 호출.


	//BufferType buffer{};
	//cout << "Waiting for Send...\n";
	//if (not _SNM::doRecv(client_socket, buffer)) {
	//	cout << "workerRecv() ERROR: Recv Failed.\n";
	//	closesocket(client_socket);
	//	return 0;
	//}

	// TEMP : 테스트용 수정 (11/15 송승호) 위는 주석 해제 아래는 삭제 해야함

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

	// TEMP : 테스트용 수정 (11/15 송승호) 아래도 주석 해제 해야함
	
	// 큐에 정보 집어넣기
	//local_queue.push(buffer);


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

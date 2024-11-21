#include "pch.h"
#include "ServerNetworkManager.h"

// using
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
	EventInit();
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

void ServerNetworkManager::EventInit()
{
	for (auto& event_ : recvEvent) {
		event_ = CreateEvent(NULL, FALSE, FALSE, NULL);
	}
	for (auto& event_ : processEvent) {
		event_ = CreateEvent(NULL, FALSE, FALSE, NULL);
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
			
		if (nextId >= 2) {
			closesocket(client_socket);
		}
		else {
			CreateRecvThread(client_socket);
		}
	}
}

bool ServerNetworkManager::DoRecv(SOCKET sock, BufferType& buffer) const
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

bool ServerNetworkManager::doSend(SOCKET sock, const BufferType& buffer) const
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


	// recv
	std::list<BufferType> local_list{};
	int client_id{ SNMgr.GetNextId() };
	cout << "Hi From Recv Thread " << client_id << "\n";


	//SNMgr.SendPacket<SC_MOVE_PACKET>(client_socket,
	//	0,
	//	200.f,
	//	200.f,
	//	false
	//);
 
	while (true) {

		BufferType buffer{};
		cout << "Waiting for Send...\n";
		if (not SNMgr.DoRecv(client_socket, buffer)) {
			cout << "workerRecv() ERROR: Recv Failed.\n";
			SNMgr.DecreaseNextId();
			// ���� ���̸� 2 ����
			
			// ���� ���� �ƴϸ� 1 ����
			closesocket(client_socket);
			return 0;
		}
		
		if (not SNMgr.IsPlaying() &&
			PacketID::CS_MATCHMAKING == buffer[1]) {
			cout << "[Client " << client_id << "] Set Recv event." << "\n";
			SNMgr.SetRecvEvent(client_id);
			cout << "[Client " << client_id << "] Waiting for Process event..." << "\n";
			SNMgr.WaitforProcessEvent(client_id);
			cout << "[Client " << client_id << "] get Process event." << "\n";
		}
		
		// if �ش� Ŭ���̾�Ʈ�� ID�� ���� �������̶��
		//		ť�� �ֱ�
		//		if ���� move ��Ŷ�� �Դٸ�
		//			ť ������ ����ֱ�
		//			process �̺�Ʈ Ȱ��ȭ
		//			�Ϸ� ���
		
		// ���� ������ �ƴϰ� && ��ġ����ŷ ��Ŷ�̸�
		//		recv �̺�Ʈ Ȱ��ȭ.
		//		�Ϸ� ���
	}
		
	return 0;
}

DWORD WINAPI workerLobby(LPVOID arg)
{
	std::cout << "workerLobby(): Waiting For Recv Events...\n";
	SNMgr.WaitforRecvEvent();
	std::cout << "workerLobby(): set Process Events.\n";
	SNMgr.SetProcessEvent();

	return 0;
}

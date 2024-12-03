#include "pch.h"
#include "ServerNetworkManager.h"
#include "World.h"

// using
using _SNM = ServerNetworkManager;
using namespace myNP;

World world;

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

		DWORD opt_value = 1;
		setsockopt(client_socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt_value, sizeof(opt_value));
			
		if (playing) {
			closesocket(client_socket);
		}
		else {
			CreateRecvThread(client_socket);
		}
	}
}

bool ServerNetworkManager::DoRecv(SOCKET sock, BufferType& buffer) const
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
	//if (not (0 <= base->size and base->size <= MaxPacketSize) ||
	//	not (0 < base->id and base->id <= PacketID::END)) {
	//	// cout << "SNM::doRecv(): Invaild Packet.\n";
	//	return false;
	//}

	// Logging
	//cout << "Recv ";
	//myNP::printPacketType(base->id);

	int remain_size = (base->size - static_cast<PacketSizeType>(sizeof(BASE_PACKET)));
	if (remain_size <= 0)
		return true;

	// 가변 길이 recv
	retval = { ::recv(
		sock,
		buffer.data() + sizeof(BASE_PACKET),
		remain_size,
		MSG_WAITALL
	) };

	if (SOCKET_ERROR == retval) {
		// err_display("recv()");
		return false;
	}

	return true;
}

void ServerNetworkManager::ProcessPackets()
{
	for (int i = 0; i <= 1; ++i) {
		auto&	queue_ = processQueue[i];
		int		client_id = i;
		while (not queue_.empty()) {
			auto& buffer{ queue_.front() };
			PacketID packet_id{ static_cast<PacketID>(buffer[1]) };
			switch (packet_id)
			{
			case PacketID::CS_MOVE:
			{
				// TODO: 실제 움직임 처리
				auto packet = reinterpret_cast<CS_MOVE_PACKET*>(buffer.data());
				packet->ntohByteOrder();
				//cout << "받은 좌표\n";
				//cout << packet->p_id << " " << (client_id==packet->p_id) << " : " << packet->posX << ", " << packet->posY << "\n";
				//cout << "받은 좌표\n";
				//cout << packet->p_id << " : " << ntohf(packet->posX) << ", " << ntohf(packet->posY) << "\n";
				if (client_id == 0) {
					//cout << "설정하는 좌표 " << packet->p_id << "\n";
					world.p1.SetPos(packet->posX, packet->posY);
					world.p1.SetDirection(packet->dir);
					world.p1.Revive();
				}
				else if (client_id == 1) {
					//cout << "설정하는 좌표 " << packet->p_id << "\n";
					world.p2.SetPos(packet->posX, packet->posY);
					world.p2.SetDirection(packet->dir);
					world.p2.Revive();
				}

				//cout << "MOVE PACKET " << packet->posX << "," << packet->posY << "\n";
			}
			break;

			case PacketID::CS_FIRE:
			{
				// TODO : SC_FIRE_PAKCET 정리해야한다
				myNP::CS_FIRE_PACKET* packet = reinterpret_cast<myNP::CS_FIRE_PACKET*>(buffer.data());
				packet->ntohByteOrder();

				// TODO : time gap 만큼 보간해서 위치 조정 해줘야 함
				//		  + Bullet을 직접 건들이는게 아닌 Player의 GunFire 함수를 이용하자
				if (client_id == 0) {
					// TODO : p1.fire 함수를 불러야 함 ㅇㅇ
					//world.p1.GetBullets().emplace_back(packet->posX, packet->posY, packet->type, packet->dir); // <- TODO : 속도 설정 할 수 있어야 함 인자로
				}
				else if (client_id == 1) {
					// TODO : p2.fire 함수를 불러야 함 ㅇㅇ
					//world.p2.GetBullets().emplace_back(packet->posX, packet->posY, packet->type, packet->dir); // <- TODO : 속도 설정 할 수 있어야 함 인자로
				}

				int other_player_id = 1 - client_id;
				SendPacket<myNP::SC_FIRE_PACKET>(other_player_id,
					packet->b_id, packet->posX, packet->posY, packet->dir, packet->type, packet->fire_t
				);
			}
				break;

			default:
				break;
			}
			
			queue_.pop();
		}
	}


	SendPacket<myNP::SC_MOVE_PACKET>(0,
		0, world.p2.GetPos().posX, world.p2.GetPos().posY, world.p2.getDirection()
	);

	SendPacket<myNP::SC_MOVE_PACKET>(1,
		0, world.p1.GetPos().posX, world.p1.GetPos().posY, world.p1.getDirection()
	);
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
	// Logging
	//cout << "Send ";
	//myNP::printPacketType(buffer[1]);

	// 고정 길이 send
	auto retval{ ::send(
		sock,
		buffer.data(),
		sizeof(BASE_PACKET),
		0
	) };

	//std::cout << (int)buffer[0] << "," << (int)buffer[1] << std::endl;

	if (SOCKET_ERROR == retval) {
		// err_display("send()");
		return false;
	}

	//cout << "Send Size: " << sizeof(BASE_PACKET) << " real : " << retval << endl;
	// 가변 길이 send
	int remain_size = static_cast<int>(buffer[0]) - sizeof(BASE_PACKET);
	if (remain_size > 0) {
		auto retval{ ::send(
			sock,
			buffer.data() + sizeof(BASE_PACKET),
			remain_size,
			0
		) };
		if (SOCKET_ERROR == retval) {
			// err_display("send()");
			return false;
		}
		//cout << "Send Size: " << remain_size << " real : " << retval << endl;
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
	QueueType local_queue{};
	int client_id{ SNMgr.GetNextId() };
	SNMgr.setSocketArr(client_socket, client_id);
	cout << "Hi From Recv Thread " << client_id << "\n";


 
	while (true) {

		BufferType buffer{};
		// cout << "Waiting for Send...\n";

		if (not SNMgr.DoRecv(client_socket, buffer)) {
			cout << "workerRecv() ERROR: Recv Failed.\n";
			if (not SNMgr.IsPlaying()) {
				SNMgr.DecreaseNextID();
			}
			closesocket(client_socket);
			return 0;
			continue;
		}
		PacketID packet_id{ static_cast<PacketID>(buffer[1]) };
		
		if (not SNMgr.IsPlaying() &&
			PacketID::CS_MATCHMAKING == packet_id) {
			// cout << "[Client " << client_id << "] Set Recv event." << "\n";
			SNMgr.SetRecvEvent(client_id);
			// cout << "[Client " << client_id << "] Waiting for Process event..." << "\n";
			SNMgr.WaitforProcessEvent(client_id);
			// cout << "[Client " << client_id << "] get Process event." << "\n";
		}
		

		if (SNMgr.IsPlaying())
		{
			local_queue.push(buffer);
			if (PacketID::CS_MOVE == packet_id) {

				SNMgr.setProcessQueue(local_queue, client_id);
				while (not local_queue.empty()) local_queue.pop();
				// cout << "[Client " << client_id << "] Set Recv event." << "\n";
				SNMgr.SetRecvEvent(client_id);
				// cout << "[Client " << client_id << "] Waiting for Process event..." << "\n";
				SNMgr.WaitforProcessEvent(client_id);
				// cout << "[Client " << client_id << "] get Process event." << "\n";
			}
		}
	}
		
	return 0;
}

DWORD WINAPI workerLobby(LPVOID arg)
{
	//World world; <- 테스트 용 지워도 됨

	while (true) {

		// 게임 중이 아닐 경우
		if (not SNMgr.IsPlaying()) {
			SNMgr.WaitforRecvEvent();

			cout << "Playing = true" << endl;
			SNMgr.setPlaying(true);
			for (int i = 0; i <= 1; ++i) {
				SNMgr.SendPacket<myNP::SC_MATCHMAKING_PACKET>(i, true, i);
				//cout << "Send 7 to " << i << endl;
			}
			world.p1.SetPlayerID(0);
			world.p2.SetPlayerID(1);
			SNMgr.SetProcessEvent();
		}

		// 게임 중일 경우
		else {
			world.Update();

			SNMgr.WaitforRecvEvent();

			SNMgr.ProcessPackets();

			SNMgr.SetProcessEvent();

		}
	}
}

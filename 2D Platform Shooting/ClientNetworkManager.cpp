#include "pch.h"
#include "ClientNetworkManager.h"

DWORD WINAPI WorkerRecv(LPVOID arg)
{
    WSAEVENT events[1] = { network_mgr.GetRecvEvent() };
    char buf[MAX_SIZE];

    while (true) {
        // 이벤트 대기
        DWORD result = WSAWaitForMultipleEvents(1, events, FALSE, WSA_INFINITE, FALSE);
        if (result == WSA_WAIT_FAILED) break;

        // 이벤트 확인
        WSANETWORKEVENTS networkEvents;
        if (WSAEnumNetworkEvents(network_mgr.GetSocket(), network_mgr.GetRecvEvent(), &networkEvents) == SOCKET_ERROR)
            break;

        // 연결 종료 확인
        if (networkEvents.lNetworkEvents & FD_CLOSE) {
            break;
        }

        // 데이터 수신
        if (networkEvents.lNetworkEvents & FD_READ) {
            // 고정 길이 recv()
            int recvLen = recv(network_mgr.GetSocket(), buf, network_mgr.GetSocket(), MSG_WAITALL);

            if (recvLen > 0) {
                // base_packet 길이 만큼 읽었으므로 나머지 데이터 길이 계산
                int remainingPacketLen = *(reinterpret_cast<int*>(buf)) - network_mgr.GetSocket();

                // 가변 길이 recv()
                if (remainingPacketLen > 0) {
                    recvLen = recv(network_mgr.GetSocket(), buf + network_mgr.GetSocket(), remainingPacketLen, MSG_WAITALL);
                    if (recvLen > 0) {
                        network_mgr.PushBuffer(buf);
                    }
                }
            }
            else if (recvLen == 0 || recvLen == SOCKET_ERROR) {
                break;
            }
        }
    }
    return 0;
}

ClientNetworkManager::ClientNetworkManager() {}

ClientNetworkManager::~ClientNetworkManager() { }

void ClientNetworkManager::Init()
{
    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return;

    // 소켓 생성
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) return;

    // 이벤트 핸들 생성
    recvEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (recvEvent == NULL) {
        closesocket(clientSocket);
        return;
    }

    processEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (processEvent == NULL) {
        CloseHandle(recvEvent);
        closesocket(clientSocket);
        return;
    }

    // 큐 초기화
    process_queue = std::queue<std::array<char, MAX_SIZE>>();

    // 클라이언트 ID 초기화
    ClientID = -1;

    // 스레드 핸들 초기화
    clientThread = NULL;
    
    Connect();
}

void ClientNetworkManager::Connect()
{
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serveraddr.sin_addr) != 1) {
        closesocket(clientSocket);
        WSACleanup();
        return;
    }

    int result = connect(clientSocket, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (result == SOCKET_ERROR) {
        closesocket(clientSocket);
        WSACleanup();
        return;
    }

    result = WSAEventSelect(clientSocket, recvEvent, FD_READ | FD_CLOSE);
    if (result == SOCKET_ERROR) {
        closesocket(clientSocket);
        WSACleanup();
        return;
    }

    CreateRecvThread();
}

void ClientNetworkManager::CreateRecvThread()
{
    // 클라이언트 스레드 초기화
    if (clientThread != NULL)
    {
        CloseHandle(clientThread);
        clientThread = NULL;
    }

    // 클라이언트 스레드 생성
    clientThread = CreateThread(NULL, 0, WorkerRecv, NULL, 0, NULL);
    // 잘 생성됐나?
    if (clientThread == NULL)
    {
        if (clientSocket != INVALID_SOCKET)
        {
            closesocket(clientSocket);
            clientSocket = INVALID_SOCKET;
        }

        if (recvEvent != NULL)
        {
            CloseHandle(recvEvent);
            recvEvent = NULL;
        }

        if (processEvent != NULL)
        {
            CloseHandle(processEvent);
            processEvent = NULL;
        }

        while (!process_queue.empty())
        {
            process_queue.pop();
        }

        ClientID = -1;
        WSACleanup();
    }
}

void ClientNetworkManager::PushBuffer(char buf[MAX_SIZE])
{
    std::array<char, MAX_SIZE> newBuffer;
    memcpy(newBuffer.data(), buf, MAX_SIZE);
    process_queue.push(newBuffer);
    SetEvent(processEvent);
}

void ClientNetworkManager::SendPacket(char* buf, uint8_t packet_id)
{
    // 패킷 ID별 다른 처리
    switch (packet_id)
    {
        case myNP::CS_MOVE:
        {
            int sendLen = send(clientSocket, buf, sizeof(myNP::CS_MOVE_PACKET), 0);
            if (sendLen == SOCKET_ERROR) {
                closesocket(clientSocket);
                clientSocket = INVALID_SOCKET;
                WSACleanup();
            }
        }
        case myNP::CS_MATCHMAKING:
        {
            int sendLen = send(clientSocket, buf, sizeof(myNP::CS_MATCHMAKING_PACKET), 0);
            if (sendLen == SOCKET_ERROR) {
                closesocket(clientSocket);
                clientSocket = INVALID_SOCKET;
                WSACleanup();
            }
        }
        case myNP::CS_FIRE:
        {
            int sendLen = send(clientSocket, buf, sizeof(myNP::CS_FIRE_PACKET), 0);
            if (sendLen == SOCKET_ERROR) {
                closesocket(clientSocket);
                clientSocket = INVALID_SOCKET;
                WSACleanup();
            }
        }

    }
}

void ClientNetworkManager::Update()
{
    while (!process_queue.empty()) {
        std::array<char, MAX_SIZE> packet = process_queue.front();
        process_queue.pop();

        // 뭘 업뎃해야하지?
    }

    // 이벤트 재설정
    ResetEvent(processEvent);
}

void ClientNetworkManager::ProcessPacket()
{
    WaitForSingleObject(processEvent, INFINITE);

    while (!process_queue.empty()) {
        std::array<char, MAX_SIZE> buffer = process_queue.front();
        process_queue.pop();

        // 두 번째 바이트가 패킷 타입
        uint8_t packetType = static_cast<uint8_t>(buffer[1]);

        switch (packetType) {
        case myNP::CS_MOVE:
        {
            myNP::SC_MOVE_PACKET* move_packet = reinterpret_cast<myNP::SC_MOVE_PACKET*>(buffer.data());

            ProcessPlayerMove(move_packet);
            break;
        }
        case myNP::CS_MATCHMAKING:
        {
            myNP::SC_MATCHMAKING_PACKET* matchmaking_packet = reinterpret_cast<myNP::SC_MATCHMAKING_PACKET*>(buffer.data());

            ProcessMatchMaking(matchmaking_packet);
            break;
        }
        case myNP::CS_FIRE:
        {
            myNP::SC_FIRE_PACKET* fire_packet = reinterpret_cast<myNP::SC_FIRE_PACKET*>(buffer.data());

            ProcessFirebullet(fire_packet);
            break;
        }
        }
    }

    ResetEvent(processEvent);
}

void ClientNetworkManager::ProcessPlayerMove(myNP::SC_MOVE_PACKET* move_packet)
{
    if (std::shared_ptr<GameScene> gameScene = std::dynamic_pointer_cast<GameScene>(currentScene)) {

        auto& players = gameScene->GetPlayers();
        // 자기 자신이 아니면
        if (move_packet->p_id != ClientID) {
            players[move_packet->p_id].setPosition(move_packet->posX, move_packet->posY);
        }

    }
}

void ClientNetworkManager::ProcessMatchMaking(myNP::SC_MATCHMAKING_PACKET* move_packet)
{
    // TODO : 기본 인스턴스들 초기화 (위치 초기값 받아서 설정)
    // 씬 전환
}

void ClientNetworkManager::ProcessFirebullet(myNP::SC_FIRE_PACKET* move_packet)
{

}
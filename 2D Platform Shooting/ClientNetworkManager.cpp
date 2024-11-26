#include "pch.h"
#include "ClientNetworkManager.h"

DWORD WINAPI WorkerRecv(LPVOID arg)
{
    WSAEVENT events[1] = { network_mgr.GetRecvEvent() };
    char buf[MAX_SIZE];

    while (true) {
        // �̺�Ʈ ���
        DWORD result = WSAWaitForMultipleEvents(1, events, FALSE, WSA_INFINITE, FALSE);
        if (result == WSA_WAIT_FAILED) break;

        // �̺�Ʈ Ȯ��
        WSANETWORKEVENTS networkEvents;
        if (WSAEnumNetworkEvents(network_mgr.GetSocket(), network_mgr.GetRecvEvent(), &networkEvents) == SOCKET_ERROR)
            break;

        // ���� ���� Ȯ��
        if (networkEvents.lNetworkEvents & FD_CLOSE) {
            break;
        }

        // ������ ����
        if (networkEvents.lNetworkEvents & FD_READ) {
            // ���� ���� recv()
            int recvLen = recv(network_mgr.GetSocket(), buf, network_mgr.GetSocket(), MSG_WAITALL);

            if (recvLen > 0) {
                // base_packet ���� ��ŭ �о����Ƿ� ������ ������ ���� ���
                int remainingPacketLen = *(reinterpret_cast<int*>(buf)) - network_mgr.GetSocket();

                // ���� ���� recv()
                if (remainingPacketLen > 0) {
                    recvLen = recv(network_mgr.GetSocket(), buf + network_mgr.GetSocket(), remainingPacketLen, MSG_WAITALL);
                    if (recvLen > 0) {
                        cout << "RECV DATA\n";
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
    // ���� �ʱ�ȭ
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return;

    // ���� ����
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) return;

    // �̺�Ʈ �ڵ� ����
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

    // ť �ʱ�ȭ
    process_queue = std::queue<std::array<char, MAX_SIZE>>();

    // Ŭ���̾�Ʈ ID �ʱ�ȭ
    ClientID = -1;

    // ������ �ڵ� �ʱ�ȭ
    clientThread = NULL;

    currentScene = sceneManager.GetActiveScene();
    
    Connect();

    CreateRecvThread();
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
}

void ClientNetworkManager::CreateRecvThread()
{
    // Ŭ���̾�Ʈ ������ �ʱ�ȭ
    if (clientThread != NULL)
    {
        CloseHandle(clientThread);
        clientThread = NULL;
    }

    // Ŭ���̾�Ʈ ������ ����
    clientThread = CreateThread(NULL, 0, WorkerRecv, NULL, 0, NULL);
    // �� �����Ƴ�?
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
    // ��Ŷ ID�� �ٸ� ó��
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
    //while (!process_queue.empty()) {
    //    std::array<char, MAX_SIZE> packet = process_queue.front();
    //    process_queue.pop();

    //    // �� �����ؾ�����?
    //}

    ProcessPacket();

    // �̺�Ʈ �缳��
    // ResetEvent(processEvent);
}

void ClientNetworkManager::ProcessPacket()
{
    // WaitForSingleObject(processEvent, INFINITE);

    while (!process_queue.empty()) {
        std::array<char, MAX_SIZE> buffer = process_queue.front();
        process_queue.pop();

        // �� ��° ����Ʈ�� ��Ŷ Ÿ��
        uint8_t packetType = static_cast<uint8_t>(buffer[1]);

        switch (packetType) {
        case myNP::SC_MY_MOVE:
        {
            myNP::SC_MOVE_PACKET* move_packet = reinterpret_cast<myNP::SC_MOVE_PACKET*>(buffer.data());

            ProcessPlayerMove(move_packet);
            break;
        }
        case myNP::SC_MATCHMAKING:
        {
            myNP::SC_MATCHMAKING_PACKET* matchmaking_packet = reinterpret_cast<myNP::SC_MATCHMAKING_PACKET*>(buffer.data());

            ProcessMatchMaking(matchmaking_packet);
            break;
        }
        case myNP::SC_FIRE:
        {
            myNP::SC_FIRE_PACKET* fire_packet = reinterpret_cast<myNP::SC_FIRE_PACKET*>(buffer.data());

            ProcessFirebullet(fire_packet);
            break;
        }
        case myNP::SC_LIFE_UPDATE:
        {
            myNP::SC_LIFE_UPDATE_PACKET* lift_packet = reinterpret_cast<myNP::SC_LIFE_UPDATE_PACKET*>(buffer.data());

            ProcessLifeUpdate(lift_packet);
            break;
        }
        }
    }

    // ResetEvent(processEvent);
}

void ClientNetworkManager::ProcessPlayerMove(myNP::SC_MOVE_PACKET* move_packet)
{
    std::shared_ptr<GameScene> gameScene = std::dynamic_pointer_cast<GameScene>(currentScene);
    gameScene->GetDummyEnemy().setPosition(move_packet->posX, move_packet->posY);
}

void ClientNetworkManager::ProcessMatchMaking(myNP::SC_MATCHMAKING_PACKET* matchmaking_packet)
{
    // ��ġ����ŷ�� �ҽ� ClientNetworkManager�� ID�� p_id �ֱ�
    ClientID = matchmaking_packet->p_id;
    sceneManager.LoadGameScene(ClientID);
}

// ��Ŷ�� ������ gunId�� Ȯ���� ��
// �Ѿ��� Scene�� �߰��ϰ�
// Id�� ���� �Ѿ��� �ӵ��� Ÿ�Ӱ����� ����
void ClientNetworkManager::ProcessFirebullet(myNP::SC_FIRE_PACKET* fire_packet)
{
    std::shared_ptr<GameScene> gameScene = std::dynamic_pointer_cast<GameScene>(currentScene);

    // �ð� ���� ��� (ms ����)
    auto time_gap = timer.timeGap(fire_packet->fire_t);
    int elapsed_ms = time_gap.count();

    // �Ѿ��� �ʱ� ��ġ ����
    float startX = fire_packet->posX;
    float startY = fire_packet->posY;

    // �Ѿ� �ӵ� (����: �ȼ�/ms)
    const float BULLET_SPEED = 0.5f;

    // �ð� ���̸�ŭ �Ѿ� ��ġ ����
    float currentX = startX + (fire_packet->dir * BULLET_SPEED * elapsed_ms);
    float currentY = startY + (fire_packet->dir * BULLET_SPEED * elapsed_ms);

    gameScene->AddPlayerBullet(currentX, currentY, fire_packet->dir, fire_packet->type);
}

void ClientNetworkManager::ProcessLifeUpdate(myNP::SC_LIFE_UPDATE_PACKET* life_packet)
{
    std::shared_ptr<GameScene> gameScene = std::dynamic_pointer_cast<GameScene>(currentScene);
    
    // �����̸�
    if(ClientID == life_packet->p_id) gameScene->GetPlayers().revivePlayer();
    // ����
    else gameScene->GetDummyEnemy().reviveDummy();
}
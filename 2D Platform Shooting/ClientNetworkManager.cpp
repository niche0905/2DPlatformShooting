#include "pch.h"
#include "ClientNetworkManager.h"

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

    // �� �ʱ�ȭ
    currentScene = sceneManager.GetActiveScene();
    
    // Ŀ��Ʈ
    Connect();

    // Recv ������ ����
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

    // ���� ó��
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

DWORD WINAPI WorkerRecv(LPVOID arg)
{
    HANDLE event = { network_mgr.GetRecvEvent() };
    char buf[MAX_SIZE];

    while (true) {
        // �̺�Ʈ ���
        DWORD result = WaitForSingleObject(event, WSA_INFINITE);

        // ���� ���� recv()
        int recvLen = recv(network_mgr.GetSocket(), buf, sizeof(myNP::BASE_PACKET), MSG_WAITALL);

        if (recvLen > 0) {
            // base_packet ���� ��ŭ �о����Ƿ� ������ ������ ���� ���
            auto remainingPacketLen = *(reinterpret_cast<int*>(buf)) - sizeof(myNP::BASE_PACKET);

            // ���� ���� recv()
            if (remainingPacketLen > 0) {
                recvLen = recv(network_mgr.GetSocket(), buf + network_mgr.GetSocket(), remainingPacketLen, MSG_WAITALL);
                if (recvLen > 0) {
                    cout << "RECV DATA\n";

                    // ���۸� Push
                    network_mgr.PushBuffer(buf);

                    // ��Ŷ Ÿ�� Ȯ�� �� ó��
                    uint8_t packetType = static_cast<uint8_t>(buf[1]); // ��Ŷ Ÿ�� Ȯ��
                    
                        // Move ��Ŷ�� �������� ��Ŷ ó��
                    network_mgr.ProcessPacket();
                  
                }
            }
        }
        else if (recvLen == 0 || recvLen == SOCKET_ERROR) {
            break;
        }
    }
    return 0;
}

// �޾ƿ� ���۸� ť�� push
void ClientNetworkManager::PushBuffer(char buf[MAX_SIZE])
{
    std::array<char, MAX_SIZE> newBuffer;
    memcpy(newBuffer.data(), buf, MAX_SIZE);
    process_queue.push(newBuffer);
    SetEvent(processEvent);
}

// ��Ŷ send
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

void ClientNetworkManager::ProcessPacket()
{
    // WaitForSingleObject(processEvent, INFINITE);

    while (!process_queue.empty()) {
        std::array<char, MAX_SIZE> buffer = process_queue.front();
        process_queue.pop();

        // �� ��° ����Ʈ�� ��Ŷ Ÿ��
        uint8_t packetType = static_cast<uint8_t>(buffer[1]);

        switch (packetType) {
        // �̵� ó��
        case myNP::SC_MY_MOVE:
        {
            myNP::SC_MOVE_PACKET* move_packet = reinterpret_cast<myNP::SC_MOVE_PACKET*>(buffer.data());

            ProcessDummyMove(move_packet);
            break;
        }
        // ��ġ����ŷ ó��
        case myNP::SC_MATCHMAKING:
        {
            myNP::SC_MATCHMAKING_PACKET* matchmaking_packet = reinterpret_cast<myNP::SC_MATCHMAKING_PACKET*>(buffer.data());

            ProcessMatchMaking(matchmaking_packet);
            break;
        }
        // Fire ó��
        case myNP::SC_FIRE:
        {
            myNP::SC_FIRE_PACKET* fire_packet = reinterpret_cast<myNP::SC_FIRE_PACKET*>(buffer.data());

            ProcessFirebullet(fire_packet);
            break;
        }
        // Life ó��
        case myNP::SC_LIFE_UPDATE:
        {
            myNP::SC_LIFE_UPDATE_PACKET* life_packet = reinterpret_cast<myNP::SC_LIFE_UPDATE_PACKET*>(buffer.data());

            ProcessLifeUpdate(life_packet);
            break;
        }
        // Gun ������Ʈ ó��
        case myNP::SC_GUN_UPDATE:
        {
            myNP::SC_GUN_UPDATE_PACKET* gun_update_packet = reinterpret_cast<myNP::SC_GUN_UPDATE_PACKET*>(buffer.data());

            ProcessGunUpdate(gun_update_packet);
            break;
        }
        }
    }

    // ResetEvent(processEvent);
}

// ��� �̵� ó��
void ClientNetworkManager::ProcessDummyMove(myNP::SC_MOVE_PACKET* move_packet)
{
    // ����Ʈ ����
    move_packet->ntohByteOrder();
    std::shared_ptr<GameScene> gameScene = std::dynamic_pointer_cast<GameScene>(currentScene);
    gameScene->GetDummyEnemy().setPosition(move_packet->posX, move_packet->posY);
}

// ��ġ����ŷ ó��
void ClientNetworkManager::ProcessMatchMaking(myNP::SC_MATCHMAKING_PACKET* matchmaking_packet)
{
    // ����Ʈ ����
    matchmaking_packet->ntohByteOrder();
    // ��ġ����ŷ�� �ҽ� ClientNetworkManager�� ID�� p_id �ֱ�
    ClientID = matchmaking_packet->p_id;
    sceneManager.LoadGameScene(ClientID);
}

// �Ѿ� ó��
void ClientNetworkManager::ProcessFirebullet(myNP::SC_FIRE_PACKET* fire_packet)
{
    // ����Ʈ ����
    fire_packet->ntohByteOrder();
    // ��Ŷ�� ������ gunId�� Ȯ���� ��
    // �Ѿ��� Scene�� �߰��ϰ�
    // Id�� ���� �Ѿ��� �ӵ��� Ÿ�Ӱ����� ����
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

    gameScene->AddEnemyBullet(currentX, currentY, fire_packet->dir, fire_packet->type);
}

// ���(��Ȱ) ó��
void ClientNetworkManager::ProcessLifeUpdate(myNP::SC_LIFE_UPDATE_PACKET* life_packet)
{
    // ����Ʈ ����
    life_packet->ntohByteOrder();
    std::shared_ptr<GameScene> gameScene = std::dynamic_pointer_cast<GameScene>(currentScene);
    
    // �����̸�
    if(ClientID == life_packet->p_id) gameScene->GetPlayers().revivePlayer();
    // ����
    else gameScene->GetDummyEnemy().reviveDummy();
}

// �� ������Ʈ ó��
void ClientNetworkManager::ProcessGunUpdate(myNP::SC_GUN_UPDATE_PACKET* gun_packet)
{
    // ����Ʈ ����
    gun_packet->ntohByteOrder();
    std::shared_ptr<GameScene> gameScene = std::dynamic_pointer_cast<GameScene>(currentScene);

    // �����̸�
    if (ClientID == gun_packet->p_id) gameScene->GetPlayers().setPlayerGun(gun_packet->g_id);
    // ����
    else gameScene->GetDummyEnemy().setDummyGun(gun_packet->g_id);
}

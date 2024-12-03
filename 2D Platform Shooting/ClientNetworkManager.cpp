#include "pch.h"
#include "ClientNetworkManager.h"
#include <thread>
#include <chrono>

ClientNetworkManager::ClientNetworkManager()
{
    std::cin.get(addr.data(), addr.size());
    if ('m' == tolower(addr[0])) {
        strncpy_s(addr.data(), addr.size() - 1, "127.0.0.1", addr.size() - 1);
    }
}

// �ʱ�ȭ
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

// Connect
void ClientNetworkManager::Connect()
{
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, addr.data(), &serveraddr.sin_addr) != 1) {
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

    DWORD opt_value = 1;
    setsockopt(clientSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt_value, sizeof(opt_value));
}

// Recv ������ ����
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

// Recv ������
DWORD WINAPI WorkerRecv(LPVOID arg)
{
    char buf[MAX_SIZE];

    while (true) {
        // �̺�Ʈ ���
        //DWORD result = WaitForSingleObject(event, WSA_INFINITE);

        // ���� ���� recv()
        int recvLen = recv(network_mgr.GetSocket(), buf, sizeof(myNP::BASE_PACKET), MSG_WAITALL);
        //cout << recvLen << "\n";

        if (recvLen > 0) {
            // base_packet ���� ��ŭ �о����Ƿ� ������ ������ ���� ���
            auto remainingPacketLen = buf[0] - sizeof(myNP::BASE_PACKET);

            //cout << "RECV " << static_cast<int>(buf[1]) << ": ";
            //myNP::printPacketType(buf[1]);

            // ���� ���� recv()
            if (remainingPacketLen > 0) {
                while (true)
                {
                    //cout << "Test1\n";
                    recvLen = recv(network_mgr.GetSocket(), buf + sizeof(myNP::BASE_PACKET), remainingPacketLen, MSG_WAITALL);
                    //cout << recvLen << "\n";
                    if (recvLen > 0) {

                        // ���۸� Push
                        network_mgr.PushBuffer(buf);

                        if (buf[1] == myNP::SC_MY_MOVE or buf[1] == myNP::SC_MATCHMAKING) {
                            // Move ��Ŷ�� �������� ��Ŷ ó��
                            //cout << "Sync\n";

                            SetEvent(network_mgr.GetProcessEvent());

                            WaitForSingleObject(network_mgr.GetRecvEvent(), WSA_INFINITE);
                        }

                        //cout << "RECV : ";
                        //myNP::printPacketType(buf[1]);

                        break;
                    }
                    //std::this_thread::sleep_for(std::chrono::seconds(1));
                }
            }
            else {
                // ���۸� Push
                //cout << "Test2\n";
                network_mgr.PushBuffer(buf);
            }
        }
        else if (recvLen == 0 || recvLen == SOCKET_ERROR) {
            cout << "Recv Failed Error\n";
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
    //cout << "SEND : ";
    switch (packet_id)
    {
        case myNP::CS_MOVE:
        {
            int sendLen = send(clientSocket, buf, sizeof(myNP::CS_MOVE_PACKET), 0);
            if (sendLen == SOCKET_ERROR) {
                cout << "Send Failed Error\n";
                closesocket(clientSocket);
                clientSocket = INVALID_SOCKET;
                WSACleanup();
            }
            break;
        }
        case myNP::CS_MATCHMAKING:
        {
            int sendLen = send(clientSocket, buf, sizeof(myNP::CS_MATCHMAKING_PACKET), 0);
            if (sendLen == SOCKET_ERROR) {
                cout << "Send Failed Error\n";
                closesocket(clientSocket);
                clientSocket = INVALID_SOCKET;
                WSACleanup();
            }
            break;
        }
        case myNP::CS_FIRE:
        {
            int sendLen = send(clientSocket, buf, sizeof(myNP::CS_FIRE_PACKET), 0);
            if (sendLen == SOCKET_ERROR) {
                cout << "Send Failed Error\n";
                closesocket(clientSocket);
                clientSocket = INVALID_SOCKET;
                WSACleanup();
            }
            break;
        }
    }
    //myNP::printPacketType(packet_id);
}

// ��Ŷ�� ó��
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

            ProcessPlayerMove(move_packet);
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
        // Item ���� ó��
        case myNP::SC_ITEM_CREATE:
        {
            myNP::SC_ITEM_CREATE_PACKET* item_create_packet = reinterpret_cast<myNP::SC_ITEM_CREATE_PACKET*>(buffer.data());

            ProcessCreateItem(item_create_packet);
            break;
        }
        // Item ���� ó��
        case myNP::SC_ITEM_REMOVE:
        {
            myNP::SC_ITEM_REMOVE_PACKET* item_remove_packet = reinterpret_cast<myNP::SC_ITEM_REMOVE_PACKET*>(buffer.data());

            ProcessRemoveItem(item_remove_packet);
            break;
        }
        // �� Bullet ���� ó��
        case myNP::SC_BULLET_REMOVE:
        {
            myNP::SC_BULLET_REMOVE_PACKET* bullet_remove_packet = reinterpret_cast<myNP::SC_BULLET_REMOVE_PACKET*>(buffer.data());

            ProcessRemoveBullet(bullet_remove_packet);
            break;
        }
        // �÷��̾� Damage ó��
        case myNP::SC_PLAYER_DAMAGE:
        {
            myNP::SC_PLAYER_DAMAGE_PACKET* player_damage_packet = reinterpret_cast<myNP::SC_PLAYER_DAMAGE_PACKET*>(buffer.data());

            ProcessPlayerDamage(player_damage_packet);
            break;
        }
        // ���� ���� ó��
        case myNP::SC_GAMEOVER:
        {
            myNP::SC_GAMEOVER_PACKET* gameover_packet = reinterpret_cast<myNP::SC_GAMEOVER_PACKET*>(buffer.data());

            ProcessGameover(gameover_packet);
            break;
        }
        }
    }

    // ResetEvent(processEvent);
}

// ��� �̵� ó��
void ClientNetworkManager::ProcessPlayerMove(myNP::SC_MOVE_PACKET* move_packet)
{
    // ����Ʈ ����
    move_packet->ntohByteOrder();
    std::shared_ptr<GameScene> gameScene = std::dynamic_pointer_cast<GameScene>(currentScene);
    //cout << "���� ��ǥ\n";
    //cout << 1-ClientID << " " << move_packet->posX << ", " << move_packet->posY << "\n";
    gameScene->GetOtherPlayer().setPosition(move_packet->posX, move_packet->posY);
    gameScene->GetOtherPlayer().SetDirection(move_packet->dir);
}

// ��ġ����ŷ ó��
void ClientNetworkManager::ProcessMatchMaking(myNP::SC_MATCHMAKING_PACKET* matchmaking_packet)
{
    // ����Ʈ ����
    matchmaking_packet->ntohByteOrder();

    std::shared_ptr<GameScene> gameScene = std::dynamic_pointer_cast<GameScene>(currentScene);
    gameScene->SetTitleFalse();

    // ��ġ����ŷ�� �ҽ� ClientNetworkManager�� ID�� p_id �ֱ�
    ClientID = static_cast<int32_t>(matchmaking_packet->p_id);
    cout << "Client ID: " << ClientID << endl;
    playing = true;
    timer.Init();
    // TODO : ��ġ����ŷ�� ����� �Ʒ�ó�� Scene�� �ε��ؾ���
    //        ������ ��ġ ���� ����� �� �� ����
    //sceneManager.LoadGameScene();
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
    int elapsed_ms = time_gap.count() / myNP::microToSecond;

    //// �Ѿ��� �ʱ� ��ġ ����
    float startX = fire_packet->posX;
    float startY = fire_packet->posY;

    // �Ѿ� �ӵ� (����: �ȼ�/ms)
    const float BULLET_SPEED = GunLoader::Instance().GetGunTable()[fire_packet->type].speed;

    // �ð� ���̸�ŭ �Ѿ� ��ġ ����
    float currentX;
    if (fire_packet->dir)
        currentX = startX - (BULLET_SPEED * elapsed_ms);
    else
        currentX = startX + (BULLET_SPEED * elapsed_ms);

    gameScene->AddEnemyBullet(currentX, fire_packet->posY, fire_packet->dir, fire_packet->type, fire_packet->b_id);
}

// ���(��Ȱ) ó��
void ClientNetworkManager::ProcessLifeUpdate(myNP::SC_LIFE_UPDATE_PACKET* life_packet)
{
    // ����Ʈ ����
    life_packet->ntohByteOrder();
    std::shared_ptr<GameScene> gameScene = std::dynamic_pointer_cast<GameScene>(currentScene);
    
    // 0�� �÷��̾ �׾�����
    if (0 == life_packet->p_id) {
        std::cout << "1Ŭ�� ���\n";
        gameScene->GetPlayer1().revivePlayer();
    }
    // 1�� �÷��̾ �׾�����
    else if (1 == life_packet->p_id) {
        std::cout << "2Ŭ�� ���\n";
        gameScene->GetPlayer2().revivePlayer();
    }
}

// �� ������Ʈ ó��
void ClientNetworkManager::ProcessGunUpdate(myNP::SC_GUN_UPDATE_PACKET* gun_packet)
{
    // ����Ʈ ����
    gun_packet->ntohByteOrder();
    std::shared_ptr<GameScene> gameScene = std::dynamic_pointer_cast<GameScene>(currentScene);

    // �����̸�
    if (0 == gun_packet->p_id) gameScene->GetPlayer1().setPlayerGun(gun_packet->g_id);
    // ����
    else gameScene->GetPlayer2().setPlayerGun(gun_packet->g_id);
}

// ������ ���� ó��
void ClientNetworkManager::ProcessCreateItem(myNP::SC_ITEM_CREATE_PACKET* item_create_packet)
{
    std::shared_ptr<GameScene> gameScene = std::dynamic_pointer_cast<GameScene>(currentScene);
    
    gameScene->MakeItem(item_create_packet->i_id, item_create_packet->posX, item_create_packet->posY);
}

// ������ ���� ó��
void ClientNetworkManager::ProcessRemoveItem(myNP::SC_ITEM_REMOVE_PACKET* item_remove_packet)
{
    std::shared_ptr<GameScene> gameScene = std::dynamic_pointer_cast<GameScene>(currentScene);

    gameScene->RemoveItem(item_remove_packet->i_id);
}

// �Ѿ� ���� ó��
void ClientNetworkManager::ProcessRemoveBullet(myNP::SC_BULLET_REMOVE_PACKET* bullet_remove_packet)
{
    std::shared_ptr<GameScene> gameScene = std::dynamic_pointer_cast<GameScene>(currentScene);

    gameScene->RemoveBullet(bullet_remove_packet->b_id);
}

// �÷��̾� Damage ó��
void ClientNetworkManager::ProcessPlayerDamage(myNP::SC_PLAYER_DAMAGE_PACKET* player_damage_packet)
{
    std::shared_ptr<GameScene> gameScene = std::dynamic_pointer_cast<GameScene>(currentScene);

    gameScene->PlayerDamage(player_damage_packet->damage, ClientID);
}

// ���� ���� ó��
void ClientNetworkManager::ProcessGameover(myNP::SC_GAMEOVER_PACKET* gameover_packet)
{
    std::shared_ptr<GameScene> gameScene = std::dynamic_pointer_cast<GameScene>(currentScene);
    
    gameScene->Gameover();
}

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

// 초기화
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

    // 씬 초기화
    currentScene = sceneManager.GetActiveScene();
    
    // 커넥트
    Connect();

    // Recv 스레드 생성
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

// Recv 스레드 생성
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

    // 예외 처리
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

// Recv 스레드
DWORD WINAPI WorkerRecv(LPVOID arg)
{
    char buf[MAX_SIZE];

    while (true) {
        // 이벤트 대기
        //DWORD result = WaitForSingleObject(event, WSA_INFINITE);

        // 고정 길이 recv()
        int recvLen = recv(network_mgr.GetSocket(), buf, sizeof(myNP::BASE_PACKET), MSG_WAITALL);
        //cout << recvLen << "\n";

        if (recvLen > 0) {
            // base_packet 길이 만큼 읽었으므로 나머지 데이터 길이 계산
            auto remainingPacketLen = buf[0] - sizeof(myNP::BASE_PACKET);

            //cout << "RECV " << static_cast<int>(buf[1]) << ": ";
            //myNP::printPacketType(buf[1]);

            // 가변 길이 recv()
            if (remainingPacketLen > 0) {
                while (true)
                {
                    //cout << "Test1\n";
                    recvLen = recv(network_mgr.GetSocket(), buf + sizeof(myNP::BASE_PACKET), remainingPacketLen, MSG_WAITALL);
                    //cout << recvLen << "\n";
                    if (recvLen > 0) {

                        // 버퍼를 Push
                        network_mgr.PushBuffer(buf);

                        if (buf[1] == myNP::SC_MY_MOVE or buf[1] == myNP::SC_MATCHMAKING) {
                            // Move 패킷을 기준으로 패킷 처리
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
                // 버퍼를 Push
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

// 받아온 버퍼를 큐에 push
void ClientNetworkManager::PushBuffer(char buf[MAX_SIZE])
{
    std::array<char, MAX_SIZE> newBuffer;
    memcpy(newBuffer.data(), buf, MAX_SIZE);
    process_queue.push(newBuffer);
    SetEvent(processEvent);
}

// 패킷 send
void ClientNetworkManager::SendPacket(char* buf, uint8_t packet_id)
{
    // 패킷 ID별 다른 처리
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

// 패킷별 처리
void ClientNetworkManager::ProcessPacket()
{
    // WaitForSingleObject(processEvent, INFINITE);

    while (!process_queue.empty()) {
        std::array<char, MAX_SIZE> buffer = process_queue.front();
        process_queue.pop();

        // 두 번째 바이트가 패킷 타입
        uint8_t packetType = static_cast<uint8_t>(buffer[1]);

        switch (packetType) {
        // 이동 처리
        case myNP::SC_MY_MOVE:
        {
            myNP::SC_MOVE_PACKET* move_packet = reinterpret_cast<myNP::SC_MOVE_PACKET*>(buffer.data());

            ProcessPlayerMove(move_packet);
            break;
        }
        // 매치메이킹 처리
        case myNP::SC_MATCHMAKING:
        {
            myNP::SC_MATCHMAKING_PACKET* matchmaking_packet = reinterpret_cast<myNP::SC_MATCHMAKING_PACKET*>(buffer.data());

            ProcessMatchMaking(matchmaking_packet);
            break;
        }
        // Fire 처리
        case myNP::SC_FIRE:
        {
            myNP::SC_FIRE_PACKET* fire_packet = reinterpret_cast<myNP::SC_FIRE_PACKET*>(buffer.data());

            ProcessFirebullet(fire_packet);
            break;
        }
        // Life 처리
        case myNP::SC_LIFE_UPDATE:
        {
            myNP::SC_LIFE_UPDATE_PACKET* life_packet = reinterpret_cast<myNP::SC_LIFE_UPDATE_PACKET*>(buffer.data());

            ProcessLifeUpdate(life_packet);
            break;
        }
        // Gun 업데이트 처리
        case myNP::SC_GUN_UPDATE:
        {
            myNP::SC_GUN_UPDATE_PACKET* gun_update_packet = reinterpret_cast<myNP::SC_GUN_UPDATE_PACKET*>(buffer.data());

            ProcessGunUpdate(gun_update_packet);
            break;
        }
        // Item 생성 처리
        case myNP::SC_ITEM_CREATE:
        {
            myNP::SC_ITEM_CREATE_PACKET* item_create_packet = reinterpret_cast<myNP::SC_ITEM_CREATE_PACKET*>(buffer.data());

            ProcessCreateItem(item_create_packet);
            break;
        }
        // Item 삭제 처리
        case myNP::SC_ITEM_REMOVE:
        {
            myNP::SC_ITEM_REMOVE_PACKET* item_remove_packet = reinterpret_cast<myNP::SC_ITEM_REMOVE_PACKET*>(buffer.data());

            ProcessRemoveItem(item_remove_packet);
            break;
        }
        // 적 Bullet 삭제 처리
        case myNP::SC_BULLET_REMOVE:
        {
            myNP::SC_BULLET_REMOVE_PACKET* bullet_remove_packet = reinterpret_cast<myNP::SC_BULLET_REMOVE_PACKET*>(buffer.data());

            ProcessRemoveBullet(bullet_remove_packet);
            break;
        }
        // 플레이어 Damage 처리
        case myNP::SC_PLAYER_DAMAGE:
        {
            myNP::SC_PLAYER_DAMAGE_PACKET* player_damage_packet = reinterpret_cast<myNP::SC_PLAYER_DAMAGE_PACKET*>(buffer.data());

            ProcessPlayerDamage(player_damage_packet);
            break;
        }
        // 게임 종료 처리
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

// 상대 이동 처리
void ClientNetworkManager::ProcessPlayerMove(myNP::SC_MOVE_PACKET* move_packet)
{
    // 바이트 정렬
    move_packet->ntohByteOrder();
    std::shared_ptr<GameScene> gameScene = std::dynamic_pointer_cast<GameScene>(currentScene);
    //cout << "받은 좌표\n";
    //cout << 1-ClientID << " " << move_packet->posX << ", " << move_packet->posY << "\n";
    gameScene->GetOtherPlayer().setPosition(move_packet->posX, move_packet->posY);
    gameScene->GetOtherPlayer().SetDirection(move_packet->dir);
}

// 매치메이킹 처리
void ClientNetworkManager::ProcessMatchMaking(myNP::SC_MATCHMAKING_PACKET* matchmaking_packet)
{
    // 바이트 정렬
    matchmaking_packet->ntohByteOrder();

    std::shared_ptr<GameScene> gameScene = std::dynamic_pointer_cast<GameScene>(currentScene);
    gameScene->SetTitleFalse();

    // 매치메이킹을 할시 ClientNetworkManager의 ID에 p_id 넣기
    ClientID = static_cast<int32_t>(matchmaking_packet->p_id);
    cout << "Client ID: " << ClientID << endl;
    playing = true;
    timer.Init();
    // TODO : 매치메이킹을 만들면 아래처럼 Scene을 로드해야함
    //        지금은 위치 세팅 해줘야 할 듯 싶음
    //sceneManager.LoadGameScene();
}

// 총알 처리
void ClientNetworkManager::ProcessFirebullet(myNP::SC_FIRE_PACKET* fire_packet)
{
    // 바이트 정렬
    fire_packet->ntohByteOrder();
    // 패킷을 받으면 gunId를 확인한 후
    // 총알을 Scene에 추가하고
    // Id에 따라서 총알의 속도를 타임갭으로 보간
    std::shared_ptr<GameScene> gameScene = std::dynamic_pointer_cast<GameScene>(currentScene);

    // 시간 차이 계산 (ms 단위)
    auto time_gap = timer.timeGap(fire_packet->fire_t);
    int elapsed_ms = time_gap.count() / myNP::microToSecond;

    //// 총알의 초기 위치 설정
    float startX = fire_packet->posX;
    float startY = fire_packet->posY;

    // 총알 속도 (단위: 픽셀/ms)
    const float BULLET_SPEED = GunLoader::Instance().GetGunTable()[fire_packet->type].speed;

    // 시간 차이만큼 총알 위치 보간
    float currentX;
    if (fire_packet->dir)
        currentX = startX - (BULLET_SPEED * elapsed_ms);
    else
        currentX = startX + (BULLET_SPEED * elapsed_ms);

    gameScene->AddEnemyBullet(currentX, fire_packet->posY, fire_packet->dir, fire_packet->type, fire_packet->b_id);
}

// 목숨(부활) 처리
void ClientNetworkManager::ProcessLifeUpdate(myNP::SC_LIFE_UPDATE_PACKET* life_packet)
{
    // 바이트 정렬
    life_packet->ntohByteOrder();
    std::shared_ptr<GameScene> gameScene = std::dynamic_pointer_cast<GameScene>(currentScene);
    
    // 0번 플레이어가 죽었으면
    if (0 == life_packet->p_id) {
        std::cout << "1클라 사망\n";
        gameScene->GetPlayer1().revivePlayer();
    }
    // 1번 플레이어가 죽었으면
    else if (1 == life_packet->p_id) {
        std::cout << "2클라 사망\n";
        gameScene->GetPlayer2().revivePlayer();
    }
}

// 총 업데이트 처리
void ClientNetworkManager::ProcessGunUpdate(myNP::SC_GUN_UPDATE_PACKET* gun_packet)
{
    // 바이트 정렬
    gun_packet->ntohByteOrder();
    std::shared_ptr<GameScene> gameScene = std::dynamic_pointer_cast<GameScene>(currentScene);

    // 본인이면
    if (0 == gun_packet->p_id) gameScene->GetPlayer1().setPlayerGun(gun_packet->g_id);
    // 상대면
    else gameScene->GetPlayer2().setPlayerGun(gun_packet->g_id);
}

// 아이템 생성 처리
void ClientNetworkManager::ProcessCreateItem(myNP::SC_ITEM_CREATE_PACKET* item_create_packet)
{
    std::shared_ptr<GameScene> gameScene = std::dynamic_pointer_cast<GameScene>(currentScene);
    
    gameScene->MakeItem(item_create_packet->i_id, item_create_packet->posX, item_create_packet->posY);
}

// 아이템 삭제 처리
void ClientNetworkManager::ProcessRemoveItem(myNP::SC_ITEM_REMOVE_PACKET* item_remove_packet)
{
    std::shared_ptr<GameScene> gameScene = std::dynamic_pointer_cast<GameScene>(currentScene);

    gameScene->RemoveItem(item_remove_packet->i_id);
}

// 총알 삭제 처리
void ClientNetworkManager::ProcessRemoveBullet(myNP::SC_BULLET_REMOVE_PACKET* bullet_remove_packet)
{
    std::shared_ptr<GameScene> gameScene = std::dynamic_pointer_cast<GameScene>(currentScene);

    gameScene->RemoveBullet(bullet_remove_packet->b_id);
}

// 플레이어 Damage 처리
void ClientNetworkManager::ProcessPlayerDamage(myNP::SC_PLAYER_DAMAGE_PACKET* player_damage_packet)
{
    std::shared_ptr<GameScene> gameScene = std::dynamic_pointer_cast<GameScene>(currentScene);

    gameScene->PlayerDamage(player_damage_packet->damage, ClientID);
}

// 게임 종료 처리
void ClientNetworkManager::ProcessGameover(myNP::SC_GAMEOVER_PACKET* gameover_packet)
{
    std::shared_ptr<GameScene> gameScene = std::dynamic_pointer_cast<GameScene>(currentScene);
    
    gameScene->Gameover();
}

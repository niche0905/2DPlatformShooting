#include "pch.h"
#include "ClientNetworkManager.h"

DWORD WINAPI WorkerRecv(LPVOID arg)
{
    ClientNetworkManager* network = (ClientNetworkManager*)arg;
    WSAEVENT events[1] = { network->GetRecvEvent() };
    char buf[MAX_SIZE];

    while (true) {
        // �̺�Ʈ ���
        DWORD result = WSAWaitForMultipleEvents(1, events, FALSE, WSA_INFINITE, FALSE);
        if (result == WSA_WAIT_FAILED) break;

        // �̺�Ʈ Ȯ��
        WSANETWORKEVENTS networkEvents;
        if (WSAEnumNetworkEvents(network->GetSocket(), network->GetRecvEvent(), &networkEvents) == SOCKET_ERROR)
            break;

        // ���� ���� Ȯ��
        if (networkEvents.lNetworkEvents & FD_CLOSE) {
            break;
        }

        // ������ ����
        if (networkEvents.lNetworkEvents & FD_READ) {
            int recvLen = recv(network->GetSocket(), buf, MAX_SIZE, 0);
            if (recvLen > 0) {
                network->PushBuffer(buf);
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
    if (clientThread != NULL)
    {
        CloseHandle(clientThread);
        clientThread = NULL;
    }

    clientThread = CreateThread(NULL, 0, WorkerRecv, (LPVOID)this, 0, NULL);
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
    switch (packet_id)
    {
        case myNP::CS_MOVE:
            int sendLen = send(clientSocket, buf, sizeof(myNP::CS_MOVE_PACKET), 0);
            if (sendLen == SOCKET_ERROR) {
                closesocket(clientSocket);
                clientSocket = INVALID_SOCKET;
                WSACleanup();
            }
        case myNP::CS_MATCHMAKING:
            int sendLen = send(clientSocket, buf, sizeof(myNP::CS_MATCHMAKING_PACKET), 0);
            if (sendLen == SOCKET_ERROR) {
                closesocket(clientSocket);
                clientSocket = INVALID_SOCKET;
                WSACleanup();
            }
        case myNP::CS_FIRE:
            int sendLen = send(clientSocket, buf, sizeof(myNP::CS_FIRE_PACKET), 0);
            if (sendLen == SOCKET_ERROR) {
                closesocket(clientSocket);
                clientSocket = INVALID_SOCKET;
                WSACleanup();
            }

    }
}

void ClientNetworkManager::Update()
{
    while (!process_queue.empty()) {
        std::array<char, MAX_SIZE> packet = process_queue.front();
        process_queue.pop();

        // �� �����ؾ�����?
    }

    // �̺�Ʈ �缳��
    ResetEvent(processEvent);
}

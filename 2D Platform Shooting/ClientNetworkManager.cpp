#include "pch.h"
#include "ClientNetworkManager.h"

DWORD WINAPI WorkerRecv(LPVOID arg)
{
    ClientNetworkManager* network = (ClientNetworkManager*)arg;
    WSAEVENT events[1] = { network->GetRecvEvent()};
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

ClientNetworkManager::ClientNetworkManager()
{

}

ClientNetworkManager::~ClientNetworkManager()
{

}

void ClientNetworkManager::Init()
{
    // ���� �ʱ�ȭ
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return;

    // ���� ����
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) //err_quit("socket()");

    // �̺�Ʈ �ڵ� ����
    recvEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (recvEvent == NULL) {
        closesocket(clientSocket);
        //err_quit("Failed to create receive event");
    }

    processEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (processEvent == NULL) {
        CloseHandle(recvEvent);
        closesocket(clientSocket);
        //err_quit("Failed to create process event");
    }

    // ť �ʱ�ȭ
    process_queue = std::queue<std::array<char, MAX_SIZE>>();

    // Ŭ���̾�Ʈ ID �ʱ�ȭ
    ClientID = -1;

    // ������ �ڵ� �ʱ�ȭ
    clientThread = NULL;
}

void ClientNetworkManager::Connect()
{
    // ���� �ּ� ����
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serveraddr.sin_addr) != 1) {
        closesocket(clientSocket);
        WSACleanup();
        return;
    }

    // ���� �õ�
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
    // �̹� �����尡 �����ϴ� ��� ó��
    if (clientThread != NULL)
    {
        CloseHandle(clientThread);
        clientThread = NULL;
    }

    // WorkerRecv ������ ����
    clientThread = CreateThread(NULL, 0, WorkerRecv, (LPVOID)this, 0, NULL);

    // ������ ���� ���� �� ó��
    if (clientThread == NULL)
    {
        // ���� �ݱ�
        if (clientSocket != INVALID_SOCKET)
        {
            closesocket(clientSocket);
            clientSocket = INVALID_SOCKET;
        }

        // �̺�Ʈ �ڵ� �ݱ�
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

        // ť ����
        while (!process_queue.empty())
        {
            process_queue.pop();
        }

        // Ŭ���̾�Ʈ ID �ʱ�ȭ
        ClientID = -1;

        WSACleanup();
    }
}

void ClientNetworkManager::PushBuffer(char buf[MAX_SIZE])
{
    // ���ο� ���� �迭 ����
    std::array<char, MAX_SIZE> newBuffer;

    // ���� ������ �����͸� �� �迭�� ����
    memcpy(newBuffer.data(), buf, MAX_SIZE);

    // ť�� ���� �߰�
    process_queue.push(newBuffer);

    // ó�� �̺�Ʈ ����
    SetEvent(processEvent);
}

void ClientNetworkManager::SendPacket(char buf[MAX_SIZE])
{

}

void ClientNetworkManager::Update()
{

}